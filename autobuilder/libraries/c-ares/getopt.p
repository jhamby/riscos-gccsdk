--- acountry.c	2009-03-26 20:49:27.000000000 +0000
+++ acountry.c	2009-03-26 20:53:41.000000000 +0000
@@ -113,8 +113,8 @@
         Abort(usage);
       }
 
-  argc -= optind;
-  argv += optind;
+  argc -= ares_optind;
+  argv += ares_optind;
   if (argc < 1)
      Abort(usage);
 
--- ahost.c	2009-03-26 20:49:27.000000000 +0000
+++ ahost.c	2009-03-26 20:55:13.000000000 +0000
@@ -71,9 +71,9 @@
 #endif
           break;
         case 't':
-          if (!strcasecmp(optarg,"a"))
+          if (!strcasecmp(ares_optarg,"a"))
             addr_family = AF_INET;
-          else if (!strcasecmp(optarg,"aaaa"))
+          else if (!strcasecmp(ares_optarg,"aaaa"))
             addr_family = AF_INET6;
           else
             usage();
@@ -85,8 +85,8 @@
         }
     }
 
-  argc -= optind;
-  argv += optind;
+  argc -= ares_optind;
+  argv += ares_optind;
   if (argc < 1)
     usage();
 
--- ares_getopt.c	2009-03-26 20:49:27.000000000 +0000
+++ ares_getopt.c	2009-03-26 20:51:38.000000000 +0000
@@ -49,11 +49,11 @@
 #include <string.h>
 #include "ares_getopt.h"
 
-int   opterr = 1,     /* if error message should be printed */
-      optind = 1,     /* index into parent argv vector */
+int   ares_opterr = 1,     /* if error message should be printed */
+      ares_optind = 1,     /* index into parent argv vector */
       optopt,         /* character checked for validity */
       optreset;       /* reset getopt */
-char  *optarg;        /* argument associated with option */
+char  *ares_optarg;        /* argument associated with option */
 
 #define  BADCH   (int)'?'
 #define  BADARG  (int)':'
@@ -71,12 +71,12 @@
 
     if (optreset || !*place) {                /* update scanning pointer */
         optreset = 0;
-        if (optind >= nargc || *(place = nargv[optind]) != '-') {
+        if (ares_optind >= nargc || *(place = nargv[ares_optind]) != '-') {
             place = EMSG;
             return (EOF);
         }
         if (place[1] && *++place == '-') {    /* found "--" */
-            ++optind;
+            ++ares_optind;
             place = EMSG;
             return (EOF);
         }
@@ -90,34 +90,34 @@
         if (optopt == (int)'-')
             return (EOF);
         if (!*place)
-            ++optind;
-        if (opterr && *ostr != ':')
+            ++ares_optind;
+        if (ares_opterr && *ostr != ':')
             (void)fprintf(stderr,
                 "%s: illegal option -- %c\n", __FILE__, optopt);
         return (BADCH);
     }
     if (*++oli != ':') {                      /* don't need argument */
-        optarg = NULL;
+        ares_optarg = NULL;
         if (!*place)
-            ++optind;
+            ++ares_optind;
     }
     else {                                    /* need an argument */
         if (*place)                           /* no white space */
-            optarg = place;
-        else if (nargc <= ++optind) {         /* no arg */
+            ares_optarg = place;
+        else if (nargc <= ++ares_optind) {         /* no arg */
             place = EMSG;
             if (*ostr == ':')
                 return (BADARG);
-            if (opterr)
+            if (ares_opterr)
                 (void)fprintf(stderr,
                     "%s: option requires an argument -- %c\n",
                     __FILE__, optopt);
             return (BADCH);
         }
          else                                 /* white space */
-            optarg = nargv[optind];
+            ares_optarg = nargv[ares_optind];
         place = EMSG;
-        ++optind;
+        ++ares_optind;
     }
     return (optopt);                          /* dump back option letter */
 }
--- ares_getopt.h	2009-03-26 20:49:27.000000000 +0000
+++ ares_getopt.h	2009-03-26 20:49:49.000000000 +0000
@@ -33,9 +33,9 @@
 
 int ares_getopt(int nargc, char * const nargv[], const char *ostr);
 
-extern char *optarg;
-extern int optind;
-extern int opterr;
+extern char *ares_optarg;
+extern int ares_optind;
+extern int ares_opterr;
 
 
 #endif /* ARES_GETOPT_H */
--- adig.c.orig	2008-10-18 06:32:02.000000000 -0700
+++ adig.c	2009-07-26 08:30:14.000000000 -0700
@@ -204,7 +204,7 @@
           /* Add a flag. */
           for (i = 0; i < nflags; i++)
             {
-              if (strcmp(flags[i].name, optarg) == 0)
+              if (strcmp(flags[i].name, ares_optarg) == 0)
                 break;
             }
           if (i == nflags)
@@ -214,12 +214,12 @@
 
         case 's':
           /* Add a server, and specify servers in the option mask. */
-          if (ares_inet_pton(AF_INET, optarg, &inaddr) <= 0)
+          if (ares_inet_pton(AF_INET, ares_optarg, &inaddr) <= 0)
             {
-              hostent = gethostbyname(optarg);
+              hostent = gethostbyname(ares_optarg);
               if (!hostent || hostent->h_addrtype != AF_INET)
                 {
-                  fprintf(stderr, "adig: server %s not found.\n", optarg);
+                  fprintf(stderr, "adig: server %s not found.\n", ares_optarg);
                   return 1;
                 }
               memcpy(&inaddr, hostent->h_addr, sizeof(struct in_addr));
@@ -241,7 +241,7 @@
           /* Set the query class. */
           for (i = 0; i < nclasses; i++)
             {
-              if (strcasecmp(classes[i].name, optarg) == 0)
+              if (strcasecmp(classes[i].name, ares_optarg) == 0)
                 break;
             }
           if (i == nclasses)
@@ -253,7 +253,7 @@
           /* Set the query type. */
           for (i = 0; i < ntypes; i++)
             {
-              if (strcasecmp(types[i].name, optarg) == 0)
+              if (strcasecmp(types[i].name, ares_optarg) == 0)
                 break;
             }
           if (i == ntypes)
@@ -263,23 +263,23 @@
 
         case 'T':
           /* Set the TCP port number. */
-          if (!ISDIGIT(*optarg))
+          if (!ISDIGIT(*ares_optarg))
             usage();
-          options.tcp_port = (unsigned short)strtol(optarg, NULL, 0);
+          options.tcp_port = (unsigned short)strtol(ares_optarg, NULL, 0);
           optmask |= ARES_OPT_TCP_PORT;
           break;
 
         case 'U':
           /* Set the UDP port number. */
-          if (!ISDIGIT(*optarg))
+          if (!ISDIGIT(*ares_optarg))
             usage();
-          options.udp_port = (unsigned short)strtol(optarg, NULL, 0);
+          options.udp_port = (unsigned short)strtol(ares_optarg, NULL, 0);
           optmask |= ARES_OPT_UDP_PORT;
           break;
         }
     }
-  argc -= optind;
-  argv += optind;
+  argc -= ares_optind;
+  argv += ares_optind;
   if (argc == 0)
     usage();
 
