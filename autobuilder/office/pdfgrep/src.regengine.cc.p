--- src/regengine.cc.orig	2020-04-04 09:52:16.472984242 +0100
+++ src/regengine.cc	2020-04-04 09:57:29.891716914 +0100
@@ -32,6 +32,32 @@
 #include "output.h"
 #include "pdfgrep.h"
 
+#include <sys/cdefs.h>
+
+#include <ctype.h>
+
+/*
+ * Find the first occurrence of find in s, ignore case.
+ */
+char *
+strcasestr(const char *s, const char *find)
+{
+	char c, sc;
+	size_t len;
+
+	if ((c = *find++) != 0) {
+		c = tolower((unsigned char)c);
+		len = strlen(find);
+		do {
+			do {
+				if ((sc = *s++) == 0)
+					return (NULL);
+			} while ((char)tolower((unsigned char)sc) != c);
+		} while (strncasecmp(s, find, len) != 0);
+		s--;
+	}
+	return ((char *)s);
+}
 // regex(3)
 
 using namespace std;
