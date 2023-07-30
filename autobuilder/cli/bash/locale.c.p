--- locale.c.orig	2020-02-24 12:08:43.000000000 -0800
+++ locale.c	2023-07-30 15:22:49.759008636 -0700
@@ -91,7 +91,7 @@
 #if defined (HANDLE_MULTIBYTE)
   locale_shiftstates = mblen ((char *)NULL, 0);
 #else
-  local_shiftstates = 0;
+  locale_shiftstates = 0;
 #endif
 }
 
@@ -117,7 +117,7 @@
 #    if defined (HANDLE_MULTIBYTE)
       locale_shiftstates = mblen ((char *)NULL, 0);
 #    else
-      local_shiftstates = 0;
+      locale_shiftstates = 0;
 #    endif
 
       u32reset ();
@@ -226,7 +226,7 @@
 #  if defined (HANDLE_MULTIBYTE)
       locale_shiftstates = mblen ((char *)NULL, 0);
 #  else
-      local_shiftstates = 0;
+      locale_shiftstates = 0;
 #  endif
       u32reset ();
       return r;
@@ -250,7 +250,7 @@
 #if defined (HANDLE_MULTIBYTE)
 	  locale_shiftstates = mblen ((char *)NULL, 0);
 #else
-	  local_shiftstates = 0;
+	  locale_shiftstates = 0;
 #endif
 	  u32reset ();
 	}
@@ -391,7 +391,7 @@
 #  if defined (HANDLE_MULTIBYTE)
   locale_shiftstates = mblen ((char *)NULL, 0);
 #  else
-  local_shiftstates = 0;
+  locale_shiftstates = 0;
 #  endif
   u32reset ();
 #endif
