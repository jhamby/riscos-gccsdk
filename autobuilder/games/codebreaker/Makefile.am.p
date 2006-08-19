--- src/Makefile.am	2006-08-19 17:03:48.202896680 +1200
+++ src/Makefile.am.001	2006-08-19 17:19:46.297244200 +1200
@@ -1,6 +1,6 @@
 ## Process this file with automake to produce Makefile.in
 
-INCLUDES = @GTK_CFLAGS@ -DHOWTO=\"${prefix}/share/doc/$(PACKAGE)/HOWTO\"
+INCLUDES = @GTK_CFLAGS@ -DHOWTO=\"\<CodeBreaker\$$Dir\>/HOWTO\"
 
 bin_PROGRAMS = codebreaker
 
