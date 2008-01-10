--- btcontent.cpp.orig	2008-01-09 19:58:34.000000000 -0800
+++ btcontent.cpp	2008-01-09 20:02:24.000000000 -0800
@@ -198,7 +198,8 @@
 #ifdef HAVE_CTIME_R_3
     ctime_r(&m_create_date, s, sizeof(s));
 #else
-    ctime_r(&m_create_date, s);
+    //ctime_r(&m_create_date, s);
+   strcpy(s, ctime(&m_create_date));
 #endif
     if( s[strlen(s)-1] == '\n' ) s[strlen(s)-1] = '\0';
     CONSOLE.Print("Created On: %s", s);
