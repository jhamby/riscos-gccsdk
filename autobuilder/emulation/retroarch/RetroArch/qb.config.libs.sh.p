--- qb/config.libs.sh.orig	2020-03-07 15:47:51.614027206 +0000
+++ qb/config.libs.sh	2020-03-07 15:51:21.082050413 +0000
@@ -46,6 +46,8 @@
    SOCKETLIB=-lws2_32
    SOCKETHEADER="#include <winsock2.h>"
    DYLIB=
+elif [ "$OS" = 'riscos' ]; then
+   DYLIB=-ldl
 elif [ "$OS" = 'Cygwin' ]; then
    die 1 'Error: Cygwin is not a supported platform. See https://bot.libretro.com/docs/compilation/windows/'
 elif [ "$OS" = 'SunOS' ]; then
@@ -126,10 +128,12 @@
    LIBRETRO="-lretro"
 fi
 
+if [ "$OS" != 'riscos' ]; then
 [ "$HAVE_DYNAMIC" = 'yes' ] || {
    check_lib '' RETRO "$LIBRETRO" retro_init "$DYLIB" '' '' 'Cannot find libretro, did you forget --with-libretro="-lretro"?'
    add_define MAKEFILE libretro "$LIBRETRO"
 }
+fi
 
 add_define MAKEFILE ASSETS_DIR "${ASSETS_DIR:-$SHARE_DIR}/retroarch"
 add_define MAKEFILE BIN_DIR "${BIN_DIR:-${PREFIX}/bin}"
@@ -151,6 +155,8 @@
 
 if [ "$OS" = 'Win32' ]; then
    add_opt DYLIB yes
+elif [ "$OS" = 'riscos' ]; then
+   add_opt DYLIB yes
 else
    check_lib '' DYLIB "$DYLIB" dlopen
 fi
@@ -407,7 +413,9 @@
 fi
 
 if [ "$OS" != 'Win32' ]; then
-   check_lib '' DYNAMIC "$DYLIB" dlopen
+   if [ "$OS" != 'riscos' ]; then
+      check_lib '' DYNAMIC "$DYLIB" dlopen
+   fi
 fi
 
 if [ "$HAVE_KMS" != "no" ]; then
@@ -419,6 +427,10 @@
 check_enabled GBM KMS KMS 'GBM is' true
 check_enabled EGL KMS KMS 'EGL is' true
 
+if [ "$OS" = 'riscos' ]; then
+   EXTRA_GL_LIBS="-lbcm_host"
+fi
+
 if [ "$HAVE_EGL" = "yes" ]; then
    if [ "$HAVE_OPENGLES" != "no" ]; then
       if [ "$OPENGLES_LIBS" ] || [ "$OPENGLES_CFLAGS" ]; then
