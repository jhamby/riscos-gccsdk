--- src/gtest-port.cc.orig	2013-09-18 16:48:30.000000000 +0100
+++ src/gtest-port.cc	2015-09-21 12:39:42.887083678 +0100
@@ -154,7 +154,12 @@
   if (!re.is_valid_) return false;
 
   regmatch_t match;
+#ifdef GTEST_OS_RISCOS
+  // RISC OS regexec definition missing const modifier
+  return regexec(const_cast<regex_t *>(&re.full_regex_), str, 1, &match, 0) == 0;
+#else  
   return regexec(&re.full_regex_, str, 1, &match, 0) == 0;
+#endif  
 }
 
 // Returns true iff regular expression re matches a substring of str
@@ -163,7 +168,12 @@
   if (!re.is_valid_) return false;
 
   regmatch_t match;
+#ifdef GTEST_OS_RISCOS
+  // RISC OS regexec definition missing const modifier
+  return regexec(const_cast<regex_t *>(&re.partial_regex_), str, 1, &match, 0) == 0;
+#else  
   return regexec(&re.partial_regex_, str, 1, &match, 0) == 0;
+#endif  
 }
 
 // Initializes an RE from its string representation.
