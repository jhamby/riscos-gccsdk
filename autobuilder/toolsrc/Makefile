# Make website building programs

.PHONY: all clean

all: libpkg/libpkg.a ../pkgindex ../pkgweb

libpkg/libpkg.a:
	make -C libpkg

../pkgindex:
	make -C pkgindex

../pkgweb:
	make -C pkgweb

clean:
	make -C libpkg clean
	make -C pkgindex clean
	make -C pkgweb clean
