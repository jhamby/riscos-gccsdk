# By Lee Noar
# 30th June, 2004

include		config.mk

LIB_PATH = $(GCCSDK_INSTALL_CROSSBIN)/../arm-unknown-riscos/lib

DIRS = module2 dynamic/ld.so-1.9.9 ln

.PHONY: module2
.PHONY: dynamic/ld.so-1.9.9
.PHONY: dist
.PHONY: ln

all: $(DIRS)

$(DIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

clean:
	-rm -frv build
	-rm -frv loaders-bin.zip
	-$(MAKE) clean -C dist

# Use 'make dist' to generate a zip archive containing everything required to use
# dynamically linked executables on RISC OS. This includes the runtime system, and
# shared versions of libunixlib, libgcc and libstdc++, etc. It assumes that the
# GCCSDK is installed and has been built.
dist:
	$(MAKE)

	cp build/module2/SoManager,ffa dist/!DSO-libs/SoManager,ffa

	mkdir -p dist/!DSO-libs/lib/system

	cp build/dynamic/ld-1.9.9.so,e1f dist/!DSO-libs/lib/system/ld-1.9.9.so,e1f
	$(LN) -s -f system.ld-1/9/9/so dist/!DSO-libs/lib/ld-riscos.so.1,1c8

	cp build/dynamic/libdl.1.9.9.so,e1f dist/!DSO-libs/lib/system/libdl.1.9.9.so,e1f
	$(LN) -s -f system.libdl/1/9/9/so dist/!DSO-libs/lib/libdl.1.so,1c8
	$(LN) -s -f system.libdl/1/9/9/so dist/!DSO-libs/lib/libdl.so,1c8

	cp $(LIB_PATH)/libunixlib.5.0.0.so dist/!DSO-libs/lib/system/libunixlib.5.0.0.so,e1f
	$(LN) -s -f system.libunixlib/5/0/0/so dist/!DSO-libs/lib/libunixlib.so.5,1c8
	$(LN) -s -f system.libunixlib/5/0/0/so dist/!DSO-libs/lib/libunixlib.so,1c8

	cp $(LIB_PATH)/libgcc_s.so.1 dist/!DSO-libs/lib/system/libgcc_s.so.1,e1f
	$(LN) -s -f system.libgcc_s/so/1 dist/!DSO-libs/lib/libgcc_s.so.1,1c8
	$(LN) -s -f system.libgcc_s/so/1 dist/!DSO-libs/lib/libgcc_s.so,1c8

	cp $(LIB_PATH)/libm.1.0.0.so dist/!DSO-libs/lib/system/libm.1.0.0.so,e1f
	$(LN) -s -f system.libm/1/0/0/so dist/!DSO-libs/lib/libm.so.1,1c8
	$(LN) -s -f system.libm/1/0/0/so dist/!DSO-libs/lib/libm.so,1c8

	cp $(LIB_PATH)/libstdc++.6.0.8.so dist/!DSO-libs/lib/system/libstdc++.6.0.8.so,e1f
	$(LN) -s -f system.libstdc++/6/0/8/so dist/!DSO-libs/lib/libstdc++.so.6,1c8
	$(LN) -s -f system.libstdc++/6/0/8/so dist/!DSO-libs/lib/libstdc++.so,1c8

	mkdir -p dist/!DSO-libs/bin
	cp build/ln/cross/ln,e1f dist/!DSO-libs/bin/ln,e1f

	$(MAKE) zip -C dist

# Use 'make install' to copy libdl to a place where gcc can find it using -ldl
install:
	$(MAKE)

	cp build/dynamic/libdl.1.9.9.so,e1f $(LIB_PATH)/libdl.so
