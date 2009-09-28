--- gio/gsocket.c.orig	2009-09-28 14:48:07.000000000 -0700
+++ gio/gsocket.c	2009-09-28 14:50:25.000000000 -0700
@@ -2616,6 +2616,9 @@
       vectors = &one_vector;
     }
 
+#ifdef __riscos__
+  return -1;
+#else
 #ifndef G_OS_WIN32
   {
     struct msghdr msg;
@@ -2802,6 +2805,7 @@
     return bytes_sent;
   }
 #endif
+#endif
 }
 
 /**
@@ -2914,6 +2918,9 @@
       vectors = &one_vector;
     }
 
+#ifdef __riscos__
+  return -1;
+#else
 #ifndef G_OS_WIN32
   {
     struct msghdr msg;
@@ -3157,6 +3164,7 @@
     return bytes_received;
   }
 #endif
+#endif
 }
 
 #define __G_SOCKET_C__
