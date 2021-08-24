--- src/tracker/TrackerStartUp.cpp.orig	2013-11-08 16:12:01.000000000 -0800
+++ src/tracker/TrackerStartUp.cpp	2021-08-19 17:57:51.216967705 -0700
@@ -126,7 +126,7 @@
 		screen->paintSplash(LogoBig::rawData, LogoBig::width, LogoBig::height, LogoBig::width*3, 3, (int)shade); 		
 #endif
 		shade+=deltaT * (1.0f/6.25f);
-		deltaT = abs(::PPGetTickCount() - startTime);
+		deltaT = abs(static_cast<pp_int32>(::PPGetTickCount()) - startTime);
 		if (!deltaT) deltaT++;
 	}
 #if defined(__EXCLUDE_BIGLOGO__) || defined(__LOWRES__)
@@ -157,7 +157,7 @@
 		screen->paintSplash(LogoBig::rawData, LogoBig::width, LogoBig::height, LogoBig::width*3, 3, (int)shade); 		
 #endif
 		shade-=deltaT * (1.0f/6.25f);
-		deltaT = abs(::PPGetTickCount() - startTime);
+		deltaT = abs(static_cast<pp_int32>(::PPGetTickCount()) - startTime);
 		if (!deltaT) deltaT++;
 	}
 	screen->clear(); 	
