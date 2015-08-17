# Make website building programs

TARGETS=libpkg pkgindex pkgweb

.PHONY: $(TARGETS)

all: $(TARGETS)

$(TARGETS):
	$(MAKE) -C $@

clean:
	$(MAKE) -C libpkg clean
	$(MAKE) -C pkgindex clean
	$(MAKE) -C pkgweb clean

tools.tar.gz: $(TARGETS)
	cd .. && tar cf toolsrc/tools.tar.gz pkgweb pkgindex build-website build-stats
