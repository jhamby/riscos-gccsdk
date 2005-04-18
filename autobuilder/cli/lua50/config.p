--- config.old	Fri Apr 15 13:23:28 2005
+++ config	Fri Apr 15 13:36:13 2005
@@ -25,15 +25,15 @@
 # interface (e.g., Linux, Solaris, IRIX, BSD, AIX, HPUX, and probably others),
 # uncomment the next two lines.
 #
-LOADLIB= -DUSE_DLOPEN=1
-DLLIB= -ldl
+#LOADLIB= -DUSE_DLOPEN=1
+#DLLIB= -ldl
 #
 # In Linux with gcc, you should also uncomment the next definition for
 # MYLDFLAGS, which passes -E (= -export-dynamic) to the linker. This option
 # allows dynamic libraries to link back to the `lua' program, so that they do
 # not need the Lua libraries. (Other systems may have an equivalent facility.)
 #
-MYLDFLAGS= -Wl,-E
+#MYLDFLAGS= -Wl,-E
 #
 # On Windows systems. support for dynamic loading is enabled by default.
 # To disable this support, uncomment the next line.
@@ -100,8 +100,8 @@
 # to add -lreadline (and perhaps also -lhistory and -lcurses or -lncurses)
 # to EXTRA_LIBS.
 #
-USERCONF=-DLUA_USERCONFIG='"$(LUA)/etc/saconfig.c"' -DUSE_READLINE
-EXTRA_LIBS= -lreadline -lm -ldl # -lhistory -lcurses -lncurses
+#USERCONF=-DLUA_USERCONFIG='"$(LUA)/etc/saconfig.c"' -DUSE_READLINE
+#EXTRA_LIBS= -lreadline -lm -ldl # -lhistory -lcurses -lncurses
 
 # ------------------------------------------------------------------ C compiler
 
@@ -119,7 +119,7 @@
 # debug information. If you only want the shared libraries, you may want to
 # add -fPIC to MYCFLAGS.
 #
-MYCFLAGS= -O3 -g
+MYCFLAGS= -O3
 #MYCFLAGS= -O3 -fomit-frame-pointer # -fPIC
 
 # Write here any options you may need for your C linker.
--- config.orig	2005-04-18 10:16:48.000000000 +0100
+++ config	2005-04-18 10:16:51.000000000 +0100
@@ -160,8 +160,8 @@
 #
 #INSTALL_EXEC= cp
 #INSTALL_DATA= cp
-INSTALL_EXEC= install -m 0755
-INSTALL_DATA= install -m 0644
+INSTALL_EXEC= /home/riscos/env/ro-install -m 0755
+INSTALL_DATA= /home/riscos/env/ro-install -m 0644
 
 # == END OF USER SETTINGS. NO NEED TO CHANGE ANYTHING BELOW THIS LINE =========
 
--- config.orig	2005-04-18 10:18:59.000000000 +0100
+++ config	2005-04-18 10:19:07.000000000 +0100
@@ -148,7 +148,7 @@
 
 # Locations for "make install". You may need to be root do "make install".
 #
-INSTALL_ROOT= $(PREFIX)/usr/
+INSTALL_ROOT= $(PREFIX)/home/riscos/env
 INSTALL_BIN= $(INSTALL_ROOT)/bin
 INSTALL_INC= $(INSTALL_ROOT)/include/lua50
 INSTALL_LIB= $(INSTALL_ROOT)/lib
