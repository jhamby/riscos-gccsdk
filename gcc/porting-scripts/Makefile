

SCRIPTS=ro-ar ro-config ro-install ro-libpack ro-make ro-path

all: scripts links dummies wrappers


scripts:
	mkdir -p $(ro_env)
	cp $(SCRIPTS) $(ro_env)

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
	cp uname $(ux_bin_dir)
	cp cp $(ux_bin_dir)
	cp rm $(ux_bin_dir)
