--- unix/Makefile.in.orig	2010-01-01 13:19:57.000000000 -0800
+++ unix/Makefile.in	2010-01-01 13:18:27.000000000 -0800
@@ -733,7 +733,7 @@
 	mv "$(LIB_INSTALL_DIR)"/$(LIB_FILE) "$(LIB_INSTALL_DIR)"/$(LIB_FILE).0
 	ln -sf $(LIB_FILE).0 "$(LIB_INSTALL_DIR)"/$(LIB_FILE)
 	ln -sf "$(LIB_INSTALL_DIR)"/$(LIB_FILE).0 ./
-	@chmod 555 "$(LIB_INSTALL_DIR)"/$(LIB_FILE).0
+	@chmod 755 "$(LIB_INSTALL_DIR)"/$(LIB_FILE).0
 	@if test "$(TCL_BUILD_EXP_FILE)" != ""; then \
 	    echo "Installing $(TCL_EXP_FILE) to $(LIB_INSTALL_DIR)/"; \
 	    $(INSTALL_DATA) $(TCL_BUILD_EXP_FILE) \
@@ -741,10 +741,10 @@
 	    fi
 	@echo "Installing tclsh as $(BIN_INSTALL_DIR)/tclsh$(VERSION)"
 	@$(INSTALL_PROGRAM) tclsh "$(BIN_INSTALL_DIR)"/tclsh$(VERSION)
-	@echo "Installing tclConfig.sh to $(CONFIG_INSTALL_DIR)/"
+	@echo "Installing tclConfig.sh to $(TCL_LIBRARY)/"
 	@$(INSTALL_DATA) tclConfig.sh "$(CONFIG_INSTALL_DIR)"/tclConfig.sh
 	@if test "$(STUB_LIB_FILE)" != "" ; then \
-	    echo "Installing $(STUB_LIB_FILE) to $(LIB_INSTALL_DIR)/"; \
+	    echo "Installing $(STUB_LIB_FILE) to $(TCL_LIBRARY)/"; \
 	    @INSTALL_STUB_LIB@ ; \
 	fi
 	@EXTRA_INSTALL_BINARIES@
@@ -821,14 +821,14 @@
 	@echo "Installing time zone data"
 	@@LD_LIBRARY_PATH_VAR@="`pwd`:$${@LD_LIBRARY_PATH_VAR@}"; export @LD_LIBRARY_PATH_VAR@; \
 	TCL_LIBRARY="${TCL_BUILDTIME_LIBRARY}"; export TCL_LIBRARY; \
-	./tclsh $(TOOL_DIR)/installData.tcl \
+	tclsh $(TOOL_DIR)/installData.tcl \
 	    $(TOP_DIR)/library/tzdata "$(SCRIPT_INSTALL_DIR)"/tzdata
 
 install-msgs: tclsh
 	@echo "Installing message catalogs"
 	@@LD_LIBRARY_PATH_VAR@="`pwd`:$${@LD_LIBRARY_PATH_VAR@}"; export @LD_LIBRARY_PATH_VAR@; \
 	TCL_LIBRARY="${TCL_BUILDTIME_LIBRARY}"; export TCL_LIBRARY; \
-	./tclsh $(TOOL_DIR)/installData.tcl \
+	tclsh $(TOOL_DIR)/installData.tcl \
 	    $(TOP_DIR)/library/msgs "$(SCRIPT_INSTALL_DIR)"/msgs
 
 install-doc: doc
