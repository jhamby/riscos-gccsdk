bin_PROGRAMS=asasm

SVN_ROOT := $(shell svn info ${srcdir} | grep "^Repository Root: " | sed -e "s/^Repository Root: //")
SVN_BRANCH := $(shell svn info ${srcdir} | grep "^URL: " | sed -e "s/^URL: //")
SVN_REV := r$(shell svnversion -n ${srcdir}) at $(subst $(SVN_ROOT),,$(SVN_BRANCH))

# We want to define the AsAsm version number as PVtbbbb with
# P = major version, V = minor version, t = patch release, bbbb = build number
# P, V and t are derived from the PACKAGE_VERSION in config.h (which is derived
# from AC_INIT in configure.ac) and bbbb is our SVN revision number.
ASASM_VERSION := $(shell grep PACKAGE_VERSION config.h | sed -e "s/.*\"\([0-9]\)\.\([0-9][0-9]\)\"/\1\2/")$(shell svnversion -n ${srcdir} | sed -e "s/[0-9]*://" -e "s/\([0-9]*\).*/\1/")

# _GNU_SOURCE to make strndup available
# Add -DDEBUG if you want to enable the debug routines.
asasm_CFLAGS = -D_GNU_SOURCE -std=c99 \
	-D_LARGEFILE_SOURCE \
	-fstrict-aliasing -Wstrict-aliasing \
	-W -Wall -pedantic -Wbad-function-cast -Wcast-align \
	-Wchar-subscripts -Winline \
	-Wmissing-prototypes -Wnested-externs -Wpointer-arith \
	-Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings \
	-DGCCSDK_REVISION="\"$(SVN_REV)\"" \
	-DASASM_VERSION=$(ASASM_VERSION)
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
