--- yt_dlp/utils.py.orig	2022-03-24 17:27:17.530502733 +0000
+++ yt_dlp/utils.py	2022-03-27 08:29:25.767874719 +0100
@@ -5098,10 +5098,11 @@
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
