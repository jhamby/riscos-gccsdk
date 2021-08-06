--- !LibPkg/libpkg/source_table.cc.orig	2021-08-05 16:46:35.140458599 -0700
+++ !LibPkg/libpkg/source_table.cc	2021-08-05 16:47:00.928686036 -0700
@@ -41,8 +41,8 @@
 bool source_table::read(const string& pathname)
 {
 	std::ifstream in(pathname.c_str());
-	bool found=in;
-	while (in&&!in.eof())
+	bool found=in.is_open();
+	while (in.is_open() && !in.eof())
 	{
 		// Read line from input stream.
 		string line;
