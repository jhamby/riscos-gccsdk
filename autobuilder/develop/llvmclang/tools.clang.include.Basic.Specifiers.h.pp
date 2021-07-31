--- tools/clang/include/clang/Basic/Specifiers.h.orig	2013-12-18 15:29:05.000000000 +0000
+++ tools/clang/include/clang/Basic/Specifiers.h	2015-02-08 17:55:53.980031826 +0000
@@ -73,6 +73,9 @@
     /*DeclSpec::TSS*/ unsigned Sign  : 2;
     /*DeclSpec::TSW*/ unsigned Width : 2;
     bool ModeAttr : 1;
+#ifdef __riscos__
+    unsigned pad : 16;
+#endif
   };  
 
   /// \brief A C++ access specifier (public, private, protected), plus the
