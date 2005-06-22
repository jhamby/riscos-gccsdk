Index: unix/uxnoise.c
===================================================================
--- unix/uxnoise.c	(revision 5964)
+++ unix/uxnoise.c	(working copy)
@@ -45,12 +45,12 @@
 void noise_get_heavy(void (*func) (void *, int))
 {
     char buf[512];
-    FILE *fp;
-    int ret;
+/*    FILE *fp;
+    int ret;*/
 
     if (read_dev_urandom(buf, 32))
 	func(buf, 32);
-
+#if 0
     fp = popen("ps -axu 2>/dev/null", "r");
     while ( (ret = fread(buf, 1, sizeof(buf), fp)) > 0)
 	func(buf, ret);
@@ -60,7 +60,10 @@
     while ( (ret = fread(buf, 1, sizeof(buf), fp)) > 0)
 	func(buf, ret);
     pclose(fp);
-
+#else
+    if (read_dev_urandom(buf, 32))
+	func(buf, 32);
+#endif
     read_random_seed(func);
     random_save_seed();
 }
@@ -84,8 +87,11 @@
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
@@ -94,11 +100,11 @@
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
@@ -111,6 +117,10 @@
     }
     getrusage(RUSAGE_SELF, &rusage);
     random_add_noise(&rusage, sizeof(rusage));
+#else
+    if (read_dev_urandom(buf, 32))
+	random_add_noise(buf, 32);
+#endif
 }
 
 /*
@@ -121,7 +131,10 @@
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

