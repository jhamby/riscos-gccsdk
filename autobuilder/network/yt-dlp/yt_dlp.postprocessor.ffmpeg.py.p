--- yt_dlp/postprocessor/ffmpeg.py.orig	2022-03-24 17:27:17.534502884 +0000
+++ yt_dlp/postprocessor/ffmpeg.py	2022-03-27 08:31:57.874254342 +0100
@@ -92,7 +92,8 @@
         programs = ['avprobe', 'avconv', 'ffmpeg', 'ffprobe']
 
         def get_ffmpeg_version(path, prog):
-            out = _get_exe_version_output(path, ['-bsfs'])
+            out = None
+            #out = _get_exe_version_output(path, ['-bsfs'])
             ver = detect_exe_version(out) if out else False
             if ver:
                 regexs = [
