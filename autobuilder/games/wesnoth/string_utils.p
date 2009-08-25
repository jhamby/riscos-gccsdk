--- src/serialization/string_utils.cpp.orig	2009-02-04 13:48:16.000000000 -0800
+++ src/serialization/string_utils.cpp	2009-08-24 14:32:44.000000000 -0700
@@ -648,7 +648,7 @@
 {
 	if(s.size() > 0) {
 		utf8_iterator itor(s);
-#if defined(__APPLE__) || defined(__AMIGAOS4__)
+#if defined(__APPLE__) || defined(__AMIGAOS4__) || (__riscos__)
 		/** @todo FIXME: Should we support towupper on recent OSX platforms? */
 		wchar_t uchar = *itor;
 		if(uchar >= 0 && uchar < 0x100)
@@ -670,7 +670,7 @@
 		std::string res;
 
 		for(;itor != utf8_iterator::end(s); ++itor) {
-#if defined(__APPLE__) || defined(__AMIGAOS4__)
+#if defined(__APPLE__) || defined(__AMIGAOS4__) || defined(__riscos__)
 			/** @todo FIXME: Should we support towupper on recent OSX platforms? */
 			wchar_t uchar = *itor;
 			if(uchar >= 0 && uchar < 0x100)
@@ -693,7 +693,7 @@
 		std::string res;
 
 		for(;itor != utf8_iterator::end(s); ++itor) {
-#if defined(__APPLE__) || defined(__OpenBSD__) || defined(__AMIGAOS4__)
+#if defined(__APPLE__) || defined(__OpenBSD__) || defined(__AMIGAOS4__) || defined(__riscos__)
 			/** @todo FIXME: Should we support towupper on recent OSX platforms? */
 			wchar_t uchar = *itor;
 			if(uchar >= 0 && uchar < 0x100)
