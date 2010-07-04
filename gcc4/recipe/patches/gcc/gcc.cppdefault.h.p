--- gcc/cppdefault.h.orig	2010-07-04 15:44:04.034401758 +0200
+++ gcc/cppdefault.h	2010-07-04 15:43:36.816901426 +0200
@@ -43,6 +43,9 @@ struct default_include
 				   C++.  */
   const char add_sysroot;	/* FNAME should be prefixed by
 				   cpp_SYSROOT.  */
+  const char multilib;		/* FNAME should have the multilib path
+				   specified with -imultilib
+				   appended.  */
 };
 
 extern const struct default_include cpp_include_defaults[];
