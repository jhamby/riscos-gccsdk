--- subversion/include/svn_props.h.orig	2008-05-21 14:24:24.000000000 -0700
+++ subversion/include/svn_props.h	2008-12-27 22:13:42.000000000 -0800
@@ -247,6 +247,9 @@
 /** Set to either TRUE or FALSE if we want a file to be executable or not. */
 #define SVN_PROP_EXECUTABLE  SVN_PROP_PREFIX "executable"
 
+/** Set to RISC OS filetype. */
+#define SVN_PROP_FILETYPE  SVN_PROP_PREFIX "riscosfiletype"
+
 /** The value to force the executable property to when set.
  *
  * @deprecated Provided for backward compatibility with the 1.4 API.
