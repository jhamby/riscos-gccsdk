--- Makefile.orig	2012-02-19 13:35:18.000000000 +0000
+++ Makefile	2012-02-19 13:35:34.000000000 +0000
@@ -790,7 +790,7 @@
 	$(CC) -o $@ $^ $(EXTRALIBS)
 
 codec-cfg$(EXESUF): codec-cfg.c codec-cfg.h help_mp.h
-	$(HOST_CC) -O -DCODECS2HTML -I. -Iffmpeg -o $@ $<
+	/usr/bin/gcc -O -DCODECS2HTML -I. -Iffmpeg -o $@ $<
 
 codecs.conf.h: codec-cfg$(EXESUF) etc/codecs.conf
 	./$^ > $@
