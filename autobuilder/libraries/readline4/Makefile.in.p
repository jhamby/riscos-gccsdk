--- Makefile.in.orig	2005-07-08 10:45:32.000000000 +0100
+++ Makefile.in	2005-07-08 10:46:24.000000000 +0100
@@ -208,10 +208,10 @@
 install:	$(INSTALL_TARGETS)
 
 install-static: installdirs $(STATIC_LIBS) install-headers
-	-$(MV) $(DESTDIR)$(libdir)/libreadline.a $(DESTDIR)$(libdir)/libreadline.old
+	#-$(MV) $(DESTDIR)$(libdir)/libreadline.a $(DESTDIR)$(libdir)/libreadline.old
 	$(INSTALL_DATA) libreadline.a $(DESTDIR)$(libdir)/libreadline.a
 	-test -n "$(RANLIB)" && $(RANLIB) $(DESTDIR)$(libdir)/libreadline.a
-	-$(MV) $(DESTDIR)$(libdir)/libhistory.a $(DESTDIR)$(libdir)/libhistory.old
+	#-$(MV) $(DESTDIR)$(libdir)/libhistory.a $(DESTDIR)$(libdir)/libhistory.old
 	$(INSTALL_DATA) libhistory.a $(DESTDIR)$(libdir)/libhistory.a
 	-test -n "$(RANLIB)" && $(RANLIB) $(DESTDIR)$(libdir)/libhistory.a
 	-( if test -d doc ; then \
