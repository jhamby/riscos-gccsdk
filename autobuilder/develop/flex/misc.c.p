--- misc.c.orig	2014-03-27 01:46:44.000000000 +1300
+++ misc.c	2015-04-10 11:53:58.890835000 +1200
@@ -49,7 +49,8 @@
 #define CMD_IF_BISON_BRIDGE  "%if-bison-bridge"
 #define CMD_IF_NOT_BISON_BRIDGE  "%if-not-bison-bridge"
 #define CMD_ENDIF            "%endif"
-
+/* RISC OS temp file instead of stdout */
+extern FILE* flout;
 /* we allow the skeleton to push and pop. */
 struct sko_state {
     bool dc; /**< do_copy */
@@ -672,60 +673,60 @@
 void out (str)
      const char *str;
 {
-	fputs (str, stdout);
+	fputs (str, flout);
 }
 
 void out_dec (fmt, n)
      const char *fmt;
      int n;
 {
-	fprintf (stdout, fmt, n);
+	fprintf (flout, fmt, n);
 }
 
 void out_dec2 (fmt, n1, n2)
      const char *fmt;
      int n1, n2;
 {
-	fprintf (stdout, fmt, n1, n2);
+	fprintf (flout, fmt, n1, n2);
 }
 
 void out_hex (fmt, x)
      const char *fmt;
      unsigned int x;
 {
-	fprintf (stdout, fmt, x);
+	fprintf (flout, fmt, x);
 }
 
 void out_str (fmt, str)
      const char *fmt, str[];
 {
-	fprintf (stdout,fmt, str);
+	fprintf (flout,fmt, str);
 }
 
 void out_str3 (fmt, s1, s2, s3)
      const char *fmt, s1[], s2[], s3[];
 {
-	fprintf (stdout,fmt, s1, s2, s3);
+	fprintf (flout,fmt, s1, s2, s3);
 }
 
 void out_str_dec (fmt, str, n)
      const char *fmt, str[];
      int n;
 {
-	fprintf (stdout,fmt, str, n);
+	fprintf (flout,fmt, str, n);
 }
 
 void outc (c)
      int c;
 {
-	fputc (c, stdout);
+	fputc (c, flout);
 }
 
 void outn (str)
      const char *str;
 {
-	fputs (str,stdout);
-    fputc('\n',stdout);
+	fputs (str,flout);
+    fputc('\n',flout);
 }
 
 /** Print "m4_define( [[def]], [[val]])m4_dnl\n".
@@ -736,7 +737,7 @@
 void out_m4_define (const char* def, const char* val)
 {
     const char * fmt = "m4_define( [[%s]], [[%s]])m4_dnl\n";
-    fprintf(stdout, fmt, def, val?val:"");
+    fprintf(flout, fmt, def, val?val:"");
 }
 
 
