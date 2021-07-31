--- sdl/i_sdlmixer.c.orig	2009-06-22 20:23:27.000000000 +0100
+++ sdl/i_sdlmixer.c	2012-04-25 19:18:05.000000000 +0100
@@ -346,6 +346,7 @@
 	Uint16 audio_format;
 	int audio_channels;
 	int audio_buffers;
+	int i;

 	if (snd_initialized)
 		return;
@@ -356,6 +357,10 @@
 		return;
 	}

+	i = M_CheckParm("-samplerate");
+	if (i && i < myargc - 1)
+        	snd_samplerate = atoi(myargv[i + 1]);
+
 	fprintf(stdout, "I_StartupSound (SDL_mixer):\n");

 	/* Initialize variables */
@@ -374,7 +379,7 @@
 	snd_initialized = true;
 	SAMPLECOUNT = audio_buffers;
 	Mix_SetPostMix(audio_loop, NULL);
-	fprintf(stdout, "Configured audio device with %d samples/slice\n", SAMPLECOUNT);
+	fprintf(stdout, "Configured audio device with %d samples/slice at %d Hz\n", SAMPLECOUNT, audio_rate);
 	snd_SfxAvail = true;
 //	SDL_PauseAudio(0);

