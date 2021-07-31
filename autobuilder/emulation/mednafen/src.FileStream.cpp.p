--- src/FileStream.cpp.orig	2020-09-08 12:07:07.138173259 +0100
+++ src/FileStream.cpp	2020-09-08 12:13:08.975302025 +0100
@@ -383,6 +383,7 @@
  if(locked)
   return;
 
+#ifndef __riscos__
  #ifdef WIN32
  OVERLAPPED olp;
 
@@ -401,7 +402,7 @@
   throw MDFN_Error(ene.Errno(), _("Error locking opened file \"%s\": %s"), path_save.c_str(), ene.StrError());
  } 
  #endif
-
+#endif
  locked = true;
 }
 
@@ -410,6 +411,7 @@
  if(!locked)
   return;
 
+#ifndef __riscos__
  #ifdef WIN32
  if(!UnlockFile((HANDLE)_get_osfhandle(fileno(fp)), ~(DWORD)0, ~(DWORD)0, 1, 0))
  {
@@ -423,6 +425,7 @@
   throw MDFN_Error(ene.Errno(), _("Error unlocking opened file \"%s\": %s"), path_save.c_str(), ene.StrError());
  } 
  #endif
+#endif
 
  locked = false;
 }
