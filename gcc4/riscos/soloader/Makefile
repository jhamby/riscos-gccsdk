# By Lee Noar
# 30th June, 2004

include		config.mk

LIBDIR ?= dist/\!SharedLibs/lib

DIRS = dynamic/ld.so-1.9.9

.PHONY: dynamic/ld.so-1.9.9
.PHONY: dist

all: $(DIRS)

$(DIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

clean:
	-rm -frv build
	-rm -frv loaders-bin.zip
	-$(MAKE) clean -C dist

dist:
	$(MAKE) install-riscos
	$(MAKE) zip -C dist

# Use 'make install-riscos' to copy libdl and ld to !SharedLibs.
install-riscos:
	$(MAKE)
	mkdir -p $(LIBDIR)
	cp build/dynamic/ld-1.9.9.so,e1f $(LIBDIR)/ld-1.9.9.so,e1f
	$(LN) -s -f ld-1/9/9/so $(LIBDIR)/ld-riscos.so.1,1c8
	cp build/dynamic/libdl.1.9.9.so,e1f $(LIBDIR)/libdl.1.9.9.so,e1f
	$(LN) -s -f libdl/1/9/9/so $(LIBDIR)/libdl.so.1,1c8
	$(LN) -s -f libdl/1/9/9/so $(LIBDIR)/libdl.so,1c8

# Use 'make install-cross' to copy libdl to a place where gcc can find it using -ldl
install-cross:
	$(MAKE)
	mkdir -p $(LIBDIR)
	cp build/dynamic/ld-1.9.9.so,e1f $(LIBDIR)/ld-riscos.so.1
	cp build/dynamic/libdl.1.9.9.so,e1f $(LIBDIR)/libdl.so
