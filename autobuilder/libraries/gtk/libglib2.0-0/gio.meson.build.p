--- gio/meson.build.orig	2020-02-23 13:30:31.000000000 +0000
+++ gio/meson.build	2020-02-23 13:32:47.871740695 +0000
@@ -918,34 +918,34 @@
   'gio-tool-tree.c',
 ]
 
-executable('gio', gio_tool_sources,
+executable('gio,e1f', gio_tool_sources,
   install : true,
   c_args : gio_c_args,
   # intl.lib is not compatible with SAFESEH
   link_args : noseh_link_args,
   dependencies : [libgio_dep, libgobject_dep, libgmodule_dep, libglib_dep])
 
-executable('gresource', 'gresource-tool.c',
+executable('gresource,e1f', 'gresource-tool.c',
   install : true,
   # intl.lib is not compatible with SAFESEH
   link_args : noseh_link_args,
   dependencies : [libelf, libgio_dep, libgobject_dep, libgmodule_dep, libglib_dep])
 
-gio_querymodules = executable('gio-querymodules', 'gio-querymodules.c', 'giomodule-priv.c',
+gio_querymodules = executable('gio-querymodules,e1f', 'gio-querymodules.c', 'giomodule-priv.c',
   install : true,
   c_args : gio_c_args,
   # intl.lib is not compatible with SAFESEH
   link_args : noseh_link_args,
   dependencies : [libgio_dep, libgobject_dep, libgmodule_dep, libglib_dep])
 
-glib_compile_schemas = executable('glib-compile-schemas',
+glib_compile_schemas = executable('glib-compile-schemas,e1f',
   [gconstructor_as_data_h, 'gvdb/gvdb-builder.c', 'glib-compile-schemas.c'],
   install : true,
   # intl.lib is not compatible with SAFESEH
   link_args : noseh_link_args,
   dependencies : [libgio_dep, libgobject_dep, libgmodule_dep, libglib_dep])
 
-glib_compile_resources = executable('glib-compile-resources',
+glib_compile_resources = executable('glib-compile-resources,e1f',
   [gconstructor_as_data_h, 'gvdb/gvdb-builder.c', 'glib-compile-resources.c'],
   install : true,
   c_args : gio_c_args,
@@ -953,7 +953,7 @@
   link_args : noseh_link_args,
   dependencies : [libgio_dep, libgobject_dep, libgmodule_dep, libglib_dep])
 
-executable('gsettings', 'gsettings-tool.c',
+executable('gsettings,e1f', 'gsettings-tool.c',
   install : true,
   c_args : gio_c_args,
   # intl.lib is not compatible with SAFESEH
@@ -965,7 +965,7 @@
 install_data(['gschema.loc', 'gschema.its'],
   install_dir : join_paths(get_option('datadir'), 'gettext/its'))
 
-executable('gdbus', 'gdbus-tool.c',
+executable('gdbus,e1f', 'gdbus-tool.c',
   install : true,
   c_args : gio_c_args,
   # intl.lib is not compatible with SAFESEH
