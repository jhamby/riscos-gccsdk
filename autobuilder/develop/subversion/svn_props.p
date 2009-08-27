--- subversion/include/svn_props.h.orig	2009-01-30 18:11:29.000000000 +0100
+++ subversion/include/svn_props.h	2009-08-13 20:00:15.000000000 +0200
@@ -276,6 +276,9 @@
 /** Set to either TRUE or FALSE if we want a file to be executable or not. */
 #define SVN_PROP_EXECUTABLE  SVN_PROP_PREFIX "executable"
 
+/** Set to RISC OS filetype. */
+#define SVN_PROP_FILETYPE  SVN_PROP_PREFIX "riscosfiletype"
+
 /** The value to force the executable property to when set.
  *
  * @deprecated Provided for backward compatibility with the 1.4 API.
