#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <swis.h>

#include "asmutils.h"
#include "debug.h"
#include "header.h"
#include "session.h"

/** Session currently being initialised */
static session_ctx *init_session;
static uint32_t init_timeout;

_kernel_oserror *
mod_init (const char *tail __attribute__ ((unused)),
	  int podule_base __attribute__ ((unused)),
	  void *pw)
{
  _kernel_oserror *e = _swix (OS_Claim, _INR (0, 2), 0x10 /* EventV */ , internet, pw);
  if (e != NULL)
    return e;

  /* Don't really care if this fails */
  _swix (OS_Byte, _INR (0, 1), 14 /* Enable event */ , 19);

  /* Claim CPU vectors. Disable IRQs while we do this. */
  _swix (OS_IntOff, _IN (0), 0);

  uint32_t prev_handler;

  _swix (OS_ClaimProcessorVector, _INR (0, 1) | _OUT (1),
	 1 | (1 << 8), undef_handler, &prev_handler);
  set_prev_undef_handler (prev_handler);

  _swix (OS_ClaimProcessorVector, _INR (0, 1) | _OUT (1),
	 2 | (1 << 8), swi_handler, &prev_handler);
  set_prev_swi_handler (prev_handler);

  _swix (OS_ClaimProcessorVector, _INR (0, 1) | _OUT (1),
	 3 | (1 << 8), prefetch_handler, &prev_handler);
  set_prev_prefetch_handler (prev_handler);

  _swix (OS_ClaimProcessorVector, _INR (0, 1) | _OUT (1),
	 4 | (1 << 8), dabort_handler, &prev_handler);
  set_prev_dabort_handler (prev_handler);

  _swix (OS_ClaimProcessorVector, _INR (0, 1) | _OUT (1),
	 6 | (1 << 8), irq_handler, &prev_handler);
  set_prev_irq_handler (prev_handler);

  _swix (OS_IntOn, _IN (0), 0);

  return NULL;
}

_kernel_oserror *
mod_fini (int fatal __attribute__ ((unused)),
	  int podule_base __attribute__ ((unused)),
	  void *pw)
{
  session_fini ();

  /* Release CPU vectors. Disable IRQs while we do this. */
  _swix (OS_IntOff, _IN (0), 0);

  uint32_t prev_handler;
  prev_handler = get_prev_undef_handler ();
  _swix (OS_ClaimProcessorVector, _INR (0, 2),
	 1, prev_handler, undef_handler);

  prev_handler = get_prev_swi_handler ();
  _swix (OS_ClaimProcessorVector, _INR (0, 2), 2, prev_handler, swi_handler);

  prev_handler = get_prev_prefetch_handler ();
  _swix (OS_ClaimProcessorVector, _INR (0, 2),
	 3, prev_handler, prefetch_handler);

  prev_handler = get_prev_dabort_handler ();
  _swix (OS_ClaimProcessorVector, _INR (0, 2),
	 4, prev_handler, dabort_handler);

  prev_handler = get_prev_irq_handler ();
  _swix (OS_ClaimProcessorVector, _INR (0, 2), 6, prev_handler, irq_handler);

  _swix (OS_IntOn, _IN (0), 0);

  _swix (OS_Byte, _INR (0, 1), 13 /* Disable event */ , 19);

  _swix (OS_Release, _INR (0, 2), 0x10, internet, pw);

  return NULL;
}

_kernel_oserror *
command_handler (const char *arg_string, int argc __attribute__ ((unused)),
	         int number, void *pw)
{
  switch (number)
    {
    case CMD_GDB: /* *GDB <filename> [<args>] */
      {
	/* Check if there is already another pending debug session on which
	   we're waiting for.  */
	if (init_session)
	  return kErr_AlreadyWaitingForGDB;

	/* Open a new session. */
	init_session = session_ctx_create (SESSION_TCP);
	if (init_session == NULL)
	  return NULL;

	_swix (OS_ReadMonotonicTime, _OUT (0), &init_timeout);

	/* 10 seconds */
	init_timeout += 1000;

	dprintf ("Waiting 10s for debugger\n");

	/* Register callback which will trigger when the kernel
	 * threads out but before the application is entered.
	 *
	 * Crucially, the callback will be called after the new
	 * application has been loaded into application space but
	 * before it is run.
	 */
	_swix (OS_AddCallBack, _INR (0, 1), post_run, pw);

	/* Run application, passing it arguments.  */
	_kernel_oserror *err = _swix (OS_FSControl, _INR (0, 1), 4, arg_string);
	dprintf ("OS_FSControl 4: err %p (%d, %s)\n", (void *)err,
		 err ? err->errnum : 0, err ? err->errmess : "");

	/* Remove the callback in case we still have one outstanding.  */
	_swix (OS_RemoveCallBack, _INR (0, 1), post_run, pw);

	/* It might be that we have a time-out and only after that an
	   error got detected and returned to here.  Then init_session is
	   NULL.  */
	if (init_session)
	  {
	    session_ctx_destroy (init_session);
	    init_session = NULL;
	  }
	init_timeout = 0;
	return err;
      }
      break;
    }

  return NULL;
}

