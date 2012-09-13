--- doc/Makefile.in.orig	2012-09-13 09:33:04.000000000 +0100
+++ doc/Makefile.in	2012-09-13 09:33:27.000000000 +0100
@@ -396,9 +396,9 @@
 @HAVE_DOXYGEN_TRUE@		$(INSTALL_DATA) -D $$f $(DESTDIR)$(docdir)/$$f;	\
 @HAVE_DOXYGEN_TRUE@	done
 
-@HAVE_DOXYGEN_TRUE@	$(INSTALL) -d $(DESTDIR)$(mandir)/man3
-@HAVE_DOXYGEN_TRUE@	cd man && find man3 -type f -name opus_*.3 \
-@HAVE_DOXYGEN_TRUE@		  -exec $(INSTALL_DATA) \{} $(DESTDIR)$(mandir)/man3 \;
+#@HAVE_DOXYGEN_TRUE@	$(INSTALL) -d $(DESTDIR)$(mandir)/man3
+#@HAVE_DOXYGEN_TRUE@	cd man && find man3 -type f -name opus_*.3 \
+#@HAVE_DOXYGEN_TRUE@		  -exec $(INSTALL_DATA) \{} $(DESTDIR)$(mandir)/man3 \;
 
 @HAVE_DOXYGEN_TRUE@clean-local:
 @HAVE_DOXYGEN_TRUE@	$(RM) -r html
