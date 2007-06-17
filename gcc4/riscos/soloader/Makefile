# By Lee Noar
# 30th June, 2004

#include		config.mk

DIRS = module2 dynamic/ld.so-1.9.9

.PHONY: module2
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
	$(MAKE)
	cp build/module2/SoManager,ffa dist/!DSO-libs/SoManager,ffa
	cp build/dynamic/ld-1.9.9.so,e1f dist/!DSO-libs/lib/system/ld-1.9.9.so,e1f
	cp build/dynamic/libdl.1.9.9.so,e1f dist/!DSO-libs/lib/system/libdl.1.9.9.so,e1f
	$(MAKE) zip -C dist
