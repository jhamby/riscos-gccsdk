# Copyright (c) 2005, 2006 GCCSDK Developers

SCRIPTS=ro-ar ro-config ro-install ro-libpack ro-make ro-path ro-pkg-config
WRAPPERS=uname cp ln rm
DOCS=Porting\ Tools.html

SED=sed -e s:GCCSDK_ENV:$(ro_env):g -e s:GCCSDK_BIN:$(ux_bin_dir):g

all: scripts links dummies wrappers docs

.PHONY: scripts
.PHONY: links
.PHONY: dummies
.PHONY: wrappers
.PHONY: docs

scripts:
	mkdir -p $(ro_env)
	for script in $(SCRIPTS) ; do \
	  $(SED) < $$script > $(ro_env)/$$script; \
	  chmod +x $(ro_env)/$$script; \
	done

links:
	mkdir -p $(ux_bin_dir)
	rm -f $(ux_bin_dir)cc $(ux_bin_dir)c++
	ln -sf gcc $(ux_bin_dir)cc
	ln -sf g++ $(ux_bin_dir)c++

dummies:
	mkdir -p $(ux_bin_dir)
	cp ranlib $(ux_bin_dir)ranlib
	chmod +x $(ux_bin_dir)ranlib
	cp strip $(ux_bin_dir)strip
	chmod +x $(ux_bin_dir)strip

wrappers:
	mkdir -p $(ro_env)
	for script in $(WRAPPERS) ; do \
	  $(SED) < $$script > $(ro_env)/$$script; \
	  chmod +x $(ro_env)/$$script; \
	done

docs:
	mkdir -p $(ro_env)
	for script in $(DOCS) ; do \
	  $(SED) < $$script > $(ro_env)/$$script; \
	done
