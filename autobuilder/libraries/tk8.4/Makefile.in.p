--- unix/Makefile.in.orig	2009-08-25 10:29:56.000000000 -0700
+++ unix/Makefile.in	2009-08-25 10:59:34.000000000 -0700
@@ -640,7 +640,7 @@
 	    fi
 	@echo "Installing $(LIB_FILE) to $(LIB_INSTALL_DIR)/"
 	@@INSTALL_LIB@
-	@chmod 555 $(LIB_INSTALL_DIR)/$(LIB_FILE)
+	@true || chmod 555 $(LIB_INSTALL_DIR)/$(LIB_FILE)
 	@if test "$(TK_BUILD_EXP_FILE)" != ""; then \
 	    echo "Installing $(TK_EXP_FILE) to $(LIB_INSTALL_DIR)/"; \
 	    $(INSTALL_DATA) $(TK_BUILD_EXP_FILE) \
@@ -1321,14 +1321,14 @@
 		$(UNIX_DIR)/README $(UNIX_DIR)/installManPage $(DISTDIR)/unix
 	chmod 775 $(DISTDIR)/unix/configure $(DISTDIR)/unix/configure.in
 	chmod +x $(DISTDIR)/unix/install-sh
-	mkdir $(DISTDIR)/bitmaps
+	mkdir -p $(DISTDIR)/bitmaps
 	@(cd $(TOP_DIR); for i in bitmaps/* ; do \
 	    if [ -f $$i ] ; then \
 		sed -e 's/static char/static unsigned char/' \
 		       $$i > $(DISTDIR)/$$i; \
 	    fi; \
 	done;)
-	mkdir $(DISTDIR)/generic
+	mkdir -p $(DISTDIR)/generic
 	cp -p $(GENERIC_DIR)/*.c $(GENERIC_DIR)/*.h $(GENERIC_DIR)/prolog.ps \
 		$(DISTDIR)/generic
 	cp -p $(GENERIC_DIR)/*.decls $(DISTDIR)/generic
@@ -1336,7 +1336,7 @@
 	cp -p $(TOP_DIR)/changes $(TOP_DIR)/ChangeLog $(TOP_DIR)/README \
 		$(TOP_DIR)/license.terms $(DISTDIR)
 	rm -f $(DISTDIR)/generic/blt*.[ch]
-	mkdir $(DISTDIR)/win
+	mkdir -p $(DISTDIR)/win
 	cp $(TOP_DIR)/win/Makefile.in $(DISTDIR)/win
 	cp $(TOP_DIR)/win/configure.in \
 		$(TOP_DIR)/win/configure \
@@ -1353,14 +1353,14 @@
 	cp -p $(TOP_DIR)/win/README $(DISTDIR)/win
 	cp -p $(TOP_DIR)/win/lamp.bmp $(DISTDIR)/win
 	cp -p $(TOP_DIR)/license.terms $(DISTDIR)/win
-	mkdir $(DISTDIR)/win/rc
+	mkdir -p $(DISTDIR)/win/rc
 	cp -p $(TOP_DIR)/win/wish.exe.manifest.in $(DISTDIR)/win/
 	cp -p $(TOP_DIR)/win/rc/*.rc $(TOP_DIR)/win/rc/*.cur \
 		$(TOP_DIR)/win/rc/*.ico $(TOP_DIR)/win/rc/*.bmp \
 		$(DISTDIR)/win/rc
 	$(TCL_EXE) $(TOOL_DIR)/eolFix.tcl -crlf $(DISTDIR)/win/rc/*.rc
 	$(TCL_EXE) $(TOOL_DIR)/eolFix.tcl -crlf $(DISTDIR)/win/wish.exe.manifest.in
-	mkdir $(DISTDIR)/mac
+	mkdir -p $(DISTDIR)/mac
 	cp -p $(TOP_DIR)/mac/tkMacProjects.sea.hqx $(DISTDIR)/mac
 	cp -p $(TOP_DIR)/mac/*.c $(TOP_DIR)/mac/*.h $(TOP_DIR)/mac/*.r \
 		$(DISTDIR)/mac
@@ -1369,41 +1369,41 @@
 	cp -p $(TOP_DIR)/mac/*.pch $(DISTDIR)/mac
 	cp -p $(TOP_DIR)/mac/*.doc $(DISTDIR)/mac
 	cp -p $(TOP_DIR)/mac/*.tcl $(DISTDIR)/mac
-	mkdir $(DISTDIR)/macosx
+	mkdir -p $(DISTDIR)/macosx
 	cp -p $(MAC_OSX_DIR)/Makefile $(MAC_OSX_DIR)/README \
 		$(MAC_OSX_DIR)/Wish.icns $(MAC_OSX_DIR)/*.c \
 		$(MAC_OSX_DIR)/*.h $(MAC_OSX_DIR)/*.in \
 		$(MAC_OSX_DIR)/*.r $(DISTDIR)/macosx
 	cp -p $(TOP_DIR)/license.terms $(DISTDIR)/macosx
-	mkdir $(DISTDIR)/macosx/Wish.pbproj
+	mkdir -p $(DISTDIR)/macosx/Wish.pbproj
 	cp -p $(MAC_OSX_DIR)/Wish.pbproj/*.pbx* $(DISTDIR)/macosx/Wish.pbproj
-	mkdir $(DISTDIR)/compat
+	mkdir -p $(DISTDIR)/compat
 	cp -p $(TOP_DIR)/license.terms $(TCLDIR)/compat/unistd.h \
 		$(TCLDIR)/compat/stdlib.h $(TCLDIR)/compat/limits.h \
 		$(DISTDIR)/compat
-	mkdir $(DISTDIR)/xlib
+	mkdir -p $(DISTDIR)/xlib
 	cp -p $(XLIB_DIR)/*.h $(XLIB_DIR)/*.c $(DISTDIR)/xlib
 	cp -p $(TOP_DIR)/license.terms $(DISTDIR)/xlib
-	mkdir $(DISTDIR)/xlib/X11
+	mkdir -p $(DISTDIR)/xlib/X11
 	cp -p $(XLIB_DIR)/X11/*.h $(DISTDIR)/xlib/X11
 	cp -p $(TOP_DIR)/license.terms $(DISTDIR)/xlib/X11
-	mkdir $(DISTDIR)/library
+	mkdir -p $(DISTDIR)/library
 	cp -p $(TOP_DIR)/license.terms $(TOP_DIR)/library/*.tcl \
 		$(TOP_DIR)/library/tclIndex \
 		$(DISTDIR)/library
-	mkdir $(DISTDIR)/library/images
+	mkdir -p $(DISTDIR)/library/images
 	@(cd $(TOP_DIR); for i in library/images/* ; do \
 	    if [ -f $$i ] ; then \
 		cp $$i $(DISTDIR)/$$i; \
 	    fi; \
 	done;)
-	mkdir $(DISTDIR)/library/msgs
+	mkdir -p $(DISTDIR)/library/msgs
 	@(cd $(TOP_DIR); for i in library/msgs/*.msg ; do \
 	    if [ -f $$i ] ; then \
 		cp $$i $(DISTDIR)/$$i; \
 	    fi; \
 	done;)
-	mkdir $(DISTDIR)/library/demos
+	mkdir -p $(DISTDIR)/library/demos
 	cp -pr $(TOP_DIR)/library/demos/*.tcl \
 		$(TOP_DIR)/library/demos/tclIndex \
 		$(TOP_DIR)/library/demos/browse \
@@ -1415,16 +1415,16 @@
 		$(TOP_DIR)/library/demos/widget \
 		$(TOP_DIR)/library/demos/README \
 		$(TOP_DIR)/license.terms $(DISTDIR)/library/demos
-	mkdir $(DISTDIR)/library/demos/images
+	mkdir -p $(DISTDIR)/library/demos/images
 	@(cd $(TOP_DIR); for i in library/demos/images/* ; do \
 	    if [ -f $$i ] ; then \
 		cp $$i $(DISTDIR)/$$i; \
 	    fi; \
 	done;)
-	mkdir $(DISTDIR)/doc
+	mkdir -p $(DISTDIR)/doc
 	cp -p $(TOP_DIR)/license.terms $(TOP_DIR)/doc/*.[13n] \
 		$(TCLDIR)/doc/man.macros $(DISTDIR)/doc
-	mkdir $(DISTDIR)/tests
+	mkdir -p $(DISTDIR)/tests
 	cp -p $(TOP_DIR)/license.terms $(TOP_DIR)/tests/*.test \
 		$(TOP_DIR)/tests/*.tcl $(TOP_DIR)/tests/README \
 		$(TOP_DIR)/tests/option.file* $(DISTDIR)/tests
--- unix/Makefile.in.orig	2009-08-25 11:43:10.000000000 -0700
+++ unix/Makefile.in	2009-08-25 11:44:33.000000000 -0700
@@ -649,9 +649,9 @@
 	@echo "Installing wish as $(BIN_INSTALL_DIR)/wish$(VERSION)"
 	@$(INSTALL_PROGRAM) wish $(BIN_INSTALL_DIR)/wish$(VERSION)
 	@echo "Installing tkConfig.sh to $(LIB_INSTALL_DIR)/"
-	@$(INSTALL_DATA) tkConfig.sh $(LIB_INSTALL_DIR)/tkConfig.sh
+	@$(INSTALL_DATA) tkConfig.sh $(TK_LIBRARY)/tkConfig.sh
 	@if test "$(STUB_LIB_FILE)" != "" ; then \
-	    echo "Installing $(STUB_LIB_FILE) to $(LIB_INSTALL_DIR)/"; \
+	    echo "Installing $(STUB_LIB_FILE) to $(TK_LIBRARY)/"; \
 	    @INSTALL_STUB_LIB@ ; \
 	fi
 	@EXTRA_INSTALL_BINARIES@
