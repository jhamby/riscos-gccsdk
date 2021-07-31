--- Makefile.in.orig	2008-04-24 06:46:58.000000000 -0700
+++ Makefile.in	2008-04-24 06:47:05.000000000 -0700
@@ -220,10 +220,8 @@
 install:	$(INSTALL_TARGETS)
 
 install-static: installdirs $(STATIC_LIBS) install-headers install-doc
-	-$(MV) $(DESTDIR)$(libdir)/libreadline.a $(DESTDIR)$(libdir)/libreadline.old
 	$(INSTALL_DATA) libreadline.a $(DESTDIR)$(libdir)/libreadline.a
 	-test -n "$(RANLIB)" && $(RANLIB) $(DESTDIR)$(libdir)/libreadline.a
-	-$(MV) $(DESTDIR)$(libdir)/libhistory.a $(DESTDIR)$(libdir)/libhistory.old
 	$(INSTALL_DATA) libhistory.a $(DESTDIR)$(libdir)/libhistory.a
 	-test -n "$(RANLIB)" && $(RANLIB) $(DESTDIR)$(libdir)/libhistory.a
 
