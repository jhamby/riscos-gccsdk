--- fujiturn.c.orig	2008-01-18 11:32:02.000000000 -0800
+++ fujiturn.c	2008-01-18 11:34:30.000000000 -0800
@@ -18,6 +18,7 @@
 
 #ifdef _16BIT
 typedef unsigned short value;
+#include <arpa/inet.h>
 #else
 typedef unsigned char value;
 #define ntohs(x) (x)
