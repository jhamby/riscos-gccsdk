--- src/Makefile.old	2003-01-08 19:32:48.000000000 +0000
+++ src/Makefile	2003-01-08 19:33:28.000000000 +0000
@@ -75,7 +75,7 @@
 	$(RANLIB) $@
 
 $S: $(SOBJS)
-	$(CC) -Wl,-soname=liblua40.so.$V -o $@ -shared $(SOBJS)
+#	$(CC) -Wl,-soname=liblua40.so.$V -o $@ -shared $(SOBJS)
 
 
 
--- Makefile	2003-01-08 20:19:02.000000000 +0000
+++ Makefile.new	2003-01-08 20:18:03.000000000 +0000
@@ -27,7 +27,8 @@
 	mkdir -p $(INSTALL_BIN) $(INSTALL_INC) $(INSTALL_LIB) $(INSTALL_MAN)
 	$(INSTALL_EXEC) bin/* $(INSTALL_BIN)
 	$(INSTALL_DATA) include/*.h $(INSTALL_INC)
-	$(INSTALL_DATA) lib/*.a $(INSTALL_LIB)
+	$(INSTALL_DATA) lib/liblua.a  $(INSTALL_LIB)/liblua40.a
+	$(INSTALL_DATA) lib/liblualib.a $(INSTALL_LIB)/liblualib40.a
 	$(INSTALL_DATA) doc/*.1 $(INSTALL_MAN)
 
 # shared libraries (for Linux)
--- src/lib/Makefile.old	2003-04-23 21:22:59.000000000 +0100
+++ src/lib/Makefile	2003-04-23 20:23:16.000000000 +0100
@@ -22,9 +22,9 @@
 	$(RANLIB) $@
 
 $S: $(SOBJS)
-	cp $(LIB)/liblua.so $(LIB)/liblua-build.so
-	$(CC) -v -Wl,-soname=liblualib40.so.$V -o $@ -shared $(SOBJS) -L$(LIB) -llua-build -lm
-	rm -f $(LIB)/liblua-build.so
+	#cp $(LIB)/liblua.so $(LIB)/liblua-build.so
+	#$(CC) -v -Wl,-soname=liblualib40.so.$V -o $@ -shared $(SOBJS) -L$(LIB) -llua-build -lm
+	#rm -f $(LIB)/liblua-build.so
 
 clean:
 	rm -f $(OBJS) $T $S $(SOBJS)
