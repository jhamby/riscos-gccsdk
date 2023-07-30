--- crypto/threads_pthread.c.orig	2022-12-17 12:35:19.535535858 +0000
+++ crypto/threads_pthread.c	2022-12-17 12:35:51.681969498 +0000
@@ -200,7 +200,7 @@
 
 int CRYPTO_atomic_add(int *val, int amount, int *ret, CRYPTO_RWLOCK *lock)
 {
-# if defined(__GNUC__) && defined(__ATOMIC_ACQ_REL) && !defined(BROKEN_CLANG_ATOMICS)
+# if defined(__GNUC__) && defined(__ATOMIC_ACQ_REL) && !defined(BROKEN_CLANG_ATOMICS) && !defined(__riscos__)
     if (__atomic_is_lock_free(sizeof(*val), val)) {
         *ret = __atomic_add_fetch(val, amount, __ATOMIC_ACQ_REL);
         return 1;
@@ -227,7 +227,7 @@
 int CRYPTO_atomic_or(uint64_t *val, uint64_t op, uint64_t *ret,
                      CRYPTO_RWLOCK *lock)
 {
-# if defined(__GNUC__) && defined(__ATOMIC_ACQ_REL) && !defined(BROKEN_CLANG_ATOMICS)
+# if defined(__GNUC__) && defined(__ATOMIC_ACQ_REL) && !defined(BROKEN_CLANG_ATOMICS) && !defined(__riscos__)
     if (__atomic_is_lock_free(sizeof(*val), val)) {
         *ret = __atomic_or_fetch(val, op, __ATOMIC_ACQ_REL);
         return 1;
@@ -252,7 +252,7 @@
 
 int CRYPTO_atomic_load(uint64_t *val, uint64_t *ret, CRYPTO_RWLOCK *lock)
 {
-# if defined(__GNUC__) && defined(__ATOMIC_ACQUIRE) && !defined(BROKEN_CLANG_ATOMICS)
+# if defined(__GNUC__) && defined(__ATOMIC_ACQUIRE) && !defined(BROKEN_CLANG_ATOMICS) && !defined(__riscos__)
     if (__atomic_is_lock_free(sizeof(*val), val)) {
         __atomic_load(val, ret, __ATOMIC_ACQUIRE);
         return 1;
