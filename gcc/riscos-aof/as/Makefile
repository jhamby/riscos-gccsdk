INCLUDES = -I. -I../../
CFLAGS = -Wall -O2 $(OPTS) $(TARGET_DEFS)
ifeq ($(crosscompile),no)
CFLAGS += -DUNIXLIB
endif
CC = gcc $(CFLAGS)

$(objdir)/riscos-aof/as/%.o: %.c
	$(CC) -o $@ -c $< $(INCLUDES)

$(objdir)/riscos-aof/as/%.o: %.s
	$(CC) -o $@ -c $< $(INCLUDES)

OBJS =	$(objdir)/riscos-aof/as/area.o \
	$(objdir)/riscos-aof/as/asm.o \
	$(objdir)/riscos-aof/as/commands.o \
	$(objdir)/riscos-aof/as/depend.o \
	$(objdir)/riscos-aof/as/decode.o \
	$(objdir)/riscos-aof/as/error.o \
	$(objdir)/riscos-aof/as/eval.o \
	$(objdir)/riscos-aof/as/expr.o \
	$(objdir)/riscos-aof/as/get.o \
	$(objdir)/riscos-aof/as/hash.o \
	$(objdir)/riscos-aof/as/help_cop.o \
	$(objdir)/riscos-aof/as/help_cpu.o \
	$(objdir)/riscos-aof/as/input.o \
	$(objdir)/riscos-aof/as/lex.o \
	$(objdir)/riscos-aof/as/main.o \
	$(objdir)/riscos-aof/as/m_cop.o \
	$(objdir)/riscos-aof/as/m_copmem.o \
	$(objdir)/riscos-aof/as/m_cpu.o \
	$(objdir)/riscos-aof/as/m_cpuctrl.o \
	$(objdir)/riscos-aof/as/m_cpumem.o \
	$(objdir)/riscos-aof/as/m_fpu.o \
	$(objdir)/riscos-aof/as/m_fpumem.o \
	$(objdir)/riscos-aof/as/option.o \
	$(objdir)/riscos-aof/as/output.o \
	$(objdir)/riscos-aof/as/put.o \
	$(objdir)/riscos-aof/as/symbol.o \
	$(objdir)/riscos-aof/as/code.o \
	$(objdir)/riscos-aof/as/help_eval.o \
	$(objdir)/riscos-aof/as/fix.o \
	$(objdir)/riscos-aof/as/reloc.o \
	$(objdir)/riscos-aof/as/value.o \
	$(objdir)/riscos-aof/as/help_lex.o \
	$(objdir)/riscos-aof/as/storage.o \
	$(objdir)/riscos-aof/as/riscos_c.o \
	$(objdir)/riscos-aof/as/filestack.o \
	$(objdir)/riscos-aof/as/include.o \
	$(objdir)/riscos-aof/as/lexAcorn.o \
	$(objdir)/riscos-aof/as/lit.o \
	$(objdir)/riscos-aof/as/uname.o \
	$(objdir)/riscos-aof/as/macros.o \
	$(objdir)/riscos-aof/as/strdup.o \
	$(objdir)/riscos-aof/as/variables.o \
	$(objdir)/riscos-aof/as/whileif.o \
	$(objdir)/riscos-aof/as/local.o \
	$(objdir)/riscos-aof/as/targetcpu.o

ifeq (${crosscompile},no)
	OBJS += $(objdir)/riscos-aof/as/os.o
endif

all:	$(TBIN)/as

$(TBIN)/as:	$(OBJS)
	gcc -o $@ $(OBJS) -lm

$(objdir)/riscos-aof/as/area.o: area.c
$(objdir)/riscos-aof/as/asm.o: asm.c
$(objdir)/riscos-aof/as/commands.o: commands.c
$(objdir)/riscos-aof/as/depend.o: depend.c
$(objdir)/riscos-aof/as/decode.o: decode.c
$(objdir)/riscos-aof/as/error.o: error.c
$(objdir)/riscos-aof/as/eval.o: eval.c
$(objdir)/riscos-aof/as/expr.o: expr.c
$(objdir)/riscos-aof/as/get.o: get.c
$(objdir)/riscos-aof/as/hash.o: hash.c
$(objdir)/riscos-aof/as/help_cop.o: help_cop.c
$(objdir)/riscos-aof/as/help_cpu.o: help_cpu.c
$(objdir)/riscos-aof/as/input.o: input.c
$(objdir)/riscos-aof/as/lex.o: lex.c
$(objdir)/riscos-aof/as/main.o: main.c
$(objdir)/riscos-aof/as/m_cop.o: m_cop.c
$(objdir)/riscos-aof/as/m_copmem.o: m_copmem.c
$(objdir)/riscos-aof/as/m_cpu.o: m_cpu.c
$(objdir)/riscos-aof/as/m_cpuctrl.o: m_cpuctrl.c
$(objdir)/riscos-aof/as/m_cpumem.o: m_cpumem.c
$(objdir)/riscos-aof/as/m_fpu.o: m_fpu.c
$(objdir)/riscos-aof/as/m_fpumem.o: m_fpumem.c
$(objdir)/riscos-aof/as/option.o: option.c
$(objdir)/riscos-aof/as/output.o: output.c
$(objdir)/riscos-aof/as/put.o: put.c
$(objdir)/riscos-aof/as/symbol.o: symbol.c
$(objdir)/riscos-aof/as/code.o: code.c
$(objdir)/riscos-aof/as/help_eval.o: help_eval.c
$(objdir)/riscos-aof/as/fix.o: fix.c
$(objdir)/riscos-aof/as/reloc.o: reloc.c
$(objdir)/riscos-aof/as/value.o: value.c
$(objdir)/riscos-aof/as/help_lex.o: help_lex.c
$(objdir)/riscos-aof/as/storage.o: storage.c
$(objdir)/riscos-aof/as/riscos_c.o: riscos_c.c
$(objdir)/riscos-aof/as/filestack.o: filestack.c
$(objdir)/riscos-aof/as/include.o: include.c
$(objdir)/riscos-aof/as/lexAcorn.o: lexAcorn.c
$(objdir)/riscos-aof/as/lit.o: lit.c
$(objdir)/riscos-aof/as/uname.o: uname.c
$(objdir)/riscos-aof/as/os.o: os.s
$(objdir)/riscos-aof/as/macros.o: macros.c
$(objdir)/riscos-aof/as/strdup.o: strdup.c
$(objdir)/riscos-aof/as/variables.o: variables.c
$(objdir)/riscos-aof/as/whileif.o: whileif.c
$(objdir)/riscos-aof/as/local.o: local.c
$(objdir)/riscos-aof/as/targetcpu.o: targetcpu.c
