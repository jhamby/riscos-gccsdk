--- dist/configure.old	2003-04-06 17:36:03.000000000 +0100
+++ dist/configure	2003-04-06 16:38:08.000000000 +0100
@@ -3248,6 +3248,7 @@
 # Don't override anything if it's already set from the environment.
 optimize_def="-O"
 case "$host_os" in
+aof*)	optimize_def="-O2" ;;
 aix4.3.*|aix5*)
 	optimize_def="-O2"
 	CC=${CC-"xlc_r"}
--- dist/configure.old	2004-09-15 17:41:30.000000000 +0100
+++ dist/configure	2004-09-15 17:42:20.000000000 +0100
@@ -9575,7 +9575,7 @@



-INSTALLER="\$(LIBTOOL) --mode=install cp -p"
+INSTALLER="\$(GCCSDK_INSTALL_ENV)/ro-install -p"

 MAKEFILE_CC="\$(LIBTOOL) --mode=compile ${MAKEFILE_CC}"
 MAKEFILE_SOLINK="\$(LIBTOOL) --mode=link ${MAKEFILE_CCLINK} -avoid-version"
