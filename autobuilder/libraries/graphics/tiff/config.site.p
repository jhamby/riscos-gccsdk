--- config.site.old	2004-11-30 12:19:13.000000000 +0000
+++ config.site	2004-11-30 12:19:46.000000000 +0000
@@ -153,6 +153,8 @@
 #TARGET=hppa1.1-hp-hpux11.11            # Force TARGET platform setting.
 #PATH="$PATH:/usr/ccs/bin"		# Modify path for finding compilers,etc
 
+GCOPTS="-I/home/riscos/env/include -L/home/riscos/env/lib"
+
 #
 # Makefile construction parameters.
 #
