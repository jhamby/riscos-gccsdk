--- Imlib/Makefile.in.old	2002-12-18 13:08:12.000000000 +0000
+++ Imlib/Makefile.in	2002-12-18 13:09:46.000000000 +0000
@@ -241,7 +241,7 @@
 	done
 t-load$(EXEEXT): $(t_load_OBJECTS) $(t_load_DEPENDENCIES) 
 	@rm -f t-load$(EXEEXT)
-	$(LINK) $(t_load_LDFLAGS) $(t_load_OBJECTS) $(t_load_LDADD) $(LIBS)
+	$(LINK) $(t_load_LDFLAGS) $(t_load_OBJECTS) $(t_load_LDADD) $(LIBS) -lX11 -lXext
 
 mostlyclean-compile:
 	-rm -f *.$(OBJEXT) core *.core
