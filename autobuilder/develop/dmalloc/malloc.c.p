--- malloc.c.org	2004-10-19 16:51:21.000000000 +0200
+++ malloc.c	2006-07-31 22:44:38.000000000 +0200
@@ -126,6 +126,96 @@
 static	unsigned long	start_size = 0;		/* start after X bytes */
 static	int		thread_lock_c = 0;	/* lock counter */

+#define __UNIXLIB_INTERNALS
+#include <unixlib/unix.h>
+
+static char in_trace[1024];
+static int features; // FIXME: not filled in - only used
+
+static const char *
+__get_backtrace (void)
+{
+  int *fp = __backtrace_getfp(), *oldfp = NULL;
+  static char trace[1024];
+  int tracelen = 1;
+  char *ret;
+
+  trace[0] = '\0';
+
+  /* fp[0]  => pc
+     fp[-1] => lr
+     fp[-2] => sp
+     fp[-3] => previous fp  */
+  while (fp != NULL)
+    {
+      unsigned int *pc;
+
+      /* Check that FP is different.  */
+      if (fp == oldfp)
+	break;
+
+      /* Validate FP address.  */
+      if (!__valid_address(fp - 3, fp))
+	break;
+
+      /* Retrieve PC counter.  */
+      if (__32bit)
+	pc = (unsigned int *)(fp[0] & 0xfffffffc);
+      else
+	pc = (unsigned int *)(fp[0] & 0x03fffffc);
+      if (!(features & 0x8))
+	pc += 1;
+
+      if (!__valid_address(pc, pc))
+	break;
+
+      /* Retrieve function name.  */
+      if (!__valid_address(pc - 7, pc))
+	break;
+      else
+	{
+	  int address;
+	  const char *name = NULL;
+
+	  for (address = -3; address > -8; address--)
+	    {
+	      if ((pc[address] & 0xffffff00) == 0xff000000)
+		{
+		  name = (const char *)(pc + address) - (pc[address] & 0xff);
+		  break;
+		}
+	    }
+
+	  /* Function name sanity check.  */
+	  if (name != NULL
+	      && (!__valid_address(name, (name + 256))
+		  || strnlen(name, 256) == 256))
+	    name = NULL;
+
+	  if (!name)
+	    {
+	      tracelen += sizeof("?()/")-1;
+	      if (tracelen > sizeof (trace))
+	        break;
+	      strcat(trace, "?()/");
+	    }
+	  else
+	    {
+	      tracelen += strlen (name) + sizeof("()/")-1;
+	      if (tracelen > sizeof (trace))
+	        break;
+	      strcat(trace, name);
+	      strcat(trace, "()/");
+	    }
+	}
+
+      oldfp = fp;
+      fp = (int *)fp[-3];
+    }
+
+  return trace;
+}
+
 /****************************** thread locking *******************************/

 #if LOCK_THREADS
@@ -191,7 +281,8 @@
   /* we only lock if the lock-on counter has reached 0 */
   if (thread_lock_c == 0) {
 #if HAVE_PTHREAD_MUTEX_LOCK
-    pthread_mutex_lock(&dmalloc_mutex);
+    if (pthread_mutex_lock(&dmalloc_mutex))
+      abort();
 #endif
   }
 }
@@ -222,13 +313,15 @@
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
@@ -376,6 +469,19 @@
     process_environ(env_str);
 #endif /* GETENV_SAFE */
 #endif /* ! __CYGWIN__ */
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
