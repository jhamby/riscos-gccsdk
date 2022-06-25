--- yt_dlp/postprocessor/ffmpeg.py.orig	2022-06-25 13:25:26.241000825 +0100
+++ yt_dlp/postprocessor/ffmpeg.py	2022-06-25 13:27:20.224410619 +0100
@@ -127,7 +127,8 @@
         path = self._paths.get(prog)
         if path in self._version_cache:
             return self._version_cache[path], self._features_cache.get(path, {})
-        out = _get_exe_version_output(path, ['-bsfs'], to_screen=self.write_debug)
+        #out = _get_exe_version_output(path, ['-bsfs'], to_screen=self.write_debug)
+        out = None
         ver = detect_exe_version(out) if out else False
         if ver:
             regexs = [
