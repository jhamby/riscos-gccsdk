--- configure.orig	2011-02-05 15:59:29.000000000 +0000
+++ configure	2011-02-05 16:00:55.000000000 +0000
@@ -1784,7 +1784,7 @@
     esac
 }
 
-EXESUF=$(exesuf $target_os)
+EXESUF=$AB_EXEEXT
 HOSTEXESUF=$(exesuf $host_os)
 
 # set temporary file name
@@ -2799,7 +2799,7 @@
     die "ERROR: No version of libdc1394 found "
 fi
 
-SDL_CONFIG="${cross_prefix}sdl-config"
+SDL_CONFIG="${prefix}/bin/sdl-config"
 if "${SDL_CONFIG}" --version > /dev/null 2>&1; then
     sdl_cflags=$("${SDL_CONFIG}" --cflags)
     sdl_libs=$("${SDL_CONFIG}" --libs)
