--- lib/getloadavg.c~	2005-03-06 21:47:11.000000000 +0000
+++ lib/getloadavg.c	2005-03-06 21:50:07.000000000 +0000
@@ -365,7 +365,7 @@
 
 /* LOAD_AVE_TYPE should only get defined if we're going to use the
    nlist method.  */
-# if !defined (LOAD_AVE_TYPE) && (defined (BSD) || defined (LDAV_CVT) || defined (KERNEL_FILE) || defined (LDAV_SYMBOL))
+# if !defined (LOAD_AVE_TYPE) && (defined (BSD) || defined (LDAV_CVT) || defined (KERNEL_FILE) || defined (LDAV_SYMBOL)) && !defined(__riscos__)
 #  define LOAD_AVE_TYPE double
 # endif
 
