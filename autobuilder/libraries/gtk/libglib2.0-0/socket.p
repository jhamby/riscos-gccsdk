--- gio/gsocket.c.orig	2013-08-07 15:34:32.000000000 +0100
+++ gio/gsocket.c	2013-10-09 12:21:28.185944500 +0100
@@ -3690,6 +3690,9 @@
       vectors = &one_vector;
     }
 
+#ifdef __riscos__
+  return -1;
+#else
 #ifndef G_OS_WIN32
   {
     struct msghdr msg;
@@ -3884,6 +3887,7 @@
     return bytes_sent;
   }
 #endif
+#endif
 }
 
 static GSocketAddress *
@@ -4056,6 +4060,9 @@
       vectors = &one_vector;
     }
 
+#ifdef __riscos__
+  return -1;
+#else
 #ifndef G_OS_WIN32
   {
     struct msghdr msg;
@@ -4313,6 +4320,7 @@
     return bytes_received;
   }
 #endif
+#endif
 }
 
 /**
