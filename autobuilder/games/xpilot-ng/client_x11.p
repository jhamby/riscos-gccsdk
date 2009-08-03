--- src/client/x11/Makefile.in.orig	2005-09-16 01:35:54.000000000 +0200
+++ src/client/x11/Makefile.in	2005-10-31 16:37:03.000000000 +0100
@@ -65,7 +65,7 @@
 depcomp = $(SHELL) $(top_srcdir)/config/depcomp
 am__depfiles_maybe = depfiles
 COMPILE = $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
-	$(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
+	$(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS) $(X_CFLAGS)
 CCLD = $(CC)
 LINK = $(CCLD) $(AM_CFLAGS) $(CFLAGS) $(AM_LDFLAGS) $(LDFLAGS) -o $@
 SOURCES = $(xpilot_ng_x11_SOURCES)
--- src/client/x11/Makefile.in.orig	2009-08-02 21:36:35.000000000 -0700
+++ src/client/x11/Makefile.in	2009-08-02 21:36:45.000000000 -0700
@@ -213,7 +213,7 @@
 
 
 #xpilot_CFLAGS = @X_CFLAGS@
-xpilot_ng_x11_LDADD = $(top_builddir)/src/client/libxpclient.a $(top_builddir)/src/common/libxpcommon.a @X_LIBS@ @X_PRE_LIBS@ @X_EXTENSIONS_LIB@ @X_EXTRA_LIBS@ -lX11 @SOUND_LIBS@
+xpilot_ng_x11_LDADD = $(top_builddir)/src/client/libxpclient.a $(top_builddir)/src/common/libxpcommon.a @X_LIBS@ @X_PRE_LIBS@ @X_EXTENSIONS_LIB@ @X_EXTRA_LIBS@ -lCX11 -lDesk @SOUND_LIBS@
 all: all-am
 
 .SUFFIXES:
