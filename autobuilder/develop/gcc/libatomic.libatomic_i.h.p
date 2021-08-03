--- libatomic/libatomic_i.h.orig	2021-04-08 04:56:29.753761012 -0700
+++ libatomic/libatomic_i.h	2021-08-02 19:21:58.272642119 -0700
@@ -236,7 +236,7 @@
 
 #if IFUNC_ALT
 # define EXPORT_ALIAS(X)	/* exported symbol in non-alternate file */
-#elif defined(N) && IFUNC_NCOND(N)
+#elif defined(N) && 0
 # if IFUNC_NCOND(N) == 1
 #  define GEN_SELECTOR(X)					\
 	extern typeof(C2(libat_,X)) C3(libat_,X,_i1) HIDDEN;	\
