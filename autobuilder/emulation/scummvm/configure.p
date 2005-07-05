Index: configure
===================================================================
RCS file: /cvsroot/scummvm/scummvm/configure,v
retrieving revision 1.125
diff -u -r1.125 configure
--- configure	2 Jul 2005 12:52:30 -0000	1.125
+++ configure	5 Jul 2005 21:46:57 -0000
@@ -450,7 +450,7 @@
 	_host_os=linux
 	_host_cpu=arm
 	;;
-arm-riscos-aof)
+arm-riscos)
 	_host_os=riscos
 	_host_cpu=arm
 	;;
@@ -473,7 +473,7 @@
 mingw* | cygwin*)
 	EXEEXT=".exe"
 	;;
-arm-riscos-aof)
+arm-riscos)
 	EXEEXT=",ff8"
 	;;
 *)
@@ -645,7 +645,7 @@
 if test -n "$_host"; then
 	# Cross-compiling mode - add your target here if needed
 	case "$_host" in
-		linupy|arm-riscos-aof)
+		linupy|arm-riscos)
 			echo "Cross-compiling to $_host, forcing endianness, alignment and type sizes"
 			DEFINES="$DEFINES -DUNIX"
 			_def_endianness='#define SCUMM_LITTLE_ENDIAN'
@@ -666,7 +666,7 @@
 			LDFLAGS="$LDFLAGS -newlib"
 			;;
 		*)
-			echo "Cross-compiling to unknown target, please add your target to configure."
+			echo "Cross-compiling to unknown target $_host, please add your target to configure."
 			exit 1
 			;;
 	esac
