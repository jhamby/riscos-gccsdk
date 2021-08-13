--- host_applications/linux/apps/hello_pi/Makefile.include.orig	2021-07-28 19:35:44.877572545 -0700
+++ host_applications/linux/apps/hello_pi/Makefile.include	2021-07-28 19:37:39.274416050 -0700
@@ -1,22 +1,25 @@
 
-CFLAGS+=-DSTANDALONE -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -DTARGET_POSIX -D_LINUX -fPIC -DPIC -D_REENTRANT -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -U_FORTIFY_SOURCE -Wall -g -DHAVE_LIBOPENMAX=2 -DOMX -DOMX_SKIP64BIT -ftree-vectorize -pipe -DUSE_EXTERNAL_OMX -DHAVE_LIBBCM_HOST -DUSE_EXTERNAL_LIBBCM_HOST -DUSE_VCHIQ_ARM -Wno-psabi
+CFLAGS+=-DSTANDALONE -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -DTARGET_POSIX -D_LINUX -fPIC -DPIC -D_REENTRANT -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -U_FORTIFY_SOURCE -Wall -g -DHAVE_LIBOPENMAX=2 -DOMX -DOMX_SKIP64BIT -ftree-vectorize -pipe -DUSE_EXTERNAL_OMX -DHAVE_LIBBCM_HOST -DUSE_EXTERNAL_LIBBCM_HOST -DUSE_VCHIQ_ARM -Wno-psabi
 
-LDFLAGS+=-L$(SDKSTAGE)/opt/vc/lib/ -lbrcmGLESv2 -lbrcmEGL -lopenmaxil -lbcm_host -lvcos -lvchiq_arm -lpthread -lrt -lm -L$(SDKSTAGE)/opt/vc/src/hello_pi/libs/ilclient -L$(SDKSTAGE)/opt/vc/src/hello_pi/libs/vgfont -L$(SDKSTAGE)/opt/vc/src/hello_pi/libs/revision
+LDFLAGS+=-L$(GCCSDK_INSTALL_ENV)/lib -lGLESv2 -lEGL -lbcm_host -L../libs/revision
 
-INCLUDES+=-I$(SDKSTAGE)/opt/vc/include/ -I$(SDKSTAGE)/opt/vc/include/interface/vcos/pthreads -I$(SDKSTAGE)/opt/vc/include/interface/vmcs_host/linux -I./ -I$(SDKSTAGE)/opt/vc/src/hello_pi/libs/ilclient -I$(SDKSTAGE)/opt/vc/src/hello_pi/libs/vgfont -I$(SDKSTAGE)/opt/vc/src/hello_pi/libs/revision
+INCLUDES+=-I$(GCCSDK_INSTALL_ENV)/include \
+	  -I$(GCCSDK_INSTALL_ENV)/include/interface/vcos/riscos \
+	  -I$(GCCSDK_INSTALL_ENV)/include/interface/vmcs_host/linux \
+	  -I../libs/revision
 
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
+	$(CC) -o $@ -Wl,--whole-archive $(OBJS) $(LDFLAGS) -Wl,--no-whole-archive
 
 %.a: $(OBJS)
 	$(AR) r $@ $^
