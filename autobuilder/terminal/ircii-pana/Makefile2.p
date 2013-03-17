--- source/Makefile.in.old	2002-12-17 20:58:36.000000000 +0000
+++ source/Makefile.in	2002-12-17 20:22:50.000000000 +0000
@@ -50,7 +50,7 @@
 
 # Set this to -g if you want to be able to debug the client, otherwise
 # use -O to have the compiler do some optimization instead.
-CFLAGS = @CFLAGS@
+CFLAGS = @CFLAGS@ -O2 -DGETPGRP_VOID -I$GCCSDK_INSTALL_ENV/include
 
 # Set this to -s if you want the binary to be stripped.
 LDFLAGS = @LDFLAGS@
