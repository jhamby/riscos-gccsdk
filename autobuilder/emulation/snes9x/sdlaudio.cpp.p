--- sdl/sdlaudio.cpp.orig	2012-04-10 14:57:55.000000000 +0100
+++ sdl/sdlaudio.cpp	2012-04-10 14:59:21.000000000 +0100
@@ -114,6 +114,9 @@
 	audiospec->channels = Settings.Stereo ? 2 : 1;
 	audiospec->format = Settings.SixteenBitSound ? AUDIO_S16SYS : AUDIO_U8;
 	audiospec->samples = (sound_buffer_size * audiospec->freq / 1000) >> 1;
+	printf("samples: %d\n", audiospec->samples);
+	audiospec->samples = 1024;
+
 	audiospec->callback = sdl_audio_callback;

 	printf ("SDL sound driver initializing...\n");
