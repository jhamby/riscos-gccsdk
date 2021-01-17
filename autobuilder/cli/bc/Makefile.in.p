--- bc/Makefile.in.orig	2021-01-17 13:00:57.518522238 +0000
+++ bc/Makefile.in	2021-01-17 13:01:35.701787646 +0000
@@ -625,11 +625,11 @@
 global.o: libmath.h
 
 libmath.h: libmath.b $(fbcOBJ) $(LIBBC)
-	echo '{0}' > libmath.h
+#	echo '{0}' > libmath.h
- 	$(MAKE) global.o
+# 	$(MAKE) global.o
- 	$(LINK) -o fbc $(fbcOBJ) global.o $(LIBBC) $(LIBL) $(READLINELIB) $(LIBS)
+# 	$(LINK) -o fbc $(fbcOBJ) global.o $(LIBBC) $(LIBL) $(READLINELIB) $(LIBS)
-	./fbc -c $(srcdir)/libmath.b </dev/null >libmath.h
-	$(srcdir)/fix-libmath_h
+#	./fbc -c $(srcdir)/libmath.b </dev/null >libmath.h
+#	$(srcdir)/fix-libmath_h
 	rm -f ./fbc ./global.o
 sbc.o: sbc.c
 sbc: $(sbcOBJ) $(LIBBC)
