--- data/bison.m4.orig	2015-01-17 03:47:42.000000000 +1300
+++ data/bison.m4	2015-03-10 21:49:38.152130548 +1300
@@ -114,7 +114,7 @@
 #
 #   b4_cat([[@complain(invalid input@)]])
 m4_define([b4_cat],
-[m4_syscmd([cat <<'_m4eof'
+[m4_syscmd([concat
 ]m4_bpatsubst(m4_dquote($1), [_m4eof], [_m4@`eof])[@
 _m4eof
 ])dnl
