#ifndef src_session_h_
#define srd_session_h_

#include <stdbool.h>

typedef enum session_type
{
  SESSION_TCP,
} session_type;

typedef struct session_ctx session_ctx;

void session_fini (void);

session_ctx *session_ctx_create (session_type type);
void session_ctx_destroy (session_ctx * ctx);

void session_handle_break (session_ctx * ctx, int reason);
void session_wait_for_continue (session_ctx * ctx);

bool session_is_connected (const session_ctx *ctx);

void session_change_environment (session_ctx * ctx, void *pw);
void session_restore_environment (session_ctx * ctx);

void session_wimp_initialise (session_ctx * ctx);

void session_get_error (session_ctx * ctx, _kernel_oserror * error);

session_ctx *session_find_by_socket (int socket);

int session_tcp_process_input (session_ctx * ctx, int socket);
void session_tcp_notify_closed (session_ctx * ctx, int socket);

/* Assembler routines */

session_ctx *session_get_current (void);
void session_set_current (session_ctx * ctx);

void swi_handler (void);
uint32_t get_prev_swi_handler (void);
void set_prev_swi_handler (uint32_t addr);
void irq_handler (void);
uint32_t get_prev_irq_handler (void);
void set_prev_irq_handler (uint32_t addr);
void dabort_handler (void);
uint32_t get_prev_dabort_handler (void);
void set_prev_dabort_handler (uint32_t addr);
void prefetch_handler (void);
uint32_t get_prev_prefetch_handler (void);
void set_prev_prefetch_handler (uint32_t addr);
void undef_handler (void);
uint32_t get_prev_undef_handler (void);
void set_prev_undef_handler (uint32_t addr);

void error_veneer (void);
void exit_veneer (void);
void upcall_veneer (void);

#endif
