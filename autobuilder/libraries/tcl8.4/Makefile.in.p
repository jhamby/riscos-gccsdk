--- unix/Makefile.in.org	2007-12-30 21:01:27.000000000 -0800
+++ unix/Makefile.in	2007-12-30 21:02:18.000000000 -0800
@@ -634,7 +634,7 @@
 	    fi
 	@echo "Installing $(LIB_FILE) to $(LIB_INSTALL_DIR)/"
 	@@INSTALL_LIB@
-	@chmod 555 $(LIB_INSTALL_DIR)/$(LIB_FILE)
+	@chmod 755 $(LIB_INSTALL_DIR)/$(LIB_FILE)
 	@if test "$(TCL_BUILD_EXP_FILE)" != ""; then \
 	    echo "Installing $(TCL_EXP_FILE) to $(LIB_INSTALL_DIR)/"; \
 	    $(INSTALL_DATA) $(TCL_BUILD_EXP_FILE) \
--- unix/Makefile.in.orig	2009-08-19 15:02:35.000000000 -0700
+++ unix/Makefile.in	2009-08-19 15:10:42.000000000 -0700
@@ -643,9 +643,9 @@
 	@echo "Installing tclsh as $(BIN_INSTALL_DIR)/tclsh$(VERSION)"
 	@$(INSTALL_PROGRAM) tclsh $(BIN_INSTALL_DIR)/tclsh$(VERSION)
 	@echo "Installing tclConfig.sh to $(LIB_INSTALL_DIR)/"
-	@$(INSTALL_DATA) tclConfig.sh $(LIB_INSTALL_DIR)/tclConfig.sh
+	@$(INSTALL_DATA) tclConfig.sh $(TCL_LIBRARY)/tclConfig.sh
 	@if test "$(STUB_LIB_FILE)" != "" ; then \
-	    echo "Installing $(STUB_LIB_FILE) to $(LIB_INSTALL_DIR)/"; \
+	    echo "Installing $(STUB_LIB_FILE) to $(TCL_LIBRARY)/"; \
 	    @INSTALL_STUB_LIB@ ; \
 	fi
 	@EXTRA_INSTALL_BINARIES@
