--- configure.orig	2012-04-06 17:14:54.000000000 +0100
+++ configure	2012-04-06 17:19:06.000000000 +0100
@@ -1,7 +1,7 @@
 #!/bin/sh
 
 VERSION=2.10
-prefix=/usr/local
+prefix=$GCCSDK_INSTALL_ENV
 configured_profiles=
 while test x$1 != x; do
         case $1 in
