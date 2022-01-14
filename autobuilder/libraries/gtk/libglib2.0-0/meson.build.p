--- meson.build.orig	2021-03-18 13:47:48.374693600 +0000
+++ meson.build	2022-01-14 11:33:57.761886901 +0000
@@ -1427,7 +1427,11 @@
   glibconfig_conf.set_quoted('g_pid_format', 'i')
   glibconfig_conf.set_quoted('g_pollfd_format', '%d')
   glibconfig_conf.set('g_dir_separator', '/')
-  glibconfig_conf.set('g_searchpath_separator', ':')
+  if host_system == 'riscos'
+    glibconfig_conf.set('g_searchpath_separator', ',')
+  else
+    glibconfig_conf.set('g_searchpath_separator', ':')
+  endif
 endif
 
 g_sizet_compatibility = {
@@ -1784,8 +1788,18 @@
 #endif
 '''
 
+armeabi_test = '''#ifdef __ARM_EABI__
+#error arm_unknown_riscos = false
+#endif
+void func() { }
+'''
+
+# arm-unknown-riscos-gcc has __sync_bool_compare_and_swap, but it's not lock
+# free.
+arm_unknown_riscos = cc.compiles(armeabi_test, name : 'arm-unknown-riscos specific code')
+
 # We know that we can always use real ("lock free") atomic operations with MSVC
-if cc.get_id() == 'msvc' or cc.get_id() == 'clang-cl' or cc.links(atomictest, name : 'atomic ops')
+if cc.get_id() == 'msvc' or cc.get_id() == 'clang-cl' or (cc.links(atomictest, name : 'atomic ops') and not arm_unknown_riscos)
   have_atomic_lock_free = true
   if cc.get_id() == 'gcc' and not cc.compiles(atomicdefine, name : 'atomic ops define')
     # Old gcc release may provide
@@ -1810,7 +1824,7 @@
   glibconfig_conf.set('g_threads_impl_def', 'WIN32')
   glib_conf.set('THREADS_WIN32', 1)
 else
-  thread_dep = dependency('threads')
+  thread_dep = [] # dependency('threads') # RISC OS - don't want -pthread
   threads_implementation = 'posix'
   pthread_prefix = '''
       #ifndef _GNU_SOURCE
