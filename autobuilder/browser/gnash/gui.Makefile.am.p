--- gui/Makefile.am	10 Nov 2006 23:07:47 -0000	1.44
+++ gui/Makefile.am	18 Nov 2006 17:34:00 -0000
@@ -92,9 +92,11 @@
   AM_CPPFLAGS += $(AGG_CFLAGS)
   AM_LDFLAGS += $(AGG_LIBS)
   GTK_AGG_SRCS = gtk_glue_agg.cpp gtk_glue_agg.h
+  RISCOS_AGG_SRCS = riscos_glue_agg.cpp riscos_glue_agg.h
   SDL_AGG_SRCS = sdl_agg_glue.cpp sdl_agg_glue.h
 else # }{
   GTK_AGG_SRCS =
+  RISCOS_AGG_SRCS =
   SDL_AGG_SRCS =
 endif # }
 
@@ -107,6 +109,13 @@
  GTK_SRCS = 
 endif # }
 
+if USE_GUI_RISCOS # {
+ RISCOS_SRCS = riscos.cpp riscossup.h riscos_glue.h $(RISCOS_AGG_SRCS)
+ AM_LDFLAGS += -lOSLib32
+else # }{
+ RISCOS_SRCS =
+endif # }
+
 if USE_GUI_SDL # {
  SDL_SRCS = sdl.cpp sdlsup.h sdl_glue.h $(SDL_CAIRO_SRCS) $(SDL_OPENGL_SRCS) $(SDL_AGG_SRCS)
 else # }{
@@ -166,6 +175,7 @@
 	$(GTK2_SRCS) \
 	$(KDE_SRCS) \
 	$(FB_SRCS) \
+	$(RISCOS_SRCS) \
 	gui.cpp gui.h \
 	NullGui.cpp \
 	NullGui.h
