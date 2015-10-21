--- ./openbsd-compat/openbsd-compat.h.orig	2015-09-23 16:04:35.249552336 +0200
+++ ./openbsd-compat/openbsd-compat.h	2015-09-23 16:04:35.261552336 +0200
@@ -149,11 +149,6 @@
 int getgrouplist(const char *, gid_t, gid_t *, int *);
 #endif
 
-#if !defined(HAVE_GETOPT) || !defined(HAVE_GETOPT_OPTRESET)
-int BSDgetopt(int argc, char * const *argv, const char *opts);
-#include "openbsd-compat/getopt.h"
-#endif
-
 #if defined(HAVE_DECL_WRITEV) && HAVE_DECL_WRITEV == 0
 # include <sys/types.h>
 # include <sys/uio.h>
