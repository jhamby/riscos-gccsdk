--- c/messages.orig	2003-12-13 09:42:56.000000000 -0800
+++ c/messages	2021-08-05 15:53:20.802500165 -0700
@@ -97,7 +97,7 @@
 
 const char *lookup_static(const char *token)
 {
-  return lookup_static_sub(token, 0, 0);
+  return lookup_static_var(token, 0, 0);
 }
 
 
