--- entropy.c	2003-02-03 21:35:37.000000000 +0000
+++ ../openssh-3.5p1-ro/entropy.c	2002-11-24 11:29:16.000000000 +0000
@@ -55,6 +55,7 @@
 void
 seed_rng(void)
 {
+#ifndef __riscos__
 #ifndef OPENSSL_PRNG_ONLY
 	int devnull;
 	int p[2];
@@ -129,6 +130,19 @@
 	memset(buf, '\0', sizeof(buf));
 
 #endif /* OPENSSL_PRNG_ONLY */
+#else
+       {
+          int num;
+          int val;
+
+          srand(clock());
+
+          for (num = 0; num < 1000; num++) {
+            val = rand();
+            RAND_add(&val, sizeof(val), 1.0);
+          }
+       }
+#endif
 	if (RAND_status() != 1)
 		fatal("PRNG is not seeded");
 }
