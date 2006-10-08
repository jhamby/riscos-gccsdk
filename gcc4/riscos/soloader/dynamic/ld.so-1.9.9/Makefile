include Version.mk
include Config.mk
include ../../config.mk

OBJDIR = ../$(BUILDDIR)/dynamic

SUBDIRS = d-link

.PHONY: $(SUBDIRS)

all: $(OBJDIR) $(SUBDIRS)

$(OBJDIR):
	mkdir -p $(OBJDIR)
	
$(SUBDIRS):
	$(MAKE) -C $@
