Index: Makefile
===================================================================
RCS file: /cvsroot/scummvm/scummvm/Makefile,v
retrieving revision 1.103
diff -u -r1.103 Makefile
--- Makefile	24 Jun 2005 15:26:06 -0000	1.103
+++ Makefile	5 Jul 2005 22:09:20 -0000
@@ -22,7 +22,7 @@
 # CXXFLAGS+= -Werror
 
 CXXFLAGS:= -Wall $(CXXFLAGS)
-CXXFLAGS+= -O -Wuninitialized
+CXXFLAGS+= -Wuninitialized
 CXXFLAGS+= -Wno-long-long -Wno-multichar -Wno-unknown-pragmas
 # Even more warnings...
 CXXFLAGS+= -pedantic -Wpointer-arith -Wcast-qual -Wconversion