int
internet_handler (_kernel_swi_regs *r, void *pw __attribute__ ((unused)))
{
  switch (r->r[1])
    {
    case 1: /* Input pending */
      {
	/* Work out what socket this is.  */
	session_ctx *session = session_find_by_socket (r->r[2]);
	if (session == NULL)
	  return 1;
	dprintf ("Input on %d\n", r->r[2]);

	return session_tcp_process_input (session, r->r[2]);
      }
      break;

    case 2: /* OOB data pending */
      /* Ignore this */
      dprintf ("OOB on %d\n", r->r[2]);
      break;

    case 3: /* Socket closed */
      {
	/* If it's one of our clients, then tidy up.  */
	session_ctx *session = session_find_by_socket (r->r[2]);
	if (session == NULL)
	  return 1;

	session_tcp_notify_closed (session, r->r[2]);
      }
      break;

    default: /* Don't care */
      break;
    }

  return 0;
}

/**
 * Called as callback just after running the application, i.e. before its
 * startup.
 */
_kernel_oserror *
post_run_handler (_kernel_swi_regs *r __attribute__ ((unused)),
		  void *pw __attribute__ ((unused)))
{
  /* Wait (with timeout) for the debugger to connect 
   * 
   * Then we can set breakpoints as appropriate before
   * permitting the debuggee to start.
   */
  uint32_t now;
  _swix (OS_ReadMonotonicTime, _OUT (0), &now);

  if (now >= init_timeout)
    {
      dprintf ("post_run_handler(): timed out\n");

      /* It might be that service call 0x2A (New Application) has kicked in
	 so, restore environment.  If we didn't had that service call, restoring
	 the environment is harmless.  */
      session_restore_environment (init_session);

      /* After a timeout, we don't have any hooks in the application anymore so
         basically it will run or fail to start but we don't have anything
         to do with.  */
      session_ctx_destroy (init_session);
      init_timeout = 0;
      init_session = NULL;

      return NULL;
    }

  if (!session_is_connected (init_session))
    {
      /* Force pending callbacks to run */
      trigger_callbacks ();
      /* Then add ourselves back to the pending callback chain */
      return _swix (OS_AddCallBack, _INR (0, 1), post_run, pw);
    }
  
  dprintf ("post_run_handler(): gdb is attached\n");

  session_ctx *session = init_session;

  init_timeout = 0;
  init_session = NULL;

  session_set_current (session);

  /* Drop into the break handler */
  session_wait_for_continue (session);

  return NULL;
}

void
mod_service (int service, _kernel_swi_regs *r __attribute__ ((unused)),
	     void *pw)
{
  switch (service)
    {
    case 0x2a:			/* New Application */
      if (init_session != NULL)
	{
	  /* New debuggee starting, set up the environment.  */
	  dprintf ("mod_service(): set our environment.\n");
	  session_change_environment (init_session, pw);
	}
      break;
    }
}

/**
 * Application error handler got called but as we've overruled its error
 * handler, error_veneer get called, which calls apperror finally calling
 * apperror_handler.
 */
_kernel_oserror *
apperror_handler (_kernel_swi_regs *r, void *pw __attribute__ ((unused)))
{
  static _kernel_oserror error;
  session_ctx *session = (session_ctx *) r->r[0];
  dprintf ("apperror_handler(): ctx %p\n", (void *)session);

  session_restore_environment (session);

  session_get_error (session, &error);

  session_ctx_destroy (session);

  return &error;
}

/**
 * Application exit_veneer handler got called but as we've overruled its exit
 * handler, exit_veneer get called, which calls appexit finally calling
 * appexit_handler.
 */
_kernel_oserror *
appexit_handler (_kernel_swi_regs *r, void *pw __attribute__ ((unused)))
{
  session_ctx *session = (session_ctx *) r->r[0];
  dprintf ("appexit_handler(): ctx %p\n", (void *)session);

  session_restore_environment (session);

  session_ctx_destroy (session);

  return NULL;
}

/**
 * Application upcall handler got called but as we've overruled its upcall
 * handler, upcall_veneer get called, which calls appupcall finally calling
 * appupcall_handler.
 */
_kernel_oserror *
appupcall_handler (_kernel_swi_regs *r, void *pw __attribute__ ((unused)))
{
  session_ctx *session = (session_ctx *) r->r[0];
  dprintf ("appupcall_handler(): ctx %p\n", (void *)session);

  session_restore_environment (session);

  session_ctx_destroy (session);

  return NULL;
}
