#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <swis.h>

#include "asmutils.h"
#include "debug.h"
#include "header.h"
#include "session.h"
#include "utils.h"

/** Session currently being initialised */
static session_ctx *init_session;
static uint32_t init_timeout;

_kernel_oserror *mod_init(const char *tail, int podule_base, void *pw)
{
	uint32_t prev_handler;
	_kernel_oserror *e;

	UNUSED(tail);
	UNUSED(podule_base);

	e = _swix(OS_Claim, _INR(0,2), 0x10 /* EventV */, internet, pw);
	if (e != NULL)
		return e;

	/* Don't really care if this fails */
	_swix(OS_Byte, _INR(0,1), 14 /* Enable event */, 19);

	/* Claim CPU vectors. Disable IRQs while we do this. */
	_swix(OS_IntOff, _IN(0), 0);

	_swix(OS_ClaimProcessorVector, _INR(0,1) | _OUT(1),
			1 | (1<<8), undef_handler, &prev_handler);
	set_prev_undef_handler(prev_handler);

	_swix(OS_ClaimProcessorVector, _INR(0,1) | _OUT(1), 
			2 | (1<<8), swi_handler, &prev_handler);
	set_prev_swi_handler(prev_handler);

	_swix(OS_ClaimProcessorVector, _INR(0,1) | _OUT(1),
			3 | (1<<8), prefetch_handler, &prev_handler);
	set_prev_prefetch_handler(prev_handler);

	_swix(OS_ClaimProcessorVector, _INR(0,1) | _OUT(1),
			4 | (1<<8), dabort_handler, &prev_handler);
	set_prev_dabort_handler(prev_handler);

	_swix(OS_ClaimProcessorVector, _INR(0,1) | _OUT(1),
			6 | (1<<8), irq_handler, &prev_handler);
	set_prev_irq_handler(prev_handler);

	_swix(OS_IntOn, _IN(0), 0);

	return NULL;
}

_kernel_oserror *mod_fini(int fatal, int podule_base, void *pw)
{
	uint32_t prev_handler;
	_kernel_oserror *e;

	UNUSED(fatal);
	UNUSED(podule_base);

	session_fini();
	
	/* Release CPU vectors. Disable IRQs while we do this. */
	_swix(OS_IntOff, _IN(0), 0);

	prev_handler = get_prev_undef_handler();
	_swix(OS_ClaimProcessorVector, _INR(0,2),
			1, prev_handler, undef_handler);

	prev_handler = get_prev_swi_handler();
	_swix(OS_ClaimProcessorVector, _INR(0,2), 
			2, prev_handler, swi_handler);

	prev_handler = get_prev_prefetch_handler();
	_swix(OS_ClaimProcessorVector, _INR(0,2),
			3, prev_handler, prefetch_handler);

	prev_handler = get_prev_dabort_handler();
	_swix(OS_ClaimProcessorVector, _INR(0,2),
			4, prev_handler, dabort_handler);

	prev_handler = get_prev_irq_handler();
	_swix(OS_ClaimProcessorVector, _INR(0,2),
			6, prev_handler, irq_handler);

	_swix(OS_IntOn, _IN(0), 0);

	e = _swix(OS_Byte, _INR(0,1), 13 /* Disable event */, 19);

	e = _swix(OS_Release, _INR(0,2), 0x10, internet, pw);

	return NULL;
}

