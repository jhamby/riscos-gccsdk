--- demo/Makefile.in.old	2003-04-02 09:26:48.000000000 +0100
+++ demo/Makefile.in	2003-04-02 08:27:50.000000000 +0100
@@ -229,7 +229,7 @@
 
 pixbuf-demo$(EXEEXT): $(pixbuf_demo_OBJECTS) $(pixbuf_demo_DEPENDENCIES)
 	@rm -f pixbuf-demo$(EXEEXT)
-	$(LINK) $(pixbuf_demo_LDFLAGS) $(pixbuf_demo_OBJECTS) $(pixbuf_demo_LDADD) $(LIBS)
+	$(LINK) $(pixbuf_demo_LDFLAGS) $(pixbuf_demo_OBJECTS) $(pixbuf_demo_LDADD) $(LIBS) $(STATIC_LIB_DEPS)
 
 tags: TAGS
 
