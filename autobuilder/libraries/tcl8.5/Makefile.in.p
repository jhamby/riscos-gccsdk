--- unix/Makefile.in.orig	2011-12-30 08:28:42.000000000 +0000
+++ unix/Makefile.in	2011-12-30 08:31:05.000000000 +0000
@@ -725,13 +725,13 @@
 	mv "$(LIB_INSTALL_DIR)"/$(LIB_FILE) "$(LIB_INSTALL_DIR)"/$(LIB_FILE).0
 	ln -sf $(LIB_FILE).0 "$(LIB_INSTALL_DIR)"/$(LIB_FILE)
 	ln -sf "$(LIB_INSTALL_DIR)"/$(LIB_FILE).0 ./
-	@chmod 555 "$(LIB_INSTALL_DIR)"/$(LIB_FILE).0
+	@chmod 755 "$(LIB_INSTALL_DIR)"/$(LIB_FILE).0
 	@echo "Installing ${TCL_EXE} as $(BIN_INSTALL_DIR)/tclsh$(VERSION)@EXEEXT@"
 	@$(INSTALL_PROGRAM) ${TCL_EXE} "$(BIN_INSTALL_DIR)"/tclsh$(VERSION)@EXEEXT@
-	@echo "Installing tclConfig.sh to $(CONFIG_INSTALL_DIR)/"
+	@echo "Installing tclConfig.sh to $(TCL_LIBRARY)/"
 	@$(INSTALL_DATA) tclConfig.sh "$(CONFIG_INSTALL_DIR)"/tclConfig.sh
 	@if test "$(STUB_LIB_FILE)" != "" ; then \
-	    echo "Installing $(STUB_LIB_FILE) to $(LIB_INSTALL_DIR)/"; \
+	    echo "Installing $(STUB_LIB_FILE) to $(TCL_LIBRARY)/"; \
 	    @INSTALL_STUB_LIB@ ; \
 	fi
 	@EXTRA_INSTALL_BINARIES@
@@ -808,14 +808,14 @@
 	@echo "Installing time zone data"
 	@@LD_LIBRARY_PATH_VAR@="`pwd`:$${@LD_LIBRARY_PATH_VAR@}"; export @LD_LIBRARY_PATH_VAR@; \
 	TCL_LIBRARY="${TCL_BUILDTIME_LIBRARY}"; export TCL_LIBRARY; \
-	./${TCL_EXE} $(TOOL_DIR)/installData.tcl \
+	tclsh $(TOOL_DIR)/installData.tcl \
 	    $(TOP_DIR)/library/tzdata "$(SCRIPT_INSTALL_DIR)"/tzdata
 
 install-msgs: ${TCL_EXE}
 	@echo "Installing message catalogs"
 	@@LD_LIBRARY_PATH_VAR@="`pwd`:$${@LD_LIBRARY_PATH_VAR@}"; export @LD_LIBRARY_PATH_VAR@; \
 	TCL_LIBRARY="${TCL_BUILDTIME_LIBRARY}"; export TCL_LIBRARY; \
-	./${TCL_EXE} $(TOOL_DIR)/installData.tcl \
+	tclsh $(TOOL_DIR)/installData.tcl \
 	    $(TOP_DIR)/library/msgs "$(SCRIPT_INSTALL_DIR)"/msgs
 
 install-doc: doc
