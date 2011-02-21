bin_PROGRAMS=asasm

# _GNU_SOURCE to make strndup available
# Add -DDEBUG if you want to enable the debug routines.
asasm_CFLAGS = -D_GNU_SOURCE -std=c99 \
	-W -Wall -pedantic -Wbad-function-cast -Wcast-align \
	-Wchar-subscripts -Winline \
	-Wmissing-prototypes -Wnested-externs -Wpointer-arith \
	-Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings \
	-DGCCSDK_REVISION=\"$(shell svnversion -n ${srcdir} /trunk/gcc4/riscos/asasm)\"
asasm_LDFLAGS = -lm
asasm_SOURCES=area.c asm.c code.c commands.c decode.c depend.c error.c \
	eval.c expr.c filestack.c fix.c get.c help_cop.c help_cpu.c \
	include.c input.c lexAcorn.c lex.c lit.c \
	local.c macros.c main.c m_cop.c m_copmem.c m_cpu.c m_cpuctrl.c \
	m_cpumem.c m_fpe.c option.c output.c put.c reloc.c \
	riscos_c.c rname2.c rname.c storage.c strdup.c symbol.c targetcpu.c \
	value.c variables.c whileif.c

tooldir = $(exec_prefix)/$(target_alias)

.PHONY: install-exec-local

# Local install exec hook to install a copy of asasm copy in <target>/bin
# directory where gcc's collect2 will look.
install-exec-local: $(bin_PROGRAMS)
	$(mkdir_p) "$(DESTDIR)$(tooldir)/bin"
	list='$(bin_PROGRAMS)'; for p in $$list; do \
	  p1=`echo $$p|sed 's/$(EXEEXT)$$//'`; \
	  if test -f $$p; then \
	    f=`echo "$$p1" | sed 's,^.*/,,;$(transform);s/$$/$(EXEEXT)/'`; \
	    rm -f $(DESTDIR)$(tooldir)/bin/$$f; \
	    ln $(DESTDIR)$(bindir)/$$p $(DESTDIR)$(tooldir)/bin/$$f >/dev/null 2>/dev/null \
	    || $(INSTALL_PROGRAM_ENV) $(binPROGRAMS_INSTALL) "$$p" "$(DESTDIR)$(tooldir)/bin/$$f" || exit 1; \
	  else :; fi; \
	done
