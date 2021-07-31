--- sdl/i_sdlmixer.c.orig	2012-04-25 16:09:47.000000000 +0100
+++ sdl/i_sdlmixer.c	2012-04-25 16:12:45.000000000 +0100
@@ -350,6 +350,7 @@
 	Uint16 audio_format;
 	int audio_channels;
 	int audio_buffers;
+	int i;

 	if (snd_initialized)
 		return;
@@ -360,6 +361,10 @@
 		return;
 	}

+	i = M_CheckParm("-samplerate");
+	if (i && i < myargc - 1)
+		snd_samplerate = atoi(myargv[i + 1]);
+
 	fprintf(stdout, "I_StartupSound (SDL_mixer):\n");

 	/* Initialize variables */
@@ -368,7 +373,7 @@
 	audio_format = SAMPLE_FORMAT;
 	audio_channels = SAMPLE_CHANNELS;
 	SAMPLECOUNT = 512;
-	audio_buffers = SAMPLECOUNT*snd_samplerate/11025;
+	audio_buffers = SAMPLECOUNT*snd_samplerate/snd_samplerate;

 	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) < 0)
 	{
@@ -378,7 +383,7 @@
 	snd_initialized = true;
 	SAMPLECOUNT = audio_buffers;
 	Mix_SetPostMix(audio_loop, NULL);
-	fprintf(stdout, "Configured audio device with %d samples/slice\n", SAMPLECOUNT);
+	fprintf(stdout, "Configured audio device with %d samples/slice at %d Hz\n", SAMPLECOUNT, audio_rate);
 	snd_SfxAvail = true;
 //	SDL_PauseAudio(0);

