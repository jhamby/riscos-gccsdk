# The install-minimum target will only be install the bare asasm code, not
# decaof nor elftoolchain.
install-minimum:
	$(MAKE) $(AM_MAKEFLAGS)
	$(MAKE) $(AM_MAKEFLAGS) install SUBDIRS=src

# Only do the asasm unittests (and not those of decaof or elftoolchain).
check-minimum:
	$(MAKE) $(AM_MAKEFLAGS) check SUBDIRS='unittests/compare unittests/reloc'

SUBDIRS = decaof elftoolchain src unittests/compare unittests/reloc
