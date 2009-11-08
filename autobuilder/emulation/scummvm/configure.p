--- configure.orig	2009-08-21 13:48:06.000000000 -0700
+++ configure	2009-11-08 09:29:18.000000000 -0800
@@ -269,8 +269,8 @@
 	mingw* | *os2-emx | wince)
 		_exeext=".exe"
 		;;
-	arm-riscos)
-		_exeext=",ff8"
+	arm-*riscos)
+		_exeext=",e1f"
 		;;
 	gp2xwiz-linux)
 		_exeext=".wiz"
@@ -779,7 +779,7 @@
 	_host_os=linux
 	_host_cpu=arm
 	;;
-arm-riscos)
+arm-*riscos)
 	_host_os=riscos
 	_host_cpu=arm
 	;;
@@ -1171,7 +1171,7 @@
 if test -n "$_host"; then
 	# Cross-compiling mode - add your target here if needed
 	case "$_host" in
-		linupy|arm-riscos)
+		linupy|arm-*riscos)
 			echo "Cross-compiling to $_host, forcing endianness, alignment and type sizes"
 			DEFINES="$DEFINES -DUNIX"
 			_endian=little
