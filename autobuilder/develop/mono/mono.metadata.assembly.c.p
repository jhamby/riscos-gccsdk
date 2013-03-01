--- mono/metadata/assembly.c.orig	2013-01-08 18:41:07.000000000 +0000
+++ mono/metadata/assembly.c	2013-01-16 19:41:47.000000000 +0000
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
@@ -1408,7 +1413,11 @@
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
