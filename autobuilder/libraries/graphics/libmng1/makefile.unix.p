--- makefiles/makefile.unix.org	2007-12-26 16:20:25.000000000 +0100
+++ makefiles/makefile.unix	2007-12-26 16:18:02.000000000 +0100
@@ -7,20 +7,23 @@
 # Configuration options are now in libmng_conf.h
 
 # The name of your C compiler:
-CC= cc
+CC= $(wildcard $(GCCSDK_INSTALL_CROSSBIN)/*gcc)
+
+# where "make install" puts libmng.a,libmng.so*,libmng.h,libmng_conf.h,libmng_types.h
+prefix=$(GCCSDK_INSTALL_ENV)
 
 # Location of jpeg header files
-JPEG_INC= /cs/include/jpeg
+JPEG_INC= $(prefix)/include
 
 # Location of zlib header files
-ZLIB_INC= /cs/include
+ZLIB_INC= $(prefix)/include
 
 # Location of lcms header files
 # (switch on MNG_FULL_CMS in libmng_conf.h if you want to use this)
-LCMS_INC= /ltmp/lcms-1.06/source
+LCMS_INC= $(prefix)/include
 
 # compiler options:
-CFLAGS= -O -I. -I$(ZLIB_INC) -I$(JPEG_INC) -I$(LCMS_INC)
+CFLAGS= -O3 -I. -I$(ZLIB_INC) -I$(JPEG_INC) -I$(LCMS_INC)
 
 # source files
 SOURCES= \
@@ -60,6 +63,15 @@
 
 libmng.a: $(OBJECTS)
 	ar r libmng.a $(OBJECTS)
+
+INCPATH=$(prefix)/include
+LIBPATH=$(prefix)/lib
+
+install: libmng.a 
+	-@mkdir $(INCPATH) $(LIBPATH)
+	cp libmng.h libmng_conf.h libmng_types.h $(INCPATH)
+	chmod 644 $(INCPATH)/libmng.h $(INCPATH)/libmng_conf.h $(INCPATH)/libmng_types.h
+	cp libmng.a $(LIBPATH)
 
 depend:
 	makedepend -- $(CFLAGS) $(IFLAGS) -- *.c
