--- gas/config/tc-arm.c.orig	2007-10-09 23:47:36.000000000 +0200
+++ gas/config/tc-arm.c	2007-10-09 23:38:14.000000000 +0200
@@ -3842,7 +3842,7 @@
   unsigned const char *upat = pattern;
   char *backtrack_pos = 0;
   const char *backtrack_error = 0;
-  int i, val, backtrack_index = 0;
+  int i, val = 0, backtrack_index = 0;
 
 #define po_char_or_fail(chr) do {		\
   if (skip_past_char (&str, chr) == FAIL)	\
