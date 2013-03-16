--- lib/unistd.in.h.orig	2010-06-08 18:16:34.202102620 +0100
+++ lib/unistd.in.h	2010-06-08 18:17:02.682156681 +0100
@@ -591,7 +591,7 @@
    entries are available in GROUPS.  Return -1 and set errno if N is
    not 0 and not large enough.  Fails with ENOSYS on some systems.  */
 # if @REPLACE_GETGROUPS@
-#  if !(defined __cplusplus && defined GNULIB_NAMESPACE)
+#  if !(defined __cplusplus && defined GNULIB_NAMESPACE) && !(defined __riscos__)
 #   undef getgroups
 #   define getgroups rpl_getgroups
 #  endif
