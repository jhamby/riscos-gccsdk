--- configure.orig	2010-09-11 19:17:49.000000000 +0100
+++ configure	2010-09-11 19:18:00.000000000 +0100
@@ -10541,7 +10541,7 @@
 
 WFLAGS="-Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers \
 	 -Wmissing-declarations -Wmissing-format-attribute \
-	 -Wvla -Winit-self -Wwrite-strings -Wcast-align -Wshadow"
+	 -Winit-self -Wwrite-strings -Wcast-align -Wshadow"
 WCFLAGS="-Wdeclaration-after-statement -Wnested-externs -Wbad-function-cast \
 	 -Wstrict-prototypes -Wmissing-prototypes -Wold-style-definition"
 # Temporarily here until #542031 gets fixed in ncurses
