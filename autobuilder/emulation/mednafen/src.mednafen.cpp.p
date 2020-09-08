--- src/mednafen.cpp.orig	2020-09-08 09:39:18.020389780 +0100
+++ src/mednafen.cpp	2020-09-08 11:58:07.244471685 +0100
@@ -21,6 +21,7 @@
 #include	<sys/stat.h>
 #include	<unistd.h>
 #include	<trio/trio.h>
+#include	<sstream>
 
 #include	"netplay.h"
 #include	"netplay-driver.h"
@@ -223,6 +224,17 @@
 
 static std::vector<DriveMediaStatus> DMStatus;
 
+
+namespace patch
+{
+    template < typename T > std::string to_string( const T& n )
+    {
+        std::ostringstream stm ;
+        stm << n ;
+        return stm.str() ;
+    }
+}
+
 static void SettingChanged(const char* name)
 {
  if(!strcmp(name, "video.deinterlacer"))
@@ -711,7 +723,7 @@
      if(vfszs.size())
       vfszs += _(", ");
 
-     vfszs += std::to_string(3 * *vec);
+     vfszs += patch::to_string(3 * *vec);
     }
 
     throw MDFN_Error(0, _("Custom palette file's size(%llu bytes) is incorrect.  Valid sizes are: %s"), (unsigned long long)fpsz, vfszs.c_str());
