--- configure.orig	2023-07-01 10:42:32.328452588 +0100
+++ configure	2023-07-01 10:43:02.003791179 +0100
@@ -3820,6 +3820,9 @@
 	*-*-linux*)
 		ac_sys_system=Linux
 		;;
+	*-*-riscos*)
+		ac_sys_system=Linux
+		;;
 	*-*-cygwin*)
 		ac_sys_system=Cygwin
 		;;
@@ -3876,6 +3876,15 @@
 			_host_cpu=$host_cpu
 		esac
 		;;
+	*-*-riscos*)
+		case "$host_cpu" in
+		arm*)
+			_host_cpu=arm
+			;;
+		*)
+			_host_cpu=$host_cpu
+		esac
+		;;
 	*-*-cygwin*)
 		_host_cpu=
 		;;
