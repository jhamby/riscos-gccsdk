--- host_applications/linux/apps/hello_pi/Makefile.include.orig	2016-05-30 18:44:39.914887276 +0100
+++ host_applications/linux/apps/hello_pi/Makefile.include	2016-05-30 18:42:35.718885667 +0100
@@ -1,22 +1,27 @@
- 
-CFLAGS+=-DSTANDALONE -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -DTARGET_POSIX -D_LINUX -fPIC -DPIC -D_REENTRANT -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -U_FORTIFY_SOURCE -Wall -g -DHAVE_LIBOPENMAX=2 -DOMX -DOMX_SKIP64BIT -ftree-vectorize -pipe -DUSE_EXTERNAL_OMX -DHAVE_LIBBCM_HOST -DUSE_EXTERNAL_LIBBCM_HOST -DUSE_VCHIQ_ARM -Wno-psabi
 
-LDFLAGS+=-L$(SDKSTAGE)/opt/vc/lib/ -lGLESv2 -lEGL -lopenmaxil -lbcm_host -lvcos -lvchiq_arm -lpthread -lrt -lm -L$(SDKSTAGE)/opt/vc/src/hello_pi/libs/ilclient -L$(SDKSTAGE)/opt/vc/src/hello_pi/libs/vgfont
-
-INCLUDES+=-I$(SDKSTAGE)/opt/vc/include/ -I$(SDKSTAGE)/opt/vc/include/interface/vcos/pthreads -I$(SDKSTAGE)/opt/vc/include/interface/vmcs_host/linux -I./ -I$(SDKSTAGE)/opt/vc/src/hello_pi/libs/ilclient -I$(SDKSTAGE)/opt/vc/src/hello_pi/libs/vgfont
+CFLAGS+=-DSTANDALONE -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -DTARGET_POSIX -D_LINUX \
+	-D_REENTRANT -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -U_FORTIFY_SOURCE -Wall \
+	-DHAVE_LIBOPENMAX=2 -DOMX -DOMX_SKIP64BIT -ftree-vectorize -pipe -DUSE_EXTERNAL_OMX \
+	-DHAVE_LIBBCM_HOST -DUSE_EXTERNAL_LIBBCM_HOST -DUSE_VCHIQ_ARM -Wno-psabi -mfpu=vfp
+
+LDFLAGS+=-L$(GCCSDK_INSTALL_ENV)/lib -lGLESv2 -lEGL -lbcm_host
+
+INCLUDES+=-I$(GCCSDK_INSTALL_ENV)/include \
+	  -I$(GCCSDK_INSTALL_ENV)/include/interface/vcos/riscos \
+	  -I$(GCCSDK_INSTALL_ENV)/include/interface/vmcs_host/linux
 
 all: $(BIN) $(LIB)
 
 %.o: %.c
 	@rm -f $@ 
-	$(CC) $(CFLAGS) $(INCLUDES) -g -c $< -o $@ -Wno-deprecated-declarations
+	$(CC) $(CFLAGS) $(INCLUDES) -O3 -c $< -o $@ -Wno-deprecated-declarations
 
 %.o: %.cpp
 	@rm -f $@ 
-	$(CXX) $(CFLAGS) $(INCLUDES) -g -c $< -o $@ -Wno-deprecated-declarations
+	$(CXX) $(CFLAGS) $(INCLUDES) -O3 -c $< -o $@ -Wno-deprecated-declarations
 
-%.bin: $(OBJS)
-	$(CC) -o $@ -Wl,--whole-archive $(OBJS) $(LDFLAGS) -Wl,--no-whole-archive -rdynamic
+%,e1f: $(OBJS)
+	$(CC) -o $@ -Wl,--whole-archive $(OBJS) $(LDFLAGS) -Wl,--no-whole-archive -mfpu=vfp
 
 %.a: $(OBJS)
 	$(AR) r $@ $^
