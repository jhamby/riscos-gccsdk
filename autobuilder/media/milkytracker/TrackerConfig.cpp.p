--- src/tracker/TrackerConfig.cpp.orig	2009-02-22 10:20:39.000000000 +0000
+++ src/tracker/TrackerConfig.cpp	2013-11-12 17:01:56.710706703 +0000
@@ -149,6 +149,6 @@
 const PPSystemString TrackerConfig::untitledSong("Untitled");
 
 const pp_int32 TrackerConfig::numMixFrequencies = 4;
-const pp_int32 TrackerConfig::mixFrequencies[] = {11025, 22050, 44100, 48000};
+const pp_int32 TrackerConfig::mixFrequencies[] = {22050, 44100, 48000, 88200};
 
 const pp_uint32 TrackerConfig::version = (MILKYTRACKER_HIVER << 16) + (MILKYTRACKER_LOVER);
