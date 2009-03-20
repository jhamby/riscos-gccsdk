#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asmutils.h"
#include "gdb.h"

typedef enum gdb_state {
	WAITING_FOR_PACKET,
	READ_PACKET_DATA,
	READ_CHECKSUM1,
	READ_CHECKSUM2,
} gdb_state;

struct gdb_ctx {
	gdb_state state;

	uint8_t first;

	uint8_t data_buf[256];
	uint8_t data_len;
	uint8_t checksum;
	uint8_t refcksum;

	uint8_t last_packet[256];
	uint8_t last_packet_len;

	gdb_send_cb send;
	gdb_break_cb brk;
	gdb_continue_cb cont;
	gdb_get_regs_cb get_regs;
	gdb_set_bkpt_cb set_bkpt;
	gdb_clear_bkpt_cb clear_bkpt;
	gdb_step_cb step;
	uintptr_t pw;
};

#define N_CTX 5
static gdb_ctx ctxs[N_CTX];

static void process_packet(gdb_ctx *ctx);
static void send_packet(gdb_ctx *ctx, const uint8_t *buf, size_t len);

static const char hexDigits[] = "0123456789abcdef";

static int hexToInt(uint8_t c)
{
	if ('0' <= c && c <= '9')
		return c - '0';
	else if ('A' <= (c & ~0x20) && (c & ~0x20) <= 'F')
		return 10 + (c & ~0x20) - 'A';

	return 0;
}

gdb_ctx *gdb_ctx_create(gdb_send_cb send, gdb_break_cb brk, 
		gdb_continue_cb cont, gdb_get_regs_cb get_regs,
		gdb_set_bkpt_cb set_bkpt, gdb_clear_bkpt_cb clear_bkpt,
		gdb_step_cb step,
		uintptr_t pw)
{
	int i;
	gdb_ctx *ctx;

	for (i = 0; i < N_CTX; i++) {
		if (ctxs[i].send == NULL)
			break;
	}
	if (i == N_CTX)
		return NULL;

	ctx = &ctxs[i];

	ctx->state = WAITING_FOR_PACKET;

	ctx->first = 1;

	ctx->send = send;
	ctx->brk = brk;
	ctx->cont = cont;
	ctx->get_regs = get_regs;
	ctx->set_bkpt = set_bkpt;
	ctx->clear_bkpt = clear_bkpt;
	ctx->step = step;
	ctx->pw = pw;

	return ctx;
}

void gdb_ctx_destroy(gdb_ctx *ctx)
{
	/** \todo Need to tell debugger that we're disappearing */

	memset(ctx, 0, sizeof(gdb_ctx));
}

void gdb_process_input(gdb_ctx *ctx, const uint8_t *data, size_t len)
{
	while (len > 0) {
		switch (ctx->state) {
		case WAITING_FOR_PACKET:
			switch (data[0]) {
			case 3:
				/* Tell client to interrupt application */
				ctx->brk(ctx->pw);
				break;
			case '$':
				ctx->checksum = 0;
				ctx->data_len = 0;
				ctx->state = READ_PACKET_DATA;
				break;
			default:
				break;
			}

			break;
		case READ_PACKET_DATA:
			if (data[0] == '#') {
				ctx->state = READ_CHECKSUM1;
			} else {
				ctx->checksum += data[0];
				ctx->data_buf[ctx->data_len++] = data[0];
			}

			break;
		case READ_CHECKSUM1:
			ctx->refcksum = hexToInt(data[0]) << 4;
			ctx->state = READ_CHECKSUM2;

			break;
		case READ_CHECKSUM2:
			ctx->refcksum += hexToInt(data[0]);

			/* Ensure the buffer is NUL terminated */
			ctx->data_buf[ctx->data_len] = '\0';

			/** \todo fix this properly */
			/* Nasty hack as, for some reason, gdb sends us
			 * the first packet twice. */
			if (ctx->first == 0) {
				if (ctx->checksum != ctx->refcksum) {
					ctx->send(ctx->pw, 
						(const uint8_t *) "-", 1);
				} else {
					ctx->send(ctx->pw, 
						(const uint8_t *) "+", 1);

					process_packet(ctx);
				}
			} else {
				ctx->first = 0;
			}

			ctx->state = WAITING_FOR_PACKET;

			break;
		}

		data++;
		len--;
	}
}

