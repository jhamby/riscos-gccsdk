--- makefile.old	2002-12-16 18:50:47.000000000 +0000
+++ makefile	2002-12-16 18:50:36.000000000 +0000
@@ -24,7 +24,7 @@
 #
 # -DAPPLEDOUBLE if you want to be able to use an AppleDouble file system
 #
-CF =	-DBSD -DTYPES_H -DDIRENT_H -DTERMIOS_H -DNODOT -DAPPLEDOUBLE
+CF =	-DBSD -DTYPES_H -DDIRENT_H -DTERMIOS_H -DNODOT -DAPPLEDOUBLE -O2
 
 all:
 	(cd crc; make CF='$(CF)')
