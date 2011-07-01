--- src/image.cpp.orig	2011-06-14 13:07:57.033846600 +0100
+++ src/image.cpp	2011-06-14 13:11:21.472950700 +0100
@@ -1061,8 +1061,13 @@
 		std::map<image::locator,bool>::const_iterator b =  image_existence_map.find(i_locator);
 		if (b != image_existence_map.end())
 			return b->second;
+#ifndef __riscos__
+			// The RISC OS virtual file system capabilities used for speed
+			// mean that the file will not be iterated through so continue
+			// to code below to test the actual file name.
 		else
 			return false;
+#endif
 	}
 
 	// The insertion will fail if there is already an element in the cache
