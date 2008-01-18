--- xft-config.in.orig	2007-12-30 16:14:28.000000000 -0800
+++ xft-config.in	2007-12-30 16:14:41.000000000 -0800
@@ -89,7 +89,7 @@
 fi
 
 if test "$echo_libs" = "yes" ; then
- libs="-lXft -lX11 ${freetypelibs} ${fontconfiglibs} ${xrenderlibs}"
+ libs="-lXft -lCX11 -lDesk ${freetypelibs} ${fontconfiglibs} ${xrenderlibs}"
  if test "${libdir}" != "/usr/lib" ; then
   echo -L${libdir} $libs
  else
