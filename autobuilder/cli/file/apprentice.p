--- src/apprentice.c.orig	2003-05-18 22:01:48.000000000 +0100
+++ src/apprentice.c	2003-05-18 21:03:16.000000000 +0100
@@ -159,12 +161,14 @@
 		return rv;
 	}
 #ifndef COMPILE_ONLY
+#ifndef __riscos__
 	if ((rv = apprentice_map(ms, &magic, &nmagic, fn)) == -1) {
 		if (ms->flags & MAGIC_CHECK)
 			file_magwarn("Using regular magic file `%s'", fn);
 		rv = apprentice_file(ms, &magic, &nmagic, fn, action);
 		mapped = 0;
 	}
+#endif
 
 	if (rv == -1)
 		return rv;
