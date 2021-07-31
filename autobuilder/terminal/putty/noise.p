--- unix/uxnoise.c.orig	2009-01-13 11:48:57.000000000 -0800
+++ unix/uxnoise.c	2009-08-26 17:12:24.000000000 -0700
@@ -48,8 +48,8 @@
 void noise_get_heavy(void (*func) (void *, int))
 {
     char buf[512];
-    FILE *fp;
-    int ret;
+/*    FILE *fp;
+    int ret; */
     int got_dev_urandom = 0;
 
     if (read_dev_urandom(buf, 32)) {
@@ -57,6 +57,7 @@
 	func(buf, 32);
     }
 
+#if 0
     fp = popen("ps -axu 2>/dev/null", "r");
     if (fp) {
 	while ( (ret = fread(buf, 1, sizeof(buf), fp)) > 0)
@@ -79,6 +80,11 @@
 	exit(1);
     }
 
+#else
+    if (read_dev_urandom(buf, 32))
+       func(buf, 32);
+#endif
+
     read_random_seed(func);
     random_save_seed();
 }
@@ -102,8 +108,11 @@
 void noise_get_light(void (*func) (void *, int))
 {
     struct timeval tv;
+    char buf[512];
     gettimeofday(&tv, NULL);
     func(&tv, sizeof(tv));
+    if (read_dev_urandom(buf, 32))
+	random_add_noise(buf, 32);
 }
 
 /*
@@ -112,11 +121,11 @@
  */
 void noise_regular(void)
 {
-    int fd;
-    int ret;
+/*    int fd;
+    int ret;*/
     char buf[512];
-    struct rusage rusage;
-
+/*    struct rusage rusage;*/
+#if 0
     if ((fd = open("/proc/meminfo", O_RDONLY)) >= 0) {
 	while ( (ret = read(fd, buf, sizeof(buf))) > 0)
 	    random_add_noise(buf, ret);
@@ -129,6 +138,10 @@
     }
     getrusage(RUSAGE_SELF, &rusage);
     random_add_noise(&rusage, sizeof(rusage));
+#else
+    if (read_dev_urandom(buf, 32))
+	random_add_noise(buf, 32);
+#endif
 }
 
 /*
@@ -139,7 +152,10 @@
 void noise_ultralight(unsigned long data)
 {
     struct timeval tv;
+    char buf[512];
     gettimeofday(&tv, NULL);
     random_add_noise(&tv, sizeof(tv));
     random_add_noise(&data, sizeof(data));
+    if (read_dev_urandom(buf, 4))
+	random_add_noise(buf, 4);
 }
