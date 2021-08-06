--- !LibPkg/libpkg/status_table.cc.orig	2021-08-05 16:10:39.182340223 -0700
+++ !LibPkg/libpkg/status_table.cc	2021-08-05 16:18:20.400359358 -0700
@@ -121,9 +121,9 @@
 bool status_table::read(const string& pathname)
 {
 	std::ifstream in(pathname.c_str());
-	bool done=in;
+	bool done=in.is_open();
 	in.peek();
-	while (in&&!in.eof())
+	while (in.is_open() && !in.eof())
 	{
 		std::pair<key_type,mapped_type> p;
 		in >> p;
