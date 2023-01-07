--- yt_dlp/plugins.py.orig	2023-01-07 10:32:52.210802507 +0000
+++ yt_dlp/plugins.py	2023-01-07 10:33:54.922582393 +0000
@@ -79,7 +79,8 @@

         parts = Path(*fullname.split('.'))
         locations = set()
-        for path in dict.fromkeys(candidate_locations):
+        if sys.platform != "riscos":
+         for path in dict.fromkeys(candidate_locations):
             candidate = path / parts
             if candidate.is_dir():
                 locations.add(str(candidate))
@@ -157,10 +157,11 @@
     with contextlib.suppress(FileNotFoundError):
         spec = importlib.util.spec_from_file_location(
             name, Path(get_executable_path(), COMPAT_PACKAGE_NAME, name, '__init__.py'))
-        plugins = importlib.util.module_from_spec(spec)
-        sys.modules[spec.name] = plugins
-        spec.loader.exec_module(plugins)
-        classes.update(load_module(plugins, spec.name, suffix))
+        if sys.platform != "riscos":
+         plugins = importlib.util.module_from_spec(spec)
+         sys.modules[spec.name] = plugins
+         spec.loader.exec_module(plugins)
+         classes.update(load_module(plugins, spec.name, suffix))
 
     return classes
 
