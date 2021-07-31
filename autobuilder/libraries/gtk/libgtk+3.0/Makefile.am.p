--- Makefile.am.orig	2013-10-17 00:28:04.000000000 +0100
+++ Makefile.am	2014-05-11 20:18:40.585274545 +0100
@@ -55,11 +55,11 @@
 
 
 ## Copy .pc files to target-specific names
-gtk+-x11-3.0.pc gtk+-win32-3.0.pc gtk+-quartz-3.0.pc gtk+-broadway-3.0.pc gtk+-wayland-3.0.pc: gtk+-3.0.pc
+gtk+-x11-3.0.pc gtk+-win32-3.0.pc gtk+-quartz-3.0.pc gtk+-broadway-3.0.pc gtk+-wayland-3.0.pc gtk+-riscos-3.0.pc: gtk+-3.0.pc
 	rm -f $@ && \
 	cp gtk+-3.0.pc $@
 
-gdk-x11-3.0.pc gdk-win32-3.0.pc gdk-quartz-3.0.pc gdk-broadway-3.0.pc gdk-wayland-3.0.pc: gdk-3.0.pc
+gdk-x11-3.0.pc gdk-win32-3.0.pc gdk-quartz-3.0.pc gdk-broadway-3.0.pc gdk-wayland-3.0.pc gdk-riscos-3.0.pc: gdk-3.0.pc
 	rm -f $@ && \
 	cp gdk-3.0.pc $@
 
