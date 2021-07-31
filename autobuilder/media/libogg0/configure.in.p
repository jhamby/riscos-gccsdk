--- configure.in.orig	2014-05-28 19:38:26.000000000 +0100
+++ configure.in	2019-06-07 21:05:24.353968005 +0100
@@ -58,6 +58,18 @@
 		CFLAGS="-fno-common -O4 -Wall -fsigned-char -ffast-math"
 		PROFILE="-fno-common -O4 -Wall -pg -g -fsigned-char -ffast-math"
 		;;
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
                 DEBUG="-g -Wall -fsigned-char"
                 CFLAGS="-O20 -fsigned-char"
