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
	$(asobjdir)/variables.o \
	$(asobjdir)/whileif.o \
	$(asobjdir)/local.o \
	$(asobjdir)/strdup.o \
	$(asobjdir)/targetcpu.o

ifeq (${STAGE},stage2)
	OBJS += $(asobjdir)/os.o
	DEPS += $(gcc_bin_dir)/$(TRG)/o/unixlib
endif

all:	$(gcc_bin_dir)/as$(exesfx)

$(gcc_bin_dir)/as$(exesfx):	$(OBJS) $(DEPS)
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
$(asobjdir)/variables.o: variables.c
$(asobjdir)/whileif.o: whileif.c
$(asobjdir)/local.o: local.c
$(asobjdir)/strdup.o: strdup.c
$(asobjdir)/targetcpu.o: targetcpu.c

$(asobjdir)/area.o:  \
 area.c ../../sdk-config.h lex.h global.h hash.h symbol.h value.h \
  commands.h error.h input.h macros.h whileif.h expr.h area.h reloc.h \
  lit.h get.h
$(asobjdir)/asm.o:  \
 asm.c ../../sdk-config.h input.h global.h macros.h lex.h whileif.h \
  symbol.h value.h error.h decode.h area.h reloc.h lit.h asm.h code.h
$(asobjdir)/code.o:  \
 code.c ../../sdk-config.h code.h global.h lex.h symbol.h value.h reloc.h \
  error.h eval.h area.h lit.h option.h input.h macros.h whileif.h \
  aoffile.h storage.h
$(asobjdir)/commands.o:  \
 commands.c ../../sdk-config.h commands.h symbol.h lex.h global.h value.h \
  error.h input.h macros.h whileif.h output.h help_lex.h expr.h code.h \
  reloc.h put.h fix.h include.h filestack.h area.h lit.h hash.h \
  local.h os.h
$(asobjdir)/decode.o:  \
 decode.c ../../sdk-config.h decode.h symbol.h lex.h global.h value.h \
  input.h macros.h whileif.h commands.h variables.h mnemonics.h storage.h \
  option.h error.h filestack.h asm.h local.h
$(asobjdir)/depend.o:  \
 depend.c error.h global.h depend.h
$(asobjdir)/error.o:  \
 error.c ../../sdk-config.h filestack.h error.h global.h input.h macros.h \
  lex.h whileif.h symbol.h value.h output.h os.h
$(asobjdir)/eval.o:  \
 eval.c global.h eval.h lex.h value.h help_eval.h error.h include.h
$(asobjdir)/expr.o:  \
 expr.c ../../sdk-config.h expr.h value.h global.h lex.h error.h symbol.h \
  code.h reloc.h area.h lit.h
$(asobjdir)/filestack.o:  \
 filestack.c ../../sdk-config.h input.h global.h macros.h lex.h whileif.h \
  symbol.h value.h error.h
$(asobjdir)/fix.o:  \
 fix.c error.h global.h help_cpu.h fix.h m_cpu.h m_fpu.h option.h
$(asobjdir)/get.o:  \
 get.c ../../sdk-config.h get.h global.h error.h input.h macros.h lex.h \
  whileif.h symbol.h value.h expr.h reloc.h help_cpu.h fix.h
$(asobjdir)/hash.o:  \
 hash.c
$(asobjdir)/help_cop.o:  \
 help_cop.c ../../sdk-config.h error.h global.h expr.h value.h help_cop.h \
  input.h macros.h lex.h whileif.h symbol.h area.h reloc.h lit.h get.h \
  option.h code.h fix.h
$(asobjdir)/help_cpu.o:  \
 help_cpu.c help_cpu.h error.h global.h
$(asobjdir)/help_eval.o:  \
 help_eval.c ../../sdk-config.h global.h help_eval.h value.h eval.h lex.h \
  error.h symbol.h
$(asobjdir)/help_lex.o:  \
 help_lex.c global.h error.h help_lex.h
$(asobjdir)/include.o:  \
 include.c uname.h
$(asobjdir)/input.o:  \
 input.c ../../sdk-config.h error.h global.h input.h macros.h lex.h \
  whileif.h symbol.h value.h os.h filestack.h uname.h
$(asobjdir)/lex.o:  \
 lex.c ../../sdk-config.h error.h global.h lex.h local.h help_lex.h \
  input.h macros.h whileif.h symbol.h value.h hash.h area.h \
  reloc.h lit.h
$(asobjdir)/lexAcorn.o:  \
 lexAcorn.c ../../sdk-config.h lex.h global.h input.h macros.h whileif.h \
  symbol.h value.h decode.h
$(asobjdir)/lit.o:  \
 lit.c ../../sdk-config.h error.h global.h input.h macros.h lex.h \
  whileif.h symbol.h value.h area.h reloc.h lit.h put.h fix.h
