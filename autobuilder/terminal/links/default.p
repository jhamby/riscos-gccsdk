--- default.c.old	2002-12-16 18:29:55.000000000 +0000
+++ default.c	2002-12-16 18:29:54.000000000 +0000
@@ -4,7 +4,7 @@
 {
 	FILE *f;
 	unsigned char *p;
-#ifdef HAVE_POPEN
+#if defined(HAVE_POPEN) && !defined(__riscos__)
 	memset(system_name, 0, MAX_STR_LEN);
 	if (!(f = popen("uname -srm", "r"))) goto fail;
 	if (fread(system_name, 1, MAX_STR_LEN - 1, f) <= 0) {
