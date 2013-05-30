--- glib/gatomic.c.orig	2013-05-29 16:08:24.199941277 +0100
+++ glib/gatomic.c	2013-05-29 16:08:52.911789037 +0100
@@ -682,7 +682,7 @@
 static pthread_mutex_t g_atomic_lock = PTHREAD_MUTEX_INITIALIZER;
 
 gint
-(g_atomic_int_get) (volatile gint *atomic)
+(g_atomic_int_get) (const volatile gint *atomic)
 {
   gint value;
 
@@ -797,7 +797,7 @@
 
 
 gpointer
-(g_atomic_pointer_get) (volatile void *atomic)
+(g_atomic_pointer_get) (const volatile void *atomic)
 {
   volatile gpointer *ptr = atomic;
   gpointer value;
