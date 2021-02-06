--- magick/utility.c.orig	2015-06-18 17:52:17.889874046 +0100
+++ magick/utility.c	2015-06-18 17:53:05.481873529 +0100
@@ -1718,6 +1718,8 @@
   }
 #elif defined(__BEOS__)
   snooze(1000*milliseconds);
+#elif defined(__riscos__)
+  return;
 #else
 # error "Time delay method not defined."
 #endif
