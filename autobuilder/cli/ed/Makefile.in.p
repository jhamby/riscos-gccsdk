--- Makefile.in.orig	2007-11-30 21:22:51.000000000 -0800
+++ Makefile.in	2007-11-30 21:23:13.000000000 -0800
@@ -15,7 +15,7 @@
 all : $(progname)
 
 $(progname) : $(objs)
-	$(CC) $(LDFLAGS) -o $(progname) $(objs)
+	$(CC) $(LDFLAGS) -o $(progname)$(AB_EXEEXT) $(objs)
 
 p$(progname) : $(objs)
 	$(CC) $(LDFLAGS) -pg -o p$(progname) $(objs)
