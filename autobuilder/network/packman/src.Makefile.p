--- src/Makefile.orig	2021-08-05 17:41:56.597712321 -0700
+++ src/Makefile	2021-08-05 17:42:33.257929465 -0700
@@ -15,11 +15,11 @@
   include FixDeps:Rules/make
 endif
 
-CXX=g++
+#CXX=g++
 CPPFLAGS= -ITBX: -ILibPKG: -ILibCurl:
 CXXFLAGS= -std=c++11 -O2 -Wall -mpoke-function-name -mthrowback
 
-LD = g++
+LD = $(CXX)
 
 CCRES=ccres
 LDFLAGS = -LTBX: -ltbx -LLibPkg: -lpkg -LLibCurl: -lcurl -lcares -lssh2 -lssl -lcrypto -LZLib: -lz -lstdc++ -static
@@ -40,10 +40,10 @@
 all:	$(TARGET) $(TARGETRES)
 
 $(TARGET): packman
-	elf2aif packman $(TARGET)
+	cp -p packman $(TARGET)
 
 packman:	$(OBJECTS)
-	$(LD) $(LDFLAGS) -o packman $(OBJECTS)
+	$(LD) -o packman $(OBJECTS) $(LDFLAGS)
 
 $(TARGETRES): CCRes
 	$(CCRES) CCRes $(TARGETRES)
