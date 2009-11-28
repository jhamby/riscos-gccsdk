--- libssp/configure.ac.orig	2009-11-28 17:23:37.000000000 +0000
+++ libssp/configure.ac	2009-11-28 17:22:55.000000000 +0000
@@ -40,7 +40,14 @@ AC_LANG_C
 m4_rename([_AC_ARG_VAR_PRECIOUS],[real_PRECIOUS])
 m4_define([_AC_ARG_VAR_PRECIOUS],[])
 AC_PROG_CC
-m4_rename([real_PRECIOUS],[_AC_ARG_VAR_PRECIOUS])
+
+# The following line is the equivalent of:
+#
+# m4_rename_force([real_PRECIOUS],[_AC_ARG_VAR_PRECIOUS])
+#
+# however, m4_rename_force is only defined in newer versions of autoconf and 
+# not in autoconf2.59.
+m4_ifdef([_AC_ARG_VAR_PRECIOUS], [m4_undefine([_AC_ARG_VAR_PRECIOUS])])m4_rename([real_PRECIOUS],[_AC_ARG_VAR_PRECIOUS])
 
 AC_SUBST(CFLAGS)
 
