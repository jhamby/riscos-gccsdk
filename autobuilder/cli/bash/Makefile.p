--- Makefile.in.old	2004-09-25 14:49:09.000000000 +0100
+++ Makefile.in	2004-09-25 14:48:16.000000000 +0100
@@ -513,7 +513,7 @@
 	$(RM) $@
 	$(PURIFY) $(CC) $(BUILTINS_LDFLAGS) $(LIBRARY_LDFLAGS) $(LDFLAGS) -o $(Program) $(OBJECTS) $(LIBS)
 	ls -l $(Program)
-	size $(Program)
+	#size $(Program)
 
 .build:	$(SOURCES) config.h Makefile version.h $(VERSPROG)
 	@echo
