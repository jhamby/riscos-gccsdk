--- src/serialization/string_utils.cpp.orig	2008-02-16 08:47:16.000000000 +0000
+++ src/serialization/string_utils.cpp	2008-03-25 12:31:08.086526300 +0000
@@ -808,7 +808,7 @@
 {
 	if(s.size() > 0) {
 		utf8_iterator itor(s);
-#if defined(__APPLE__) || defined(__AMIGAOS4__)
+#if defined(__APPLE__) || defined(__AMIGAOS4__) || defined(__riscos__)
 		//! @todo FIXME: Should we support towupper on recent OSX platforms?
 		wchar_t uchar = *itor;
 		if(uchar >= 0 && uchar < 0x100)
@@ -830,7 +830,7 @@
 		std::string res;
 
 		for(;itor != utf8_iterator::end(s); ++itor) {
-#if defined(__APPLE__) || defined(__AMIGAOS4__)
+#if defined(__APPLE__) || defined(__AMIGAOS4__) || defined(__riscos__)
 			//! @todo FIXME: Should we support towupper on recent OSX platforms?
 			wchar_t uchar = *itor;
 			if(uchar >= 0 && uchar < 0x100)
@@ -853,7 +853,7 @@
 		std::string res;
 
 		for(;itor != utf8_iterator::end(s); ++itor) {
-#if defined(__APPLE__) || defined(__OpenBSD__) || defined(__AMIGAOS4__)
+#if defined(__APPLE__) || defined(__OpenBSD__) || defined(__AMIGAOS4__) || defined(__riscos__)
 			//! @todo FIXME: Should we support towupper on recent OSX platforms?
 			wchar_t uchar = *itor;
 			if(uchar >= 0 && uchar < 0x100)
