--- src/sound.c.orig	2020-05-09 12:21:48.688775440 +0100
+++ src/sound.c	2020-05-09 12:24:21.225740169 +0100
@@ -53,9 +53,10 @@
 
 	/* The initial default sample rate for the Risc PC is not 44100 */
 	samplefreq = 41666;
-
+#ifndef __riscos__
 	/* Call the platform specific code to start the audio playing */
 	plt_sound_init(BUFFERLENBYTES);
+#endif
 }
 
 /**
@@ -65,9 +66,10 @@
 sound_restart(void)
 {
 	assert(config.soundenabled);
-
+#ifndef __riscos__
 	/* Pass the call on to the platform specific code */
 	plt_sound_restart();
+#endif
 }
 
 /**
@@ -77,9 +79,10 @@
 sound_pause(void)
 {
 	assert(!config.soundenabled);
-
+#ifndef __riscos__
 	/* Pass the call on to the platform specific code */
 	plt_sound_pause();
+#endif
 }
 
 /**
@@ -173,6 +176,7 @@
 void
 sound_buffer_update(void)
 {
+#ifndef __riscos__
 	while (bigsoundbuffertail != bigsoundbufferhead) {
 		if(plt_sound_buffer_free() >= (BUFFERLENBYTES)) {
 			if (config.soundenabled) {
@@ -186,5 +190,6 @@
 			break;
 		}
 	}
+#endif
 }
 
