--- Makefile.orig	2017-01-05 12:40:48.591558100 +0000
+++ Makefile	2017-01-05 13:32:22.409823100 +0000
@@ -50,6 +50,8 @@
 
 # Compiler...
 
+CCNATIVE=/usr/bin/gcc
+
 ifeq ($(ENABLE_TILT),wii)
 	# -std=c99 because we need isnormal and -fms-extensions because
 	# libwiimote headers make heavy use of the "unnamed fields" GCC
@@ -64,8 +66,8 @@
 
 # Preprocessor...
 
-SDL_CPPFLAGS := $(shell sdl2-config --cflags)
-PNG_CPPFLAGS := $(shell libpng-config --cflags)
+SDL_CPPFLAGS := $(shell $(GCCSDK_INSTALL_ENV)/vfp/bin/sdl2-config --cflags)
+PNG_CPPFLAGS := $(shell $(GCCSDK_INSTALL_ENV)/bin/libpng-config --cflags)
 
 ALL_CPPFLAGS := $(SDL_CPPFLAGS) $(PNG_CPPFLAGS) -Ishare
 
@@ -128,8 +130,8 @@
 #------------------------------------------------------------------------------
 # Libraries
 
-SDL_LIBS := $(shell sdl2-config --libs)
-PNG_LIBS := $(shell libpng-config --libs)
+SDL_LIBS := $(shell $(GCCSDK_INSTALL_ENV)/vfp/bin/sdl2-config --libs)
+PNG_LIBS := $(shell $(GCCSDK_INSTALL_ENV)/bin/libpng-config --libs)
 
 ifeq ($(ENABLE_FS),stdio)
 FS_LIBS :=
@@ -157,9 +159,9 @@
 endif
 
 ifeq ($(ENABLE_OPENGLES),1)
-	OGL_LIBS := -lGLESv1_CM
+	OGL_LIBS := -lGLESv2 -lbcm_host -lEGL
 else
-	OGL_LIBS := -lGL
+	OGL_LIBS := -lGLESv2 -lbcm_host -lEGL
 endif
 
 ifeq ($(PLATFORM),mingw)
@@ -180,6 +182,10 @@
 	OGL_LIBS  := -framework OpenGL
 endif
 
+# Add lib for RISC OS
+INTL_LIBS := -lintl
+
+
 BASE_LIBS := -ljpeg $(PNG_LIBS) $(FS_LIBS) -lm
 
 ifeq ($(PLATFORM),darwin)
@@ -218,19 +224,19 @@
 #------------------------------------------------------------------------------
 
 MAPC_OBJS := \
-	share/vec3.o        \
-	share/base_image.o  \
-	share/solid_base.o  \
-	share/binary.o      \
-	share/base_config.o \
-	share/common.o      \
-	share/fs_common.o   \
-	share/fs_png.o      \
-	share/fs_jpg.o      \
-	share/dir.o         \
-	share/array.o       \
-	share/list.o        \
-	share/mapc.o
+	share/vec3.O        \
+	share/base_image.O  \
+	share/solid_base.O  \
+	share/binary.O      \
+	share/base_config.O \
+	share/common.O      \
+	share/fs_common.O   \
+	share/fs_png.O      \
+	share/fs_jpg.O      \
+	share/dir.O         \
+	share/array.O       \
+	share/list.O        \
+	share/mapc.O
 BALL_OBJS := \
 	share/lang.o        \
 	share/st_common.o   \
@@ -352,11 +358,11 @@
 ifeq ($(ENABLE_FS),stdio)
 BALL_OBJS += share/fs_stdio.o
 PUTT_OBJS += share/fs_stdio.o
-MAPC_OBJS += share/fs_stdio.o
+MAPC_OBJS += share/fs_stdio.O
 else
 BALL_OBJS += share/fs_physfs.o
 PUTT_OBJS += share/fs_physfs.o
-MAPC_OBJS += share/fs_physfs.o
+MAPC_OBJS += share/fs_physfs.O
 endif
 
 ifeq ($(ENABLE_TILT),wii)
@@ -393,7 +399,7 @@
 
 BALL_DEPS := $(BALL_OBJS:.o=.d)
 PUTT_DEPS := $(PUTT_OBJS:.o=.d)
