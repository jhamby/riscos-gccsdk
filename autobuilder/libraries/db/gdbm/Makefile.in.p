--- Makefile.in.old	2003-06-19 18:21:26.000000000 +0100
+++ Makefile.in	2003-06-19 17:18:00.000000000 +0100
@@ -131,11 +131,11 @@
 		$(INSTALL_ROOT)$(includedir) $(INSTALL_ROOT)$(man3dir) \
 		$(INSTALL_ROOT)$(infodir)
 	$(LIBTOOL) $(INSTALL) -c libgdbm.la $(INSTALL_ROOT)$(libdir)/libgdbm.la
-	$(INSTALL_DATA) -o $(BINOWN) -g $(BINGRP) gdbm.h \
+	$(INSTALL_DATA) gdbm.h \
 		$(INSTALL_ROOT)$(includedir)/gdbm.h
-	$(INSTALL_DATA) -o $(BINOWN) -g $(BINGRP) $(srcdir)/gdbm.3 \
+	$(INSTALL_DATA) $(srcdir)/gdbm.3 \
 		$(INSTALL_ROOT)$(man3dir)/gdbm.3
-	$(INSTALL_DATA) -o $(BINOWN) -g $(BINGRP) $(srcdir)/gdbm.info \
+	$(INSTALL_DATA) $(srcdir)/gdbm.info \
 		$(INSTALL_ROOT)$(infodir)/gdbm.info
 
 install-compat:
@@ -143,9 +143,9 @@
 		$(INSTALL_ROOT)$(includedir)
 	$(LIBTOOL) $(INSTALL) -c libgdbm_compat.la \
 		$(INSTALL_ROOT)$(libdir)/libgdbm_compat.la
-	$(INSTALL_DATA) -o $(BINOWN) -g $(BINGRP) $(srcdir)/dbm.h \
+	$(INSTALL_DATA) $(srcdir)/dbm.h \
 		$(INSTALL_ROOT)$(includedir)/dbm.h
-	$(INSTALL_DATA) -o $(BINOWN) -g $(BINGRP) $(srcdir)/ndbm.h \
+	$(INSTALL_DATA) $(srcdir)/ndbm.h \
 		$(INSTALL_ROOT)$(includedir)/ndbm.h
 
 #libgdbm.a: $(OBJS) gdbm.h
