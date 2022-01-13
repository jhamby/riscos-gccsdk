--- gdk/meson.build.orig	2020-06-11 03:45:39.000000000 +0100
+++ gdk/meson.build	2020-11-22 20:30:37.009625614 +0000
@@ -166,6 +166,7 @@
 gdkconfig_cdata.set('GDK_WINDOWING_WIN32', win32_enabled)
 gdkconfig_cdata.set('GDK_WINDOWING_BROADWAY', broadway_enabled)
 gdkconfig_cdata.set('GDK_WINDOWING_QUARTZ', quartz_enabled)
+gdkconfig_cdata.set('GDK_WINDOWING_RISCOS', riscos_enabled)
 
 gdkconfig = configure_file(
   input  : 'gdkconfig.h.meson',
@@ -205,7 +206,6 @@
   fribidi_dep,
   cairogobj_dep,
   glib_dep,
-  epoxy_dep,
   fontconfig_dep,
   platform_gio_dep,
   pangocairo_dep
@@ -234,6 +234,10 @@
   endif
 endif
 
+if riscos_enabled
+  gdk_deps += [cc.find_library('OSLib32')]
+endif
+
 gdk_sources = [
   # Generated
   gdkconfig,
@@ -258,7 +262,7 @@
 
 gdk_backends = []
 gdk_backends_gen_headers = []  # non-public generated headers
-foreach backend : ['broadway', 'quartz', 'wayland', 'win32', 'x11']
+foreach backend : ['broadway', 'quartz', 'wayland', 'win32', 'x11', 'riscos']
   if get_variable('@0@_enabled'.format(backend))
     subdir(backend)
     gdk_deps += get_variable('gdk_@0@_deps'.format(backend))
