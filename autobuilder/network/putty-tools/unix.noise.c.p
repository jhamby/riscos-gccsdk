--- unix/noise.c.orig	2020-08-10 19:20:18.216732008 +0100
+++ unix/noise.c	2020-08-10 19:20:25.976584505 +0100
@@ -50,26 +50,29 @@
 void noise_get_heavy(void (*func) (void *, int))
 {
     char buf[512];
+#ifndef __riscos__
     FILE *fp;
     int ret;
+
     bool got_dev_urandom = false;
+#endif
 
     if (read_dev_urandom(buf, 32)) {
+#ifndef __riscos__
         got_dev_urandom = true;
+#endif
         func(buf, 32);
     }
 
+#ifndef __riscos__
     fp = popen("ps -axu 2>/dev/null", "r");
     if (fp) {
-        while ( (ret = fread(buf, 1, sizeof(buf), fp)) > 0)
-            func(buf, ret);
         pclose(fp);
     } else if (!got_dev_urandom) {
         fprintf(stderr, "popen: %s\n"
                 "Unable to access fallback entropy source\n", strerror(errno));
         exit(1);
     }
-
     fp = popen("ls -al /tmp 2>/dev/null", "r");
     if (fp) {
         while ( (ret = fread(buf, 1, sizeof(buf), fp)) > 0)
@@ -80,6 +83,10 @@
                 "Unable to access fallback entropy source\n", strerror(errno));
         exit(1);
     }
+#else
+    if (read_dev_urandom(buf, 32))
+       func(buf, 32);
+#endif
 
     read_random_seed(func);
 }
@@ -90,9 +97,12 @@
  */
 void noise_regular(void)
 {
+#ifndef __riscos__
     int fd;
     int ret;
+#endif
     char buf[512];
+#ifndef __riscos__
     struct rusage rusage;
 
     if ((fd = open("/proc/meminfo", O_RDONLY)) >= 0) {
@@ -107,6 +117,10 @@
     }
     getrusage(RUSAGE_SELF, &rusage);
     random_add_noise(NOISE_SOURCE_RUSAGE, &rusage, sizeof(rusage));
+#else
+    if (read_dev_urandom(buf, 32))
+	random_add_noise(NOISE_SOURCE_RUSAGE, buf, 32);
+#endif
 }
 
 /*
@@ -117,9 +131,12 @@
 void noise_ultralight(NoiseSourceId id, unsigned long data)
 {
     struct timeval tv;
+    char buf[512];
     gettimeofday(&tv, NULL);
     random_add_noise(NOISE_SOURCE_TIME, &tv, sizeof(tv));
     random_add_noise(id, &data, sizeof(data));
+    if (read_dev_urandom(buf, 4))
+	random_add_noise(id, buf, 4);
 }
 
 uint64_t prng_reseed_time_ms(void)
