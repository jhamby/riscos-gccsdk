--- os_dep.h  2002-04-06 17:51:24.000000000 +0100
+++ os_dep.h  2002-08-02 21:43:08.000000000 +0100
@@ -123,6 +123,7 @@
 
 static inline int dir_sep(char x) { return x == '/' || x == '\\'; }
 #define NEWLINE        "\n"
+#define NO_ASYNC_LOOKUP
 #define SYSTEM_ID SYS_RISCOS
 #define SYSTEM_NAME "RISC OS"
 #define DEFAULT_SHELL "gos"
