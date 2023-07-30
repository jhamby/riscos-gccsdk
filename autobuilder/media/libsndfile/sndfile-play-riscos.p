From dcae57aa3622a7a1c2c2c45faa4f59930f1410e5 Mon Sep 17 00:00:00 2001
From: Cameron Cawley <ccawley2011@gmail.com>
Date: Sat, 6 May 2023 17:35:33 +0100
Subject: [PATCH] sndfile-play: Add RISC OS support

---
 programs/sndfile-play.c | 6 +++---
 1 file changed, 3 insertions(+), 3 deletions(-)

diff --git a/programs/sndfile-play.c b/programs/sndfile-play.c
index 16aaf4d2..dfd818fa 100644
--- programs/sndfile-play.c
+++ programs/sndfile-play.c
@@ -56,7 +56,7 @@
 
 #if defined (__ANDROID__)
 
-#elif defined (__linux__) || defined (__FreeBSD_kernel__) || defined (__FreeBSD__)
+#elif defined (__linux__) || defined (__FreeBSD_kernel__) || defined (__FreeBSD__) || defined(__riscos__)
 	#include 	<fcntl.h>
 	#include 	<sys/ioctl.h>
 	#include 	<sys/soundcard.h>
@@ -355,7 +355,7 @@ alsa_write_float (snd_pcm_t *alsa_dev, float *data, int frames, int channels)
 **	Linux/OSS functions for playing a sound.
 */
 
-#if !defined (__ANDROID__) && (defined (__linux__) || defined (__FreeBSD_kernel__) || defined (__FreeBSD__))
+#if !defined (__ANDROID__) && (defined (__linux__) || defined (__FreeBSD_kernel__) || defined (__FreeBSD__) || defined (__riscos__))
 
 static	int	opensoundsys_open_device (int channels, int srate) ;
 
@@ -841,7 +841,7 @@ main (int argc, char *argv [])
 		else
 	#endif
 		opensoundsys_play (argc, argv) ;
-#elif defined (__FreeBSD_kernel__) || defined (__FreeBSD__)
+#elif defined (__FreeBSD_kernel__) || defined (__FreeBSD__) || defined(__riscos__)
 	opensoundsys_play (argc, argv) ;
 #elif HAVE_SNDIO_H
 	sndio_play (argc, argv) ;
-- 
2.30.2

