--- Makefile.old	2003-02-15 13:19:52.000000000 +0000
+++ Makefile	2003-02-15 15:01:20.000000000 +0000
@@ -26,7 +26,7 @@
 
 all: libbz2.a bzip2 bzip2recover test
 
-bzip2: libbz2.so bzip2.o
+bzip2: libbz2.a bzip2.o
 	$(CC) $(CFLAGS) $(LDFLAGS) -o bzip2 bzip2.o -L. -lbz2
 
 bzip2recover: bzip2recover.o
@@ -52,7 +52,7 @@
 	  -Wl,-soname,libbz2.so.$(somajor) $^ -lc
 
 %.sho: %.c
-	$(CC) $(CFLAGS) -D_REENTRANT -fPIC -o $@ -c $<
+	$(CC) $(CFLAGS) -D_REENTRANT -o $@ -c $<
 
 %.o: %.c
 	$(CC) $(CFLAGS) -D_REENTRANT -o $@ -c $<
