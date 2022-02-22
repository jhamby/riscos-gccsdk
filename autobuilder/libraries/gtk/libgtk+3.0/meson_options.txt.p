--- meson_options.txt.orig	2019-11-27 15:23:51.000000000 +0000
+++ meson_options.txt	2020-02-11 18:29:56.998818639 +0000
@@ -9,6 +9,8 @@
   description : 'Enable the Windows gdk backend (only when building on Windows)')
 option('quartz_backend', type: 'boolean', value: true,
   description : 'Enable the macOS gdk backend (only when building on macOS)')
+option('riscos_backend', type: 'boolean', value: true,
+  description : 'Enable the RISC OS gdk backend')
 
 # Optional dependencies
 option('xinerama', type: 'combo', choices : ['yes', 'no', 'auto'], value : 'auto',
