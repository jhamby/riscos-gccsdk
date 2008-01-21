--- Configure.old	2007-04-04 22:43:10.000000000 +0200
+++ Configure	2007-04-04 23:00:22.000000000 +0200
@@ -1,4 +1,4 @@
-#!/usr/local/bin/perl
+#!/usr/bin/perl
 eval 'exec perl -S $0 ${1+"$@"}'
     if $running_under_some_shell;
 ##
@@ -422,6 +422,10 @@
 "qnx4",	"cc:-DL_ENDIAN -DTERMIO::(unknown):::${x86_gcc_des} ${x86_gcc_opts}:",
 "qnx6",	"cc:-DL_ENDIAN -DTERMIOS::(unknown)::-lsocket:${x86_gcc_des} ${x86_gcc_opts}:",
 
+# RISC OS on ARM
+"riscos-aof-arm","gcc:-DL_ENDIAN -DTERMIOS -DNO_SYS_UN_H -O3 -fomit-frame-pointer -Wall::-D_REENTRANT:::BN_LLONG DES_RISC1:::::::::::::::echo",
+"arm-unknown-riscos","arm-unknown-riscos-gcc:-DL_ENDIAN -DTERMIOS -DNO_SYS_UN_H -O3 -fomit-frame-pointer -Wall::-D_REENTRANT:::BN_LLONG DES_RISC1:::::::::::::::echo",
+
 #### SCO/Caldera targets.
 #
 # Originally we had like unixware-*, unixware-*-pentium, unixware-*-p6, etc.
@@ -963,6 +967,8 @@
 $IsMK1MF=1 if ($target eq "mingw" && $^O ne "cygwin" && !is_msys());
 
 $exe_ext=".exe" if ($target eq "Cygwin" || $target eq "DJGPP" || $target eq "mingw");
+$exe_ext=",ff8" if ($target eq "riscos-aof-arm");
+$exe_ext=",e1f" if ($target eq "arm-unknown-riscos");
 $exe_ext=".pm"  if ($target =~ /vos/);
 $openssldir="/usr/local/ssl" if ($openssldir eq "" and $prefix eq "");
 $prefix=$openssldir if $prefix eq "";
