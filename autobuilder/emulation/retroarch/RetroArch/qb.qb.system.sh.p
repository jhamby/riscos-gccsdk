--- qb/qb.system.sh.orig	2018-01-21 10:06:31.597901454 +0000
+++ qb/qb.system.sh	2018-01-21 10:07:03.501383558 +0000
@@ -36,8 +36,7 @@
 		'CYGWIN'*) OS='Cygwin';;
 		'Haiku') OS='Haiku';;
 		'MINGW'*) OS='Win32';;
-		'SunOS') OS='SunOS';;
-		*) OS="Win32";;
+		*) OS="riscos";;
 	esac
 fi
 
