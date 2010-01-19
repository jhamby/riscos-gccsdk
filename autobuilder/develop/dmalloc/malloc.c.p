--- malloc.c.orig	2007-05-14 10:26:14.000000000 -0700
+++ malloc.c	2009-12-07 17:37:59.000000000 -0800
@@ -191,7 +191,8 @@
   /* we only lock if the lock-on counter has reached 0 */
   if (thread_lock_c == 0) {
 #if HAVE_PTHREAD_MUTEX_LOCK
-    pthread_mutex_lock(&dmalloc_mutex);
+    if (pthread_mutex_lock(&dmalloc_mutex))
+      abort();
 #endif
   }
 }
@@ -222,13 +223,15 @@
        * want to force the initialization to happen beforehand with a
        * call to pthread_mute_init.
        */
-      pthread_mutex_init(&dmalloc_mutex, THREAD_LOCK_INIT_VAL);
+      if (pthread_mutex_init(&dmalloc_mutex, THREAD_LOCK_INIT_VAL))
+        abort();
 #endif
     }
   }
   else if (thread_lock_c == 0) {
 #if HAVE_PTHREAD_MUTEX_UNLOCK
-    pthread_mutex_unlock(&dmalloc_mutex);
+    if (pthread_mutex_unlock(&dmalloc_mutex))
+      abort();
 #endif
   }
 }
@@ -254,7 +257,7 @@
   dmalloc_message("address '%#lx' found in '%s' at pass %ld from '%s'",
 		  (unsigned long)pnt, label, addr_c,
 		  _dmalloc_chunk_desc_pnt(where_buf, sizeof(where_buf), file,
-					  line));
+					  line, 0));
   
   /* NOTE: if address_seen_n == 0 then never quit */
   if (_dmalloc_address_seen_n > 0 && addr_c >= _dmalloc_address_seen_n) {
@@ -378,6 +381,19 @@
 #endif /* ! __CYGWIN__ */
     /* process the environmental variable(s) */
     process_environ(env_str);
+
+#if LOCK_THREADS
+    /*
+     * Initialise mutex in case it needs to be done at the very
+     * beginning.
+     */
+    if (thread_lock_c == 0) {
+#if HAVE_PTHREAD_MUTEX_INIT
+      if (pthread_mutex_init(&dmalloc_mutex, THREAD_LOCK_INIT_VAL))
+        abort();
+#endif
+    }
+#endif
     
     /*
      * Tune the environment here.  If we have a start-file,
@@ -817,7 +833,7 @@
 		       "Out of memory while allocating %d bytes from '%s'\n",
 		       size,
 		       _dmalloc_chunk_desc_pnt(desc, sizeof(desc),
-					       file, line));
+					       file, line, 0));
     (void)write(STDERR, mess, strlen(mess));
     _exit(1);
   }
@@ -924,7 +940,7 @@
     (void)loc_snprintf(mess, sizeof(mess),
 		       "Out of memory while reallocating %d bytes from '%s'\n",
 		       new_size, _dmalloc_chunk_desc_pnt(desc, sizeof(desc),
-							 file, line));
+							 file, line, 0));
     (void)write(STDERR, mess, strlen(mess));
     _exit(1);
   }
