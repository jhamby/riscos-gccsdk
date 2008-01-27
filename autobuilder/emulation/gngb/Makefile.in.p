--- src/Makefile.in.orig	2008-01-26 16:21:34.000000000 -0800
+++ src/Makefile.in	2008-01-26 16:22:01.000000000 -0800
@@ -68,7 +68,7 @@
 SDL_CONFIG = @SDL_CONFIG@
 VERSION = @VERSION@
 
-bin_PROGRAMS = gngb 
+bin_PROGRAMS = gngb$(AB_EXEEXT) 
 
 gngb_SOURCES = cpu.c cpu.h emu.c emu.h frame_skip.c frame_skip.h global.h interrupt.c interrupt.h main.c memory.c memory.h message.c message.h rom.c rom.h serial.c  serial.h sgb.c sgb.h sound.c sound.h tiny_font.h vram.c vram.h video_yuy2.c video_yuv.c video_yuv.h video_yv12.c video_std.c video_gl.c video_gl.h video_std.h menu.h menu.c menu_image.h fileio.h fileio.c save.h save.c unzip.c unzip.h
 
@@ -158,7 +158,7 @@
 
 maintainer-clean-compile:
 
-gngb: $(gngb_OBJECTS) $(gngb_DEPENDENCIES)
+gngb$(AB_EXEEXT): $(gngb_OBJECTS) $(gngb_DEPENDENCIES)
 	@rm -f gngb
 	$(LINK) $(gngb_LDFLAGS) $(gngb_OBJECTS) $(gngb_LDADD) $(LIBS)
 
