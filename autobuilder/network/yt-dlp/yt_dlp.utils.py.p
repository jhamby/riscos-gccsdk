--- yt_dlp/utils.py.orig	2021-11-18 14:26:22.595464307 +0000
+++ yt_dlp/utils.py	2021-11-18 18:50:29.670000000 +0000
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
