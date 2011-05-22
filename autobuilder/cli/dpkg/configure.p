--- configure.orig	2011-05-22 10:52:25.000000000 +0100
+++ configure	2011-05-22 10:52:57.000000000 +0100
@@ -10620,7 +10620,7 @@
 WFLAGS="-Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers \
 	 -Wmissing-declarations -Wmissing-format-attribute \
 	 -Wformat-security -Wpointer-arith \
-	 -Wvla -Winit-self -Wwrite-strings -Wcast-align -Wshadow"
+	 -Winit-self -Wwrite-strings -Wcast-align -Wshadow"
 WCFLAGS="-Wdeclaration-after-statement -Wnested-externs -Wbad-function-cast \
 	 -Wstrict-prototypes -Wmissing-prototypes -Wold-style-definition"
 # Temporarily here until #542031 gets fixed in ncurses
