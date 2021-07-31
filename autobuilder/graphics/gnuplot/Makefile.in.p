--- Makefile.in.orig	2002-10-25 13:32:54.000000000 +0200
+++ Makefile.in	2016-01-01 20:10:24.117637550 +0100
@@ -47,9 +47,10 @@
 DEFS  = @DEFS@
 INCLUDES = -I. -I$(top_srcdir)
 CFLAGS = @CFLAGS@
+ROCFLAGS =  -D__RISCOS -DBB_UNIXLIBBUG=0
 LDFLAGS = @LDFLAGS@
 
-COMPILE = $(CC) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)
+COMPILE = $(CC) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) $(ROCFLAGS)
 LINK = $(CC) $(CFLAGS) $(LDFLAGS) -o $@
 
 # This stuff is obsolete now. Use the appropriate configure options!
@@ -200,13 +201,13 @@
 # W as the prefix for Windows (eg win/)
 
 ####################################################################
-# List of object files except version.$(O)
+# List of object files except version.$(O) & set.$(O)
 COREOBJS = alloc.$(O) binary.$(O) bitmap.$(O) command.$(O) contour.$(O)\
 	datafile.$(O) eval.$(O) fit.$(O) graphics.$(O) graph3d.$(O) help.$(O)\
 	hidden3d.$(O) internal.$(O) interpol.$(O) matrix.$(O) misc.$(O)\
 	parse.$(O) plot.$(O) plot2d.$(O) plot3d.$(O) readline.$(O)\
-	scanner.$(O) set.$(O) show.$(O) specfun.$(O) standard.$(O) stdfn.$(O)\
-	term.$(O) time.$(O) util.$(O) util3d.$(O)
+	scanner.$(O) show.$(O) specfun.$(O) standard.$(O) stdfn.$(O)\
+	term.$(O) time.$(O) util.$(O) util3d.$(O) settype.$(O) viewer.$(O)
 
 ####################################################################
 # List of source files
@@ -216,7 +217,7 @@
 	  fit.c graphics.c graph3d.c help.c hidden3d.c internal.c interpol.c \
 	  matrix.c misc.c parse.c plot.c plot2d.c plot3d.c readline.c \
 	  scanner.c set.c show.c specfun.c standard.c stdfn.c term.c time.c \
-	  util.c util3d.c version.c
+	  util.c util3d.c version.c settype.c viewer.c
 
 HEADERS = alloc.h ansichek.h binary.h bitmap.h fit.h fnproto.h help.h \
 	  matrix.h national.h plot.h protos.h setshow.h stdfn.h syscfg.h term.h
@@ -239,7 +240,8 @@
 	   $(T)t410x.trm $(T)table.trm \
 	   $(T)tek.trm $(T)texdraw.trm $(T)tgif.trm $(T)tkcanvas.trm \
 	   $(T)tpic.trm $(T)unixpc.trm $(T)unixplot.trm $(T)v384.trm \
-	   $(T)vws.trm $(T)win.trm $(T)x11.trm $(T)xlib.trm
+	   $(T)vws.trm $(T)win.trm $(T)x11.trm $(T)xlib.trm \
+           $(T)rodraw.trm $(T)rosprite.trm
 
 CSOURCE8 = bf_test.c gplt_x11.c
 
@@ -352,7 +354,7 @@
 ####################################################
 # (that's the end of makefile.all, by the way...)
 
-# List of object files except version.o
+# List of object files except version.o & set.o
 
 OBJS = $(COREOBJS)
 
@@ -385,8 +387,10 @@
 .c.o:
 	$(COMPILE) -c $<
 
-gnuplot: $(OBJS) version.o
-	$(LINK) $(OBJS) version.o $(TERMLIBS) $(TERMXLIBS) $(LIBS)
+gnuplot: $(OBJS) version.o set.o
+	$(LINK) $(OBJS) version.o set.o $(TERMLIBS) $(TERMXLIBS) $(LIBS)
+	cp gnuplot GnuPlot373,e1f
+#MRB 27.12.15 exe for ROS is called GnuPlot373,e1f
 
 gnuplot_x11: gplt_x11.o stdfn.o
 	$(LINK) gplt_x11.o stdfn.o $(XLIBS) $(LIBS)
@@ -395,7 +399,8 @@
 	cd docs && $(MAKE) $(MFLAGS)
 
 demo/binary1 demo/binary2 demo/binary3: bf_test
-	wd=`pwd` && cd $(srcdir)/demo && $$wd/bf_test
+# MRB 27.12.15 running a ROS program under linux won't work
+#	wd=`pwd` && cd $(srcdir)/demo && $$wd/bf_test
 
 bf_test: bf_test.o binary.o alloc.o
 	$(LINK) bf_test.o binary.o alloc.o $(LIBS)
@@ -405,17 +410,25 @@
 	touch errorfix
 
 command.o: command.c fit.h help.h setshow.h
-	$(COMPILE) -DHELPFILE=\"$(HELPFILE)\" -c $<
+# MRB 01.01.16 ignore HELPFILE - use define in syscfg.h
+#	$(COMPILE) -DHELPFILE=\"$(HELPFILE)\" -c $<
+	$(COMPILE) -c $<
 
 show.o: show.c plot.h setshow.h
-	$(COMPILE) -DHELPFILE=\"$(HELPFILE)\" -DGNUPLOT_BINDIR=\"$(bindir)\" -c $<
+# MRB 01.01.16 ignore HELPFILE - use define in syscfg.h
+#	$(COMPILE) -DHELPFILE=\"$(HELPFILE)\" -DGNUPLOT_BINDIR=\"$(bindir)\" -c $<
+	$(COMPILE) -DGNUPLOT_BINDIR=\"$(bindir)\" -c $<
 
 term.o: term.c term.h $(CORETERM)
-	$(COMPILE) -I./term -I$(top_srcdir)/term -c $<
+	$(COMPILE) -I./term -I$(top_srcdir)/term -DDEFAULTTERM=\"rosprite\" -c $<
 
 version.o: version.c
 	$(COMPILE) -DCONTACT=\"$(EMAIL)\" -c $<
 
+#MRB 31.12.15 set.c doesn't compile using gcc 4.7.3 with the -O3 option unless -fno-builtin-log is set
+set.o: set.c
+	$(COMPILE) -fno-builtin-log -c $<
+
 $(COREOBJS) version.o bf_test.o : $(CONFIG_HEADER)
 
 ################################################################
