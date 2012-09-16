--- Configure.orig	2012-09-16 17:40:30.000000000 +0100
+++ Configure	2012-09-16 17:42:59.000000000 +0100
@@ -447,6 +447,10 @@
 "QNX6",       "gcc:-DTERMIOS::::-lsocket::${no_asm}:dlfcn:bsd-gcc-shared:-fPIC::.so.\$(SHLIB_MAJOR).\$(SHLIB_MINOR)",
 "QNX6-i386",  "gcc:-DL_ENDIAN -DTERMIOS -O2 -Wall::::-lsocket:${x86_gcc_des} ${x86_gcc_opts}:${x86_elf_asm}:dlfcn:bsd-gcc-shared:-fPIC::.so.\$(SHLIB_MAJOR).\$(SHLIB_MINOR)",
 
+# RISC OS on ARM
+"riscos-aof-arm","gcc:-DL_ENDIAN -DTERMIOS -DNO_SYS_UN_H -O3 -fomit-frame-pointer -Wall::-D_REENTRANT:::BN_LLONG DES_RISC1:::::::::::::::echo",
+"arm-unknown-riscos","arm-unknown-riscos-gcc:-DL_ENDIAN -DTERMIOS -DNO_SYS_UN_H -O3 -fomit-frame-pointer -Wall::-D_REENTRANT:::BN_LLONG DES_RISC1:::::::::::::::echo",
+
 #### SCO/Caldera targets.
 #
 # Originally we had like unixware-*, unixware-*-pentium, unixware-*-p6, etc.
@@ -1130,6 +1134,8 @@
 $no_shared = 0 if ($fipsdso && !$IsMK1MF);
 
 $exe_ext=".exe" if ($target eq "Cygwin" || $target eq "DJGPP" || $target eq "mingw");
+$exe_ext=",ff8" if ($target eq "riscos-aof-arm");
+$exe_ext=",e1f" if ($target eq "arm-unknown-riscos");
 $exe_ext=".nlm" if ($target =~ /netware/);
 $exe_ext=".pm"  if ($target =~ /vos/);
 if ($openssldir eq "" and $prefix eq "")
