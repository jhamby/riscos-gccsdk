

SCRIPTS=ro-ar ro-config ro-install ro-libpack ro-make ro-path

ENV=$(ro_gccpkg)/../env

all: scripts links dummies wrappers


scripts:
	mkdir -p $(ENV)
	cp $(SCRIPTS) $(ENV)

links:
	mkdir -p $(ro_gccpkg)/bin
	ln -sf gcc $(ro_gccpkg)/bin/cc
	ln -sf g++ $(ro_gccpkg)/bin/c++

dummies:
	mkdir -p $(ro_gccpkg)/bin
	touch $(ro_gccpkg)/bin/ranlib
	touch $(ro_gccpkg)/bin/strip

wrappers:
	mkdir -p $(ro_gccpkg)/bin
	cp uname $(ro_gccpkg)/bin
	cp cp $(ro_gccpkg)/bin
	cp rm $(ro_gccpkg)/bin
