--- sdlsound.c.orig	2008-01-09 13:27:54.179917200 +0000
+++ sdlsound.c	2008-01-09 13:32:15.153917200 +0000
@@ -203,12 +203,20 @@
 
 /* Load the specified sample */
 int load_sample(int num) {
-  
+#ifdef __riscos__
+  /* Avoid problem code below, RISC OS always uses DATADIR */
+  if ((samples[num] = Mix_LoadWAV(datafilename(DATADIR, samplename[num]))))
+     return 1;
+#else
+  /* The following code doesn't work on RISC OS GCC 3.4.6
+   * I'm not sure is should work anywhere
+   */
   if((samples[num] = Mix_LoadWAV(samplename[num])) ||
      Mix_LoadWAV(datafilename(NULL, samplename[num])) || 
      Mix_LoadWAV(datafilename(DATADIR, samplename[num])) || 
      Mix_LoadWAV(datafilename(bindir, samplename[num])))
     return 1;
+#endif
   return 0;
 }
 
