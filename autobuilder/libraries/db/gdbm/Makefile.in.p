--- Makefile.in.orig	2009-07-24 12:39:47.000000000 -0700
+++ Makefile.in	2009-07-24 12:44:07.000000000 -0700
@@ -130,18 +130,18 @@
 	$(srcdir)/mkinstalldirs $(INSTALL_ROOT)$(libdir) \
 		$(INSTALL_ROOT)$(includedir) $(INSTALL_ROOT)$(man3dir) \
 		$(INSTALL_ROOT)$(infodir)
-	$(LIBTOOL) install $(INSTALL) -T libgdbm.la $(INSTALL_ROOT)$(libdir)/libgdbm.la
-	$(INSTALL_DATA) -o $(BINOWN) -g $(BINGRP) gdbm.h \
+	$(INSTALL) -T libgdbm.la $(INSTALL_ROOT)$(libdir)/libgdbm.la
+	$(INSTALL_DATA) gdbm.h \
 		$(INSTALL_ROOT)$(includedir)/gdbm.h
-	$(INSTALL_DATA) -o $(BINOWN) -g $(BINGRP) $(srcdir)/gdbm.3 \
+	$(INSTALL_DATA) $(srcdir)/gdbm.3 \
 		$(INSTALL_ROOT)$(man3dir)/gdbm.3
-	$(INSTALL_DATA) -o $(BINOWN) -g $(BINGRP) $(srcdir)/gdbm.info \
+	$(INSTALL_DATA) $(srcdir)/gdbm.info \
 		$(INSTALL_ROOT)$(infodir)/gdbm.info
 
 install-compat:
 	$(srcdir)/mkinstalldirs $(INSTALL_ROOT)$(libdir) \
 		$(INSTALL_ROOT)$(includedir)
-	$(LIBTOOL) install $(INSTALL) -T libgdbm_compat.la \
+	$(INSTALL) -T libgdbm_compat.la \
 		$(INSTALL_ROOT)$(libdir)/libgdbm_compat.la
 	$(INSTALL_DATA) -o $(BINOWN) -g $(BINGRP) $(srcdir)/dbm.h \
 		$(INSTALL_ROOT)$(includedir)/dbm.h
