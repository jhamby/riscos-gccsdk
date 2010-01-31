--- libcpp/include/cpplib.h.orig	2007-12-21 00:08:03.000000000 +0100
+++ libcpp/include/cpplib.h	2007-12-19 02:29:36.000000000 +0100
@@ -475,6 +475,12 @@
      This callback receives the translated message.  */
   void (*error) (cpp_reader *, int, const char *, va_list *)
        ATTRIBUTE_FPTR_PRINTF(3,0);
+
+  /* Similar to 'error' callback but here it does not intercept the message
+     and apart from a copy you get here, you get the normal output on stdout/
+     stderr.  */
+  void (*throwback)(int, const char *, int, const char *, va_list *)
+       ATTRIBUTE_FPTR_PRINTF(4,0);
 };
 
 /* Chain of directories to look for include files in.  */
