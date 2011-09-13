--- src/util/system.cpp.orig	2011-09-06 13:05:32.051824100 +0100
+++ src/util/system.cpp	2011-09-06 13:46:14.225781900 +0100
@@ -384,7 +384,12 @@
 	std::cout << " bytes read " << bytes_read << " feof " << feof(in) << std::endl;
 	if (ferror(in)) perror("Got error");
 
+#ifdef __riscos__
+	/* GCG 4.1.1 on RISC OS did not set feof */
+    if (bytes_read != 4096 && ferror(in))
+#else
     if (bytes_read != 4096 && !feof(in))
+#endif
     {
       throw std::runtime_error("System:checksum: file read error " + filename);
     }
