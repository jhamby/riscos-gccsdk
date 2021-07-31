--- imake/imakemdep.h.orig	2009-08-26 10:05:59.000000000 -0700
+++ imake/imakemdep.h	2009-08-26 10:54:13.000000000 -0700
@@ -152,6 +152,10 @@
 #  endif
 # endif
 
+# ifdef __riscos__
+#  define imake_ccflags "-D__riscos__"
+# endif
+
 # if defined(Lynx) || defined(__Lynx__)
 #  define imake_ccflags "-DLynx"
 # endif /* Lynx */
