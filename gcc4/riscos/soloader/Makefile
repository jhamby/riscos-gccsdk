# By Lee Noar
# 30th June, 2004

#include		config.mk

DIRS = rt_lib utils static module dynamic/ld.so-1.9.9

.PHONY: rt_lib
.PHONY: utils
.PHONY: static
.PHONY: module
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
	cp build/static/loader,ff8 dist/!DSO-libs/loader,ff8
	cp build/module/SoManager,ffa dist/!DSO-libs/SoManager,ffa
	cp build/dynamic/ld-1.9.9.so,e1f dist/!DSO-libs/lib/system/ld-1.9.9.so,e1f
	cp build/dynamic/libdl.1.9.9.so,e1f dist/!DSO-libs/lib/system/libdl.1.9.9.so,e1f
	$(MAKE) zip -C dist
