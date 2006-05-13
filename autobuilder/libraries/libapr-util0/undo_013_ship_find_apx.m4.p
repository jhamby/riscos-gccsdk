--- srclib/apr/Makefile.in.orig	2006-04-27 12:59:11.126674222 +0200
+++ srclib/apr/Makefile.in	2006-04-27 12:59:11.077683986 +0200
@@ -84,13 +84,6 @@
 	fi;
 	$(LIBTOOL) --mode=install cp apr-config.out $(DESTDIR)$(bindir)/apr-config
 	chmod 755 $(DESTDIR)$(bindir)/apr-config
-
-	if [ ! -d $(DESTDIR)$(installbuilddir) ]; then \
-	    $(top_srcdir)/build/mkdir.sh $(DESTDIR)$(installbuilddir); \
-	fi
-	$(LIBTOOL) --mode=install cp $(top_srcdir)/build/find_apr.m4 \
-	    $(DESTDIR)$(installbuilddir)/find_apr.m4	
-
 	@if [ $(INSTALL_SUBDIRS) != "none" ]; then \
             for i in $(INSTALL_SUBDIRS); do \
 	        ( cd $$i ; $(MAKE) DESTDIR=$(DESTDIR) install ); \
--- srclib/apr-util/Makefile.in.orig	2006-04-27 13:04:35.533019811 +0200
+++ srclib/apr-util/Makefile.in	2006-04-27 13:04:35.424041530 +0200
@@ -80,12 +80,6 @@
 	$(LIBTOOL) --mode=install cp apu-config.out $(DESTDIR)$(bindir)/apu-config
 	chmod 755 $(DESTDIR)$(bindir)/apu-config
 
-	if [ ! -d $(DESTDIR)$(installbuilddir) ]; then \
-		@APR_SOURCE_DIR@/build/mkdir.sh $(DESTDIR)$(installbuilddir); \
-	fi
-	$(LIBTOOL) --mode=install cp $(top_srcdir)/build/find_apu.m4 \
-	  $(DESTDIR)$(installbuilddir)/find_apu.m4
-
 $(TARGET_LIB):
 	@objects="`find $(SUBDIRS) -name expat -prune -o -name 'gen_uri_delims.@so_ext@' -prune -o -name '*.@so_ext@' -print`"; \
 	    tmpcmd="$(LINK) $(APRUTILL_LDFLAGS) @lib_target@ @EXTRA_OS_LINK@ "; \
