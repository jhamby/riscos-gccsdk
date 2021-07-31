--- atk/meson.build.orig	2018-09-08 14:27:59.000000000 +0100
+++ atk/meson.build	2020-07-26 11:02:05.209314065 +0100
@@ -122,7 +122,7 @@
 
 atk_inc = include_directories('.')
 
-libatk = shared_library('atk-@0@'.format(atk_api_version),
+libatk = library('atk-@0@'.format(atk_api_version),
                         sources: atk_sources + atk_enums + atk_marshals,
                         soversion: atk_soversion,
                         version: atk_libversion,
