--- unix/Makefile.in.orig	2010-01-02 10:17:08.000000000 -0800
+++ unix/Makefile.in	2010-01-02 10:23:45.000000000 -0800
@@ -718,7 +718,7 @@
 	    fi
 	@echo "Installing $(LIB_FILE) to $(LIB_INSTALL_DIR)/"
 	@@INSTALL_LIB@
-	@chmod 555 "$(LIB_INSTALL_DIR)"/$(LIB_FILE)
+	@chmod 755 "$(LIB_INSTALL_DIR)"/$(LIB_FILE)
 	@if test "$(TK_BUILD_EXP_FILE)" != ""; then \
 	    echo "Installing $(TK_EXP_FILE) to $(LIB_INSTALL_DIR)/"; \
 	    $(INSTALL_DATA) $(TK_BUILD_EXP_FILE) \
@@ -726,10 +726,10 @@
 	    fi
 	@echo "Installing wish as $(BIN_INSTALL_DIR)/wish$(VERSION)"
 	@$(INSTALL_PROGRAM) wish "$(BIN_INSTALL_DIR)"/wish$(VERSION)
-	@echo "Installing tkConfig.sh to $(CONFIG_INSTALL_DIR)/"
+	@echo "Installing tkConfig.sh to $(TK_LIBRARY)/"
 	@$(INSTALL_DATA) tkConfig.sh "$(CONFIG_INSTALL_DIR)"/tkConfig.sh
 	@if test "$(STUB_LIB_FILE)" != "" ; then \
-	    echo "Installing $(STUB_LIB_FILE) to $(LIB_INSTALL_DIR)/"; \
+	    echo "Installing $(STUB_LIB_FILE) to $(TK_LIBRARY/"; \
 	    @INSTALL_STUB_LIB@ ; \
 	fi
 	@EXTRA_INSTALL_BINARIES@
@@ -1517,14 +1517,14 @@
 		$(UNIX_DIR)/tkConfig.h.in $(DISTDIR)/unix
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
 	cp -p $(GENERIC_DIR)/*.[ch] $(DISTDIR)/generic
 	cp -p $(GENERIC_DIR)/*.decls $(DISTDIR)/generic
 	cp -p $(GENERIC_DIR)/README $(DISTDIR)/generic
@@ -1535,7 +1535,7 @@
 	mkdir $(DISTDIR)/generic/ttk
 	cp -p $(TTK_DIR)/*.[ch] $(TTK_DIR)/ttk.decls \
 		$(TTK_DIR)/ttkGenStubs.tcl $(DISTDIR)/generic/ttk
-	mkdir $(DISTDIR)/win
+	mkdir -p $(DISTDIR)/win
 	cp $(TOP_DIR)/win/Makefile.in $(DISTDIR)/win
 	cp $(TOP_DIR)/win/configure.in \
 		$(TOP_DIR)/win/configure \
@@ -1550,54 +1550,54 @@
 	$(TCL_EXE) $(TOOL_DIR)/eolFix.tcl -crlf $(DISTDIR)/win/rules.vc
 	cp -p $(TOP_DIR)/win/README $(DISTDIR)/win
 	cp -p $(TOP_DIR)/license.terms $(DISTDIR)/win
-	mkdir $(DISTDIR)/win/rc
+	mkdir -p $(DISTDIR)/win/rc
 	cp -p $(TOP_DIR)/win/wish.exe.manifest.in $(DISTDIR)/win/
 	cp -p $(TOP_DIR)/win/rc/*.{rc,cur,ico,bmp} $(DISTDIR)/win/rc
 	$(TCL_EXE) $(TOOL_DIR)/eolFix.tcl -crlf $(DISTDIR)/win/rc/*.rc
 	$(TCL_EXE) $(TOOL_DIR)/eolFix.tcl -crlf $(DISTDIR)/win/wish.exe.manifest.in
-	mkdir $(DISTDIR)/macosx
+	mkdir -p $(DISTDIR)/macosx
 	cp -p $(MAC_OSX_DIR)/GNUmakefile $(MAC_OSX_DIR)/README \
 		$(MAC_OSX_DIR)/Wish.icns $(MAC_OSX_DIR)/*.[chr] \
 		$(MAC_OSX_DIR)/*.in $(MAC_OSX_DIR)/*.ac \
 		$(MAC_OSX_DIR)/*.xcconfig $(MAC_OSX_DIR)/configure \
 		$(DISTDIR)/macosx
 	cp -p $(TOP_DIR)/license.terms $(DISTDIR)/macosx
-	mkdir $(DISTDIR)/macosx/Wish.pbproj
+	mkdir -p $(DISTDIR)/macosx/Wish.pbproj
 	cp -p $(MAC_OSX_DIR)/Wish.pbproj/*.pbx* $(DISTDIR)/macosx/Wish.pbproj
-	mkdir $(DISTDIR)/macosx/Wish.xcode
+	mkdir -p $(DISTDIR)/macosx/Wish.xcode
 	cp -p $(MAC_OSX_DIR)/Wish.xcode/*.pbx* $(DISTDIR)/macosx/Wish.xcode
-	mkdir $(DISTDIR)/macosx/Wish.xcodeproj
+	mkdir -p $(DISTDIR)/macosx/Wish.xcodeproj
 	cp -p $(MAC_OSX_DIR)/Wish.xcodeproj/*.pbx* \
 		$(DISTDIR)/macosx/Wish.xcodeproj
-	mkdir $(DISTDIR)/compat
+	mkdir -p $(DISTDIR)/compat
 	cp -p $(TOP_DIR)/license.terms $(TCLDIR)/compat/unistd.h \
 		$(TCLDIR)/compat/stdlib.h $(TCLDIR)/compat/limits.h \
 		$(DISTDIR)/compat
-	mkdir $(DISTDIR)/xlib
+	mkdir -p $(DISTDIR)/xlib
 	cp -p $(XLIB_DIR)/*.[ch] $(DISTDIR)/xlib
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
-	mkdir $(DISTDIR)/library/ttk
+	mkdir -p $(DISTDIR)/library/ttk
 	cp -p $(TOP_DIR)/library/ttk/*.tcl $(DISTDIR)/library/ttk
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
 		$(TOP_DIR)/library/demos/*.msg \
 		$(TOP_DIR)/library/demos/tclIndex \
@@ -1610,20 +1610,20 @@
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
 	cp -p $(TOP_DIR)/license.terms $(TEST_DIR)/*.{test,tcl} \
 		$(TEST_DIR)/README $(TEST_DIR)/*.{gif,ppm,xbm} \
 		$(TEST_DIR)/option.file* $(DISTDIR)/tests
-	mkdir $(DISTDIR)/tests/ttk
+	mkdir -p $(DISTDIR)/tests/ttk
 	cp -p $(TEST_DIR)/ttk/*.{test,tcl} $(DISTDIR)/tests/ttk
 
 #
