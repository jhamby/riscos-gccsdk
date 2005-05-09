

SCRIPTS=ro-ar ro-config ro-install ro-libpack ro-make ro-path
WRAPPERS=uname cp rm

SED=sed -e s:GCCSDK_ENV:$(ro_env):g -e s:GCCSDK_BIN:$(ux_bin_dir):g

all: scripts links dummies wrappers


scripts:
	mkdir -p $(ro_env)
	for script in $(SCRIPTS) ; do \
	  $(SED) < $$script > $(ro_env)/$$script; \
	  chmod +x $(ro_env)/$$script; \
	done

links:
	mkdir -p $(ux_bin_dir)
	ln -sf gcc $(ux_bin_dir)cc
	ln -sf g++ $(ux_bin_dir)c++

dummies:
	mkdir -p $(ux_bin_dir)
	touch $(ux_bin_dir)ranlib
	touch $(ux_bin_dir)strip

wrappers:
	mkdir -p $(ux_bin_dir)
	for script in $(SCRIPTS) ; do \
	  $(SED) < $$script > $(ro_env)/$$script; \
	  chmod +x $(ro_env)/$$script; \
	done
