--- Configure.orig	2009-07-24 12:51:28.000000000 -0700
+++ Configure	2009-07-24 12:53:00.000000000 -0700
@@ -1,4 +1,4 @@
-:
+#!/usr/bin/perl
 eval 'exec perl -S $0 ${1+"$@"}'
     if $running_under_some_shell;
 ##
@@ -437,6 +437,10 @@
 "qnx4",	"cc:-DL_ENDIAN -DTERMIO::(unknown):::${x86_gcc_des} ${x86_gcc_opts}:",
 "qnx6",	"cc:-DL_ENDIAN -DTERMIOS::(unknown)::-lsocket:${x86_gcc_des} ${x86_gcc_opts}:",
 
+# RISC OS on ARM
+"riscos-aof-arm","gcc:-DL_ENDIAN -DTERMIOS -DNO_SYS_UN_H -O3 -fomit-frame-pointer -Wall::-D_REENTRANT:::BN_LLONG DES_RISC1:::::::::::::::echo",
+"arm-unknown-riscos","arm-unknown-riscos-gcc:-DL_ENDIAN -DTERMIOS -DNO_SYS_UN_H -O3 -fomit-frame-pointer -Wall::-D_REENTRANT:::BN_LLONG DES_RISC1:::::::::::::::echo",
+
 #### SCO/Caldera targets.
 #
 # Originally we had like unixware-*, unixware-*-pentium, unixware-*-p6, etc.
@@ -1109,6 +1113,8 @@
 $no_shared = 0 if ($fipsdso && !$IsMK1MF);
 
 $exe_ext=".exe" if ($target eq "Cygwin" || $target eq "DJGPP" || $target eq "mingw");
+$exe_ext=",ff8" if ($target eq "riscos-aof-arm");
+$exe_ext=",e1f" if ($target eq "arm-unknown-riscos");
 $exe_ext=".nlm" if ($target =~ /netware/);
 $exe_ext=".pm"  if ($target =~ /vos/);
 if ($openssldir eq "" and $prefix eq "")
