--- config.orig	2009-08-23 20:29:15.000000000 -0700
+++ config	2009-08-23 20:32:26.000000000 -0700
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
@@ -100,15 +100,15 @@
 # to add -lreadline (and perhaps also -lhistory and -lcurses or -lncurses)
 # to EXTRA_LIBS.
 #
-USERCONF=-DLUA_USERCONFIG='"$(LUA)/etc/saconfig.c"' -DUSE_READLINE
-EXTRA_LIBS= -lreadline -lm -ldl # -lhistory -lcurses -lncurses
+#USERCONF=-DLUA_USERCONFIG='"$(LUA)/etc/saconfig.c"' -DUSE_READLINE
+#EXTRA_LIBS= -lreadline -lm -ldl # -lhistory -lcurses -lncurses
 
 # ------------------------------------------------------------------ C compiler
 
 # You need an ANSI C compiler. gcc is a popular one. We do not use -ansi in
 # WARN because it disables POSIX features used in the libraries.
 #
-CC= gcc
+#CC= gcc
 WARN= -Wall
 
 # ------------------------------------------------------------------ C options
@@ -148,7 +148,8 @@
 
 # Locations for "make install". You may need to be root do "make install".
 #
-INSTALL_ROOT= $(PREFIX)/usr
+#INSTALL_ROOT= $(PREFIX)/usr
+INSTALL_ROOT= $(PREFIX)/$(GCCSDK_INSTALL_ENV)
 INSTALL_BIN= $(INSTALL_ROOT)/bin
 INSTALL_INC= $(INSTALL_ROOT)/include/lua50
 INSTALL_LIB= $(INSTALL_ROOT)/lib
@@ -158,10 +159,10 @@
 # You may prefer to use "install" instead of "cp" if you have it.
 # If you use "install", you may also want to change the permissions after -m.
 #
-#INSTALL_EXEC= cp
-#INSTALL_DATA= cp
-INSTALL_EXEC= install -m 0755
-INSTALL_DATA= install -m 0644 -p
+INSTALL_EXEC= cp
+INSTALL_DATA= cp
+#INSTALL_EXEC= install -m 0755
+#INSTALL_DATA= install -m 0644 -p
 
 # == END OF USER SETTINGS. NO NEED TO CHANGE ANYTHING BELOW THIS LINE =========
 
