--- sdl/configure.orig	2012-04-10 10:55:32.000000000 +0100
+++ sdl/configure	2012-04-10 10:59:07.000000000 +0100
@@ -5704,8 +5704,9 @@
 # Check whether right shift is arithmetic or not
 
 
-
-
+       if test "${snes9x_sar_int8+set}" = set; then :
+  $as_echo_n "(cached) " >&6
+else
 	{ $as_echo "$as_me:${as_lineno-$LINENO}: checking whether right shift int8 is arithmetic" >&5
 $as_echo_n "checking whether right shift int8 is arithmetic... " >&6; }
 
@@ -5757,7 +5758,7 @@
   conftest.$ac_objext conftest.beam conftest.$ac_ext
 fi
 
-
+fi
 	CXXFLAGS="$OLD_CXXFLAGS"
 
 	if test "x$snes9x_sar_int8" = "xno"; then
@@ -5769,7 +5770,9 @@
 $as_echo "yes" >&6; }
 	fi
 
-
+       if test "${snes9x_sar_int16+set}" = set; then :
+  $as_echo_n "(cached) " >&6
+else
 	{ $as_echo "$as_me:${as_lineno-$LINENO}: checking whether right shift int16 is arithmetic" >&5
 $as_echo_n "checking whether right shift int16 is arithmetic... " >&6; }
 
@@ -5821,7 +5824,7 @@
   conftest.$ac_objext conftest.beam conftest.$ac_ext
 fi
 
-
+fi
 	CXXFLAGS="$OLD_CXXFLAGS"
 
 	if test "x$snes9x_sar_int16" = "xno"; then
@@ -5834,6 +5837,9 @@
 	fi
 
 
+       if test "${snes9x_sar_int32+set}" = set; then :
+  $as_echo_n "(cached) " >&6
+else
 	{ $as_echo "$as_me:${as_lineno-$LINENO}: checking whether right shift int32 is arithmetic" >&5
 $as_echo_n "checking whether right shift int32 is arithmetic... " >&6; }
 
@@ -5885,7 +5891,7 @@
   conftest.$ac_objext conftest.beam conftest.$ac_ext
 fi
 
-
+fi
 	CXXFLAGS="$OLD_CXXFLAGS"
 
 	if test "x$snes9x_sar_int32" = "xno"; then
@@ -5898,6 +5904,9 @@
 	fi
 
 
+       if test "${snes9x_sar_int64+set}" = set; then :
+  $as_echo_n "(cached) " >&6
+else
 	{ $as_echo "$as_me:${as_lineno-$LINENO}: checking whether right shift int64 is arithmetic" >&5
 $as_echo_n "checking whether right shift int64 is arithmetic... " >&6; }
 
@@ -5949,7 +5958,7 @@
   conftest.$ac_objext conftest.beam conftest.$ac_ext
 fi
 
-
+fi
 	CXXFLAGS="$OLD_CXXFLAGS"
 
 	if test "x$snes9x_sar_int64" = "xno"; then
