#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <swis.h>

#include "asmutils.h"
#include "debug.h"
#include "gdb.h"
#include "header.h"
#include "regs.h"
#include "session.h"
#include "socket.h"
#include "step.h"
#include "utils.h"

static const char *post_filter_name = "GDBServer";

/* Ideally, keep this synced with N_CTX in gdb.c */
#define MAX_SESSIONS 5

typedef struct session_bkpt {
	struct session_bkpt *prev;
	struct session_bkpt *next;

/* One shot flag (uses bottom bit of address -- will be zero, even in thumb) */
#define BKPT_ONE_SHOT 0x1
/* Breakpoint instruction. This is an almost guaranteed undefined intruction */
/** \todo use BKPT on architectures that support it? */
#define BKPT 0xe7f000f0
	uint32_t address;
	uint32_t instruction;
} session_bkpt;

struct session_ctx {
	uint32_t cur_pc;
	cpu_registers regs;

	void *pw;

	struct {
		uint32_t addr;
		uint32_t r12;
		uint32_t buf;
	} env_ctx[3];			/* Error, Exit, Upcall */

	uint8_t buffer[256];

	/* Keep fields above this comment in sync with session_asm.s */

	session_type type;
	uint8_t in_use;
	volatile uint8_t brk;

	uint32_t task_handle;

	union {
		struct {
			int server;
			int client;
		} tcp;
	} data;

	gdb_ctx *gdb;

	session_bkpt *bkpts;

	session_bkpt *free_bkpts;
};

static session_ctx sessions[MAX_SESSIONS];

static int session_break(uintptr_t ctx);
static int session_continue(uintptr_t ctx);
static int session_step(uintptr_t ctx);
static cpu_registers *session_get_regs(uintptr_t ctx);
static int session_set_bkpt(uintptr_t ctx, uint32_t address);
static int session_clear_bkpt(uintptr_t ctx, uint32_t address);

static session_bkpt *session_find_bkpt(session_ctx *ctx, uint32_t address);

static session_ctx *session_find_by_task_handle(uint32_t task);

static session_ctx *session_tcp_initialise(session_ctx *session);
static void session_tcp_finalise(session_ctx *session);
static size_t session_tcp_send_for_gdb(uintptr_t ctx, const uint8_t *data, 
		size_t len);

void session_fini(void)
{
	int i;

	for (i = 0; i < MAX_SESSIONS; i++) {
		if (sessions[i].in_use)
			session_ctx_destroy(&sessions[i]);
	}
}

session_ctx *session_ctx_create(session_type type)
{
	int i;

	for (i = 0; i < MAX_SESSIONS; i++) {
		if (sessions[i].in_use == 0)
			break;
	}
	if (i == MAX_SESSIONS)
		return NULL;

	sessions[i].in_use = 1;
	sessions[i].task_handle = 0;
	/* Start session with debuggee paused */
	sessions[i].brk = 1;

	switch (type) {
	case SESSION_TCP:
		return session_tcp_initialise(&sessions[i]);
	}

	return NULL;
}

void session_ctx_destroy(session_ctx *ctx)
{
	session_bkpt *b;

	if (ctx->gdb != NULL)
		gdb_ctx_destroy(ctx->gdb);

	switch (ctx->type) {
	case SESSION_TCP:
		session_tcp_finalise(ctx);
	}

	while (ctx->bkpts != NULL) {
		b = ctx->bkpts;
		ctx->bkpts = b->next;
		free(b);
	}

	while (ctx->free_bkpts != NULL) {
		b = ctx->free_bkpts;
		ctx->free_bkpts = b->next;
		free(b);
	}

	if (ctx->task_handle != 0) {
		_swix(Filter_DeRegisterPostFilter, _INR(0,4),
				post_filter_name, session_post_poll, 
				ctx->pw, ctx->task_handle, 0);
	}

	memset(ctx, 0, sizeof(session_ctx));

	/* Invalidate the current session */
	if (session_get_current() == ctx)
		session_set_current(NULL);
}

