--- musa/Makefile.in.orig	2005-07-10 11:32:12.000000000 +0100
+++ musa/Makefile.in	2005-07-10 11:32:15.000000000 +0100
@@ -196,7 +196,7 @@
 maintainer-clean-noinstPROGRAMS:
 
 m68kmake: $(m68kmake_OBJECTS) $(m68kmake_DEPENDENCIES)
-	@rm -f m68kmake
+	#@rm -f m68kmake
 	$(LINK) $(m68kmake_LDFLAGS) $(m68kmake_OBJECTS) $(m68kmake_LDADD) $(LIBS)
 
 tags: TAGS
--- sdl/Makefile.in.orig	2005-07-10 11:33:17.000000000 +0100
+++ sdl/Makefile.in	2005-07-10 11:34:03.000000000 +0100
@@ -199,7 +199,7 @@
 maintainer-clean-noinstPROGRAMS:
 
 pbm2df: $(pbm2df_OBJECTS) $(pbm2df_DEPENDENCIES)
-	@rm -f pbm2df
+	#@rm -f pbm2df
 	$(LINK) $(pbm2df_LDFLAGS) $(pbm2df_OBJECTS) $(pbm2df_LDADD) $(LIBS)
 .cpp.o:
 	$(CXXCOMPILE) -c $<
