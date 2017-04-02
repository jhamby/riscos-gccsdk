--- gio/gsocket.c.orig	2017-02-13 15:15:29.000000000 +0000
+++ gio/gsocket.c	2017-04-01 20:02:42.480064630 +0100
@@ -4316,6 +4316,9 @@
       vectors = &one_vector;
     }
 
+#ifdef __riscos__
+  return -1;
+#else
 #ifndef G_OS_WIN32
   {
     GOutputMessage output_message;
@@ -4448,6 +4451,7 @@
     return bytes_sent;
   }
 #endif
+#endif
 }
 
 /**
@@ -4781,6 +4785,9 @@
       vectors = &one_vector;
     }
 
+#ifdef __riscos__
+  return -1;
+#else
 #ifndef G_OS_WIN32
   {
     GInputMessage input_message;
@@ -4934,6 +4941,7 @@
     return bytes_received;
   }
 #endif
+#endif
 }
 
 /**
