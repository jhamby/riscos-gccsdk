--- configure.orig	2009-07-31 00:35:58.000000000 +0200
+++ configure	2009-07-31 00:35:48.000000000 +0200
@@ -3637,7 +3637,7 @@ case " $build_configdirs " in
     # For an installed makeinfo, we require it to be from texinfo 4.4 or
     # higher, else we use the "missing" dummy.
     if ${MAKEINFO} --version \
-       | egrep 'texinfo[^0-9]*([1-3][0-9]|4\.[4-9]|[5-9])' >/dev/null 2>&1; then
+       | egrep 'texinfo[^0-9]*(4\.([6-9]|[1-9][0-9])|[5-9]|[1-9][0-9])' >/dev/null 2>&1; then
       :
     else
       MAKEINFO="$MISSING makeinfo"