void process_packet(gdb_ctx *ctx)
{
	uint8_t buf[256];
	uint8_t *p = buf;

	/* Empty packet */
	if (ctx->data_len == 0)
		return;

	switch (ctx->data_buf[0]) {
	case '?':
		/** \todo Make sure we return the correct signal number */
		send_packet(ctx, (const uint8_t *) "S05", 3);
		break;
	case 'c':
		/** \todo parse addr, if any (and update PC) */
		ctx->cont(ctx->pw);
		break;
	case 'g':
	{
		const cpu_registers *regs = ctx->get_regs(ctx->pw);

		for (size_t i = 0; i < N_REGS; i++) {
			uint32_t val = regs->r[i];

			for (int j = 0; j < 4; j++) {
				uint8_t b = (val >> (8 * j)) & 0xff;

				*p++ = hexDigits[b >> 4];
				*p++ = hexDigits[b & 0xf];
			}
		}

		for (int j = 0; j < 4; j++) {
			uint8_t b = (regs->cpsr >> (8 * j)) & 0xff;

			*p++ = hexDigits[b >> 4];
			*p++ = hexDigits[b & 0xf];
		}

		send_packet(ctx, buf, p - buf);
	}	
		break;
	case 'G':
	{
		cpu_registers *regs = ctx->get_regs(ctx->pw);

		p = &ctx->data_buf[1];

		for (size_t i = 0; i < N_REGS && 
				p - ctx->data_buf <= ctx->data_len - 8; i++) {
			regs->r[i] = 0;

			for (int j = 0; j < 4; j++) {
				uint8_t b;

				b = hexToInt(*p++) << 4;
				b |= hexToInt(*p++);

				regs->r[i] |= b << (8 * j);
			}
		}

		if (p - ctx->data_buf <= ctx->data_len - 8) {
			regs->cpsr = 0;

			for (int j = 0; j < 4; j++) {
				uint8_t b;

				b = hexToInt(*p++) << 4;
				b |= hexToInt(*p++);

				regs->cpsr |= b << (8 * j);
			}
		}

		send_packet(ctx, (const uint8_t *) "OK", 2);
	}
		break;
	case 'H':
	{
		uint8_t type = ctx->data_buf[1];
		int thread = strtol((char *) &ctx->data_buf[2], NULL, 16);

		if ((thread == -1 || thread == 0) &&
				(type == 'c' || type == 'g'))
			send_packet(ctx, (const uint8_t *) "OK", 2);
		else
			send_packet(ctx, (const uint8_t *) "E00", 3);
	}
		break;
	case 'm':
	{
		char *comma;
		uint32_t addr = strtoul((char *) &ctx->data_buf[1], 
				&comma, 16);
		uint32_t length = strtoul(comma + 1, NULL, 16);

		while (length-- > 0) {
			/** \todo This may abort. */
			uint8_t b = *((uint8_t *) addr);

			*p++ = hexDigits[b >> 4];
			*p++ = hexDigits[b & 0xf];

			addr++;
		}

		send_packet(ctx, buf, p - buf);
	}
		break;
	case 'M':
	{
		char *comma;
		uint32_t addr = strtoul((char *) &ctx->data_buf[1],
				&comma, 16);
		uint32_t length = strtoul(comma + 1, &comma, 16);

		p = (uint8_t *) comma + 1;

		while (length-- > 0 && 
				p - ctx->data_buf <= ctx->data_len - 2) {
			uint8_t b = 0;

			for (int i = 0; i < 2; i++) {
				b = b << 4 | hexToInt(*p++);
			}

			/** \todo This may abort. */
			*((uint8_t *) addr++) = b;
		}

		flush_caches();

		send_packet(ctx, (const uint8_t *) "OK", 2);
	}
		break;
	case 'z':
	case 'Z':
	{
		char *comma;
		uint8_t type = ctx->data_buf[1];
		uint32_t addr = strtoul((char *) &ctx->data_buf[3], 
				&comma, 16);
		uint32_t length = strtoul(comma + 1, NULL, 16);
		int success;

		/* We only support type 0 and ARM instructions */
		if (type != '0' || length != 4) {
			send_packet(ctx, (const uint8_t *) "", 0);
			break;
		}

		if (ctx->data_buf[0] == 'z')
			success = ctx->clear_bkpt(ctx->pw, addr);
		else
			success = ctx->set_bkpt(ctx->pw, addr);

		if (success == 0)
			send_packet(ctx, (const uint8_t *) "E00", 3);
		else
			send_packet(ctx, (const uint8_t *) "OK", 2);
	}
		break;
	case 's':
		/** \todo parse addr, if any (and update PC) */
		ctx->step(ctx->pw);
		break;
	default:
		printf("Unsupported packet '%.*s'\n", 
				ctx->data_len, (char *) ctx->data_buf);

		send_packet(ctx, (const uint8_t *) "", 0);
		break;
	}
}

void send_packet(gdb_ctx *ctx, const uint8_t *buf, size_t len)
{
	const uint8_t *p;
	uint8_t cksum = 0;

	for (p = buf; p < buf + len; p++)
		cksum += *p;

	ctx->last_packet[0] = '$';
	if (len > 0)
		memcpy(&ctx->last_packet[1], buf, len);
	ctx->last_packet[len + 1] = '#';
	ctx->last_packet[len + 2] = hexDigits[cksum >> 4];
	ctx->last_packet[len + 3] = hexDigits[cksum & 0xf];

	ctx->last_packet_len = len + 4;

	ctx->send(ctx->pw, ctx->last_packet, ctx->last_packet_len);
}

