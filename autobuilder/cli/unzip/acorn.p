--- acorn/acorn.c	2007-12-23 15:49:30.000000000 +0000
+++ acorn/acorn.c	2012-07-23 20:15:01.000000000 +0100
@@ -27,7 +27,7 @@
 
 
 #define UNZIP_INTERNAL
-#include "^.unzip.h"
+#include "unzip.h"
 #include "riscos.h"
 
 #define FTYPE_FFF (1<<17)      /* set filetype to &FFF when extracting */
