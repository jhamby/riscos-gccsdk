--- !RiscPkg/Makefile.orig	2010-03-18 02:20:21.000000000 +0000
+++ !RiscPkg/Makefile	2010-03-18 02:21:54.000000000 +0000
@@ -9,14 +9,27 @@
 
 LIB = \
  -lstdc++ \
- -lrtk:a.rtk \
- -lzlib:a.libz \
- -llibcurl:a.libcurl \
- -llibpkg:a.libpkg
+ -L$(GCCSDK_INSTALL_ENV)/lib \
+ -lrtk \
+ -lz \
+ -lcurl \
+ -lpkg \
+ -lcares \
+ -lcrypto \
+ -lssl \
+ -lrtk
 
+.PHONE: all
 all: !RunImage
 
 !RunImage: $(CCSRC:.cc=.o)
-	$(LD) $(CCSRC:.cc=.o) $(LIB)
+	$(LD) -o !RunImage $(CCSRC:.cc=.o) $(LIB)
 
 include $(CCSRC:.cc=.d)
+
+.PHONY: clean
+clean:
+	rm -f *.o !RunImage *.d
+
+.PHONY: distclean
+distclean: clean