-MAPC_DEPS := $(MAPC_OBJS:.o=.d)
+MAPC_DEPS := $(MAPC_OBJS:.O=.d)
 
 MAPS := $(shell find data -name "*.map" \! -name "*.autosave.map")
 SOLS := $(MAPS:%.map=%.sol)
@@ -407,12 +413,12 @@
 #------------------------------------------------------------------------------
 
 %.o : %.c
-	$(CC) $(ALL_CFLAGS) $(ALL_CPPFLAGS) -MM -MP -MF $*.d -MT "$@" $<
-	$(CC) $(ALL_CFLAGS) $(ALL_CPPFLAGS) -o $@ -c $<
+	$(CC) -I$(GCCSDK_INSTALL_ENV)/vfp/include/SDL2 $(ALL_CFLAGS) $(ALL_CPPFLAGS) -MM -MP -MF $*.d -MT "$@" $<
+	$(CC) -I$(GCCSDK_INSTALL_ENV)/vfp/include/SDL2 $(ALL_CFLAGS) $(ALL_CPPFLAGS) -mfpu=vfp -o $@ -c $<
 
 %.o : %.cpp
-	$(CXX) $(ALL_CXXFLAGS) $(ALL_CPPFLAGS) -MM -MP -MF $*.d -MT "$@" $<
-	$(CXX) $(ALL_CXXFLAGS) $(ALL_CPPFLAGS) -o $@ -c $<
+	$(CXX) -I$(GCCSDK_INSTALL_ENV)/vfp/include/SDL2 $(ALL_CXXFLAGS) $(ALL_CPPFLAGS) -MM -MP -MF $*.d -MT "$@" $<
+	$(CXX) -I$(GCCSDK_INSTALL_ENV)/vfp/include/SDL2 $(ALL_CXXFLAGS) $(ALL_CPPFLAGS) -mfpu=vfp -o $@ -c $<
 
 %.sol : %.map $(MAPC_TARG)
 	$(MAPC) $< data
@@ -423,6 +429,10 @@
 %.ico.o: dist/ico/%.ico
 	echo "1 ICON \"$<\"" | $(WINDRES) -o $@
 
+%.O : %.c
+	$(CCNATIVE) -I/usr/include/SDL2 -MM -MP -MF $*.d -MT "$@" $<
+	$(CCNATIVE) -I/usr/include/SDL2 -o $@ -c $<
+
 #------------------------------------------------------------------------------
 
 all : $(BALL_TARG) $(PUTT_TARG) $(MAPC_TARG) sols locales desktops
@@ -438,13 +448,13 @@
 endif
 
 $(BALL_TARG) : $(BALL_OBJS)
-	$(LINK) -o $(BALL_TARG) $(BALL_OBJS) $(LDFLAGS) $(ALL_LIBS)
+	$(LINK) -o $(BALL_TARG) $(BALL_OBJS) $(LDFLAGS) $(ALL_LIBS) -mfpu=vfp
 
 $(PUTT_TARG) : $(PUTT_OBJS)
-	$(LINK) -o $(PUTT_TARG) $(PUTT_OBJS) $(LDFLAGS) $(ALL_LIBS)
+	$(LINK) -o $(PUTT_TARG) $(PUTT_OBJS) $(LDFLAGS) $(ALL_LIBS) -mfpu=vfp
 
 $(MAPC_TARG) : $(MAPC_OBJS)
-	$(CC) $(ALL_CFLAGS) -o $(MAPC_TARG) $(MAPC_OBJS) $(LDFLAGS) $(MAPC_LIBS)
+	$(CCNATIVE) $(ALL_CFLAGS) -o $(MAPC_TARG) $(MAPC_OBJS) $(MAPC_LIBS)
 
 # Work around some extremely helpful sdl-config scripts.
 
@@ -463,7 +473,7 @@
 
 clean-src :
 	$(RM) $(BALL_TARG) $(PUTT_TARG) $(MAPC_TARG)
-	find . \( -name '*.o' -o -name '*.d' \) -delete
+	find . \( -name '*.o' -o -name '*.d' -o -name '*.O' \) -delete
 
 clean : clean-src
 	$(RM) $(SOLS)
