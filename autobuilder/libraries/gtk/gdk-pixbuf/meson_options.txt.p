--- meson_options.txt.orig	2019-02-28 16:22:57.000000000 +0000
+++ meson_options.txt	2020-08-04 01:33:30.161996503 +0100
@@ -50,4 +50,7 @@
        description: 'Perform file type detection using GIO (Unused on MacOS and Windows)',
        type: 'boolean',
        value: true)
-
+option('riscos_abi',
+       description: 'ABI directory used to find the loader shared libraries at runtime',
+       type: 'string',
+       value: 'abi-2.0')
