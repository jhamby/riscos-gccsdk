--- lib/unistd.in.h.orig	2018-01-21 23:00:58.000000000 +1300
+++ lib/unistd.in.h	2018-03-27 20:49:38.320774108 +1300
@@ -718,7 +718,7 @@
    If N is 0, return the group count; otherwise, N describes how many
    entries are available in GROUPS.  Return -1 and set errno if N is
    not 0 and not large enough.  Fails with ENOSYS on some systems.  */
-# if @REPLACE_GETGROUPS@
+# if 0
 #  if !(defined __cplusplus && defined GNULIB_NAMESPACE)
 #   undef getgroups
 #   define getgroups rpl_getgroups
