--- Makefile.in.orig	2017-06-05 21:00:08.000000000 +0100
+++ Makefile.in	2017-08-15 19:51:38.600297643 +0100
@@ -40,7 +40,7 @@
 ARFLAGS=rc
 RANLIB=ranlib
 LDCONFIG=ldconfig
-LDSHAREDLIBC=-lc
+LDSHAREDLIBC=
 TAR=tar
 SHELL=/bin/sh
 EXE=
@@ -160,7 +160,7 @@
 	-@mv objs/$*.o $@
 
 placebo $(SHAREDLIBV): $(PIC_OBJS) libz.a
-	$(LDSHARED) $(SFLAGS) -o $@ $(PIC_OBJS) $(LDSHAREDLIBC) $(LDFLAGS)
+	$(LDSHARED) $(SFLAGS) -o $@ $(PIC_OBJS) $(LDSHAREDLIBC) $(LDFLAGS) -Wl,--soname=$(SHAREDLIBM)
 	rm -f $(SHAREDLIB) $(SHAREDLIBM)
 	ln -s $@ $(SHAREDLIB)
 	ln -s $@ $(SHAREDLIBM)
