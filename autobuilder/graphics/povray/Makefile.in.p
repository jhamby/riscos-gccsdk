--- Makefile.in.orig    2012-01-07 15:51:59.000000000 +0000
+++ Makefile.in 2012-01-07 15:54:34.000000000 +0000
@@ -173,9 +173,9 @@
 target_alias = @target_alias@

 # Directories.
-povlibdir = @datadir@/@PACKAGE@-@VERSION_BASE@
-povdocdir = @datadir@/doc/@PACKAGE@-@VERSION_BASE@
-povconfdir = @sysconfdir@/@PACKAGE@/@VERSION_BASE@
+povlibdir = @datadir@
+povdocdir = @datadir@/doc
+povconfdir = @datadir@
 povconfuser = $(HOME)/.@PACKAGE@/@VERSION_BASE@

 # Directories to build.

--- libraries/tiff/libtiff/Makefile.in.orig	2012-01-07 12:06:56.000000000 +0000
+++ libraries/tiff/libtiff/Makefile.in	2012-01-07 12:07:23.000000000 +0000
@@ -265,7 +265,7 @@
 RELEASE_DATE = @RELEASE_DATE_FILE@
 
 ${SRCDIR}/tiffvers.h: ${VERSION} ${SRCDIR}/mkversion.c
-	${CC} -o mkversion ${CFLAGS} ${SRCDIR}/mkversion.c
+	/usr/bin/cc -o mkversion -O3 -I. ${SRCDIR}/mkversion.c
 	rm -f ${SRCDIR}/tiffvers.h
 	./mkversion -v ${VERSION} -r ${RELEASE_DATE} ${SRCDIR}/tiffvers.h
 
@@ -277,7 +277,7 @@
 # make these rules have to be manually carried out.
 #
 tif_fax3sm.c: ${SRCDIR}/mkg3states.c ${SRCDIR}/tif_fax3.h
-	${CC} -o mkg3states ${CFLAGS} ${SRCDIR}/mkg3states.c
+	/usr/bin/cc -o mkg3states -O3 -I. ${SRCDIR}/mkg3states.c
 	rm -f tif_fax3sm.c; ./mkg3states -c const tif_fax3sm.c
 
 tif_aux.o: ${SRCDIR}/tif_aux.c
