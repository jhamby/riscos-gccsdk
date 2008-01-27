--- configure.orig	2008-01-26 16:56:27.000000000 -0800
+++ configure	2008-01-26 16:56:36.000000000 -0800
@@ -530,7 +530,7 @@
 	_host_os=linux
 	_host_cpu=arm
 	;;
-arm-riscos)
+arm-*riscos)
 	_host_os=riscos
 	_host_cpu=arm
 	;;
@@ -561,7 +561,7 @@
 mingw* | cygwin* | os2-emx*)
 	EXEEXT=".exe"
 	;;
-arm-riscos)
+arm-*riscos)
 	EXEEXT=",ff8"
 	;;
 gp2x-linux)
@@ -709,7 +709,7 @@
 if test -n "$_host"; then
 	# Cross-compiling mode - add your target here if needed
 	case "$_host" in
-		linupy|arm-riscos)
+		linupy|arm-*riscos)
 			echo "Cross-compiling to $_host, forcing endianness, alignment and type sizes"
 			DEFINES="$DEFINES -DUNIX"
 			_def_endianness='#define SCUMM_LITTLE_ENDIAN'
