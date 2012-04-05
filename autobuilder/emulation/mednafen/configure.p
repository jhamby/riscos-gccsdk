--- configure.orig	2012-03-27 14:30:30.000000000 +0100
+++ configure	2012-03-27 14:32:05.000000000 +0100
@@ -17443,9 +17443,9 @@
 
 
 
- if test x$HAVE_GL_GL_H = xfalse; then
-  as_fn_error "*** OpenGL header file not found!" "$LINENO" 5
- fi
+# if test x$HAVE_GL_GL_H = xfalse; then
+#  as_fn_error "*** OpenGL header file not found!" "$LINENO" 5
+# fi
 fi
 
 
