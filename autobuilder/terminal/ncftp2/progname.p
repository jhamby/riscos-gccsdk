--- configure      2002-08-26 15:14:58.000000000 +0100
+++ configure      2002-08-26 13:53:22.000000000 +0100
@@ -596,8 +596,8 @@
 os_v=`uname -v 2>/dev/null`
 os_r=`uname -r 2>/dev/null`
 
-PROGS=ncftp
-PROG1=ncftp
+PROGS=ncftp,ff8
+PROG1=ncftp,ff8
 PROG2=ncftp.static
 SLDFLAGS='-static'
 Z31=''
