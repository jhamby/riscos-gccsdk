# Makefile for the ARM Object Format assembler - AS

ifeq (${STAGE},stage1)
CFLAGS += -DCROSS_COMPILE -D_GNU_SOURCE
exesfx =
else
CC = $(CROSS_CC)
CFLAGS = $(CROSS_CFLAGS) -D_GNU_SOURCE -mlibscl -D__RISCOSIFY
endif

INCLUDES = -I. -I../../
CFLAGS += -Wall -W

asobjdir = $(objdir)/riscos-aof/as/$(STAGE)

all:	$(gcc_bin_dir)/as$(exesfx)

SRCS = area.c \
	asm.c \
	code.c \
	commands.c \
	decode.c \
	depend.c \
	error.c \
	eval.c \
	expr.c \
	filestack.c \
	fix.c \
	get.c \
	help_cop.c \
	help_cpu.c \
	help_eval.c \
	help_lex.c \
	include.c \
	input.c \
	lex.c \
	lexAcorn.c \
	lit.c \
	local.c \
	macros.c \
	main.c \
	m_cop.c \
	m_copmem.c \
	m_cpu.c \
	m_cpuctrl.c \
	m_cpumem.c \
	m_fpu.c \
	m_fpumem.c \
	option.c \
	output.c \
	put.c \
	reloc.c \
	riscos_c.c \
	storage.c \
	strdup.c \
	symbol.c \
	targetcpu.c \
	value.c \
	variables.c \
	whileif.c

HDRS = ../../sdk-config.h \
	aoffile.h \
	area.h \
	asm.h \
	chunkfile.h \
	code.h \
	commands.h \
	config.h \
	decode.h \
	depend.h \
	elf.h \
	error.h \
	eval.h \
	expr.h \
	filestack.h \
	fix.h \
	get.h \
	global.h \
	help_cop.h \
	help_cpu.h \
	help_eval.h \
	help_lex.h \
	include.h \
	input.h \
	lex.h \
	lit.h \
	local.h \
	macros.h \
	main.h \
	m_cpu.h \
	m_fpu.h \
	mnemonics.h \
	option.h \
	os.h \
	output.h \
	put.h \
	reloc.h \
	rname.h \
	storage.h \
	symbol.h \
	targetcpu.h \
	value.h \
	variables.h \
	version.h \
	whileif.h

ifeq (${STAGE},stage1)
# As we cannot guarantee what the host compiler is, we elect to build
# the application as seperate module units.
SRCS +=	rname.c rname2.c

# This funky line tells make to change all '.c' suffixes to '.o' and add
# '$(asobjdir)/' to each object.
OBJS = $(addprefix $(asobjdir)/, $(SRCS:.c=.o))

