--- gthread/Makefile.in	2002-11-24 14:00:28.000000000 +0000
+++ gthread/Makefile.in	2001-03-15 15:48:51.000000000 +0000
@@ -107,7 +107,7 @@
 
 libgthread_la_LIBADD = @G_THREAD_LIBS@
 
+#noinst_PROGRAMS = testgthread
-noinst_PROGRAMS = testgthread
 testgthread_LDADD = ../libglib.la libgthread.la 
 mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
 CONFIG_HEADER = ../config.h
--- Makefile.in	2003-04-02 14:16:52.000000000 +0100
+++ Makefile.in.old	2003-04-02 15:16:44.000000000 +0100
@@ -130,7 +130,7 @@
 libglib_la_LDFLAGS =  	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) 	-release $(LT_RELEASE) 	-export-dynamic
 
 
-noinst_PROGRAMS = testglib testgdate testgdateparser
+#noinst_PROGRAMS = testglib testgdate testgdateparser
 testglib_LDADD = libglib.la
 testgdate_LDADD = libglib.la
 testgdateparser_LDADD = libglib.la
@@ -158,7 +158,7 @@
 gmain.lo gmem.lo gmessages.lo gmutex.lo gnode.lo gprimes.lo grel.lo \
 gscanner.lo gslist.lo gstrfuncs.lo gstring.lo gtimer.lo gtree.lo \
 gutils.lo
-noinst_PROGRAMS =  testglib$(EXEEXT) testgdate$(EXEEXT) \
+#noinst_PROGRAMS =  testglib$(EXEEXT) testgdate$(EXEEXT) \
 testgdateparser$(EXEEXT)
 PROGRAMS =  $(noinst_PROGRAMS)
 
--- gthread/Makefile.in.old	2003-04-02 15:31:34.000000000 +0100
+++ gthread/Makefile.in	2003-04-02 14:31:56.000000000 +0100
@@ -125,7 +125,7 @@
 LIBS = @LIBS@
 libgthread_la_DEPENDENCIES = 
 libgthread_la_OBJECTS =  gthread.lo
-noinst_PROGRAMS =  testgthread$(EXEEXT)
+#noinst_PROGRAMS =  testgthread$(EXEEXT)
 PROGRAMS =  $(noinst_PROGRAMS)
 
 testgthread_SOURCES = testgthread.c
