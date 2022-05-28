--- yt_dlp/postprocessor/ffmpeg.py.orig	2022-05-28 07:38:47.078303731 +0100
+++ yt_dlp/postprocessor/ffmpeg.py	2022-05-28 07:45:27.351237773 +0100
@@ -91,7 +91,8 @@
             if path in self._version_cache:
                 self._versions[prog], self._features = self._version_cache[path], self._features_cache.get(path, {})
                 return
-            out = _get_exe_version_output(path, ['-bsfs'], to_screen=self.write_debug)
+            out = None
+            #out = _get_exe_version_output(path, ['-bsfs'], to_screen=self.write_debug)
             ver = detect_exe_version(out) if out else False
             if ver:
                 regexs = [
