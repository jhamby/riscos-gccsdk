--- !FixDeps/Makefile.orig	2015-02-25 12:40:02.834768000 +0000
+++ !FixDeps/Makefile	2015-02-25 12:45:23.659095671 +0000
@@ -6,4 +6,4 @@
 CXX = g++ -mthrowback -munixlib -Wall -W -Wno-unused
 
 fixdeps: fixdeps.cc
-	$(CXX) -o fixdeps fixdeps.cc -liostream
+	$(CXX) -o fixdeps fixdeps.cc
