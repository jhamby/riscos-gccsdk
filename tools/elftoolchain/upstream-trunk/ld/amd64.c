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
#include "ld_layout.h"
#include "ld_output.h"
#include "ld_reloc.h"
#include "ld_symbols.h"
#include "ld_utils.h"
#include "amd64.h"

ELFTC_VCSID("$Id: amd64.c 2514 2012-06-02 23:22:15Z kaiwang27 $");

static uint64_t _get_max_page_size(struct ld *ld);
static uint64_t _get_common_page_size(struct ld *ld);
static void _process_reloc(struct ld *ld, struct ld_input_section *is,
    struct ld_reloc_entry *lre, struct ld_symbol *lsb, uint8_t *buf);

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
    struct ld_reloc_entry *lre, struct ld_symbol *lsb, uint8_t *buf)
{
	struct ld_output *lo;
	uint64_t u64, s;
	int64_t s64;
	uint32_t u32;
	int32_t s32;
	uint64_t p;

	lo = ld->ld_output;
	assert(lo != NULL);

	s = lsb->lsb_value;
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

static void
_create_dynrel(struct ld *ld)
{
	struct ld_output *lo;
	struct ld_output_section *os;
	struct ld_output_data_buffer *odb;
	char dynrel_name[] = ".rela.dyn";

	lo = ld->ld_output;
	assert(lo != NULL);

	HASH_FIND_STR(lo->lo_ostbl, dynrel_name, os);
	if (os == NULL)
		os = ld_layout_insert_output_section(ld, dynrel_name,
		    SHF_ALLOC);
	os->os_type = SHT_RELA;
	os->os_align = 8;
	os->os_entsize = sizeof(Elf64_Rela);
	os->os_flags = SHF_ALLOC;
	if ((os->os_link_name = strdup(".dynsym")) == NULL)
		ld_fatal_std(ld, "strdup");
	lo->lo_rel_dyn = os;

	if ((odb = calloc(1, sizeof(*odb))) == NULL)
		ld_fatal_std(ld, "calloc");
	odb->odb_size = ld->ld_num_copy_reloc * sizeof(Elf64_Rela);
	if ((odb->odb_buf = malloc(odb->odb_size)) == NULL)
		ld_fatal_std(ld, "malloc");
	odb->odb_align = os->os_align;
	odb->odb_type = ELF_T_RELA;
	lo->lo_rel_dyn_odb = odb;
	lo->lo_rel_dyn_type = DT_RELA;

	(void) ld_output_create_element(ld, &os->os_e, OET_DATA_BUFFER, odb,
	    NULL);
}

static void
_finalize_dynrel(struct ld *ld)
{
	struct ld_output *lo;
	struct ld_output_data_buffer *odb;
	struct ld_symbol *lsb, *tmp;
	Elf64_Rela *r;
	int i;

	lo = ld->ld_output;
	assert(lo != NULL);

	if (lo->lo_rel_dyn == NULL || lo->lo_rel_dyn_odb == NULL)
		return;

	odb = lo->lo_rel_dyn_odb;

	r = (Elf64_Rela *) (uintptr_t) odb->odb_buf;

	i = 0;
	HASH_ITER(hhimp, ld->ld_symtab_import, lsb, tmp) {
		if (!lsb->lsb_copy_reloc)
			continue;

		r[i].r_offset = lsb->lsb_value;
		r[i].r_info = ELF64_R_INFO(lsb->lsb_dyn_index, R_X86_64_COPY);
		r[i].r_addend = 0;
		i++;
	}

	assert((unsigned) i == ld->ld_num_copy_reloc);
}

static void
_create_pltgot(struct ld *ld)
{
	struct ld_output *lo;
	struct ld_output_section *os;
	struct ld_output_data_buffer *got_odb, *plt_odb, *rela_plt_odb;
	struct ld_symbol *lsb, *_lsb;
	char plt_name[] = ".plt";
	char got_name[] = ".got";
	char rela_plt_name[] = ".rela.plt";
	int func_cnt;

	if (HASH_CNT(hhimp, ld->ld_symtab_import) == 0)
		return;

	lo = ld->ld_output;
	assert(lo != NULL);

	/*
	 * Create GOT section.
	 */

	HASH_FIND_STR(lo->lo_ostbl, got_name, os);
	if (os == NULL)
		os = ld_layout_insert_output_section(ld, got_name,
		    SHF_ALLOC | SHF_WRITE);
	os->os_type = SHT_PROGBITS;
	os->os_align = 8;
	os->os_entsize = 8;
	os->os_flags = SHF_ALLOC | SHF_WRITE;
	lo->lo_got = os;

	if ((got_odb = calloc(1, sizeof(*got_odb))) == NULL)
		ld_fatal_std(ld, "calloc");
	got_odb->odb_size = (ld->ld_num_import_func + 3) * 8;
	if ((got_odb->odb_buf = malloc(got_odb->odb_size)) == NULL)
		ld_fatal_std(ld, "malloc");
	got_odb->odb_align = 8;
	got_odb->odb_type = ELF_T_BYTE;
	lo->lo_got_odb = got_odb;

	(void) ld_output_create_element(ld, &os->os_e, OET_DATA_BUFFER,
	    got_odb, NULL);

	/*
	 * Create PLT section.
	 */

	func_cnt = 0;
	HASH_ITER(hhimp, ld->ld_symtab_import, lsb, _lsb) {
		if (lsb->lsb_type == STT_FUNC)
			func_cnt++;
	}

	if (func_cnt == 0)
		return;

	HASH_FIND_STR(lo->lo_ostbl, plt_name, os);
	if (os == NULL)
		os = ld_layout_insert_output_section(ld, plt_name,
		    SHF_ALLOC | SHF_EXECINSTR);
	os->os_type = SHT_PROGBITS;
	os->os_align = 4;
	os->os_entsize = 16;
	os->os_flags = SHF_ALLOC | SHF_EXECINSTR;
	lo->lo_plt = os;

	if ((plt_odb = calloc(1, sizeof(*plt_odb))) == NULL)
		ld_fatal_std(ld, "calloc");
	plt_odb->odb_size = (func_cnt + 1) * 16;
	if ((plt_odb->odb_buf = malloc(plt_odb->odb_size)) == NULL)
		ld_fatal_std(ld, "malloc");
	plt_odb->odb_align = 1;
	plt_odb->odb_type = ELF_T_BYTE;
	lo->lo_plt_odb = plt_odb;

	(void) ld_output_create_element(ld, &os->os_e, OET_DATA_BUFFER,
	    plt_odb, NULL);

	/* Create _GLOBAL_OFFSET_TABLE_ symbol. */
	ld_symbols_add_internal(ld, "_GLOBAL_OFFSET_TABLE_", 0, 0, SHN_ABS,
	    STB_LOCAL, STT_OBJECT, STV_HIDDEN, lo->lo_got);

	/*
	 * Create a relocation section for the PLT section.
	 */

	HASH_FIND_STR(lo->lo_ostbl, rela_plt_name, os);
	if (os == NULL)
		os = ld_layout_insert_output_section(ld, rela_plt_name,
		    SHF_ALLOC);
	os->os_type = SHT_RELA;
	os->os_align = 8;
	os->os_entsize = sizeof(Elf64_Rela);
	os->os_flags = SHF_ALLOC;
	if ((os->os_link_name = strdup(".dynsym")) == NULL)
		ld_fatal_std(ld, "strdup");
	os->os_info = lo->lo_plt;
	lo->lo_rel_plt = os;
	lo->lo_rel_plt_type = DT_RELA;

	if ((rela_plt_odb = calloc(1, sizeof(*rela_plt_odb))) == NULL)
		ld_fatal_std(ld, "calloc");
	rela_plt_odb->odb_size = func_cnt * os->os_entsize;
	if ((rela_plt_odb->odb_buf = malloc(rela_plt_odb->odb_size)) == NULL)
		ld_fatal_std(ld, "malloc");
	rela_plt_odb->odb_align = os->os_align;
	rela_plt_odb->odb_type = ELF_T_RELA;
	lo->lo_rel_plt_odb = rela_plt_odb;

	(void) ld_output_create_element(ld, &os->os_e, OET_DATA_BUFFER,
	    rela_plt_odb, NULL);
}

static void
_finalize_pltgot(struct ld *ld)
{
	struct ld_output *lo;
	struct ld_symbol *lsb, *_lsb;
	Elf64_Rela *r;
	uint8_t *got, *plt;
	uint64_t u64;
	int32_t s32, pltgot, gotpcrel;
	int i, j;

	lo = ld->ld_output;
	assert(lo != NULL);

	if (lo->lo_got_odb == NULL || lo->lo_got_odb->odb_size == 0)
		return;

	got = lo->lo_got_odb->odb_buf;

	plt = NULL;
	if (lo->lo_plt_odb != NULL && lo->lo_plt_odb->odb_size != 0) {
		plt = lo->lo_plt_odb->odb_buf;
		r = (Elf64_Rela *) (uintptr_t) lo->lo_rel_plt_odb->odb_buf;
	}

	/* TODO: fill in _DYNAMIC in the first got entry. */
	got += 8;

	/* Reserve the second and the third entries for the dynamic linker. */
	got += 16;

	/*
	 * Write the first PLT entry.
	 */
	if (plt != NULL) {
		/*
		 * Calculate the relative offset from PLT to GOT.
		 */
		pltgot = lo->lo_got->os_addr - lo->lo_plt->os_addr;

		/*
		 * Push the second GOT entry to the stack for the dynamic
		 * linker. (PUSH reg/memXX [RIP+disp32]) (6 bytes for push)
		 */
		WRITE_8(plt, 0xff);
		WRITE_8(plt + 1, 0x35);
		s32 = pltgot - 6 + 8;
		WRITE_32(plt + 2, s32);
		plt += 6;

		/*
		 * Jump to the address in the third GOT entry (call into
		 * the dynamic linker). (JMP reg/memXX [RIP+disp32])
		 * (6 bytes for jmp)
		 */
		WRITE_8(plt, 0xff);
		WRITE_8(plt + 1, 0x25);
		s32 = pltgot - 12 + 16;
		WRITE_32(plt + 2, s32);
		plt += 6;

		/* Padding: 4-byte nop. (NOP [rAx+disp8]) */
		WRITE_8(plt, 0x0f);
		WRITE_8(plt + 1, 0x1f);
		WRITE_8(plt + 2, 0x40);
		WRITE_8(plt + 3, 0x0);
		plt += 4;
	} else
		return;

	/*
	 * Write remaining GOT and PLT entries and create dynamic relocation
	 * entries for PLT entries.
	 */
	i = 3;
	j = 0;
	HASH_ITER(hhimp, ld->ld_symtab_import, lsb, _lsb) {
		if (lsb->lsb_type != STT_FUNC)
			continue;

		/*
		 * Create a R_X86_64_JUMP_SLOT relocation entry for the
		 * PLT slot.
		 */
		r[j].r_offset = lo->lo_got->os_addr + i * 8;
		r[j].r_info = ELF64_R_INFO(lsb->lsb_dyn_index,
		    R_X86_64_JUMP_SLOT);
		r[j].r_addend = 0;

		/*
		 * Set the value of the import symbol to point to the PLT
		 * slot.
		 */
		lsb->lsb_value = lo->lo_plt->os_addr + (i - 2) * 16;

		/*
		 * Calculate the IP-relative offset to the GOT entry for
		 * this function. (6 bytes for jmp)
		 */
		gotpcrel = pltgot + i * 8 - (i - 2) * 16 - 6;

		/*
		 * PLT: Jump to the address in the GOT entry for this function.
		 * (JMP reg/memXX [RIP+disp32])
		 */
		WRITE_8(plt, 0xff);
		WRITE_8(plt + 1, 0x25);
		WRITE_32(plt + 2, gotpcrel);
		plt += 6;

		/*
		 * PLT: Symbol is not resolved, push the relocation index to
		 * the stack. (PUSH imm32)
		 */
		WRITE_8(plt, 0x68);
		WRITE_32(plt + 1, j);
		plt += 5;

		/*
		 * PLT: Jump to the first PLT entry, eventually call the
		 * dynamic linker. (JMP rel32off)
		 */
		WRITE_8(plt, 0xe9);
		s32 = - (i - 1) * 16;
		WRITE_32(plt + 1, s32);
		plt += 5;

		/*
		 * GOT: Write the GOT entry for this function, pointing to the
		 * push op.
		 */
		u64 = lo->lo_plt->os_addr + (i - 2) * 16 + 6;
		WRITE_64(got, u64);

		/* Increase relocation entry index. */
		j++;

		/* Move to next GOT entry. */
		got += 8;
		i++;
	}

	assert(got == lo->lo_got_odb->odb_buf + lo->lo_got_odb->odb_size);
	assert(plt == lo->lo_plt_odb->odb_buf + lo->lo_plt_odb->odb_size);
	assert((size_t)j == lo->lo_rel_plt_odb->odb_size / sizeof(Elf64_Rela));
}

void
amd64_register(struct ld *ld)
{
	struct ld_arch *amd64, *amd64_alt;

	if ((amd64 = calloc(1, sizeof(*amd64))) == NULL)
		ld_fatal_std(ld, "calloc");

	snprintf(amd64->name, sizeof(amd64->name), "%s", "amd64");

	amd64->script = amd64_script;
	amd64->interp = "/libexec/ld-elf.so.1";
	amd64->get_max_page_size = _get_max_page_size;
	amd64->get_common_page_size = _get_common_page_size;
	amd64->process_reloc = _process_reloc;
	amd64->create_dynrel = _create_dynrel;
	amd64->finalize_dynrel = _finalize_dynrel;
	amd64->create_pltgot = _create_pltgot;
	amd64->finalize_pltgot = _finalize_pltgot;

	HASH_ADD_STR(ld->ld_arch_list, name, amd64);

	if ((amd64_alt = calloc(1, sizeof(*amd64_alt))) == NULL)
		ld_fatal_std(ld, "calloc");
	memcpy(amd64_alt, amd64, sizeof(struct ld_arch));
	amd64_alt->alias = amd64;
	snprintf(amd64_alt->name, sizeof(amd64_alt->name), "%s", "x86-64");

	HASH_ADD_STR(ld->ld_arch_list, name, amd64_alt);
}
