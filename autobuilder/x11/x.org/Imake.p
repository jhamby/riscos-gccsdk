Index: config/cf/Imake.cf
===================================================================
RCS file: /cvs/xorg/xc/config/cf/Imake.cf,v
retrieving revision 1.8
diff -u -r1.8 Imake.cf
--- config/cf/Imake.cf	4 May 2005 04:14:57 -0000	1.8
+++ config/cf/Imake.cf	11 May 2005 07:16:34 -0000
@@ -1133,6 +1133,19 @@
 # endif
 #endif
 
+#ifdef __riscos__
+# define RISCOSArchitecture
+# define MacroIncludeFile <riscos.cf>
+# define MacroFile riscos.cf
+# undef riscos
+# ifdef __arm__
+#  define Arm32Architecture
+#  undef arm
+#  undef __arm
+#  undef __arm__
+# endif
+#endif
+
 #ifndef MacroIncludeFile
 XCOMM WARNING:  Imake.cf not configured; guessing at definitions!!!
 XCOMM This might mean that BOOTSTRAPCFLAGS was not set when building imake.
