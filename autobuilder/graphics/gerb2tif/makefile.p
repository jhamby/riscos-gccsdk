--- makefile	2008-08-16 10:03:48.000000000 -0700
+++ makefile.new	2008-08-16 10:04:59.000000000 -0700
@@ -10,11 +10,12 @@
 TARGET = gerb2tif
 
 # to compile on linux uncommend the following line
-TARGET := $(TARGET).exe
+#TARGET := $(TARGET).exe
 
 
-CC 		= g++
-CFLAGS 	=  -DVERSION_NUM="\"$(VERSION)\"" -I/usr/local/include -static
+CC 		= $(CXX)
+#CFLAGS 	=  -DVERSION_NUM="\"$(VERSION)\"" -I/usr/local/include -static
+CFLAGS 	=  -DVERSION_NUM="\"$(VERSION)\"" -I$(GCCSDK_INSTALL_ENV)/include -L$(GCCSDK_INSTALL_ENV)/lib
 #CFLAGs +=  
 LDFLAGS =  
 DISTRIBUTIION_FILES = GPL readme.txt CHANGES.TXT
