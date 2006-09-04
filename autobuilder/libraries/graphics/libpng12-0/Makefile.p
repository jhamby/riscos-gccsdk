--- scripts/makefile.linux.orig	2005-02-06 21:54:40.000000000 +0000
+++ scripts/makefile.linux	2005-02-06 21:56:21.000000000 +0000
@@ -13,7 +13,7 @@
 # where "make install" puts libpng12.a, libpng12.so*,
 # libpng12/png.h and libpng12/pngconf.h
 # Prefix must be a full pathname.
-prefix=/usr/local
+prefix=$(GCCSDK_INSTALL_ENV)
 
 # Where the zlib library and include files are located.
 #ZLIBLIB=/usr/local/lib
@@ -32,6 +32,7 @@
 # for pgcc version 2.95.1, -O3 is buggy; don't use it.
 
 CFLAGS=-I$(ZLIBINC) -Wall -O3 -funroll-loops \
+	-L$(GCCSDK_INSTALL_ENV)/lib -I$(GCCSDK_INSTALL_ENV)/include \
 	$(ALIGN) # $(WARNMORE) -g -DPNG_DEBUG=5
 
 LDFLAGS=-L. -Wl,-rpath,. -L$(ZLIBLIB) -Wl,-rpath,$(ZLIBLIB) -lpng12 -lz -lm
@@ -71,7 +72,7 @@
 .c.pic.o:
 	$(CC) -c $(CFLAGS) -fPIC -o $@ $*.c
 
-all: libpng.a $(LIBNAME).so pngtest pngtest-static libpng.pc libpng-config
+all: libpng.a libpng.pc libpng-config
 
 libpng.a: $(OBJS)
 	ar rc $@ $(OBJS)
@@ -175,8 +176,8 @@
 
 install-config: libpng-config
 	-@if [ ! -d $(DB) ]; then mkdir $(DB); fi
-	-@/bin/rm -f $(DB)/libpng-config
-	-@/bin/rm -f $(DB)/$(LIBNAME)-config
+	#-@/bin/rm -f $(DB)/libpng-config
+	#-@/bin/rm -f $(DB)/$(LIBNAME)-config
 	cp libpng-config $(DB)/$(LIBNAME)-config
 	chmod 755 $(DB)/$(LIBNAME)-config
 	(cd $(DB); ln -sf $(LIBNAME)-config libpng-config)
--- scripts/makefile.linux.orig	2005-02-06 22:04:45.000000000 +0000
+++ scripts/makefile.linux	2005-02-06 22:05:05.000000000 +0000
@@ -182,7 +182,7 @@
 	chmod 755 $(DB)/$(LIBNAME)-config
 	(cd $(DB); ln -sf $(LIBNAME)-config libpng-config)
 
-install: install-static install-shared install-man install-config
+install: install-static install-man install-config
 
 # If you installed in $(DESTDIR), test-installed won't work until you
 # move the library to its final location.  Use test-dd to test it
--- scripts/makefile.linux.orig	2005-05-09 14:32:12.000000000 +0100
+++ scripts/makefile.linux	2005-05-09 14:34:20.000000000 +0100
@@ -86,7 +86,7 @@
 	echo prefix=\"$(prefix)\"; \
 	echo I_opts=\"-I$(INCPATH)/$(LIBNAME)\"; \
 	echo L_opts=\"-L$(LIBPATH)\"; \
-	echo R_opts=\"-Wl,-rpath,$(LIBPATH)\"; \
+	echo R_opts=\"\"; \
 	echo libs=\"-lpng12 -lz -lm\"; \
 	cat scripts/libpng-config-body.in ) > libpng-config
 	chmod +x libpng-config
--- scripts/makefile.linux.orig	2006-09-03 18:33:15.000000000 +0200
+++ scripts/makefile.linux	2006-09-03 18:33:37.000000000 +0200
@@ -132,12 +132,18 @@
 	-@/bin/rm -f $(DI)/libpng
 	(cd $(DI); ln -sf $(LIBNAME) libpng; ln -sf $(LIBNAME)/* .)
 
-install-static: install-headers libpng.a
+install-static: install-headers libpng.a libpng.pc
 	-@if [ ! -d $(DL) ]; then mkdir $(DL); fi
 	cp libpng.a $(DL)/$(LIBNAME).a
 	chmod 644 $(DL)/$(LIBNAME).a
 	-@/bin/rm -f $(DL)/libpng.a
 	(cd $(DL); ln -sf $(LIBNAME).a libpng.a)
+	-@if [ ! -d $(DL)/pkgconfig ]; then mkdir $(DL)/pkgconfig; fi
+	#-@/bin/rm -f $(DL)/pkgconfig/$(LIBNAME).pc
+	#-@/bin/rm -f $(DL)/pkgconfig/libpng.pc
+	cp libpng.pc $(DL)/pkgconfig/$(LIBNAME).pc
+	chmod 644 $(DL)/pkgconfig/$(LIBNAME).pc
+	(cd $(DL)/pkgconfig; ln -sf $(LIBNAME).pc libpng.pc)
 
 install-shared: install-headers $(LIBNAME).so.$(PNGVER) libpng.pc \
 	libpng.so.3.$(PNGMIN)
