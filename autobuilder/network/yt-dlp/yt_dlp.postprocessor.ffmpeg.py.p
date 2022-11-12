--- yt_dlp/postprocessor/ffmpeg.py.orig	2022-11-12 08:28:26.224869704 +0000
+++ yt_dlp/postprocessor/ffmpeg.py	2022-11-12 08:30:18.521133873 +0000
@@ -137,7 +137,8 @@
         path = self._paths.get(prog)
         if path in self._version_cache:
             return self._version_cache[path], self._features_cache.get(path, {})
-        out = _get_exe_version_output(path, ['-bsfs'])
+        # out = _get_exe_version_output(path, ['-bsfs'])
+        out = None
         ver = detect_exe_version(out) if out else False
         if ver:
             regexs = [
