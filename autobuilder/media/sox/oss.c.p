--- src/oss.c.orig	2012-11-04 19:48:16.000000000 +0000
+++ src/oss.c	2012-11-04 20:49:39.000000000 +0000
@@ -113,7 +113,7 @@

     if (ft->signal.channels > 2) ft->signal.channels = 2;

-    if (ioctl(fileno(ft->fp), (size_t) SNDCTL_DSP_RESET, 0) < 0)
+    if (ioctl(fileno((FILE *)ft->fp), (size_t) SNDCTL_DSP_RESET, 0) < 0)
     {
         lsx_fail_errno(ft,SOX_EOF,"Unable to reset OSS driver.  Possibly accessing an invalid file/device");
         return(SOX_EOF);
@@ -121,7 +121,7 @@

     /* Query the supported formats and find the best match
      */
-    rc = ioctl(fileno(ft->fp), SNDCTL_DSP_GETFMTS, &tmp);
+    rc = ioctl(fileno((FILE *)ft->fp), SNDCTL_DSP_GETFMTS, &tmp);
     if (rc == 0) {
         if ((tmp & sampletype) == 0)
         {
@@ -160,7 +160,7 @@

         }
         tmp = sampletype;
-        rc = ioctl(fileno(ft->fp), SNDCTL_DSP_SETFMT, &tmp);
+        rc = ioctl(fileno((FILE *)ft->fp), SNDCTL_DSP_SETFMT, &tmp);
     }
     /* Give up and exit */
     if (rc < 0 || tmp != sampletype)
@@ -173,7 +173,7 @@
     else dsp_stereo = 0;

     tmp = dsp_stereo;
-    if (ioctl(fileno(ft->fp), SNDCTL_DSP_STEREO, &tmp) < 0)
+    if (ioctl(fileno((FILE *)ft->fp), SNDCTL_DSP_STEREO, &tmp) < 0)
     {
         lsx_warn("Couldn't set to %s", dsp_stereo?  "stereo":"mono");
         dsp_stereo = 0;
@@ -183,7 +183,7 @@
         ft->signal.channels = tmp + 1;

     tmp = ft->signal.rate;
-    if (ioctl (fileno(ft->fp), SNDCTL_DSP_SPEED, &tmp) < 0 ||
+    if (ioctl (fileno((FILE *)ft->fp), SNDCTL_DSP_SPEED, &tmp) < 0 ||
         (int)ft->signal.rate != tmp) {
         /* If the rate the sound card is using is not within 1% of what
          * the user specified then override the user setting.
@@ -202,7 +202,7 @@
      * its size based on specific rates/formats.
      */
     file->size = 0;
-    ioctl (fileno(ft->fp), SNDCTL_DSP_GETBLKSIZE, &file->size);
+    ioctl (fileno((FILE *)ft->fp), SNDCTL_DSP_GETBLKSIZE, &file->size);
     if (file->size < 4 || file->size > 65536) {
             lsx_fail_errno(ft,SOX_EOF,"Invalid audio buffer size %" PRIuPTR, file->size);
             return (SOX_EOF);
@@ -211,7 +211,7 @@
     file->pos = 0;
     file->buf = lsx_malloc(file->size);

-    if (ioctl(fileno(ft->fp), (size_t) SNDCTL_DSP_SYNC, NULL) < 0) {
+    if (ioctl(fileno((FILE *)ft->fp), (size_t) SNDCTL_DSP_SYNC, NULL) < 0) {
         lsx_fail_errno(ft,SOX_EOF,"Unable to sync dsp");
         return (SOX_EOF);
     }
