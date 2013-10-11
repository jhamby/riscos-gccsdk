--- mixer.c.orig	2013-09-15 10:39:14.150115905 +0100
+++ mixer.c	2013-09-15 10:40:45.608199957 +0100
@@ -148,7 +148,9 @@
 {
 	int result = 0;
 
+#ifdef MID_MUSIC
 	soundfont_paths = SDL_strdup("/usr/share/sounds/sf2/TimGM6mb.sf2:/usr/share/sounds/sf2/FluidR3_GM.sf2");
+#endif
 
 	if (flags & MIX_INIT_FLUIDSYNTH) {
 #ifdef USE_FLUIDSYNTH_MIDI