void session_handle_break(session_ctx *ctx, int reason)
{
	enum { DATA = 0, PREFETCH = 1, UNDEF = 2 };
	session_bkpt *bkpt;

	/* Important: This function (and those called by it), *must* not
	 * attempt to access non-automatic variables (i.e. globals) or call
	 * SCL functions.
	 * The magic words at the bottom of the SVC stack are not set up
	 * by our exception handlers so relocations will fail.
	 */

	/** \todo determine reason for break & store it in the ctx */
	switch (reason) {
	case DATA:
		break;
	case PREFETCH:
		break;
	case UNDEF:
		break;
	}

	/* If this was a breakpoint, and it's one-shot, then clear it */
	bkpt = session_find_bkpt(ctx, ctx->regs.r[15]);
	if (bkpt != NULL && (bkpt->address & BKPT_ONE_SHOT))
		session_clear_bkpt((uintptr_t ) ctx, ctx->regs.r[15]);

	/* Add a callback to emit the break status */
	_swix(OS_AddCallBack, _INR(0,1), post_abort, ctx->pw);

	session_wait_for_continue(ctx);
}

_kernel_oserror *post_abort_handler(_kernel_swi_regs *r, void *pw)
{
	session_ctx *ctx = session_get_current();

	UNUSED(r);
	UNUSED(pw);

	/** \todo retrieve break status from the ctx, then improve this */

	/* Emit break status */
	if (ctx->data.tcp.client >= 0) {
		const uint8_t *msg = (const uint8_t *) "$S05#b8";
		size_t towrite = 7;

		while (towrite > 0) {
			towrite = socket_send(ctx->data.tcp.client, 
					msg + 7 - towrite, towrite);
		}
	}

	return NULL;
}

void session_wait_for_continue(session_ctx *ctx)
{
	/* Ensure we're stopped */
	ctx->brk = 1;

	/* Simply sit in a loop waiting for the session to be continued */
	while (ctx->brk) {
		/* Trigger callbacks, so the Internet stack doesn't deadlock */
		trigger_callbacks();
	}
}

int session_break(uintptr_t ctx)
{
	session_ctx *session = (session_ctx *) ctx;

	/* Ignore any attempts to break while we're paused */
	if (session->brk == 1)
		return 0;

	if (session == session_get_current()) {
		/* Inject bkpt, so debuggee breaks on resumption */
		if (session_set_bkpt(ctx, 
				session->cur_pc | BKPT_ONE_SHOT) == 0)
			return 0;
	}

	session->brk = 1;

	return 1;
}

int session_continue(uintptr_t ctx)
{
	session_ctx *session = (session_ctx *) ctx;

	session->brk = 0;

	return 1;
}

int session_step(uintptr_t ctx)
{
	session_ctx *session = (session_ctx *) ctx;
	uint32_t instruction = *((uint32_t *) session->regs.r[15]);
	uint32_t next = step_instruction(instruction, &session->regs);

	/* Inject bkpt, so debuggee breaks on resumption */
	if (session_set_bkpt(ctx, next | BKPT_ONE_SHOT) == 0)
		return 0;

	session->brk = 0;

	return 1;
}

cpu_registers *session_get_regs(uintptr_t ctx)
{
	session_ctx *session = (session_ctx *) ctx;

	return &session->regs;
}

int session_set_bkpt(uintptr_t ctx, uint32_t address)
{
	session_ctx *session = (session_ctx *) ctx;
	session_bkpt *bkpt;
	int irq_state;

	for (bkpt = session->bkpts; bkpt != NULL; bkpt = bkpt->next) {
		if ((bkpt->address & ~BKPT_ONE_SHOT) == 
				(address & ~BKPT_ONE_SHOT))
			break;
	}
	/* There's already a breakpoint at this address */
	if (bkpt != NULL)
		return 1;

	if (session->free_bkpts != NULL) {
		bkpt = session->free_bkpts;
		session->free_bkpts = bkpt->next;
	} else {
		bkpt = malloc(sizeof(session_bkpt));
		if (bkpt == NULL)
			return 0;
	}

	bkpt->address = address;

	/* Insert breakpoint into list */
	bkpt->prev = NULL;
	bkpt->next = session->bkpts;
	if (session->bkpts != NULL)
		session->bkpts->prev = bkpt;
	session->bkpts = bkpt;

	/* IRQs off while we insert the breakpoint */
	irq_state = irqs_off();

	/* Save current instruction */
	bkpt->instruction = *((uint32_t *) (address & ~BKPT_ONE_SHOT));

	/* Replace it with our breakpoint */
	*((uint32_t *) (address & ~BKPT_ONE_SHOT)) = BKPT;

	/* Clean out the Icache */
	flush_icache();

	/* And leave the critical section */
	irqs_restore(irq_state);

	return 1;
}

