--- Makefile.in.orig	2009-08-27 08:29:05.000000000 -0700
+++ Makefile.in	2009-08-27 08:30:44.000000000 -0700
@@ -12,7 +12,8 @@
 datadir     = @datadir@
 
 VERSION     = @PACKAGE_VERSION@
-KEYMAP_PATH = $(datadir)/rdesktop/keymaps/
+#KEYMAP_PATH = $(datadir)/rdesktop/keymaps/
+KEYMAP_PATH = \<RDesktop\$$Dir\>/keymaps/
 
 CC          = @CC@
 INSTALL     = @INSTALL@
@@ -39,7 +40,7 @@
 all: $(TARGETS)
 
 rdesktop: $(X11OBJ) $(SOUNDOBJ) $(RDPOBJ) $(SCARDOBJ)
-	$(CC) $(CFLAGS) -o rdesktop $(X11OBJ) $(SOUNDOBJ) $(RDPOBJ) $(SCARDOBJ) $(LDFLAGS) -lX11
+	$(CC) $(CFLAGS) -o rdesktop $(X11OBJ) $(SOUNDOBJ) $(RDPOBJ) $(SCARDOBJ) $(LDFLAGS) -lCX11 -lDesk
 
 rdp2vnc: $(VNCOBJ) $(SOUNDOBJ) $(RDPOBJ) $(SCARDOBJ) 
 	$(VNCLINK) $(CFLAGS) -o rdp2vnc $(VNCOBJ) $(SOUNDOBJ) $(RDPOBJ) $(SCARDOBJ) $(LDFLAGS) $(LDVNC)
