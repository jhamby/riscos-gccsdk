--- ./source/src/emaillog.c.orig	2009-12-08 14:17:29.000000000 -0800
+++ ./source/src/emaillog.c	2009-12-08 14:18:02.000000000 -0800
@@ -83,21 +83,22 @@
   char end[] = "\nEND ";
   int in_fd = 0;
   off_t fsize;
+  int result;
 
   if (never_send_email) return 0;
 
   if (dcc_open_read(fname, &in_fd, &fsize))
       return 1;
 
-  write(email_fileno, begin, strlen(begin));
-  write(email_fileno, description, strlen(description));
-  write(email_fileno, "\n", 1);
+  result = write(email_fileno, begin, strlen(begin));
+  result = write(email_fileno, description, strlen(description));
+  result = write(email_fileno, "\n", 1);
 
   dcc_pump_readwrite(email_fileno, in_fd, fsize);
 
-  write(email_fileno, end, strlen(end));
-  write(email_fileno, description, strlen(description));
-  write(email_fileno, "\n", 1);
+  result = write(email_fileno, end, strlen(end));
+  result = write(email_fileno, description, strlen(description));
+  result = write(email_fileno, "\n", 1);
 
   close(in_fd);
 
--- ./source/src/trace.c.orig	2009-12-08 14:18:18.000000000 -0800
+++ ./source/src/trace.c	2009-12-08 14:19:50.000000000 -0800
@@ -319,6 +319,7 @@
     /* NOTE NO TRAILING NUL */
     char buf[4090];
     size_t len;
+    int result;
 
     rs_format_msg(buf, sizeof buf, flags, fn, fmt, va);
 
@@ -327,7 +328,7 @@
         len = (int) sizeof buf - 2;
     strcpy(&buf[len], "\n");
 
-    (void) write(log_fd, buf, len+1);
+    result = write(log_fd, buf, len+1); result = result;
 }
 
 
--- ./source/src/util.c.orig	2009-12-08 14:20:03.000000000 -0800
+++ ./source/src/util.c	2009-12-08 14:20:57.000000000 -0800
@@ -420,12 +420,13 @@
     static char buf[MAXPATHLEN];
     unsigned len;
     char *p, *slash;
+    void *result;
 
     if (*path == '/')
         len = 0;
     else {
 #ifdef HAVE_GETCWD
-        getcwd(buf, sizeof buf);
+        result = getcwd(buf, sizeof buf);
 #else
         getwd(buf);
 #endif
--- ./source/src/exec.c.orig	2009-12-08 14:21:19.000000000 -0800
+++ ./source/src/exec.c	2009-12-08 14:32:05.000000000 -0800
@@ -489,7 +489,7 @@
         rs_trace("%s child %ld terminated with status %#x",
                  what, (long) ret_pid, *wait_status);
         rs_log_info("%s times: user %ld.%06lds, system %ld.%06lds, "
-                    "%ld minflt, %ld majflt",
+                    "%d minflt, %d majflt",
                     what,
                     ru.ru_utime.tv_sec, (long) ru.ru_utime.tv_usec,
                     ru.ru_stime.tv_sec, (long) ru.ru_stime.tv_usec,
@@ -519,7 +519,7 @@
                 rs_log_error("Client fd disconnected, killing job");
                 /* If killpg fails, it might means the child process is not
                  * in a new group, so, just kill the child process */
-                if (killpg(pid,SIGTERM)!=0)
+                //if (killpg(pid,SIGTERM)!=0)
                     kill(pid, SIGTERM);
                 return EXIT_IO_ERROR;
             } else if (nread == 1) {
@@ -531,7 +531,7 @@
         poll(NULL, 0, 1000);
     }
     /* If timeout, also kill the child process */
-    if (killpg(pid,SIGTERM) !=0 )
+    //if (killpg(pid,SIGTERM) !=0 )
         kill(pid, SIGTERM);
     rs_log_error("Compilation takes too long, timeout.");
 
--- ./source/src/dparent.c.orig	2009-12-08 14:32:19.000000000 -0800
+++ ./source/src/dparent.c	2009-12-08 14:32:30.000000000 -0800
@@ -329,7 +329,7 @@
 {
     int i;
     pid_t pid;
-    pid_t sid;
+//    pid_t sid;
 
     dcc_ignore_sighup();
 
--- ./source/src/ncpus.c.orig	2009-12-08 14:32:43.000000000 -0800
+++ ./source/src/ncpus.c	2009-12-08 14:33:44.000000000 -0800
@@ -133,6 +133,8 @@
 #elif defined(_SC_NPROC_ONLN)
     /* IRIX */
     *ncpus = sysconf(_SC_NPROC_ONLN);
+#elif defined(__riscos__)
+    *ncpus = 1;
 #else
 #warning "Please port this function"
     *ncpus = -1;                /* unknown */
