/*-
 * Copyright (c) 2012 Kai Wang
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "ld.h"
#include "ld_arch.h"
#include "ld_input.h"
#include "ld_output.h"
#include "ld_reloc.h"
#include "ld_utils.h"
#include "amd64.h"

ELFTC_VCSID("$Id: amd64.c 2514 2012-06-02 23:22:15Z kaiwang27 $");

static uint64_t _get_max_page_size(struct ld *ld);
static uint64_t _get_common_page_size(struct ld *ld);
static void _process_reloc(struct ld *ld, struct ld_input_section *is,
    struct ld_reloc_entry *lre, uint64_t s, uint8_t *buf);

static uint64_t
_get_max_page_size(struct ld *ld)
{

	(void) ld;
	return (0x200000);
}

static uint64_t
_get_common_page_size(struct ld *ld)
{

	(void) ld;
	return (0x1000);
}

static void
_process_reloc(struct ld *ld, struct ld_input_section *is,
    struct ld_reloc_entry *lre, uint64_t s, uint8_t *buf)
{
	struct ld_output *lo;
	uint64_t u64;
	int64_t s64;
	uint32_t u32;
	int32_t s32;
	uint64_t p;

	lo = ld->ld_output;
	assert(lo != NULL);

	p = lre->lre_offset + is->is_output->os_addr + is->is_reloff;

	switch (lre->lre_type) {
	case R_X86_64_NONE:
		break;
	case R_X86_64_64:
		WRITE_64(buf + lre->lre_offset, s + lre->lre_addend);
		break;
	case R_X86_64_PC32:
		s32 = s + lre->lre_addend - p;
		WRITE_32(buf + lre->lre_offset, s32);
		break;
	case R_X86_64_32:
		u64 = s + lre->lre_addend;
		u32 = u64 & 0xffffffff;
		if (u64 != u32)
			ld_fatal(ld, "R_X86_64_32 relocation failed");
		WRITE_32(buf + lre->lre_offset, u32);
		break;
	case R_X86_64_32S:
		s64 = s + lre->lre_addend;
		s32 = s64 & 0xffffffff;
		if (s64 != s32)
			ld_fatal(ld, "R_X86_64_32S relocation failed");
		WRITE_32(buf + lre->lre_offset, s32);
		break;
	default:
		ld_fatal(ld, "Relocation %d not supported", lre->lre_type);
		break;
	}
}

void
amd64_register(struct ld *ld)
{
	struct ld_arch *amd64, *amd64_alt;

	if ((amd64 = calloc(1, sizeof(*amd64))) == NULL)
		ld_fatal_std(ld, "calloc");

	snprintf(amd64->name, sizeof(amd64->name), "%s", "amd64");

	amd64->script = amd64_script;
	amd64->get_max_page_size = _get_max_page_size;
	amd64->get_common_page_size = _get_common_page_size;
	amd64->process_reloc = _process_reloc;

	HASH_ADD_STR(ld->ld_arch_list, name, amd64);

	if ((amd64_alt = calloc(1, sizeof(*amd64_alt))) == NULL)
		ld_fatal_std(ld, "calloc");
	memcpy(amd64_alt, amd64, sizeof(struct ld_arch));
	amd64_alt->alias = amd64;
	snprintf(amd64_alt->name, sizeof(amd64_alt->name), "%s", "x86-64");

	HASH_ADD_STR(ld->ld_arch_list, name, amd64_alt);
}
