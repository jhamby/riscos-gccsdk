--- configure.ac.orig	2023-07-31 13:23:00.386863019 -0700
+++ configure.ac	2023-07-31 13:29:53.341656033 -0700
@@ -542,6 +542,12 @@
 	*-*-linux*)
 		ac_sys_system=Linux
 		;;
+	*-*-riscos*)
+		ac_sys_system=Linux
+		;;
+	*-*-gnueabihf*)
+		ac_sys_system=Linux
+		;;
 	*-*-cygwin*)
 		ac_sys_system=Cygwin
 		;;
@@ -597,6 +603,9 @@
 			_host_cpu=$host_cpu
 		esac
 		;;
+	arm*-*-*)
+		_host_cpu=arm
+		;;
 	*-*-cygwin*)
 		_host_cpu=
 		;;
