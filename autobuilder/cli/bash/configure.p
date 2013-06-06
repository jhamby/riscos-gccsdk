--- configure.orig	2013-06-06 12:16:32.986994185 +0100
+++ configure	2013-06-06 12:16:54.790933021 +0100
@@ -5749,7 +5749,7 @@
 		# dynamic version
 		case "${host_os}" in
 		darwin[89]*|darwin10*) READLINE_LIB='${READLINE_LIBRARY}' ;;
-		*)		READLINE_LIB=-lreadline ;;
+		*)		READLINE_LIB='-lreadline -lncurses';;
 		esac
 	fi
 else
