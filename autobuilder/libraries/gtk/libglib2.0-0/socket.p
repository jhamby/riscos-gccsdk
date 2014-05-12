--- gio/gsocket.c.orig	2014-02-23 05:47:05.000000000 +0000
+++ gio/gsocket.c	2014-05-08 17:08:34.144137594 +0100
@@ -3780,6 +3780,9 @@
       vectors = &one_vector;
     }
 
+#ifdef __riscos__
+  return -1;
+#else
 #ifndef G_OS_WIN32
   {
     struct msghdr msg;
@@ -3974,6 +3977,7 @@
     return bytes_sent;
   }
 #endif
+#endif
 }
 
 static GSocketAddress *
@@ -4146,6 +4150,9 @@
       vectors = &one_vector;
     }
 
+#ifdef __riscos__
+  return -1;
+#else
 #ifndef G_OS_WIN32
   {
     struct msghdr msg;
@@ -4403,6 +4410,7 @@
     return bytes_received;
   }
 #endif
+#endif
 }
 
 /**
