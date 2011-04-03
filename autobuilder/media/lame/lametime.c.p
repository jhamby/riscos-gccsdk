--- frontend/lametime.c.orig	2011-02-18 23:52:05.000000000 +0000
+++ frontend/lametime.c	2011-02-18 23:53:19.000000000 +0000
@@ -148,7 +148,8 @@
 
 #if defined(__riscos__)
 # include <kernel.h>
-# include <sys/swis.h>
+# include <swis.h>
+# include <sys/stat.h>
 #elif defined(_WIN32)
 # include <sys/types.h>
 # include <sys/stat.h>