$(asobjdir)/local.o:  \
 local.c ../../sdk-config.h error.h global.h local.h lex.h asm.h \
  symbol.h value.h input.h macros.h whileif.h variables.h
$(asobjdir)/m_cop.o:  \
 m_cop.c ../../sdk-config.h mnemonics.h global.h error.h option.h input.h \
  macros.h lex.h whileif.h symbol.h value.h expr.h reloc.h put.h area.h \
  lit.h get.h help_cop.h targetcpu.h
$(asobjdir)/m_copmem.o:  \
 m_copmem.c ../../sdk-config.h mnemonics.h global.h error.h option.h \
  input.h macros.h lex.h whileif.h symbol.h value.h expr.h reloc.h put.h \
  area.h lit.h get.h help_cop.h
$(asobjdir)/m_cpu.o:  \
 m_cpu.c ../../sdk-config.h mnemonics.h global.h error.h option.h put.h \
  input.h macros.h lex.h whileif.h symbol.h value.h expr.h area.h reloc.h \
  lit.h get.h m_cpu.h targetcpu.h
$(asobjdir)/m_cpuctrl.o:  \
 m_cpuctrl.c ../../sdk-config.h mnemonics.h global.h error.h option.h \
  put.h input.h macros.h lex.h whileif.h symbol.h value.h expr.h code.h \
  reloc.h area.h lit.h help_cpu.h get.h fix.h os.h targetcpu.h
$(asobjdir)/m_cpumem.o:  \
 m_cpumem.c ../../sdk-config.h mnemonics.h global.h error.h option.h \
  put.h input.h macros.h lex.h whileif.h symbol.h value.h expr.h code.h \
  reloc.h area.h lit.h get.h fix.h targetcpu.h
$(asobjdir)/m_fpu.o:  \
 m_fpu.c ../../sdk-config.h mnemonics.h global.h error.h option.h input.h \
  macros.h lex.h whileif.h symbol.h value.h expr.h code.h reloc.h get.h \
  put.h area.h lit.h m_fpu.h fix.h
$(asobjdir)/m_fpumem.o:  \
 m_fpumem.c ../../sdk-config.h mnemonics.h global.h error.h expr.h \
  value.h help_cop.h input.h macros.h lex.h whileif.h symbol.h get.h \
  put.h
$(asobjdir)/macros.o:  \
 macros.c ../../sdk-config.h commands.h symbol.h lex.h global.h value.h \
  macros.h whileif.h input.h error.h variables.h
$(asobjdir)/main.o:  \
 main.c ../../sdk-config.h error.h global.h input.h macros.h \
  lex.h whileif.h symbol.h value.h output.h asm.h area.h reloc.h lit.h \
  os.h include.h version.h targetcpu.h variables.h
$(asobjdir)/option.o:  \
 option.c ../../sdk-config.h option.h global.h error.h input.h macros.h \
  lex.h whileif.h symbol.h value.h
$(asobjdir)/output.o:  \
 output.c ../../sdk-config.h error.h global.h chunkfile.h aoffile.h \
  symbol.h lex.h value.h output.h area.h reloc.h lit.h os.h version.h \
  uname.h
$(asobjdir)/put.o:  \
 put.c ../../sdk-config.h put.h global.h error.h area.h symbol.h lex.h \
  value.h reloc.h lit.h
$(asobjdir)/reloc.o:  \
 reloc.c ../../sdk-config.h code.h global.h lex.h symbol.h value.h \
  reloc.h get.h error.h eval.h area.h lit.h option.h input.h macros.h \
  whileif.h aoffile.h fix.h
$(asobjdir)/riscos_c.o:  \
 riscos_c.c ../../sdk-config.h error.h global.h
$(asobjdir)/storage.o:  \
 storage.c ../../sdk-config.h storage.h value.h global.h symbol.h lex.h \
  get.h input.h macros.h whileif.h expr.h error.h
$(asobjdir)/symbol.o:  \
 symbol.c ../../sdk-config.h symbol.h lex.h global.h value.h error.h \
  aoffile.h code.h reloc.h help_lex.h local.h area.h lit.h
$(asobjdir)/targetcpu.o:  \
 targetcpu.c targetcpu.h global.h error.h
$(asobjdir)/uname.o:  \
 uname.c
$(asobjdir)/value.o:  \
 value.c ../../sdk-config.h value.h global.h code.h lex.h symbol.h \
  reloc.h error.h
$(asobjdir)/variables.o:  \
 variables.c ../../sdk-config.h variables.h symbol.h lex.h global.h \
  value.h error.h input.h macros.h whileif.h output.h help_lex.h expr.h \
  code.h reloc.h put.h fix.h include.h filestack.h area.h lit.h \
  hash.h os.h
$(asobjdir)/whileif.o:  \
 whileif.c ../../sdk-config.h commands.h symbol.h lex.h global.h value.h \
  decode.h error.h input.h macros.h whileif.h expr.h os.h
