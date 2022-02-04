--- yt_dlp/postprocessor/ffmpeg.py.orig	2022-02-04 11:44:05.847384293 +0000
+++ yt_dlp/postprocessor/ffmpeg.py	2022-02-04 11:31:45.850000000 +0000
@@ -92,7 +92,8 @@
         programs = ['avprobe', 'avconv', 'ffmpeg', 'ffprobe']
 
         def get_ffmpeg_version(path, prog):
-            out = _get_exe_version_output(path, ['-bsfs'])
+            out = None
+            #out = _get_exe_version_output(path, ['-bsfs'])
             ver = detect_exe_version(out) if out else False
             if ver:
                 regexs = [
