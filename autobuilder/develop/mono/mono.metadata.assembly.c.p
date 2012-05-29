--- mono/metadata/assembly.c.orig	2011-12-19 21:10:24.000000000 +0000
+++ mono/metadata/assembly.c	2012-05-28 19:38:35.000000000 +0100
@@ -45,6 +45,11 @@
 #include <mach-o/dyld.h>
 #endif
 
+#ifdef __riscos__
+#include <unixlib/local.h>
+#include <sys/param.h>
+#endif
+
 /* AssemblyVersionMap: an assembly name and the assembly version set on which it is based */
 typedef struct  {
 	const char* assembly_name;
@@ -1375,7 +1380,11 @@
 			fname = g_strdup (filename);
 		}
 	} else {
+#ifdef __riscos__
+		fname = __unixify_std (filename, NULL, 0, 0);
+#else
 		fname = g_strdup (filename);
+#endif
 	}
 
 	mono_trace (G_LOG_LEVEL_INFO, MONO_TRACE_ASSEMBLY,
