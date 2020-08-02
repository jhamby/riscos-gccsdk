--- gio/gsocket.c.orig	2019-01-21 13:38:50.000000000 +0000
+++ gio/gsocket.c	2020-08-02 17:55:12.121956360 +0100
@@ -4665,6 +4665,9 @@
       vectors = &one_vector;
     }
 
+#ifdef __riscos__
+  return -1;
+#else
 #ifndef G_OS_WIN32
   {
     GOutputMessage output_message;
@@ -4797,6 +4800,7 @@
     return bytes_sent;
   }
 #endif
+#endif
 }
 
 /**
@@ -5130,6 +5134,9 @@
       vectors = &one_vector;
     }
 
+#ifdef __riscos__
+  return -1;
+#else
 #ifndef G_OS_WIN32
   {
     GInputMessage input_message;
@@ -5282,6 +5289,7 @@
     return bytes_received;
   }
 #endif
+#endif
 }
 
 /**
