--- rockdodger_globals.c.orig	2013-12-28 16:42:18.000000000 +0000
+++ rockdodger_globals.c	2014-04-11 12:52:07.000000000 +0100
@@ -109,7 +109,11 @@
   char buf[256];
   struct Setup_Data *setupptr;
 
+#ifdef __riscos__
+  snprintf(buf, sizeof(buf) - 1, "/%s/.rockdodger", getenv("Choices$Write"));
+#else
   snprintf(buf, sizeof(buf) - 1, "%s/.rockdodger", getenv("HOME"));
+#endif
   if((f = fopen(buf, "w")) == NULL) return;
 #ifdef DEBUG
   fprintf(stderr, "Writing config to '%s'.\n", buf);
@@ -148,7 +152,11 @@
   char inpbuf[32];
   int x0, x1;
 
+#ifdef __riscos__
+  snprintf(buf, sizeof(buf) - 1, "/%s/.rockdodger", getenv("Choices$Write"));
+#else
   snprintf(buf, sizeof(buf) - 1, "%s/.rockdodger", getenv("HOME"));
+#endif
   if(!(f = fopen(buf, "r"))) return 0;
   while(fgets(buf, sizeof(buf), f) != NULL) {
     if(strlen(buf) == 0 || buf[0] == '#') continue;
