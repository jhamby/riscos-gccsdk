--- Configure.orig	2013-05-29 19:27:05.143241637 +0100
+++ Configure	2013-05-29 19:28:31.210000000 +0100
@@ -486,6 +486,10 @@
 "beos-x86-r5",   "gcc:-DL_ENDIAN -DTERMIOS -O3 -fomit-frame-pointer -mcpu=pentium -Wall::-D_REENTRANT:BEOS:-lbe -lnet:BN_LLONG ${x86_gcc_des} ${x86_gcc_opts}:${x86_elf_asm}:beos:beos-shared:-fPIC -DPIC:-shared:.so",
 "beos-x86-bone", "gcc:-DL_ENDIAN -DTERMIOS -O3 -fomit-frame-pointer -mcpu=pentium -Wall::-D_REENTRANT:BEOS:-lbe -lbind -lsocket:BN_LLONG ${x86_gcc_des} ${x86_gcc_opts}:${x86_elf_asm}:beos:beos-shared:-fPIC:-shared:.so",
 
+# RISC OS on ARM
+"riscos-aof-arm","gcc:-DL_ENDIAN -DTERMIOS -DNO_SYS_UN_H -O3 -fomit-frame-pointer -Wall::-D_REENTRANT:::BN_LLONG DES_RISC1:::::::::::::::echo",
+"arm-unknown-riscos","arm-unknown-riscos-gcc:-DL_ENDIAN -DTERMIOS -DNO_SYS_UN_H -O3 -fomit-frame-pointer -Wall::-D_REENTRANT:::BN_LLONG DES_RISC1:::::::::::::::echo",
+
 #### SCO/Caldera targets.
 #
 # Originally we had like unixware-*, unixware-*-pentium, unixware-*-p6, etc.
@@ -1169,6 +1173,8 @@
 my $IsMK1MF=scalar grep /^$target$/,@MK1MF_Builds;
 
 $exe_ext=".exe" if ($target eq "Cygwin" || $target eq "DJGPP" || $target =~ /^mingw/);
+$exe_ext=",ff8" if ($target eq "riscos-aof-arm");
+$exe_ext=",e1f" if ($target eq "arm-unknown-riscos");
 $exe_ext=".nlm" if ($target =~ /netware/);
 $exe_ext=".pm"  if ($target =~ /vos/);
 $openssldir="/usr/local/ssl" if ($openssldir eq "" and $prefix eq "");
