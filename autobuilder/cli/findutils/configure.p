--- configure.orig	2020-11-30 15:26:30.261509217 +0000
+++ configure	2020-11-30 15:26:49.356885683 +0000
@@ -32324,7 +32324,7 @@
 
 
   # Determine how to get the list of mounted file systems.
-  ac_list_mounted_fs=
+  ac_list_mounted_fs=found
 
   # If the getmntent function is available but not in the standard library,
   # make sure LIBS contains the appropriate -l option.
