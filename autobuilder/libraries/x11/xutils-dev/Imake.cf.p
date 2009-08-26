--- xorg-cf-files/Imake.cf.orig	2009-08-25 20:51:05.000000000 -0700
+++ xorg-cf-files/Imake.cf	2009-08-25 20:53:15.000000000 -0700
@@ -1175,6 +1175,12 @@
 # endif
 #endif
 
+#ifdef __riscos__
+# define RISCOSArchitecture
+# define MacroIncludeFile <riscos.cf>
+# define MacroFile riscos.cf
+#endif
+
 #ifndef MacroIncludeFile
 XCOMM WARNING:  Imake.cf not configured; guessing at definitions!!!
 XCOMM This might mean that BOOTSTRAPCFLAGS was not set when building imake.
