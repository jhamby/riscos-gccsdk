--- libtiff/Makefile.in.old	2003-06-19 19:01:51.000000000 +0100
+++ libtiff/Makefile.in	2003-06-19 17:57:34.000000000 +0100
@@ -270,7 +270,7 @@
 # make these rules have to be manually carried out.
 #
 tif_fax3sm.c: ${SRCDIR}/mkg3states.c ${SRCDIR}/tif_fax3.h
-	${CC} -o mkg3states ${CFLAGS} ${SRCDIR}/mkg3states.c
+	/usr/bin/cc -o mkg3states ${CFLAGS} ${SRCDIR}/mkg3states.c
 	rm -f tif_fax3sm.c; ./mkg3states -c const tif_fax3sm.c
 
 tif_aux.o: ${SRCDIR}/tif_aux.c
