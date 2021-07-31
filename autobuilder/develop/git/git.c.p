--- git.c.orig	2014-12-18 18:42:18.000000000 +0000
+++ git.c	2015-06-08 13:20:38.117081094 +0100
@@ -6,6 +6,20 @@
 #include "run-command.h"
 #include "commit.h"
 
+#ifdef __riscos__
+
+/* Set up unixlib to use unix paths and ,filetype extensions
+ * In future we may wish to use a property for the filetype and use
+ * an environmental variable git$filetypeext to use ,filetype in the
+ * same way as the RISC OS SVN port does.
+ */
+#include <unixlib/local.h>
+
+int __riscosify_control = __RISCOSIFY_STRICT_UNIX_SPECS | __RISCOSIFY_FILETYPE_EXT | __RISCOSIFY_FILETYPE_NOT_SET;
+int __feature_imagefs_is_file = 1;
+
+#endif
+
 const char git_usage_string[] =
 	"git [--version] [--help] [-C <path>] [-c name=value]\n"
 	"           [--exec-path[=<path>]] [--html-path] [--man-path] [--info-path]\n"
@@ -503,6 +517,14 @@
 	int i;
 	static const char ext[] = STRIP_EXTENSION;
 
+#ifdef __riscos__
+	i = strlen(argv[0]) - 4;
+	if (i > 0 && (!stricmp(argv[0] + i, ",ff8") || !stricmp(argv[0]+i,",e1f"))) {
+		char *argv0 = xstrdup(argv[0]);
+		argv[0] = cmd = argv0;
+		argv0[i] = '\0';
+	}
+#else
 	if (sizeof(ext) > 1) {
 		i = strlen(argv[0]) - strlen(ext);
 		if (i > 0 && !strcmp(argv[0] + i, ext)) {
@@ -511,6 +533,7 @@
 			argv0[i] = '\0';
 		}
 	}
+#endif
 
 	/* Turn "git cmd --help" into "git help cmd" */
 	if (argc > 1 && !strcmp(argv[1], "--help")) {
