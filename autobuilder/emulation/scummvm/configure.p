--- configure.orig	2010-11-18 22:17:33.000000000 +0000
+++ configure	2010-11-18 22:20:12.000000000 +0000
@@ -365,7 +365,7 @@
 #
 get_system_exe_extension() {
 	case $1 in
-	arm-riscos)
+	arm-*riscos)
 		_exeext=",ff8"
 		;;
 	dreamcast | ds | gamecube | n64 | ps2 | psp | wii)
@@ -969,7 +969,7 @@
 	_host_cpu=arm
 	_host_alias=arm-oe-linux-androideabi
 	;;
-arm-riscos)
+arm-*riscos)
 	_host_os=riscos
 	_host_cpu=arm
 	;;
@@ -1634,7 +1634,7 @@
 			add_line_to_config_mk 'USE_ARM_COSTUME_ASM = 1'
 			add_line_to_config_mk 'USE_ARM_SCALER_ASM = 1'
 			;;
-		arm-riscos|linupy)
+		arm-*riscos|linupy)
 			DEFINES="$DEFINES -DLINUPY"
 			_unix=yes
 			_need_memalign=yes
