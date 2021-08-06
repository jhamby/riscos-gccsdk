--- !LibPkg/libpkg/component_update.cc.orig	2021-08-05 16:50:50.410705206 -0700
+++ !LibPkg/libpkg/component_update.cc	2021-08-05 16:51:59.847314834 -0700
@@ -151,9 +151,9 @@
 bool component_update::read(const string& pathname)
 {
 	std::ifstream in(pathname.c_str());
-	bool done=in;
+	bool done=in.is_open();
 	in.peek();
-	while (in&&!in.eof())
+	while (in.is_open() && !in.eof())
 	{
 		component c;
 		in >> c;
