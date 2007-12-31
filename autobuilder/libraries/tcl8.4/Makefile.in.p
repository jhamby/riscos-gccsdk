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
