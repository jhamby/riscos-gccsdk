AM_CPPFLAGS = -I$(srcdir)/elf
bin_PROGRAMS=elf2aif
elf2aif_SOURCES=elf2aif.c elf/common.h elf/external.h

tooldir = $(exec_prefix)/$(target_alias)

.PHONY: install-exec-local

# Local install exec hook to install a copy of elf2aif copy in <target>/bin
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