int session_clear_bkpt(uintptr_t ctx, uint32_t address)
{
	session_ctx *session = (session_ctx *) ctx;
	session_bkpt *bkpt;
	int irq_state;

	for (bkpt = session->bkpts; bkpt != NULL; bkpt = bkpt->next) {
		if ((bkpt->address & ~BKPT_ONE_SHOT) == 
				(address & ~BKPT_ONE_SHOT))
			break;
	}
	/* There's no breakpoint at this address */
	if (bkpt == NULL)
		return 1;

	/* Remove breakpoint from list */
	if (bkpt->prev != NULL)
		bkpt->prev->next = bkpt->next;
	else
		session->bkpts = bkpt->next;

	if (bkpt->next != NULL)
		bkpt->next->prev = bkpt->prev;

	/* Insert it into the free list */
	bkpt->prev = NULL;
	bkpt->next = session->free_bkpts;
	if (session->free_bkpts != NULL)
		session->free_bkpts->prev = bkpt;
	session->free_bkpts = bkpt;

	/* IRQs off while we remove the breakpoint */
	irq_state = irqs_off();

	/* Restore saved instruction */
	*((uint32_t *) (address & ~BKPT_ONE_SHOT)) = bkpt->instruction;

	/* Clean out the Icache */
	flush_icache();

	/* And leave the critical section */
	irqs_restore(irq_state);

	return 1;
}

session_bkpt *session_find_bkpt(session_ctx *ctx, uint32_t address)
{
	session_bkpt *bkpt;

	for (bkpt = ctx->bkpts; bkpt != NULL; bkpt = bkpt->next) {
		if ((bkpt->address & ~BKPT_ONE_SHOT) ==
				(address & ~BKPT_ONE_SHOT))
			break;
	}

	return bkpt;
}

int session_is_connected(session_ctx *ctx)
{
	if (ctx->in_use) {
		switch (ctx->type) {
		case SESSION_TCP:
			return ctx->data.tcp.client > 0;
		}
	}

	return 0;
}

void session_change_environment(session_ctx *ctx, void *pw)
{
	ctx->pw = pw;

	_swix(OS_ChangeEnvironment, _INR(0,3) | _OUTR(1,3),
			6, error_veneer, ctx, ctx->buffer,
			&ctx->env_ctx[0].addr, &ctx->env_ctx[0].r12,
			&ctx->env_ctx[0].buf);

	_swix(OS_ChangeEnvironment, _INR(0,3) | _OUTR(1,3),
			11, exit_veneer, ctx, ctx->buffer,
			&ctx->env_ctx[1].addr, &ctx->env_ctx[1].r12,
			&ctx->env_ctx[1].buf);

	_swix(OS_ChangeEnvironment, _INR(0,3) | _OUTR(1,3),
			16, upcall_veneer, ctx, ctx->buffer,
			&ctx->env_ctx[2].addr, &ctx->env_ctx[2].r12,
			&ctx->env_ctx[2].buf);
}

void session_restore_environment(session_ctx *ctx)
{
	_swix(OS_ChangeEnvironment, _INR(0,3),
			6, ctx->env_ctx[0].addr, ctx->env_ctx[0].r12,
			ctx->env_ctx[0].buf);

	_swix(OS_ChangeEnvironment, _INR(0,3),
			11, ctx->env_ctx[1].addr, ctx->env_ctx[1].r12,
			ctx->env_ctx[1].buf);

	_swix(OS_ChangeEnvironment, _INR(0,3),
			16, ctx->env_ctx[2].addr, ctx->env_ctx[2].r12,
			ctx->env_ctx[2].buf);
}

void session_set_task_handle(session_ctx *ctx, uint32_t task)
{
	/* Not us, if we've already got a task handle */
	if (ctx->task_handle != 0)
		return;

	/* Install post poll filter so that:
	 *
	 * 1) we do the right thing when the client tries to interrupt 
	 *    the debuggee.
	 * 2) we keep current_session in sync with reality.
	 *
	 * The global pre poll filter will invalidate current_session if it
	 * is set.
	 */
	_swix(Filter_RegisterPostFilter, _INR(0,4),
			post_filter_name, session_post_poll, 
			ctx->pw, task, 0);

	ctx->task_handle = task;
}

