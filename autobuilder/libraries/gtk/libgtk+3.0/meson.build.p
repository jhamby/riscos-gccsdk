--- meson.build.orig	2020-09-03 23:04:13.000000000 +0100
+++ meson.build	2020-11-23 13:51:18.079685005 +0000
@@ -32,7 +32,7 @@
 introspection_req  = '>= 1.39.0'
 wayland_proto_req  = '>= 1.14'
 wayland_req        = '>= 1.14.91'
-epoxy_req          = '>= 1.4'
+#epoxy_req          = '>= 1.4'
 cloudproviders_req = '>= 0.2.5'
 xkbcommon_req      = '>= 0.2.0'
 
@@ -132,6 +132,7 @@
 broadway_enabled       = get_option('broadway_backend')
 quartz_enabled         = get_option('quartz_backend')
 win32_enabled          = get_option('win32_backend')
+riscos_enabled         = get_option('riscos_backend')
 
 os_unix   = false
 os_linux  = false
@@ -148,6 +149,8 @@
   os_win32 = true
 elif host_machine.system() == 'linux'
   os_linux = true
+elif host_machine.system() == 'riscos'
+  os_riscos = true
 endif
 os_unix = not os_win32
 
@@ -165,6 +168,14 @@
   win32_enabled = false
 endif
 
+if os_riscos
+  wayland_enabled = false
+  x11_enabled = false
+  win32_enabled = false
+  quartz_enabled = false
+  broadway_enabled = false
+endif
+
 gtk_prefix = get_option('prefix')
 gtk_includedir = join_paths(gtk_prefix, get_option('includedir'))
 gtk_libdir = join_paths(gtk_prefix, get_option('libdir'))
@@ -445,8 +456,6 @@
                             fallback : ['pango', 'libpangocairo_dep'])
 pixbuf_dep     = dependency('gdk-pixbuf-2.0', version: gdk_pixbuf_req,
                             fallback : ['gdk-pixbuf', 'gdkpixbuf_dep'])
-epoxy_dep      = dependency('epoxy', version: epoxy_req,
-                            fallback: ['libepoxy', 'libepoxy_dep'])
 atk_dep        = dependency('atk', version: atk_req,
                             fallback : ['atk', 'libatk_dep'])
 
@@ -510,7 +519,7 @@
 foreach backend: [ ['cairo-xlib', cairo_req, x11_enabled],
                    ['cairo-win32', cairo_req, win32_enabled],
                    ['cairo-quartz', cairo_req, quartz_enabled],
-                   ['cairo', cairo_req, broadway_enabled or wayland_enabled], ]
+                   ['cairo', cairo_req, broadway_enabled or wayland_enabled or riscos_enabled], ]
   backend_enabled = backend.get(2)
   cairo_backend_req = backend.get(1)
   cairo_backend = backend.get(0)
@@ -905,7 +914,7 @@
 
 pkgconf.set('GDK_PRIVATE_PACKAGES',
             ' '.join(gio_packages + x11_pkgs + wayland_pkgs + cairo_backends +
-                     ['epoxy', epoxy_req] + cloudproviders_packages +
+                     cloudproviders_packages +
                      ['fribidi', fribidi_req]))
 
 gtk_packages = ' '.join([
@@ -919,7 +928,7 @@
 # Requires.private
 pc_gdk_extra_libs += cairo_libs
 
-gtk_private_packages = atk_pkgs + wayland_pkgs + ['epoxy', epoxy_req, 'fribidi', fribidi_req]
+gtk_private_packages = atk_pkgs + wayland_pkgs + ['fribidi', fribidi_req]
 if wayland_enabled or x11_enabled
   gtk_private_packages += ['pangoft2']
 endif
@@ -942,7 +951,7 @@
 
 pkg_targets = ''
 disabled_backends = []
-foreach backend: [ 'broadway', 'quartz', 'wayland', 'win32', 'x11']
+foreach backend: [ 'broadway', 'quartz', 'wayland', 'win32', 'x11', 'riscos']
   if get_variable('@0@_enabled'.format(backend))
     gtk_pcs += ['gtk+-@0@-3.0.pc'.format(backend)]
     gdk_pcs += ['gdk-@0@-3.0.pc'.format(backend)]
@@ -987,12 +996,13 @@
 install_data('m4macros/gtk-3.0.m4', install_dir: join_paths(get_option('datadir'), 'aclocal'))
 
 # Keep this in sync with post-install.py expected arguments
-meson.add_install_script('build-aux/meson/post-install.py',
-                         gtk_api_version,
-                         gtk_binary_version,
-                         join_paths(gtk_prefix, get_option('bindir')),
-                         gtk_libdir,
-                         gtk_datadir)
+# These will probably be cross compiled binaries, so don't bother trying to call them.
+#meson.add_install_script('build-aux/meson/post-install.py',
+#                         gtk_api_version,
+#                         gtk_binary_version,
+#                         join_paths(gtk_prefix, get_option('bindir')),
+#                         gtk_libdir,
+#                         gtk_datadir)
 
 if host_machine.system() != 'windows'
   # Install Valgrind suppression files (except on Windows,
