--- getloadavg.c	2003-02-22 19:21:44.000000000 +0000
+++ getloadavg.c.new	2003-02-22 19:22:33.000000000 +0000
@@ -355,7 +355,7 @@
 
 /* LOAD_AVE_TYPE should only get defined if we're going to use the
    nlist method.  */
-# if !defined(LOAD_AVE_TYPE) && (defined(BSD) || defined(LDAV_CVT) || defined(KERNEL_FILE) || defined(LDAV_SYMBOL))
+# if !defined(LOAD_AVE_TYPE) && (defined(BSD) || defined(LDAV_CVT) || defined(KERNEL_FILE) || defined(LDAV_SYMBOL)) && !defined(__riscos__)
 #  define LOAD_AVE_TYPE double
 # endif
 
