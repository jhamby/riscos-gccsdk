--- Makefile.in.orig	2016-10-02 09:23:56.513600065 +0100
+++ Makefile.in	2016-10-02 09:26:29.301600221 +0100
@@ -130,18 +130,18 @@
 	$(srcdir)/mkinstalldirs $(INSTALL_ROOT)$(libdir) \
 		$(INSTALL_ROOT)$(includedir) $(INSTALL_ROOT)$(man3dir) \
 		$(INSTALL_ROOT)$(infodir)
-	$(LIBTOOL) --mode=install $(INSTALL) -T libgdbm.la $(INSTALL_ROOT)$(libdir)/libgdbm.la
-	$(INSTALL_DATA) -o $(BINOWN) -g $(BINGRP) gdbm.h \
+	$(INSTALL_DATA) -T libgdbm.la $(INSTALL_ROOT)$(libdir)/libgdbm.la
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
-	$(LIBTOOL) --mode=install $(INSTALL) -T libgdbm_compat.la \
+	$(INSTALL) -T libgdbm_compat.la \
 		$(INSTALL_ROOT)$(libdir)/libgdbm_compat.la
 	$(INSTALL_DATA) -o $(BINOWN) -g $(BINGRP) $(srcdir)/dbm.h \
 		$(INSTALL_ROOT)$(includedir)/dbm.h
