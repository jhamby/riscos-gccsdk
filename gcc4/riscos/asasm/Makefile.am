bin_PROGRAMS=asasm

# _GNU_SOURCE to make strndup available
# Add -DDEBUG if you want to enable the debug routines.
asasm_CFLAGS = -D_GNU_SOURCE -std=c99 \
	-D_LARGEFILE_SOURCE \
	-fstrict-aliasing -Wstrict-aliasing \
	-W -Wall -pedantic -Wbad-function-cast -Wcast-align \
	-Wchar-subscripts -Winline \
	-Wmissing-prototypes -Wnested-externs -Wpointer-arith \
	-Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings \
	-DGCCSDK_REVISION=\"$(shell svnversion -n ${srcdir} /trunk/gcc4/riscos/asasm)\"
asasm_LDFLAGS = -lm
asasm_SOURCES=area.c asm.c code.c decode.c depend.c \
	directive_data.c directive_file.c directive_misc.c directive_symbol.c \
	error.c \
	eval.c expr.c filename.c filestack.c fix.c frame.c get.c help_cop.c \
	help_cpu.c \
	include.c input.c lexAcorn.c lex.c lit.c \
	local.c macros.c main.c m_cop.c m_copmem.c m_cpu.c m_cpuctrl.c \
	m_cpumem.c m_fpe.c opt.c option.c os.c output.c phase.c \
	predef_reg.c put.c reloc.c state.c \
	storage.c strdup.c symbol.c targetcpu.c \
	value.c variables.c whileif.c
