--- configure.ac	8 Nov 2006 09:16:17 -0000	1.191
+++ configure.ac	18 Nov 2006 17:34:00 -0000
@@ -139,6 +139,7 @@
    gtk|GTK) gui=gtk ;;
    sdl|SDL) gui=sdl  ;;
    fb|FB) gui=fb  ;;
+   riscos|RISCOS) gui=riscos ;;
    *)      AC_MSG_ERROR([invalid gui ${enableval} given]) ;;
   esac],
  [gui=gtk]
@@ -180,8 +181,8 @@
 AM_CONDITIONAL(USE_RENDERER_CAIRO, test x$renderer = xcairo)
 
 if test x$renderer = xagg; then
-  if test x"$gui" != xfb -a x"$gui" != xgtk -a x"$gui" != xsdl; then
-    AC_MSG_ERROR([agg renderer is only supported by fb and gtk gui]);
+  if test x"$gui" != xfb -a x"$gui" != xgtk -a x"$gui" != xsdl -a x"$gui" != xriscos; then
+    AC_MSG_ERROR([agg renderer is only supported by fb, gtk, riscos and sdl guis]);
   fi
   AC_DEFINE([RENDERER_AGG], [], [Use AntiGrain renderer])
 fi
@@ -547,6 +548,7 @@
 AM_CONDITIONAL(USE_GUI_GTK, test x$gui = xgtk)
 AM_CONDITIONAL(USE_GUI_SDL, test x$gui = xsdl)
 AM_CONDITIONAL(USE_GUI_KDE, test x$gui = xkde)
+AM_CONDITIONAL(USE_GUI_RISCOS, test x$gui = xriscos)
 
 case "${gui}" in
 	gtk)  AC_DEFINE([GUI_GTK],  [1], [Use GTK gui toolkit]) ;;
@@ -554,6 +556,7 @@
 	kde)  AC_DEFINE([GUI_KDE],  [1], [Use KDE gui toolkit]) ;;
 	fltk) AC_DEFINE([GUI_FLTK], [1], [Use FLTK gui toolkit]) ;;
 	fb) AC_DEFINE([GUI_FB], [1], [Use Framebuffer, no gui toolkit]) ;;
+	riscos) AC_DEFINE([GUI_RISCOS], [1], [Use RISC OS gui toolkit]) ;;
 	*)
 esac
 
