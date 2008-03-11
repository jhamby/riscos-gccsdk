include		../config.mk

zip:
	rm -frv ../loaders-bin.zip
	$(ZIP) -,r9 ../loaders-bin.zip !SharedLibs -x *svn*

clean:
	rm -frv !SharedLibs/SOManager,ffa
	rm -frv !SharedLibs/lib
	rm -frv !SharedLibs/bin
