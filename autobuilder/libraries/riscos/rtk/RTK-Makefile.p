--- !RTK/Makefile.orig	2010-03-15 21:13:26.000000000 +0000
+++ !RTK/Makefile	2010-03-15 21:31:19.000000000 +0000
@@ -3,7 +3,7 @@
 # Distribution and use are subject to the GNU Lesser General Public License,
 # a copy of which may be found in the file !RTK.Copyright.
 
-AR = libfile
+AR = ar
 
 DOXYFILES = \
  Doxyfile \
@@ -26,12 +26,12 @@
 	make -C rtk clean
 
 rtk.a: rtk/timestamp
-	$(AR) -c rtk.a $(wildcard rtk/util/*.o)
-	$(AR) -i rtk.a $(wildcard rtk/graphics/*.o)
-	$(AR) -i rtk.a $(wildcard rtk/os/*.o)
-	$(AR) -i rtk.a $(wildcard rtk/desktop/*.o)
-	$(AR) -i rtk.a $(wildcard rtk/events/*.o)
-	$(AR) -i rtk.a $(wildcard rtk/transfer/*.o)
+	$(AR) cr rtk.a $(wildcard rtk/util/*.o) \
+	$(wildcard rtk/graphics/*.o) \
+	$(wildcard rtk/os/*.o) \
+	$(wildcard rtk/desktop/*.o) \
+	$(wildcard rtk/events/*.o) \
+	$(wildcard rtk/transfer/*.o)
 
 rtk/timestamp: always
 	make -C rtk
