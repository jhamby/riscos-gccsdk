# Make website building programs

TARGETS=libpkg pkgindex pkgweb pkgmergeindices

TARTARGETS= pkgweb pkgindex pkgmergeindices build-website build-stats build-index dpkg_get_packageentry.pl

.PHONY: $(TARGETS)

all: $(TARGETS)

$(TARGETS):
	$(MAKE) -C $@

clean:
	$(MAKE) -C libpkg clean
	$(MAKE) -C pkgindex clean
	$(MAKE) -C pkgweb clean

tools.tar.gz: $(TARGETS)
	cd .. && tar cf toolsrc/tools.tar.gz $(TARTARGETS)
