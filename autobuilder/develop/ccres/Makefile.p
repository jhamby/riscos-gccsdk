--- Makefile.orig	2013-02-01 15:14:08.847278400 +0000
+++ Makefile	2013-02-01 15:12:48.039136500 +0000
@@ -18,7 +18,9 @@
 HOST_CCFLAGS = -I. -I${GCCSDK_INSTALL_ENV}/include -DHAVE_STRCASECMP -DHAVE_STRNCASECMP
 # Using the GCCSDK cross-compiler:
 CROSS_CC := $(wildcard $(GCCSDK_INSTALL_CROSSBIN)/*gcc)
-CROSS_CCFLAGS = -I${GCCSDK_INSTALL_ENV}/include -mpoke-function-name -mlibscl
+# For autobuilder using unixlib as it doesn't build a libscl version of oslib
+# CROSS_CCFLAGS = -I${GCCSDK_INSTALL_ENV}/include -mpoke-function-name -mlibscl
+CROSS_CCFLAGS = -I${GCCSDK_INSTALL_ENV}/include -mpoke-function-name -static  -DHAVE_STRCASECMP -DHAVE_STRNCASECMP
 
 ifeq ($(CROSS_COMPILE),)
 CC=$(HOST_CC)
@@ -37,6 +39,7 @@
 
 COPY = cp
 COPYFLAGS = 
+NATIVE_CCRES = $(GCCSDK_INSTALL_CROSSBIN)/ccres
 
 MKDIR = mkdir
 MKDIRFLAGS = -p
@@ -94,7 +97,9 @@
 	Release/\!CCres/Messages \
 	Release/\!CCres/\!RunImage$(APPEXT) \
 	Release/\!CCres/History
-APPEXT = ,ff8
+ 
+#APPEXT = ,ff8
+APPEXT = ,e1f
 endif
 
 DOBJS = Release/ccres$(APPEXT) $(CCRES_APPDATA)
@@ -114,7 +119,7 @@
 
 Release/\!CCres/Res,fae: Release/ccres$(APPEXT) Data/Res
 	$(MKDIR) $(MKDIRFLAGS) Release/\!CCres
-	Release/ccres$(APPEXT) Data/Res $@ $(COPYFLAGS)
+	$(NATIVE_CCRES) Data/Res $@
 
 Release/\!CCres/\!Run,feb: Data/!Run,feb
 	$(MKDIR) $(MKDIRFLAGS) Release/\!CCres
