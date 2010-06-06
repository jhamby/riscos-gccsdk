--- src/polkit/polkit-context.c.orig	2010-06-05 12:52:02.175724904 +0100
+++ src/polkit/polkit-context.c	2010-06-05 12:43:20.877195688 +0100
@@ -308,7 +308,7 @@
 			goto error;
 		}
 	}
-#else
+#elif HAVE_INOTIFY
         if (pk_context->io_add_watch_func != NULL) {
                 pk_context->inotify_fd = inotify_init ();
                 if (pk_context->inotify_fd < 0) {
@@ -571,7 +571,7 @@
 			polkit_debug ("failed to read kqueue event: %s", strerror (errno));
 		}
 	}
-#else
+#elif HAVE_INOTIFY
         if (fd == pk_context->inotify_fd) {
 /* size of the event structure, not counting name */
 #define EVENT_SIZE  (sizeof (struct inotify_event))
