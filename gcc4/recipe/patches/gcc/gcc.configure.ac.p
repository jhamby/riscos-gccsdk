--- gcc/configure.ac	2018-04-08 12:41:52.000000000 +0100
+++ gcc/configure.ac	2018-04-08 14:11:19.417147893 +0100
@@ -943,6 +943,33 @@
 fi
 
 
+AC_CHECK_PROGS([GPERF], gperf, [$MISSING gperf])
+
+GPERF_TEST="$(echo foo,bar | ${GPERF} -L ANSI-C)"
+
+AC_COMPILE_IFELSE(
+        [AC_LANG_PROGRAM([
+                #include <string.h>
+                const char * in_word_set(const char *, size_t);
+                $GPERF_TEST]
+        )],
+        [GPERF_LEN_TYPE=size_t],
+        [AC_COMPILE_IFELSE(
+                [AC_LANG_PROGRAM([
+                        #include <string.h>
+                        const char * in_word_set(const char *, unsigned);
+                        $GPERF_TEST]
+                )],
+                [GPERF_LEN_TYPE=unsigned],
+                [AC_MSG_ERROR([** unable to determine gperf len type])]
+        )]
+)
+
+AC_DEFINE_UNQUOTED([GPERF_LEN_TYPE], [$GPERF_LEN_TYPE], [gperf len type])
+
+
+
+
 # --------------------
 # Checks for C headers
 # --------------------
