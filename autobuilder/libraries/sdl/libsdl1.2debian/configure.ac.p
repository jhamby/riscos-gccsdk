--- configure.ac.orig	2021-07-29 01:14:17.282895213 -0700
+++ configure.ac	2021-07-29 12:05:54.403186055 -0700
@@ -2445,6 +2445,30 @@
             have_timers=yes
         fi
         ;;
+    arm-*riscos*)
+        ARCH=riscos
+        CheckOSS
+        CheckPTHREAD
+        # Set up files for the video library
+        if test x$enable_video = xyes; then
+            AC_DEFINE(SDL_VIDEO_DRIVER_RISCOS)
+            SOURCES="$SOURCES $srcdir/src/video/riscos/*.c"
+            SOURCES="$SOURCES $srcdir/src/video/riscos/*.S"
+            have_video=yes
+        fi
+        # Set up files for the joystick library
+        if test x$enable_joystick = xyes; then
+            AC_DEFINE(SDL_JOYSTICK_RISCOS)
+            SOURCES="$SOURCES $srcdir/src/joystick/riscos/*.c"
+            have_joystick=yes
+        fi
+        # Set up files for the timer library
+        if test x$enable_timers = xyes; then
+            AC_DEFINE(SDL_TIMER_RISCOS)
+            SOURCES="$SOURCES $srcdir/src/timer/riscos/*.c"
+            have_timers=yes
+        fi
+        ;;
     *-*-linux*|*-*-uclinux*|*-*-gnu*|*-*-k*bsd*-gnu|*-*-bsdi*|*-*-freebsd*|*-*-dragonfly*|*-*-netbsd*|*-*-openbsd*|*-*-sysv5*|*-*-solaris*|*-*-hpux*|*-*-irix*|*-*-aix*|*-*-osf*)
         case "$host" in
             *-*-linux*)         ARCH=linux ;;
@@ -2927,30 +2951,6 @@
             have_timers=yes
         fi
         ;;
-    *-riscos)
-        ARCH=riscos
-        CheckOSS
-        CheckPTHREAD
-        # Set up files for the video library
-        if test x$enable_video = xyes; then
-            AC_DEFINE(SDL_VIDEO_DRIVER_RISCOS)
-            SOURCES="$SOURCES $srcdir/src/video/riscos/*.c"
-            SOURCES="$SOURCES $srcdir/src/video/riscos/*.S"
-            have_video=yes
-        fi
-        # Set up files for the joystick library
-        if test x$enable_joystick = xyes; then
-            AC_DEFINE(SDL_JOYSTICK_RISCOS)
-            SOURCES="$SOURCES $srcdir/src/joystick/riscos/*.c"
-            have_joystick=yes
-        fi
-        # Set up files for the timer library
-        if test x$enable_timers = xyes; then
-            AC_DEFINE(SDL_TIMER_RISCOS)
-            SOURCES="$SOURCES $srcdir/src/timer/riscos/*.c"
-            have_timers=yes
-        fi
-        ;;
     *)
         AC_MSG_ERROR([
 *** Unsupported host:  Please add to configure.ac
