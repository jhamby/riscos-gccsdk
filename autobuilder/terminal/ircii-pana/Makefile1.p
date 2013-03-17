--- Makefile.in.old	2002-12-17 20:58:48.000000000 +0000
+++ Makefile.in	2002-12-17 20:24:06.000000000 +0000
@@ -74,7 +74,7 @@
 
 # Set this to -g if you want to be able to debug the client, otherwise
 # use -O to have the compiler do some optimization instead.
-CFLAGS = @CFLAGS@
+CFLAGS = @CFLAGS@ -DGETPGRP_VOID -I$GCCSDK_INSTALL_ENV/include
 
 # Set this to -s if you want the binary to be stripped.
 LDFLAGS = @LDFLAGS@
