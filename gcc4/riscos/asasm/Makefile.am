bin_PROGRAMS=asasm

SVN_ROOT := $(shell svn info ${srcdir} | grep "^Repository Root: " | sed -e "s/^Repository Root: //")
SVN_BRANCH := $(shell svn info ${srcdir} | grep "^URL: " | sed -e "s/^URL: //")
SVN_REV := r$(shell svnversion -n ${srcdir}) at $(subst $(SVN_ROOT),,$(SVN_BRANCH))

# _GNU_SOURCE to make strndup available
# Add -DDEBUG if you want to enable the debug routines.
asasm_CFLAGS = -D_GNU_SOURCE -std=c99 \
	-D_LARGEFILE_SOURCE \
	-fstrict-aliasing -Wstrict-aliasing \
	-W -Wall -pedantic -Wbad-function-cast -Wcast-align \
	-Wchar-subscripts -Winline \
	-Wmissing-prototypes -Wnested-externs -Wpointer-arith \
	-Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings \
	-DGCCSDK_REVISION="\"$(SVN_REV)\""
asasm_LDFLAGS = -lm
asasm_SOURCES=area.c asm.c code.c decode.c depend.c \
	directive_data.c directive_file.c directive_misc.c \
	directive_storagemap.c directive_symbol.c \
	error.c \
	eval.c expr.c filename.c filestack.c fix.c frame.c get.c help_cop.c \
	help_cpu.c \
	include.c input.c lex.c lit.c \
	local.c macros.c main.c m_cop.c m_copmem.c m_cpu.c m_cpuctrl.c \
	m_cpumem.c m_fpe.c opt.c option.c os.c output.c phase.c \
	predef_reg.c put.c reloc.c state.c \
	strdup.c symbol.c targetcpu.c \
	value.c variables.c whileif.c
