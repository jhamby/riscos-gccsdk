--- memtester-4.0.5/memtester.c.org	2005-03-14 15:02:30.000000000 +0100
+++ memtester-4.0.5/memtester.c	2006-01-10 01:27:43.000000000 +0100
@@ -84,7 +84,6 @@
     ptrdiff_t pagesizemask;
     void volatile *buf, *aligned;
     ulv *bufa, *bufb;
-    int do_mlock = 1, done_mem = 0;
     int exit_code = 0;

     printf("memtester version " __version__ " (%d-bit)\n", UL_LEN);
@@ -116,59 +115,22 @@
     printf("want %lluMB (%llu bytes)\n", (ull) wantmb, (ull) wantbytes);
     buf = NULL;

-    while (!done_mem) {
-        while (!buf && wantbytes) {
-            buf = (void volatile *) malloc(wantbytes);
-            if (!buf) wantbytes -= pagesize;
-        }
-        bufsize = wantbytes;
-        printf("got  %lluMB (%llu bytes)", (ull) wantbytes >> 20,
-            (ull) wantbytes);
-        fflush(stdout);
-        if (do_mlock) {
-            printf(", trying mlock ...");
-            fflush(stdout);
-            if ((size_t) buf % pagesize) {
-                /* printf("aligning to page\n"); */
-                aligned = (void volatile *) ((size_t) buf & pagesizemask);
-                bufsize -= ((size_t) aligned - (size_t) buf);
-            } else {
-                aligned = buf;
-            }
-            /* Try memlock */
-            if (mlock((void *) aligned, bufsize) < 0) {
-                switch(errno) {
-                    case ENOMEM:
-                        printf("too many pages, reducing...\n");
-                        free((void *) buf);
-                        buf = NULL;
-                        wantbytes -= pagesize;
-                        break;
-                    case EPERM:
-                        printf("insufficient permission.\n");
-                        printf("Trying again, unlocked:\n");
-                        do_mlock = 0;
-                        free((void *) buf);
-                        buf = NULL;
-                        wantbytes = wantbytes_orig;
-                        break;
-                    default:
-                        printf("failed for unknown reason.\n");
-                        do_mlock = 0;
-                        done_mem = 1;
-                }
-            } else {
-                printf("locked.\n");
-                done_mem = 1;
-            }
-        } else {
-            done_mem = 1;
-            printf("\n");
-        }
+    while (!buf && wantbytes) {
+        buf = (void volatile *) malloc(wantbytes);
+        if (!buf) wantbytes -= pagesize;
     }
-
-    if (!do_mlock) fprintf(stderr, "Continuing with unlocked memory; testing "
-        "will be slower and less reliable.\n");
+    bufsize = wantbytes;
+    printf("got  %lluMB (%llu bytes)", (ull) wantbytes >> 20,
+        (ull) wantbytes);
+    fflush(stdout);
+    if ((size_t) buf % pagesize) {
+        /* printf("aligning to page\n"); */
+        aligned = (void volatile *) ((size_t) buf & pagesizemask);
+        bufsize -= ((size_t) aligned - (size_t) buf);
+    } else {
+        aligned = buf;
+    }
+    printf("\n");

     halflen = bufsize / 2;
     count = halflen / sizeof(ul);
@@ -201,7 +163,6 @@
         printf("\n");
         fflush(stdout);
     }
-    if (do_mlock) munlock((void *) aligned, bufsize);
     printf("Done.\n");
     fflush(stdout);
     exit(exit_code);
