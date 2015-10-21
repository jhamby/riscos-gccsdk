--- ./defines.h.orig	2015-09-23 16:04:35.065552338 +0200
+++ ./defines.h	2015-09-23 16:04:35.081552338 +0200
@@ -572,21 +572,6 @@
 # define HAVE_GETADDRINFO
 #endif
 
-#ifndef HAVE_GETOPT_OPTRESET
-# undef getopt
-# undef opterr
-# undef optind
-# undef optopt
-# undef optreset
-# undef optarg
-# define getopt(ac, av, o)  BSDgetopt(ac, av, o)
-# define opterr             BSDopterr
-# define optind             BSDoptind
-# define optopt             BSDoptopt
-# define optreset           BSDoptreset
-# define optarg             BSDoptarg
-#endif
-
 #if defined(BROKEN_GETADDRINFO) && defined(HAVE_GETADDRINFO)
 # undef HAVE_GETADDRINFO
 #endif
