--- configure.ac.orig	2023-08-09 22:33:02.563068485 -0700
+++ configure.ac	2023-08-09 22:42:22.378317832 -0700
@@ -2391,6 +2391,32 @@
             have_timers=yes
         fi
         ;;
+    arm-*riscos*)
+        ARCH=riscos
+        CheckDiskAudio
+        CheckOSS
+        CheckDLOPEN
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
@@ -2878,34 +2904,6 @@
             have_timers=yes
         fi
         ;;
-    *-riscos)
-        ARCH=riscos
-        CheckDummyVideo
-        CheckDiskAudio
-        CheckDummyAudio
-        CheckOSS
-        CheckDLOPEN
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
     *-*-os2*)
         ARCH=os2
         if test "$build" != "$host"; then # cross-compiling