$(asobjdir)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(gcc_bin_dir)/as$(exesfx):	$(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -lm -L$(objdir)/libiberty/$(STAGE) -liberty 
endif
ifeq (${STAGE},stage2)
# Since we know that we are compiling with GCC 3.4, we can compile the
# application using inter-module analysis, effectivly building as if all
# functions are contained within one source file.
	DEPS += $(gcc_bin_dir)/$(TRG)/o/unixlib

$(asobjdir)/as.o: $(SRCS) $(DEPS) $(HDRS)
	$(CC) $(CFLAGS) -o $@ -c $(SRCS) $(INCLUDES)

$(gcc_bin_dir)/as$(exesfx):	$(asobjdir)/as.o $(DEPS)
	$(CC) $(CFLAGS) -o $@ $(asobjdir)/as.o -lm -L$(objdir)/libiberty/$(STAGE) -liberty 
endif


ifeq (${STAGE},stage1)
$(asobjdir)/area.o:  \
 area.c ../../sdk-config.h config.h lex.h global.h symbol.h value.h \
  commands.h error.h input.h macros.h whileif.h expr.h area.h reloc.h \
  lit.h get.h
$(asobjdir)/asm.o:  \
 asm.c ../../sdk-config.h config.h input.h global.h macros.h lex.h whileif.h \
  symbol.h value.h error.h decode.h area.h reloc.h lit.h asm.h code.h
$(asobjdir)/code.o:  \
 code.c ../../sdk-config.h config.h code.h global.h lex.h symbol.h value.h reloc.h \
  error.h eval.h area.h lit.h option.h input.h macros.h whileif.h \
  aoffile.h storage.h
$(asobjdir)/commands.o:  \
 commands.c ../../sdk-config.h config.h commands.h symbol.h lex.h global.h value.h \
  error.h input.h macros.h whileif.h output.h help_lex.h expr.h code.h \
  reloc.h put.h fix.h include.h filestack.h area.h lit.h local.h os.h
$(asobjdir)/decode.o:  \
 decode.c ../../sdk-config.h config.h decode.h symbol.h lex.h global.h value.h \
  input.h macros.h whileif.h commands.h variables.h mnemonics.h storage.h \
  option.h error.h filestack.h asm.h local.h
$(asobjdir)/depend.o:  \
 depend.c error.h global.h depend.h
$(asobjdir)/error.o:  \
 error.c ../../sdk-config.h config.h filestack.h error.h global.h input.h macros.h \
  lex.h whileif.h symbol.h value.h output.h os.h
$(asobjdir)/eval.o:  \
 eval.c global.h eval.h lex.h value.h help_eval.h error.h include.h
$(asobjdir)/expr.o:  \
 expr.c ../../sdk-config.h config.h expr.h value.h global.h lex.h error.h symbol.h \
  code.h reloc.h area.h lit.h
$(asobjdir)/filestack.o:  \
 filestack.c ../../sdk-config.h config.h input.h global.h macros.h lex.h whileif.h \
  symbol.h value.h error.h
$(asobjdir)/fix.o:  \
 fix.c error.h global.h help_cpu.h fix.h m_cpu.h m_fpu.h option.h
$(asobjdir)/get.o:  \
 get.c ../../sdk-config.h config.h get.h global.h error.h input.h macros.h lex.h \
  whileif.h symbol.h value.h expr.h reloc.h help_cpu.h fix.h
$(asobjdir)/help_cop.o:  \
 help_cop.c ../../sdk-config.h config.h error.h global.h expr.h value.h help_cop.h \
  input.h macros.h lex.h whileif.h symbol.h area.h reloc.h lit.h get.h \
  option.h code.h fix.h
$(asobjdir)/help_cpu.o:  \
 help_cpu.c help_cpu.h error.h global.h
$(asobjdir)/help_eval.o:  \
 help_eval.c ../../sdk-config.h config.h global.h help_eval.h value.h eval.h lex.h \
  error.h symbol.h
$(asobjdir)/help_lex.o:  \
 help_lex.c global.h error.h help_lex.h
$(asobjdir)/input.o:  \
 input.c ../../sdk-config.h config.h error.h global.h input.h macros.h lex.h \
  whileif.h symbol.h value.h os.h filestack.h
$(asobjdir)/lex.o:  \
 lex.c ../../sdk-config.h config.h error.h global.h lex.h local.h help_lex.h \
  input.h macros.h whileif.h symbol.h value.h area.h reloc.h lit.h
$(asobjdir)/lexAcorn.o:  \
 lexAcorn.c ../../sdk-config.h config.h lex.h global.h input.h macros.h whileif.h \
  symbol.h value.h decode.h
$(asobjdir)/lit.o:  \
 lit.c ../../sdk-config.h config.h error.h global.h input.h macros.h lex.h \
  whileif.h symbol.h value.h area.h reloc.h lit.h put.h fix.h
$(asobjdir)/local.o:  \
 local.c ../../sdk-config.h config.h error.h global.h local.h lex.h asm.h \
  symbol.h value.h input.h macros.h whileif.h variables.h
$(asobjdir)/m_cop.o:  \
 m_cop.c ../../sdk-config.h config.h mnemonics.h global.h error.h option.h input.h \
  macros.h lex.h whileif.h symbol.h value.h expr.h reloc.h put.h area.h \
  lit.h get.h help_cop.h targetcpu.h
$(asobjdir)/m_copmem.o:  \
 m_copmem.c ../../sdk-config.h config.h mnemonics.h global.h error.h option.h \
  input.h macros.h lex.h whileif.h symbol.h value.h expr.h reloc.h put.h \
  area.h lit.h get.h help_cop.h
$(asobjdir)/m_cpu.o:  \
 m_cpu.c ../../sdk-config.h config.h mnemonics.h global.h error.h option.h put.h \
  input.h macros.h lex.h whileif.h symbol.h value.h expr.h area.h reloc.h \
  lit.h get.h m_cpu.h targetcpu.h
$(asobjdir)/m_cpuctrl.o:  \
 m_cpuctrl.c ../../sdk-config.h config.h mnemonics.h global.h error.h option.h \
  put.h input.h macros.h lex.h whileif.h symbol.h value.h expr.h code.h \
  reloc.h area.h lit.h help_cpu.h get.h fix.h os.h targetcpu.h
$(asobjdir)/m_cpumem.o:  \
 m_cpumem.c ../../sdk-config.h config.h mnemonics.h global.h error.h option.h \
  put.h input.h macros.h lex.h whileif.h symbol.h value.h expr.h code.h \
  reloc.h area.h lit.h get.h fix.h targetcpu.h
$(asobjdir)/m_fpu.o:  \
 m_fpu.c ../../sdk-config.h config.h mnemonics.h global.h error.h option.h input.h \
  macros.h lex.h whileif.h symbol.h value.h expr.h code.h reloc.h get.h \
  put.h area.h lit.h m_fpu.h fix.h
$(asobjdir)/m_fpumem.o:  \
 m_fpumem.c ../../sdk-config.h config.h mnemonics.h global.h error.h expr.h \
  value.h help_cop.h input.h macros.h lex.h whileif.h symbol.h get.h \
  put.h
$(asobjdir)/macros.o:  \
 macros.c ../../sdk-config.h config.h commands.h symbol.h lex.h global.h value.h \
  macros.h whileif.h input.h error.h variables.h
$(asobjdir)/main.o:  \
 main.c ../../sdk-config.h config.h error.h global.h input.h macros.h \
  lex.h whileif.h symbol.h value.h output.h asm.h area.h reloc.h lit.h \
  os.h include.h version.h targetcpu.h variables.h
$(asobjdir)/option.o:  \
 option.c ../../sdk-config.h config.h option.h global.h error.h input.h macros.h \
  lex.h whileif.h symbol.h value.h
$(asobjdir)/output.o:  \
 output.c ../../sdk-config.h config.h error.h global.h chunkfile.h aoffile.h \
  symbol.h lex.h value.h output.h area.h reloc.h lit.h os.h version.h 
$(asobjdir)/put.o:  \
 put.c ../../sdk-config.h config.h put.h global.h error.h area.h symbol.h lex.h \
  value.h reloc.h lit.h
$(asobjdir)/reloc.o:  \
 reloc.c ../../sdk-config.h config.h code.h global.h lex.h symbol.h value.h \
  reloc.h get.h error.h eval.h area.h lit.h option.h input.h macros.h \
  whileif.h aoffile.h fix.h
$(asobjdir)/riscos_c.o:  \
 riscos_c.c ../../sdk-config.h config.h error.h global.h os.h
$(asobjdir)/storage.o:  \
 storage.c ../../sdk-config.h config.h storage.h value.h global.h symbol.h lex.h \
  get.h input.h macros.h whileif.h expr.h error.h
$(asobjdir)/symbol.o:  \
 symbol.c ../../sdk-config.h config.h symbol.h lex.h global.h value.h error.h \
  aoffile.h code.h reloc.h help_lex.h local.h area.h lit.h
$(asobjdir)/targetcpu.o:  \
 targetcpu.c targetcpu.h global.h error.h
$(asobjdir)/rname.o:  \
 rname.c
$(asobjdir)/rname2.o:  \
 rname2.c rname.h
$(asobjdir)/value.o:  \
 value.c ../../sdk-config.h config.h value.h global.h code.h lex.h symbol.h \
  reloc.h error.h
$(asobjdir)/variables.o:  \
 variables.c ../../sdk-config.h config.h variables.h symbol.h lex.h global.h \
  value.h error.h input.h macros.h whileif.h output.h help_lex.h expr.h \
  code.h reloc.h put.h fix.h include.h filestack.h area.h lit.h os.h
$(asobjdir)/whileif.o:  \
 whileif.c ../../sdk-config.h config.h commands.h symbol.h lex.h global.h value.h \
  decode.h error.h input.h macros.h whileif.h expr.h os.h
endif
