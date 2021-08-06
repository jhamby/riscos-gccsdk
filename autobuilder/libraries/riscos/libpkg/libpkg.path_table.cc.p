--- !LibPkg/libpkg/path_table.cc.orig	2021-08-05 16:48:01.573220404 -0700
+++ !LibPkg/libpkg/path_table.cc	2021-08-05 16:49:01.009743551 -0700
@@ -247,9 +247,9 @@
 bool path_table::read(const string& pathname)
 {
 	std::ifstream in(pathname.c_str());
-	bool done=in;
+	bool done=in.is_open();
 	in.peek();
-	while (in&&!in.eof())
+	while (in.is_open() && !in.eof())
 	{
 		// Read line from input stream.
 		string line;
