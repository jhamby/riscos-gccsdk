#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <swis.h>
#include <unixlib/local.h>

#include "asmutils.h"
#include "debug.h"
#include "gdb.h"
#include "header.h"
#include "regs.h"
#include "session.h"
#include "socket.h"
#include "step.h"

static const char session_filter_name[] = "GDBServer";

/* Ideally, keep this synced with N_CTX in gdb.c */
#define MAX_SESSIONS 5

typedef struct session_bkpt
{
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

#define STATIC_ASSERT(cond,tag) \
  enum { STATIC_ASSERT__ ## tag = 1/(!!(cond)) };

/* When following assert fails, adjust session_asm.s.  */
STATIC_ASSERT(sizeof (cpu_registers) == 168, sizeof_cpu_registers_is_unexpected)

struct session_ctx
{
  uint32_t cur_pc;
  cpu_registers regs;

  void *pw;

  /* Keep fields above this comment in sync with session_asm.s (see
     SESSION_CTX_OFFSET_*).  */

  struct
  {
    uint32_t addr;
    uint32_t r12;
    uint32_t buf;
  } env_ctx[3];			/* Error, Exit, Upcall */

  uint8_t buffer[256];

  session_type type;
  uint8_t in_use;
  volatile uint8_t brk;

  uint32_t task_handle;

  union
  {
    struct
    {
      int server;
      int client;
    } tcp;
  } data;

  gdb_ctx *gdb;

  session_bkpt *bkpts; /* Linked list.  */

  session_bkpt *free_bkpts; /* Linked list.  */
};

static session_ctx sessions[MAX_SESSIONS];

static session_ctx *session_find_by_task_handle (uint32_t task);

/* gdb callbacks: */
static int session_break_cb (uintptr_t ctx);
static int session_continue_cb (uintptr_t ctx);
static int session_step_cb (uintptr_t ctx);
static cpu_registers *session_get_regs_cb (uintptr_t ctx);
static int session_set_bkpt_cb (uintptr_t ctx, uint32_t address);
static int session_clear_bkpt_cb (uintptr_t ctx, uint32_t address);

static session_bkpt *session_find_bkpt (session_ctx *ctx, uint32_t address);

/* session tcp transport code: */
static session_ctx *session_tcp_initialise (session_ctx *session);
static void session_tcp_finalise (session_ctx *session);
static int session_tcp_process_input (session_ctx *ctx, int rosocket);
static void session_tcp_notify_closed (session_ctx *ctx);
static size_t session_tcp_send_for_gdb (uintptr_t ctx, const uint8_t *data,
					size_t len);
static session_ctx *session_tcp_find_by_socket (int rosocket);

void
session_fini (void)
{
  for (int i = 0; i < MAX_SESSIONS; i++)
    {
      if (sessions[i].in_use)
	session_ctx_destroy (&sessions[i]);
    }
}

session_ctx *
session_ctx_create (session_type type)
{
  int i;
  for (i = 0; i < MAX_SESSIONS; i++)
    {
      if (sessions[i].in_use == 0)
	break;
    }
  if (i == MAX_SESSIONS)
    return NULL;

  sessions[i].type = type;
  sessions[i].in_use = 1;
  sessions[i].brk = 1; /* Start session with debuggee paused.  */
  /* sessions[i].task_handle = 0; */

  sessions[i].regs.detail.r[15] = 0x8000;
  
  switch (type)
    {
      case SESSION_TCP:
 	return session_tcp_initialise (&sessions[i]);
    }

  return NULL;
}

void
session_ctx_destroy (session_ctx *ctx)
{
  dprintf ("session_ctx_destroy(): ctx %p\n", (void *)ctx);

  /* Invalidate the current session.  */
  if (session_get_current () == ctx)
    session_set_current (NULL);

  if (ctx->gdb != NULL)
    gdb_ctx_destroy (ctx->gdb);

  switch (ctx->type)
    {
      case SESSION_TCP:
	session_tcp_finalise (ctx);
	break;
    }

  while (ctx->bkpts != NULL)
    {
      session_bkpt *b = ctx->bkpts;
      ctx->bkpts = b->next;
      free (b);
    }

  while (ctx->free_bkpts != NULL)
    {
      session_bkpt *b = ctx->free_bkpts;
      ctx->free_bkpts = b->next;
      free (b);
    }

  if (ctx->task_handle != 0)
    {
      const _kernel_oserror *e;
      e = _swix (Filter_DeRegisterPreFilter, _INR (0, 3),
		 session_filter_name, session_pre_poll,
		 ctx->pw, ctx->task_handle);
      if (e)
	dprintf ("session_ctx_destroy(): deregister filter failed: 0x%x %s\n",
		 e->errnum, e->errmess);

      e = _swix (Filter_DeRegisterPostFilter, _INR (0, 4),
		 session_filter_name, session_post_poll,
		 ctx->pw, ctx->task_handle, 0);
      if (e)
	dprintf ("session_ctx_destroy(): deregister filter failed: 0x%x %s\n",
		 e->errnum, e->errmess);
    }

  memset (ctx, 0, sizeof (session_ctx));
}

static session_ctx *
session_find_by_task_handle (uint32_t task)
{
  int i;
  for (i = 0; i < MAX_SESSIONS; i++)
    {
      if (sessions[i].in_use && sessions[i].task_handle == task)
	break;
    }
  if (i == MAX_SESSIONS)
    return NULL;

  return &sessions[i];
}

enum
{
  DATA = 0,
  PREFETCH = 1,
  UNDEF = 2
};

/**
 * Core handling routine for DataAbort, Prefetch and UndefinedInstruction.
 */
static _kernel_oserror *
session_handle_break (int reason)
{
  session_ctx *ctx = session_get_current ();

  /* vvv FIXME: hack to avoid infinite loop.  */
  if (!session_is_connected (ctx))
    {
      session_set_current (NULL);
      return NULL;
    }

  /** \todo determine reason for break & store it in the ctx */
  switch (reason)
    {
      case DATA:
	break;
      case PREFETCH:
	break;
      case UNDEF:
	break;
    }

  /* If this was a breakpoint, and it's one-shot, then clear it.  */
  const session_bkpt *bkpt = session_find_bkpt (ctx, ctx->regs.detail.r[15]);
  if (bkpt != NULL && (bkpt->address & BKPT_ONE_SHOT))
    session_clear_bkpt_cb ((uintptr_t) ctx, ctx->regs.detail.r[15]);

  /* Emit break status.  */
  switch (ctx->type)
    {
      case SESSION_TCP:
	session_tcp_send_for_gdb ((uintptr_t)ctx, (const uint8_t *)"$S05#b8",
				  sizeof ("$S05#b8")-1);
	break;
    }

  session_wait_for_continue (ctx);

  return NULL;
}

/**
 * Called when our current session caused a data abort.
 */
_kernel_oserror *
session_dabort_handler (_kernel_swi_regs *r __attribute__ ((unused)),
			void *pw __attribute__ ((unused)))
{
  return session_handle_break (DATA);
}

/**
 * Called when our current session caused a prefetch abort.
 */
_kernel_oserror *
session_prefetch_handler (_kernel_swi_regs *r __attribute__ ((unused)),
			  void *pw __attribute__ ((unused)))
{
  return session_handle_break (PREFETCH);
}

/**
 * Called when our current session encountered an undefined instruction.
 */
_kernel_oserror *
session_undef_handler (_kernel_swi_regs *r __attribute__ ((unused)),
		       void *pw __attribute__ ((unused)))
{
  return session_handle_break (UNDEF);
}

void
session_wait_for_continue (session_ctx *ctx)
{
  /* Ensure we're stopped.  */
  ctx->brk = 1;

  /* Simply sit in a loop waiting for the session to be continued.  */
  while (ctx->brk && session_is_connected (ctx))
    {
      /* Trigger callbacks, so the Internet stack doesn't deadlock.  */
      trigger_callbacks ();

      if (gdb_got_killed (ctx->gdb))
	{
	  /* We got a request to kill the communication.  */
	  switch (ctx->type)
	    {
	      case SESSION_TCP:
		session_tcp_notify_closed (ctx);
		break;
	    }
	}
    }

  if (!session_is_connected (ctx))
    {
      dprintf ("session_wait_for_continue(): no longer connected -> OS_Exit\n");
      __asm__ volatile ("SWI\t%[SWI_OS_Exit]\n\t" /* OS_Exit */
			:
			: [SWI_OS_Exit] "i" (OS_Exit)
			: "r14", "cc");
    }
}

/**
 * gdb backend callback.
 */
static int
session_break_cb (uintptr_t ctx)
{
  session_ctx *session = (session_ctx *) ctx;

  dprintf ("Current break state: %d\n", session->brk);

  /* Ignore any attempts to break while we're paused.  */
  if (session->brk == 1)
    return 0;

  if (session == session_get_current ())
    {
      dprintf ("Session is current");

      /* Inject bkpt, so debuggee breaks on resumption.  */
      if (session_set_bkpt_cb (ctx, session->cur_pc | BKPT_ONE_SHOT) == 0)
	return 0;
    }

  session->brk = 1;

  return 1;
}

/**
 * gdb backend callback.
 */
static int
session_continue_cb (uintptr_t ctx)
{
  session_ctx *session = (session_ctx *) ctx;

  session->brk = 0;

  return 1;
}

/**
 * gdb backend callback.
 */
static int
session_step_cb (uintptr_t ctx)
{
  session_ctx *session = (session_ctx *) ctx;
  uint32_t instruction = *(const uint32_t *) session->regs.detail.r[15];
  uint32_t next = step_instruction (instruction, &session->regs);

  /* Inject bkpt, so debuggee breaks on resumption.  */
  if (session_set_bkpt_cb (ctx, next | BKPT_ONE_SHOT) == 0)
    return 0;

  session->brk = 0;

  return 1;
}

/**
 * gdb backend callback.
 */
static cpu_registers *
session_get_regs_cb (uintptr_t ctx)
{
  session_ctx *session = (session_ctx *) ctx;

  return &session->regs;
}

/**
 * gdb backend callback.
 */
static int
session_set_bkpt_cb (uintptr_t ctx, uint32_t address)
{
  session_ctx *session = (session_ctx *) ctx;

  session_bkpt *bkpt;
  for (bkpt = session->bkpts; bkpt != NULL; bkpt = bkpt->next)
    {
      if ((bkpt->address & ~BKPT_ONE_SHOT) == (address & ~BKPT_ONE_SHOT))
	break;
    }
  /* There's already a breakpoint at this address */
  if (bkpt != NULL)
    return 1;

  if (session->free_bkpts != NULL)
    {
      bkpt = session->free_bkpts;
      session->free_bkpts = bkpt->next;
    }
  else
    {
      bkpt = malloc (sizeof (session_bkpt));
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
  int irq_state = irqs_off ();

  /* Save current instruction */
  bkpt->instruction = *(const uint32_t *)(address & ~BKPT_ONE_SHOT);

  /* Replace it with our breakpoint */
  *(uint32_t *)(address & ~BKPT_ONE_SHOT) = BKPT;

  /* Clean out the Icache */
  flush_icache ();

  /* And leave the critical section */
  irqs_restore (irq_state);

  return 1;
}

/**
 * gdb backend callback.
 */
static int
session_clear_bkpt_cb (uintptr_t ctx, uint32_t address)
{
  session_ctx *session = (session_ctx *) ctx;

  session_bkpt *bkpt;
  for (bkpt = session->bkpts; bkpt != NULL; bkpt = bkpt->next)
    {
      if ((bkpt->address & ~BKPT_ONE_SHOT) == (address & ~BKPT_ONE_SHOT))
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
  int irq_state = irqs_off ();

  /* Restore saved instruction */
  *(uint32_t *)(address & ~BKPT_ONE_SHOT) = bkpt->instruction;

  /* Clean out the Icache */
  flush_icache ();

  /* And leave the critical section */
  irqs_restore (irq_state);

  return 1;
}

static session_bkpt *
session_find_bkpt (session_ctx * ctx, uint32_t address)
{
  session_bkpt *bkpt;
  for (bkpt = ctx->bkpts; bkpt != NULL; bkpt = bkpt->next)
    {
      if ((bkpt->address & ~BKPT_ONE_SHOT) == (address & ~BKPT_ONE_SHOT))
	break;
    }

  return bkpt;
}

bool
session_is_connected (const session_ctx *ctx)
{
  if (ctx->in_use)
    {
      switch (ctx->type)
	{
	  case SESSION_TCP:
	    return ctx->data.tcp.client >= 0;
	}
    }

  return false;
}

void
session_change_environment (session_ctx *ctx, void *pw)
{
  ctx->pw = pw;

  const _kernel_oserror *e;
  e = _swix (OS_ChangeEnvironment, _INR (0, 3) | _OUTR (1, 3),
	     6, error_veneer, ctx, ctx->buffer,
	     &ctx->env_ctx[0].addr, &ctx->env_ctx[0].r12, &ctx->env_ctx[0].buf);
  if (e)
    dprintf ("session_change_environment() err 0x%x %s\n", e->errnum, e->errmess);

  e = _swix (OS_ChangeEnvironment, _INR (0, 3) | _OUTR (1, 3),
	     11, exit_veneer, ctx, ctx->buffer,
	     &ctx->env_ctx[1].addr, &ctx->env_ctx[1].r12, &ctx->env_ctx[1].buf);
  if (e)
    dprintf ("session_change_environment() err 0x%x %s\n", e->errnum, e->errmess);
  
  e = _swix (OS_ChangeEnvironment, _INR (0, 3) | _OUTR (1, 3),
	     16, upcall_veneer, ctx, ctx->buffer,
	     &ctx->env_ctx[2].addr, &ctx->env_ctx[2].r12, &ctx->env_ctx[2].buf);
  if (e)
    dprintf ("session_change_environment() err 0x%x %s\n", e->errnum, e->errmess);
}

void
session_restore_environment (session_ctx *ctx)
{
  /* Temporary make our session non-current as this puts
     changeenv_vector_handler() in pass through mode.  */
  session_ctx *current_ctx = session_get_current ();
  session_set_current (NULL);

  const _kernel_oserror *e;
  e = _swix (OS_ChangeEnvironment, _INR (0, 3),
	     6, ctx->env_ctx[0].addr, ctx->env_ctx[0].r12, ctx->env_ctx[0].buf);
  if (e)
    dprintf ("session_restore_environment() err 0x%x %s\n", e->errnum, e->errmess);
  ctx->env_ctx[0].addr = ctx->env_ctx[0].r12 = ctx->env_ctx[0].buf = 0;
  
  e = _swix (OS_ChangeEnvironment, _INR (0, 3),
	     11, ctx->env_ctx[1].addr, ctx->env_ctx[1].r12, ctx->env_ctx[1].buf);
  if (e)
    dprintf ("session_restore_environment() err 0x%x %s\n", e->errnum, e->errmess);
  ctx->env_ctx[1].addr = ctx->env_ctx[1].r12 = ctx->env_ctx[1].buf = 0;

  e = _swix (OS_ChangeEnvironment, _INR (0, 3),
	     16, ctx->env_ctx[2].addr, ctx->env_ctx[2].r12, ctx->env_ctx[2].buf);
  if (e)
    dprintf ("session_restore_environment() err 0x%x %s\n", e->errnum, e->errmess);
  ctx->env_ctx[2].addr = ctx->env_ctx[2].r12 = ctx->env_ctx[2].buf = 0;

  session_set_current (current_ctx);
}

/**
 * Called when an application changes its environment.
 * \return VECTOR_CLAIM to claim, VECTOR_PASSON to pass on the vector call.
 */
int
changeenv_vector_handler (_kernel_swi_regs *r,
			  void *pw __attribute__ ((unused)))
{
  session_ctx *ctx = session_get_current ();
  if (ctx == NULL)
    return VECTOR_PASSON;

  dprintf ("changeenv_vector_handler: r0 %d, r1 0x%x, r2 0x%x, r3 0x%x\n",
	   r->r[0], r->r[1], r->r[2], r->r[3]);

  /* Prevent the debugee overruling the error, exit and upcall environment as
     during debugging we want to be in full control.  */
  int idx;
  switch (r->r[0])
    {
      case 6: /* Error.  */
	idx = 0;
	break;
      case 11: /* Exit.  */
	idx = 1;
	break;
      case 16: /* Upcall.  */
	idx = 2;
	break;
      default:
	return VECTOR_PASSON;
    }

  const int prev_r1 = ctx->env_ctx[idx].addr;
  if (r->r[1] != 0)
    ctx->env_ctx[idx].addr = r->r[1];
  r->r[1] = prev_r1;

  const int prev_r2 = ctx->env_ctx[idx].r12;
  if (r->r[2] != 0)
    ctx->env_ctx[idx].r12 = r->r[2];
  r->r[2] = prev_r2;

  const int prev_r3 = ctx->env_ctx[idx].buf;
  if (r->r[3] != 0)
    ctx->env_ctx[idx].buf = r->r[3];
  r->r[3] = prev_r3;

  return VECTOR_CLAIM;
}

void
session_wimp_initialise (session_ctx *ctx)
{
  /* Important: This function (and those called by it), *must* not
     attempt to access non-automatic variables (i.e. globals) or call
     SCL functions.
     The magic words at the bottom of the SVC stack are not set up
     by our exception handlers so relocations will fail.  */

  /* Ignore this if we already have a task handle. */
  if (ctx->task_handle != 0)
    return;

  /* Add a callback to grab the task handle and register filters.
     It would appear that using _swix() here breaks things.  */
  __asm__ volatile ("MOV\tr0, %[callback]\n\t"
		    "MOV\tr1, %[pw]\n\t"
		    "SWI\t%[SWI_XOS_AddCallBack]\n\t"
		    : /* No outputs */
		    : [callback] "r" (post_wimp_initialise),
		      [pw] "r" (ctx->pw),
		      [SWI_XOS_AddCallBack] "i" (OS_AddCallBack | (1<<17))
		    : "r0", "r1", "r14", "cc", "memory");
}

_kernel_oserror *
post_wimp_initialise_handler (_kernel_swi_regs *r __attribute__ ((unused)),
			      void *pw)
{
  session_ctx *ctx = session_get_current ();

  /* Read task handle from Wimp */
  uint32_t task;
  _kernel_oserror *error = _swix (Wimp_ReadSysInfo, _IN (0) | _OUT (0), 5, &task);
  if (error != NULL)
    dprintf ("Wimp_ReadSysInfo: 0x%x %s\n", error->errnum, error->errmess);

  dprintf ("Task handle: 0x%0x\n", task);

  if (task == 0)
    return NULL;

  /* Install pre filter to invalidate current_session. */
  error = _swix (Filter_RegisterPreFilter, _INR (0, 3),
		 session_filter_name, session_pre_poll, pw, task);
  if (error != NULL)
    dprintf ("Filter_RegisterPreFilter: 0x%x %s\n",
	     error->errnum, error->errmess);

  /* Install post poll filter so that:
   *
   * 1) we do the right thing when the client tries to interrupt 
   *    the debuggee.
   * 2) we keep current_session in sync with reality.
   */
  error = _swix (Filter_RegisterPostFilter, _INR (0, 4),
		 session_filter_name, session_post_poll, pw, task, 0);
  if (error != NULL)
    dprintf ("XFilter_RegisterPostFilter: 0x%x %s\n",
	     error->errnum, error->errmess);

  /* Helpfully, our filters will only ever be passed the bottom N bits of
   * the task handle. I'm going to assume that N=16. If it doesn't, fix 
   * the OS already. */
  ctx->task_handle = task & 0xffff;

  return NULL;
}

_kernel_oserror *
session_pre_poll_handler (_kernel_swi_regs *r __attribute__ ((unused)),
			  void *pw __attribute__ ((unused)))
{
  /* Invalidate current_session, as we're about to leave it.  */
  session_set_current (NULL);

  return NULL;
}

_kernel_oserror *
session_post_poll_handler (_kernel_swi_regs *r __attribute__ ((unused)),
			   void *pw __attribute__ ((unused)))
{
  session_ctx *session = session_find_by_task_handle (r->r[2]);

  dprintf ("Post poll: 0x%x (%p)\n", r->r[2], (void *) session);

  if (session != NULL)
    {
      session_set_current (session);

      /* On reentry into the debuggee through this post filter, 
       * we need to ensure that the PC resides in application space 
       * before acting on any pending break. Our SWI vector handler 
       * will have stored the return address from Wimp_Poll into 
       * the session. Therefore, we need to consider the session's 
       * break flag and, if set, set a breakpoint on the return 
       * address. The normal breakpoint handling code will take over
       * from there.
       */

      if (session->brk)
	{
	  dprintf ("Setting breakpoint at 0x%x\n", session->cur_pc);
	  session_set_bkpt_cb ((uintptr_t) session,
			       session->cur_pc | BKPT_ONE_SHOT);
	}
    }

  return NULL;
}

void
session_get_error (session_ctx * ctx, _kernel_oserror * error)
{
  memcpy (error, ctx->buffer + 4, sizeof (_kernel_oserror));
}

/* ------------------------------------------------------------------------ */

/**
 * \return 0 to claim Internet event, non-0 to pass on.
 */
int
internet_event_handler (_kernel_swi_regs *r, void *pw __attribute__ ((unused)))
{
  switch (r->r[1])
    {
    case 1: /* Input pending */
      {
	/* Work out what socket this is.  */
	session_ctx *session = session_tcp_find_by_socket (r->r[2]);
	if (session != NULL)
	  {
	    /* dprintf ("Input on %d\n", r->r[2]); */
	    return session_tcp_process_input (session, r->r[2]);
	  }
      }
      break;

    case 2: /* OOB data pending */
      /* Ignore this */
      dprintf ("OOB on %d\n", r->r[2]);
      break;

    case 3: /* Socket closed */
      {
	/* If it's one of our clients, then tidy up.  */
	session_ctx *session = session_tcp_find_by_socket (r->r[2]);
	dprintf ("Socket %d closed...\n", r->r[2]);
	if (session != NULL && r->r[2] == session->data.tcp.client)
	  {
	    dprintf ("...matching one of our sessions.\n");
	    session_tcp_notify_closed (session);
	    return 0;
	  }
      }
      break;
    }

  return 1;
}

/**
 * \param rosocket raw RISC OS socket number, not what you get with socket().
 * \return 0 to indicated we did some processing, non-0 to indicate
 * non-processing.
 */
static int
session_tcp_process_input (session_ctx *session, int rosocket)
{
  if (session->type != SESSION_TCP)
    return 1;

  if (rosocket == __get_ro_socket (session->data.tcp.server))
    {
      /* If it's a server socket, accept the connection and
	 begin a new debug session with the client.  */
      if (session->data.tcp.client >= 0)
	return 0;

      session->data.tcp.client = socket_accept (session->data.tcp.server);
      if (session->data.tcp.client == -1)
	return 0;

      dprintf ("New client on %d\n", session->data.tcp.client);

      session->gdb = gdb_ctx_create (session_tcp_send_for_gdb,
				     session_break_cb, session_continue_cb,
				     session_get_regs_cb, session_set_bkpt_cb,
				     session_clear_bkpt_cb, session_step_cb,
				     (uintptr_t) session);
      if (session->gdb == NULL)
	return 0;
    }
  else if (rosocket != __get_ro_socket (session->data.tcp.client))
    {
      dprintf ("session_tcp_process_input(): assert failed - socket mismatch\n");
      return 1;
    }

  /* If there's data on the socket, drive state machine.  */
  static uint8_t buf[1024];
  ssize_t read;
  while ((read = socket_recv (session->data.tcp.client, buf, sizeof (buf))) > 0)
    {
      dprintf ("-> %.*s\n", (int) read, (const char *)buf);

      gdb_process_input (session->gdb, buf, read);
    }

  return 0;
}

static void
session_tcp_notify_closed (session_ctx *session)
{
  dprintf ("Client %d disconnected\n", session->data.tcp.client);

  socket_close (session->data.tcp.client);
  session->data.tcp.client = -1;
}

static session_ctx *
session_tcp_initialise (session_ctx *session)
{
#define TCP_BASE_PORT 4900

  session->data.tcp.client = -1;
  session->data.tcp.server = socket_open_server (TCP_BASE_PORT + session - sessions);
  if (session->data.tcp.server == -1)
    {
      session_ctx_destroy (session);
      return NULL;
    }

  dprintf ("Listening on %ld (%d)\n", TCP_BASE_PORT + session - sessions,
	   session->data.tcp.server);

  return session;
}

static void
session_tcp_finalise (session_ctx *session)
{
  if (session->data.tcp.server >= 0)
    {
      socket_close (session->data.tcp.server);
      session->data.tcp.server = -1;
    }

  if (session->data.tcp.client >= 0)
    {
      socket_close (session->data.tcp.client);
      session->data.tcp.client = -1;
    }
}

static size_t
session_tcp_send_for_gdb (uintptr_t ctx, const uint8_t *data, size_t len)
{
  session_ctx *session = (session_ctx *) ctx;

  if (session->data.tcp.client < 0)
    return 0;

  dprintf ("<- %.*s\n", (int) len, (const char *)data);

  for (size_t towrite = len;
       towrite;
       towrite = socket_send (session->data.tcp.client,
			      data + len - towrite, towrite))
    /* */;

  return 0;
}

static session_ctx *
session_tcp_find_by_socket (int rosocket)
{
  /* dprintf ("Want RISC OS socket %d\n", rosocket); */

  int i;
  for (i = 0; i < MAX_SESSIONS; i++)
    {
      /* dprintf ("%d: %d: %d: %d\n", i, sessions[i].in_use,
	       sessions[i].data.tcp.server, sessions[i].data.tcp.client); */

      if (sessions[i].in_use
          && sessions[i].type == SESSION_TCP
	  && (__get_ro_socket (sessions[i].data.tcp.server) == rosocket
	      || __get_ro_socket (sessions[i].data.tcp.client) == rosocket))
	break;
    }
  if (i == MAX_SESSIONS)
    return NULL;

  return &sessions[i];
}
