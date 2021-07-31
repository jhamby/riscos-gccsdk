--- src/serialization/string_utils.cpp.orig	2010-01-03 18:44:55.000000000 +0000
+++ src/serialization/string_utils.cpp	2011-03-31 09:41:56.372752400 +0100
@@ -639,7 +639,7 @@
 		std::string res;
 
 		for(;itor != utf8_iterator::end(s); ++itor) {
-#if defined(__APPLE__) || defined(__OpenBSD__) || defined(__AMIGAOS4__)
+#if defined(__APPLE__) || defined(__OpenBSD__) || defined(__AMIGAOS4__) || defined(__riscos__)
 			/** @todo FIXME: Should we support towupper on recent OSX platforms? */
 			wchar_t uchar = *itor;
 			if(uchar >= 0 && uchar < 0x100)
