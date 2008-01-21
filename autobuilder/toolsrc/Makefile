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
