--- gcc/c.opt.orig	2006-09-24 23:20:22.000000000 +0200
+++ gcc/c.opt	2006-09-24 23:20:31.000000000 +0200
@@ -745,6 +745,10 @@
 ObjC ObjC++
 Dump declarations to a .decl file
 
+icrossdirafter
+C ObjC C++ ObjC++ Joined Separate
+-icrossdirafter <dir>	Add <dir> to the end of the system include path
+
 idirafter
 C ObjC C++ ObjC++ Joined Separate
 -idirafter <dir>	Add <dir> to the end of the system include path
