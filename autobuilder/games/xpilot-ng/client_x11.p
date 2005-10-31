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
