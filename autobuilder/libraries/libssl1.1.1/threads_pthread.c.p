--- crypto/threads_pthread.c.orig	2018-09-12 00:48:21.000000000 +1200
+++ crypto/threads_pthread.c	2018-11-27 21:34:39.162782732 +1300
@@ -157,12 +157,12 @@
 
 int CRYPTO_atomic_add(int *val, int amount, int *ret, CRYPTO_RWLOCK *lock)
 {
-# if defined(__GNUC__) && defined(__ATOMIC_ACQ_REL)
+/*# if defined(__GNUC__) && defined(__ATOMIC_ACQ_REL)
     if (__atomic_is_lock_free(sizeof(*val), val)) {
         *ret = __atomic_add_fetch(val, amount, __ATOMIC_ACQ_REL);
         return 1;
     }
-# endif
+# endif*/
     if (!CRYPTO_THREAD_write_lock(lock))
         return 0;
 
