--- dselect/Makefile.in.old	2004-02-04 13:53:36.000000000 +0000
+++ dselect/Makefile.in	2004-02-04 13:48:10.000000000 +0000
@@ -54,7 +54,7 @@
 
 curkeys.h: keyoverride $(srcdir)/mkcurkeys.pl
 	cursesfile=`echo '#include <curses.h>' | \
-		 $(CC) -E - | grep 'curses.h' | head -n 1 | \
+		 $(CC) -I/home/riscos/env/include -E - | grep 'curses.h' | head -n 1 | \
 		 $(SED) -e 's/^[^"]*"//; s/".*$$//'`; \
 	if [ "$$cursesfile" = "" ]; then echo "can't find curses file"; exit 1; fi; \
 	perl $(srcdir)/mkcurkeys.pl $< $$cursesfile > $@