_kernel_oserror *command_handler(const char *arg_string, int argc,
                                 int number, void *pw)
{
	UNUSED(argc);

	switch (number) {
	case CMD_GDB:	/* *GDB <filename> [<args>] */
	{
		/* Open a new session. */
		init_session = session_ctx_create(SESSION_TCP);
		if (init_session == NULL)
			return NULL;

		_swix(OS_ReadMonotonicTime, _OUT(0), &init_timeout);

		/* 10 seconds */
		init_timeout += 1000;

		debug("Waiting 10s for debugger%s\n", "");

		/* Register callback which will trigger when the kernel
		 * threads out but before the application is entered.
		 *
		 * Crucially, the callback will be called after the new
		 * application has been loaded into application space but
		 * before it is run.
		 */
		_swix(OS_AddCallBack, _INR(0,1), post_run, pw);

		/* Run application, passing it arguments. */
		if (_swix(OS_FSControl, _INR(0,1), 4, arg_string) != NULL) {
			/* Remove the callback if the run failed */
			_swix(OS_RemoveCallBack, _INR(0,1), post_run, pw);

			session_ctx_destroy(init_session);

			init_timeout = 0;
			init_session = NULL;
		}
	}
		break;
	}

	return NULL;
}

int internet_handler(_kernel_swi_regs *r, void *pw)
{
	UNUSED(pw);

	switch (r->r[1]) {
	case 1: /* Input pending */
	{
		session_ctx *session;

		debug("Input on %d\n", r->r[2]);

		/* Work out what socket this is. */
		session = session_find_by_socket(r->r[2]);
		if (session == NULL)
			return 1;

		return session_tcp_process_input(session, r->r[2]);
	}
		break;
	case 2: /* OOB data pending */
		/* Ignore this */
		debug("OOB on %d\n", r->r[2]);
		break;
	case 3: /* Socket closed */
	{
		session_ctx *session;

		/* If it's one of our clients, then tidy up */

		session = session_find_by_socket(r->r[2]);
		if (session == NULL)
			return 1;

		session_tcp_notify_closed(session, r->r[2]);
	}
		break;
	default: /* Don't care */
		break;
	}

	return 0;
}

_kernel_oserror *post_run_handler(_kernel_swi_regs *r, void *pw)
{
	session_ctx *session = init_session;
	uint32_t now;

	UNUSED(r);
	UNUSED(pw);

	/* Wait (with timeout) for the debugger to connect 
	 * 
	 * Then we can set breakpoints as appropriate before
	 * permitting the debuggee to start.
	 */
	_swix(OS_ReadMonotonicTime, _OUT(0), &now);
	if (now < init_timeout && session_is_connected(init_session) == 0) {
		/* Force pending callbacks to run */
		trigger_callbacks();
		/* Then add ourselves back to the pending callback chain */
		return _swix(OS_AddCallBack, _INR(0,1), post_run, pw);
	} else if (now >= init_timeout) {
		debug("timed out%s\n", "");

		/* Do not destroy the session here. 
		 * It will be cleaned up by the exit handler
		 */
		init_timeout = 0;
		init_session = NULL;

		return NULL;
	}

	debug("done%s\n", "");

	init_timeout = 0;
	init_session = NULL;

	session_set_current(session);

	/* Drop into the break handler */
	session_wait_for_continue(session);

	return NULL;
}

void mod_service(int service, _kernel_swi_regs *r, void *pw)
{
	UNUSED(r);

	switch (service) {
	case 0x2a: /* New Application */
		if (init_session != NULL) {
			/* New debuggee starting, set up the environment */
			session_change_environment(init_session, pw);
		}
		break;
	}
}

_kernel_oserror *appexit_handler(_kernel_swi_regs *r, void *pw)
{
	session_ctx *session = (session_ctx *) r->r[0];

	UNUSED(pw);

	session_restore_environment(session);

	session_ctx_destroy(session);

	return NULL;
}

_kernel_oserror *apperror_handler(_kernel_swi_regs *r, void *pw)
{
	static _kernel_oserror error;
	session_ctx *session = (session_ctx *) r->r[0];

	UNUSED(pw);

	session_restore_environment(session);

	session_get_error(session, &error);

	session_ctx_destroy(session);

	return &error;
}

_kernel_oserror *appupcall_handler(_kernel_swi_regs *r, void *pw)
{
	session_ctx *session = (session_ctx *) r->r[0];

	UNUSED(pw);

	session_restore_environment(session);

	session_ctx_destroy(session);

	return NULL;
}

