--- malloc.c.orig	2010-02-09 00:16:42.222689707 +0100
+++ malloc.c	2010-02-09 00:14:34.922689478 +0100
@@ -54,6 +54,10 @@
 # include <w32api/winbase.h>
 #endif
 
+#ifdef __riscos__
+#  include "kernel.h"
+#endif
+
 #include "conf.h"				/* up here for _INCLUDE */
 
 #if LOG_PNT_TIMEVAL
@@ -191,7 +195,8 @@
   /* we only lock if the lock-on counter has reached 0 */
   if (thread_lock_c == 0) {
 #if HAVE_PTHREAD_MUTEX_LOCK
-    pthread_mutex_lock(&dmalloc_mutex);
+    if (pthread_mutex_lock(&dmalloc_mutex))
+      abort();
 #endif
   }
 }
@@ -222,13 +227,15 @@
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
@@ -254,7 +261,7 @@
   dmalloc_message("address '%#lx' found in '%s' at pass %ld from '%s'",
 		  (unsigned long)pnt, label, addr_c,
 		  _dmalloc_chunk_desc_pnt(where_buf, sizeof(where_buf), file,
-					  line));
+					  line, 0));
   
   /* NOTE: if address_seen_n == 0 then never quit */
   if (_dmalloc_address_seen_n > 0 && addr_c >= _dmalloc_address_seen_n) {
@@ -363,6 +370,29 @@
     GetEnvironmentVariableA(OPTIONS_ENVIRON, env_buf, sizeof(env_buf));
     env_str = env_buf;
 #else /* ! __CYGWIN__ */
+#if defined(__riscos__)
+    /* Read environment variable using RISC OS swi as at this point we
+       can be called when UnixLib is starting up which makes getenv()
+       does not return something useful.  */
+    {
+      _kernel_swi_regs regs;
+      char buf[128];
+
+      regs.r[0] = (int) OPTIONS_ENVIRON;
+      regs.r[1] = (int) buf;
+      /* One less than buf size so can zero terminate below.  */
+      regs.r[2] = sizeof (buf) - 1;
+      regs.r[3] = 0;
+      regs.r[4] = 3;
+      if (_kernel_swi (0x23 /* OS_ReadVarVal */, &regs, &regs))
+        env_str = NULL;
+      else
+        {
+          buf[regs.r[2]] = '\0';
+          env_str = buf;
+        }
+    }
+#else
 #if GETENV_SAFE
     /* get the options flag */
     if (debug_str == NULL) {
@@ -375,9 +405,23 @@
     /* oh, well.  no idea how to get the environmental variables */
     env_str = "";
 #endif /* GETENV_SAFE */
+#endif /* ! __riscos__ */
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
@@ -817,7 +861,7 @@
 		       "Out of memory while allocating %d bytes from '%s'\n",
 		       size,
 		       _dmalloc_chunk_desc_pnt(desc, sizeof(desc),
-					       file, line));
+					       file, line, 0));
     (void)write(STDERR, mess, strlen(mess));
     _exit(1);
   }
@@ -924,7 +968,7 @@
     (void)loc_snprintf(mess, sizeof(mess),
 		       "Out of memory while reallocating %d bytes from '%s'\n",
 		       new_size, _dmalloc_chunk_desc_pnt(desc, sizeof(desc),
-							 file, line));
+							 file, line, 0));
     (void)write(STDERR, mess, strlen(mess));
     _exit(1);
   }
@@ -1076,7 +1120,10 @@
 DMALLOC_PNT	malloc(DMALLOC_SIZE size)
 {
   char	*file;
-  
+
+  if (in_alloc_b)
+    return __malloc(size);
+
   GET_RET_ADDR(file);
   return dmalloc_malloc(file, DMALLOC_DEFAULT_LINE, size, DMALLOC_FUNC_MALLOC,
 			0 /* no alignment */, 0 /* no xalloc messages */);
@@ -1106,7 +1153,10 @@
 {
   DMALLOC_SIZE	len = num_elements * size;
   char		*file;
-  
+
+  if (in_alloc_b)
+    return __calloc(num_elements, size);
+
   GET_RET_ADDR(file);
   return dmalloc_malloc(file, DMALLOC_DEFAULT_LINE, len, DMALLOC_FUNC_CALLOC,
 			0 /* no alignment */, 0 /* no xalloc messages */);
@@ -1137,7 +1187,10 @@
 DMALLOC_PNT	realloc(DMALLOC_PNT old_pnt, DMALLOC_SIZE new_size)
 {
   char	*file;
-  
+
+  if (in_alloc_b)
+    return __realloc(old_pnt, new_size);
+
   GET_RET_ADDR(file);
   return dmalloc_realloc(file, DMALLOC_DEFAULT_LINE, old_pnt, new_size,
 			 DMALLOC_FUNC_REALLOC, 0 /* no xalloc messages */);
@@ -1202,7 +1255,10 @@
 DMALLOC_PNT	memalign(DMALLOC_SIZE alignment, DMALLOC_SIZE size)
 {
   char		*file;
-  
+
+  if (in_alloc_b)
+    return __memalign(alignment, size);
+
   GET_RET_ADDR(file);
   return dmalloc_malloc(file, DMALLOC_DEFAULT_LINE, size,
 			DMALLOC_FUNC_MEMALIGN, alignment,
@@ -1232,7 +1288,10 @@
 DMALLOC_PNT	valloc(DMALLOC_SIZE size)
 {
   char	*file;
-  
+
+  if (in_alloc_b)
+    return __valloc(size);
+
   GET_RET_ADDR(file);
   return dmalloc_malloc(file, DMALLOC_DEFAULT_LINE, size, DMALLOC_FUNC_VALLOC,
 			BLOCK_SIZE, 0 /* no xalloc messages */);
@@ -1262,7 +1321,10 @@
 {
   int	len;
   char	*buf, *file;
-  
+
+  if (in_alloc_b)
+    return __strdup(string);
+
   GET_RET_ADDR(file);
   
   /* check the arguments */
@@ -1312,6 +1374,9 @@
   int		size;
   char		*buf, *file;
   const char	*string_p;
+
+  if (in_alloc_b)
+    return __strndup(string, len);
   
   GET_RET_ADDR(file);
   
@@ -1365,6 +1430,12 @@
 {
   char	*file;
   int	ret;
+
+  if (in_alloc_b)
+    {
+      __free(pnt);
+      return;
+    }
   
   GET_RET_ADDR(file);
   ret = dmalloc_free(file, DMALLOC_DEFAULT_LINE, pnt, DMALLOC_FUNC_FREE);
@@ -1396,6 +1467,12 @@
 {
   char	*file;
   int	ret;
+
+  if (in_alloc_b)
+    {
+      __cfree(pnt);
+      return;
+    }
   
   GET_RET_ADDR(file);
   ret = dmalloc_free(file, DMALLOC_DEFAULT_LINE, pnt, DMALLOC_FUNC_CFREE);
