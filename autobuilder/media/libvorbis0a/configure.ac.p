--- configure.ac.orig	2015-01-12 20:00:37.000000000 +0000
+++ configure.ac	2016-08-06 18:32:04.920104701 +0100
@@ -205,6 +205,18 @@
 		DEBUG="-g -Wall -W -D_REENTRANT -D__NO_MATH_INLINES -fsigned-char"
 		CFLAGS="-O3 -Wall -W -ffast-math -D_REENTRANT -fsigned-char"
 		PROFILE="-pg -g -O3 -ffast-math -D_REENTRANT -fsigned-char";;
+	*-*-riscos*)
+		if test "x$ENABLE_VFP" = "xyes"; then
+			DEBUG="-g -Wall -Wextra -mfpu=vfp"
+			CFLAGS="-O3 -Wall -Wextra -mfpu=vfp"
+			LDFLAGS="-mfpu=vfp"
+			PROFILE="-O3 -g -pg -mfpu=vfp"
+		else
+			DEBUG="-g -Wall -Wextra"
+			CFLAGS="-O3 -Wall -Wextra"
+			PROFILE="-O3 -g -pg"
+		fi
+		;;
  	*)
 		DEBUG="-g -Wall -Wextra -D__NO_MATH_INLINES -fsigned-char"
 		CFLAGS="-O3 -Wall -Wextra -D__NO_MATH_INLINES -fsigned-char"
