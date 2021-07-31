--- configure.in.orig	2016-10-20 04:56:27.000000000 +0100
+++ configure.in	2016-11-20 17:19:59.233454279 +0000
@@ -3483,6 +3483,46 @@
             have_timers=yes
         fi
         ;;
+    arm-*-riscos* )
+        ARCH=linux
+        RPI_CFLAGS="-I$GCCSDK_INSTALL_ENV/vfp/include/interface/vmcs_host/linux -mfpu=vfp"
+        CFLAGS="$CFLAGS $RPI_CFLAGS"
+        SDL_CFLAGS="$SDL_CFLAGS $RPI_CFLAGS"
+        EXTRA_CFLAGS="$EXTRA_CFLAGS $RPI_CFLAGS"
+        EXTRA_LDFLAGS="$EXTRA_LDFLAGS -lbcm_host -ldl -mfpu=vfp"
+
+        CheckVisibilityHidden
+        CheckDummyAudio
+        CheckDLOPEN
+        CheckOSS
+        CheckOpenGLESX11
+        CheckPTHREAD
+        CheckClockGettime
+
+        if test x$enable_video = xyes; then
+            SOURCES="$SOURCES $srcdir/src/video/raspberry/*.c"
+            $as_echo "#define SDL_VIDEO_DRIVER_RPI 1" >>confdefs.h
+            SUMMARY_video="${SUMMARY_video} raspberry"
+        fi
+        # Set up files for the filesystem library
+        if test x$enable_filesystem = xyes; then
+            AC_DEFINE(SDL_FILESYSTEM_UNIX, 1, [ ])
+            SOURCES="$SOURCES $srcdir/src/filesystem/unix/*.c"
+            have_filesystem=yes
+        fi
+        # Set up files for the joystick library
+        if test x$enable_joystick = xyes; then
+            AC_DEFINE(SDL_JOYSTICK_DUMMY, 1, [ ])
+            SOURCES="$SOURCES $srcdir/src/joystick/dummy/*.c"
+            have_joystick=yes
+        fi
+        # Set up files for the timer library
+        if test x$enable_timers = xyes; then
+            AC_DEFINE(SDL_TIMER_UNIX, 1, [ ])
+            SOURCES="$SOURCES $srcdir/src/timer/unix/*.c"
+            have_timers=yes
+        fi
+        ;;
     *)
         AC_MSG_ERROR([
 *** Unsupported host:  Please add to configure.in
