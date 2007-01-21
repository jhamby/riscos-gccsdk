--- Configure.old	2005-12-27 04:22:21.000000000 +0100
+++ Configure	2005-12-27 03:58:16.000000000 +0100
@@ -1,4 +1,4 @@
-#!/usr/local/bin/perl
+#!/usr/bin/perl
 eval 'exec perl -S $0 ${1+"$@"}'
     if $running_under_some_shell;
 ##
@@ -420,6 +420,9 @@
 "qnx4",	"cc:-DL_ENDIAN -DTERMIO::(unknown):::${x86_gcc_des} ${x86_gcc_opts}:",
 "qnx6",	"cc:-DL_ENDIAN -DTERMIOS::(unknown)::-lsocket:${x86_gcc_des} ${x86_gcc_opts}:",
 
+# RISC OS on ARM
+"riscos-aof-arm","arm-unknown-riscos-gcc:-DL_ENDIAN -DTERMIOS -DNO_SYS_UN_H -O3 -fomit-frame-pointer -Wall::-D_REENTRANT:::BN_LLONG DES_RISC1:::::::::::::::echo",
+
 #### SCO/Caldera targets.
 #
 # Originally we had like unixware-*, unixware-*-pentium, unixware-*-p6, etc.
@@ -949,6 +952,7 @@
 $IsMK1MF=1 if ($target eq "mingw" && $^O ne "cygwin");
 
 $exe_ext=".exe" if ($target eq "Cygwin" || $target eq "DJGPP" || $target eq "mingw");
+$exe_ext=",e1f" if ($target eq "riscos-aof-arm");
 $exe_ext=".pm"  if ($target =~ /vos/);
 $openssldir="/usr/local/ssl" if ($openssldir eq "" and $prefix eq "");
 $prefix=$openssldir if $prefix eq "";