_kernel_oserror *session_post_poll_handler(_kernel_swi_regs *r, void *pw)
{
	session_ctx *session = session_find_by_task_handle(r->r[2]);

	UNUSED(r);
	UNUSED(pw);

	debug("session post poll%s\n", "");

	if (session != NULL) {
		session_set_current(session);

		/* On reentry into the debuggee through this post filter, 
		 * we need to ensure that the PC resides in application space 
		 * before acting on any pending break. Our SWI vector handler 
		 * will have stored the return address from Wimp_Poll into 
		 * the session. Therefore, we need to consider the session's 
		 * break flag and, if set, set a breakpoint on the return 
		 * address. The normal breakpoint handling code will take over
		 * from there.
		 */

		if (session->brk) {
			session_set_bkpt((uintptr_t) session, 
					session->cur_pc | BKPT_ONE_SHOT);
		}
	}

	return NULL;
}

void session_get_error(session_ctx *ctx, _kernel_oserror *error)
{
	memcpy(error, ctx->buffer + 4, sizeof(_kernel_oserror));
}

session_ctx *session_find_by_task_handle(uint32_t task)
{
	int i;

	for (i = 0; i < MAX_SESSIONS; i++) {
		if (sessions[i].in_use && sessions[i].task_handle == task)
			break;
	}
	if (i == MAX_SESSIONS)
		return NULL;

	return &sessions[i];
}

session_ctx *session_find_by_socket(int socket)
{
	int i;

	for (i = 0; i < MAX_SESSIONS; i++) {
		if (sessions[i].in_use && sessions[i].type == SESSION_TCP &&
				(sessions[i].data.tcp.server == socket || 
				 sessions[i].data.tcp.client == socket))
			break;
	}
	if (i == MAX_SESSIONS)
		return NULL;

	return &sessions[i];
}

int session_tcp_process_input(session_ctx *session, int socket)
{
	if (session->type != SESSION_TCP)
		return 1;

	if (socket == session->data.tcp.server) {
		/* If it's a server socket, accept the connection and
		 * begin a new debug session with the client */
		if (session->data.tcp.client >= 0)
			return 0;

		session->data.tcp.client = socket_accept(socket);
		if (session->data.tcp.client == -1)
			return 0;

		debug("New client on %d\n", session->data.tcp.client);

		session->gdb = gdb_ctx_create(session_tcp_send_for_gdb, 
					session_break, session_continue,
					session_get_regs, session_set_bkpt,
					session_clear_bkpt, session_step,
					(uintptr_t) session);
		if (session->gdb == NULL)
			return 0;
	} else {
		/* If it's a client socket, 
		 * drive state machine some more */
		static uint8_t buf[1024];
		ssize_t read;

		while ((read = socket_recv(socket, buf, sizeof(buf))) > 0) {
			debug("-> %.*s\n", (int) read, buf);

			gdb_process_input(session->gdb, buf, read);
		}
	}

	return 0;
}

void session_tcp_notify_closed(session_ctx *session, int socket)
{
	if (socket == session->data.tcp.client) {
		debug("Client %d disconnected\n", session->data.tcp.client);

		socket_close(session->data.tcp.client);

		session->data.tcp.client = -1;
	}
}

session_ctx *session_tcp_initialise(session_ctx *session)
{
#define TCP_BASE_PORT 4900

	session->data.tcp.server = -1;
	session->data.tcp.client = -1;

	session->data.tcp.server = socket_open_server(
			TCP_BASE_PORT + session - sessions);
	if (session->data.tcp.server == -1) {
		session_ctx_destroy(session);
		return NULL;
	}

	debug("Listening on %ld (%d)\n", TCP_BASE_PORT + session - sessions,
			session->data.tcp.server);

	return session;
}

void session_tcp_finalise(session_ctx *session)
{
	if (session->data.tcp.server >= 0)
		socket_close(session->data.tcp.server);

	if (session->data.tcp.client >= 0)
		socket_close(session->data.tcp.client);
}

size_t session_tcp_send_for_gdb(uintptr_t ctx, const uint8_t *data, size_t len)
{
	session_ctx *session = (session_ctx *) ctx;
	size_t towrite;

	if (session->data.tcp.client < 0)
		return 0;

	debug("<- %.*s\n", (int) len, data);

	for (towrite = len; towrite > 0; ) {
		towrite = socket_send(session->data.tcp.client, 
				data + len - towrite, towrite);
	}

	return 0;
}

