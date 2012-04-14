--- snd_sdl.c.orig	2012-04-14 06:12:06.000000000 +0100
+++ snd_sdl.c	2012-04-14 06:18:23.000000000 +0100
@@ -24,10 +24,16 @@
 {
 	SDL_AudioSpec desired, obtained;
 
+	int i;
+
 	snd_inited = 0;
 
 	/* Set up the desired format */
-	desired.freq = desired_speed;
+	i = COM_CheckParm("-sndspeed");
+	if (i)
+		desired.freq = (int) (Q_atof(com_argv[i+1]));
+	else
+		desired.freq = desired_speed;
 	switch (desired_bits) {
 		case 8:
 			desired.format = AUDIO_U8;
