--- config.old	2003-01-08 18:11:32.000000000 +0000
+++ config	2003-01-08 19:10:10.000000000 +0000
@@ -73,7 +73,7 @@
 # ------------------------------------------------------------------ install
 
 # Locations for "make install". You may need to be root do "make install".
-INSTALL_ROOT= /usr/local
+INSTALL_ROOT= $GCCSDK_INSTALL_ENV
 INSTALL_BIN= $(INSTALL_ROOT)/bin
 INSTALL_INC= $(INSTALL_ROOT)/include
 INSTALL_LIB= $(INSTALL_ROOT)/lib
@@ -82,8 +82,8 @@
 # You might prefer to use "install" if you have it.
 INSTALL_EXEC= cp
 INSTALL_DATA= cp
-#INSTALL_EXEC= install -m 0755
-#INSTALL_DATA= install -m 0644
+INSTALL_EXEC=$(INSTALL) -m 0755
+INSTALL_DATA=$(INSTALL) -m 0644
 
 # == END OF USER SETTINGS. DO NOT CHANGE ANYTHING BELOW THIS LINE =============
 
@@ -99,7 +99,7 @@
 V=4.0
 
 %.os: %.c
-	$(CC) $(CFLAGS) -o $@ -fPIC -c $<
+	$(CC) $(CFLAGS) -o $@ -c $<
 
 
 # (end of config)
--- config.orig	2009-08-02 20:09:22.000000000 -0700
+++ config	2009-08-02 20:09:34.000000000 -0700
@@ -28,7 +28,7 @@
 # ------------------------------------------------------------------ C compiler
 
 # You need an ANSI C compiler. gcc is a popular one.
-CC= gcc
+#CC= gcc
 WARN= -ansi -pedantic -Wall
 
 # On IRIX, cc is a good ANSI compiler.
