#ifndef src_gdb_h_
#define src_gdb_h_

#include <stdint.h>

typedef struct gdb_ctx gdb_ctx;

typedef size_t (*gdb_send_cb)(uintptr_t ctx, const uint8_t *data, size_t len);
typedef int (*gdb_break_cb)(uintptr_t ctx);
typedef int (*gdb_continue_cb)(uintptr_t ctx);
typedef uint32_t *(*gdb_get_regs_cb)(uintptr_t ctx, size_t *n_regs);
typedef int (*gdb_set_bkpt_cb)(uintptr_t ctx, uint32_t address);
typedef int (*gdb_clear_bkpt_cb)(uintptr_t ctx, uint32_t address);

gdb_ctx *gdb_ctx_create(gdb_send_cb send, gdb_break_cb brk, 
		gdb_continue_cb cont, gdb_get_regs_cb get_regs,
		gdb_set_bkpt_cb set_bkpt, gdb_clear_bkpt_cb clear_bkpt,
		uintptr_t pw);
void gdb_ctx_destroy(gdb_ctx *ctx);

void gdb_process_input(gdb_ctx *ctx, const uint8_t *data, size_t len);

#endif
