--- gio/gsocket.c.orig	2021-03-18 13:47:48.286693000 +0000
+++ gio/gsocket.c	2022-01-14 11:46:15.228532142 +0000
@@ -4875,6 +4875,9 @@
       vectors = &one_vector;
     }
 
+#ifdef __riscos__
+  return -1;
+#else
 #ifndef G_OS_WIN32
   {
     GOutputMessage output_message;
@@ -5017,6 +5020,7 @@
     return G_POLLABLE_RETURN_OK;
   }
 #endif
+#endif
 }
 
 /**
@@ -5362,6 +5366,9 @@
       vectors = &one_vector;
     }
 
+#ifdef __riscos__
+  return -1;
+#else
 #ifndef G_OS_WIN32
   {
     GInputMessage input_message;
@@ -5518,6 +5525,7 @@
     return bytes_received;
   }
 #endif
+#endif
 }
 
 /**
