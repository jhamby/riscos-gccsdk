--- yt_dlp/utils.py.orig	2022-05-28 07:38:47.078303731 +0100
+++ yt_dlp/utils.py	2022-05-28 07:41:50.922582144 +0100
@@ -4920,10 +4920,11 @@
     with contextlib.suppress(FileNotFoundError):
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
