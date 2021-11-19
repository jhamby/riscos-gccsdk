--- yt_dlp/utils.py.orig	2021-11-18 14:26:22.595464307 +0000
+++ yt_dlp/utils.py	2021-11-18 18:50:29.670000000 +0000
@@ -4179,7 +4179,9 @@
         self._cache = {}
 
     def getpage(self, pagenum):
-        page_results = self._cache.get(pagenum) or list(self._pagefunc(pagenum))
+        page_results = self._cache.get(pagenum)
+        if page_results is None:
+            page_results = list(self._pagefunc(pagenum))
         if self._use_cache:
             self._cache[pagenum] = page_results
         return page_results
@@ -4195,7 +4197,9 @@
         if not isinstance(idx, int) or idx < 0:
             raise TypeError('indices must be non-negative integers')
         entries = self.getslice(idx, idx + 1)
-        return entries[0] if entries else None
+        if not entries:
+            raise IndexError()
+        return entries[0]
 
 
 class OnDemandPagedList(PagedList):
@@ -6412,10 +6416,11 @@
     try:
         plugins_spec = importlib.util.spec_from_file_location(
             name, os.path.join(get_executable_path(), 'ytdlp_plugins', name, '__init__.py'))
-        plugins = importlib.util.module_from_spec(plugins_spec)
-        sys.modules[plugins_spec.name] = plugins
-        plugins_spec.loader.exec_module(plugins)
-        for name in dir(plugins):
+        if sys.platform != "riscos":
+         plugins = importlib.util.module_from_spec(plugins_spec)
+         sys.modules[plugins_spec.name] = plugins
+         plugins_spec.loader.exec_module(plugins)
+         for name in dir(plugins):
             if name in namespace:
                 continue
             if not name.endswith(suffix):
