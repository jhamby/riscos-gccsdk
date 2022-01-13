--- tests/meson.build.orig	2020-11-22 21:31:08.000000000 +0000
+++ tests/meson.build	2020-11-22 21:44:34.522144658 +0000
@@ -5,7 +5,7 @@
   ['scrolling-performance', ['frame-stats.c', 'variable.c']],
   ['blur-performance', ['../gtk/gtkcairoblur.c']],
   ['flicker'],
-  ['gdkgears', ['gtkgears.c']],
+  #['gdkgears', ['gtkgears.c']],
   ['listmodel'],
   ['motion-compression'],
   ['styleexamples'],
@@ -54,7 +54,7 @@
   ['testfullscreen'],
   ['testgeometry'],
   ['testgiconpixbuf'],
-  ['testglblending', ['gtkgears.c']],
+  #['testglblending', ['gtkgears.c']],
   ['testgmenu'],
   ['testgrid'],
   ['testgrouping'],
