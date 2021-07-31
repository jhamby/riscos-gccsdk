--- mono/metadata/assembly.c.orig	2013-07-30 02:47:35.000000000 +0100
+++ mono/metadata/assembly.c	2013-08-02 20:24:44.855899124 +0100
@@ -46,6 +46,11 @@
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
@@ -1464,7 +1469,11 @@
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
