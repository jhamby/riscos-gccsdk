--- ./openbsd-compat/getopt.h.orig	2015-09-23 16:04:35.277552336 +0200
+++ ./openbsd-compat/getopt.h	2015-09-23 16:04:35.289552336 +0200
@@ -58,17 +58,4 @@
 	    const struct option *, int *);
 int	 getopt_long_only(int, char * const *, const char *,
 	    const struct option *, int *);
-#ifndef _GETOPT_DEFINED_
-#define _GETOPT_DEFINED_
-int	 getopt(int, char * const *, const char *);
-int	 getsubopt(char **, char * const *, char **);
-
-extern   char *optarg;                  /* getopt(3) external variables */
-extern   int opterr;
-extern   int optind;
-extern   int optopt;
-extern   int optreset;
-extern   char *suboptarg;               /* getsubopt(3) external variable */
-#endif
- 
 #endif /* !_GETOPT_H_ */
