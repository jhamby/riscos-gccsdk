--- Makefile.in.orig	2008-01-13 12:48:41.000000000 -0800
+++ Makefile.in	2008-01-13 12:49:11.000000000 -0800
@@ -35,7 +35,7 @@
 mawk_and_test :  mawk  mawk_test  fpe_test
 
 mawk : $(O)  rexp/.done
-	$(CC) $(CFLAGS) -o mawk $(O) $(REXP_O) $(MATHLIB) 
+	$(CC) $(CFLAGS) -o mawk$(AB_EXEEXT) $(O) $(REXP_O) $(MATHLIB) 
 
 mawk_test :  mawk  # test that we have a sane mawk
 	@cp mawk test/mawk
