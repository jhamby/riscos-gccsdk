--- src/misc.c.orig	2020-09-11 21:22:22.614018000 +1200
+++ src/misc.c	2020-09-11 22:11:17.699925650 +1200
@@ -49,6 +49,8 @@
 #define CMD_IF_NOT_BISON_BRIDGE  "%if-not-bison-bridge"
 #define CMD_ENDIF            "%endif"
 
+extern FILE* flout; /* RISC OS temp file instead of stdout */
+
 /* we allow the skeleton to push and pop. */
 struct sko_state {
     bool dc; /**< do_copy */
@@ -556,48 +558,48 @@
 
 void out (const char *str)
 {
-	fputs (str, stdout);
+	fputs (str, flout);
 }
 
 void out_dec (const char *fmt, int n)
 {
-	fprintf (stdout, fmt, n);
+	fprintf (flout, fmt, n);
 }
 
 void out_dec2 (const char *fmt, int n1, int n2)
 {
-	fprintf (stdout, fmt, n1, n2);
+	fprintf (flout, fmt, n1, n2);
 }
 
 void out_hex (const char *fmt, unsigned int x)
 {
-	fprintf (stdout, fmt, x);
+	fprintf (flout, fmt, x);
 }
 
 void out_str (const char *fmt, const char str[])
 {
-	fprintf (stdout,fmt, str);
+	fprintf (flout,fmt, str);
 }
 
 void out_str3 (const char *fmt, const char s1[], const char s2[], const char s3[])
 {
-	fprintf (stdout,fmt, s1, s2, s3);
+	fprintf (flout,fmt, s1, s2, s3);
 }
 
 void out_str_dec (const char *fmt, const char str[], int n)
 {
-	fprintf (stdout,fmt, str, n);
+	fprintf (flout,fmt, str, n);
 }
 
 void outc (int c)
 {
-	fputc (c, stdout);
+	fputc (c, flout);
 }
 
 void outn (const char *str)
 {
-	fputs (str,stdout);
-    fputc('\n',stdout);
+	fputs (str,flout);
+    fputc('\n',flout);
 }
 
 /** Print "m4_define( [[def]], [[val]])m4_dnl\n".
@@ -607,7 +609,7 @@
 void out_m4_define (const char* def, const char* val)
 {
     const char * fmt = "m4_define( [[%s]], [[%s]])m4_dnl\n";
-    fprintf(stdout, fmt, def, val?val:"");
+    fprintf(flout, fmt, def, val?val:"");
 }
 
 
