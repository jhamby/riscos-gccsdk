# Makefile for the ARM Object Format assembler - AS
# Written by Nick Burrett <nick@dsvr.net>

ifeq (${STAGE},stage1)
CFLAGS += -DCROSS_COMPILE
exesfx =
else
CC = $(CROSS_CC)
CFLAGS = $(CROSS_CFLAGS) -DUNIXLIB
endif

INCLUDES = -I. -I../../
CFLAGS += -Wall

asobjdir = $(objdir)/riscos-aof/as/$(STAGE)

$(asobjdir)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(asobjdir)/%.o: %.s
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

OBJS =	$(asobjdir)/area.o \
	$(asobjdir)/asm.o \
	$(asobjdir)/commands.o \
	$(asobjdir)/depend.o \
	$(asobjdir)/decode.o \
	$(asobjdir)/error.o \
	$(asobjdir)/eval.o \
	$(asobjdir)/expr.o \
	$(asobjdir)/get.o \
	$(asobjdir)/hash.o \
	$(asobjdir)/help_cop.o \
	$(asobjdir)/help_cpu.o \
	$(asobjdir)/input.o \
	$(asobjdir)/lex.o \
	$(asobjdir)/main.o \
	$(asobjdir)/m_cop.o \
	$(asobjdir)/m_copmem.o \
	$(asobjdir)/m_cpu.o \
	$(asobjdir)/m_cpuctrl.o \
	$(asobjdir)/m_cpumem.o \
	$(asobjdir)/m_fpu.o \
	$(asobjdir)/m_fpumem.o \
	$(asobjdir)/option.o \
	$(asobjdir)/output.o \
	$(asobjdir)/put.o \
	$(asobjdir)/symbol.o \
	$(asobjdir)/code.o \
	$(asobjdir)/help_eval.o \
	$(asobjdir)/fix.o \
	$(asobjdir)/reloc.o \
	$(asobjdir)/value.o \
	$(asobjdir)/help_lex.o \
	$(asobjdir)/storage.o \
	$(asobjdir)/riscos_c.o \
	$(asobjdir)/filestack.o \
	$(asobjdir)/include.o \
	$(asobjdir)/lexAcorn.o \
	$(asobjdir)/lit.o \
	$(asobjdir)/uname.o \
	$(asobjdir)/macros.o \
	$(asobjdir)/strdup.o \
	$(asobjdir)/variables.o \
	$(asobjdir)/whileif.o \
	$(asobjdir)/local.o \
	$(asobjdir)/targetcpu.o

ifeq (${STAGE},stage2)
	OBJS += $(asobjdir)/os.o
endif

all:	$(gcc_bin_dir)/as$(exesfx)

$(gcc_bin_dir)/as$(exesfx):	$(OBJS)
	$(CC) -o $@ $(OBJS) -lm

$(asobjdir)/area.o: area.c
$(asobjdir)/asm.o: asm.c
$(asobjdir)/commands.o: commands.c
$(asobjdir)/depend.o: depend.c
$(asobjdir)/decode.o: decode.c
$(asobjdir)/error.o: error.c
$(asobjdir)/eval.o: eval.c
$(asobjdir)/expr.o: expr.c
$(asobjdir)/get.o: get.c
$(asobjdir)/hash.o: hash.c
$(asobjdir)/help_cop.o: help_cop.c
$(asobjdir)/help_cpu.o: help_cpu.c
$(asobjdir)/input.o: input.c
$(asobjdir)/lex.o: lex.c
$(asobjdir)/main.o: main.c
$(asobjdir)/m_cop.o: m_cop.c
$(asobjdir)/m_copmem.o: m_copmem.c
$(asobjdir)/m_cpu.o: m_cpu.c
$(asobjdir)/m_cpuctrl.o: m_cpuctrl.c
$(asobjdir)/m_cpumem.o: m_cpumem.c
$(asobjdir)/m_fpu.o: m_fpu.c
$(asobjdir)/m_fpumem.o: m_fpumem.c
$(asobjdir)/option.o: option.c
$(asobjdir)/output.o: output.c
$(asobjdir)/put.o: put.c
$(asobjdir)/symbol.o: symbol.c
$(asobjdir)/code.o: code.c
$(asobjdir)/help_eval.o: help_eval.c
$(asobjdir)/fix.o: fix.c
$(asobjdir)/reloc.o: reloc.c
$(asobjdir)/value.o: value.c
$(asobjdir)/help_lex.o: help_lex.c
$(asobjdir)/storage.o: storage.c
$(asobjdir)/riscos_c.o: riscos_c.c
$(asobjdir)/filestack.o: filestack.c
$(asobjdir)/include.o: include.c
$(asobjdir)/lexAcorn.o: lexAcorn.c
$(asobjdir)/lit.o: lit.c
$(asobjdir)/uname.o: uname.c
$(asobjdir)/os.o: os.s
$(asobjdir)/macros.o: macros.c
$(asobjdir)/strdup.o: strdup.c
$(asobjdir)/variables.o: variables.c
$(asobjdir)/whileif.o: whileif.c
$(asobjdir)/local.o: local.c
$(asobjdir)/targetcpu.o: targetcpu.c
