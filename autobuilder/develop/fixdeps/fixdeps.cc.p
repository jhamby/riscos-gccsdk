--- !FixDeps/fixdeps.cc.orig	2015-02-25 12:40:02.834768000 +0000
+++ !FixDeps/fixdeps.cc	2015-02-25 12:44:29.768164892 +0000
@@ -9,6 +9,8 @@
 #include "limits.h"
 #include "unixlib/local.h"
 
+using namespace std;
+
 string translate(string filename)
 {
 	char buffer[PATH_MAX];
