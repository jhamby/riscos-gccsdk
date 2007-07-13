include		../config.mk

zip:
	rm -frv ../loaders-bin.zip
	$(ZIP) -,r9 ../loaders-bin.zip !DSO-libs -x *svn*

clean:
	rm -frv !DSO-libs/SoManager,ffa
	rm -frv !DSO-libs/lib
	rm -frv !DSO-libs/bin
