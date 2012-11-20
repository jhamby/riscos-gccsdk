--- test/mocklibc/src/netgroup.c.orig	2012-09-23 10:40:06.000000000 +0100
+++ test/mocklibc/src/netgroup.c	2012-09-23 10:38:13.000000000 +0100
@@ -19,6 +19,7 @@
 #include "netgroup.h"

 #include <ctype.h>
+#include <stddef.h>
 #include <regex.h>
 #include <stdio.h>
 #include <stdlib.h>
