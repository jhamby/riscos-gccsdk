--- src/apprentice.c.orig	2008-03-09 18:05:21.000000000 -0700
+++ src/apprentice.c	2008-03-09 18:05:37.000000000 -0700
@@ -533,7 +533,7 @@
 	uint32_t marraycount, i, mentrycount = 0;
 	size_t lineno = 0;
 
-	ms->flags |= MAGIC_CHECK;	/* Enable checks for parsed files */
+	//ms->flags |= MAGIC_CHECK;	/* Enable checks for parsed files */
 
 	f = fopen(ms->file = fn, "r");
 	if (f == NULL) {
