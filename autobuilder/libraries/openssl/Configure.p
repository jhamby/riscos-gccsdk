--- Configure	2002-11-13 17:08:55.000000000 +0000
+++ Configure	2002-11-12 18:25:48.000000000 +0000
@@ -1,4 +1,4 @@
-#!/usr/local/bin/perl
+#!/usr/bin/perl
 eval 'exec perl -S $0 ${1+"$@"}'
     if $running_under_some_shell;
 ##
--- Configure.old	2003-03-09 11:47:42.000000000 +0000
+++ Configure	2003-03-09 11:48:42.000000000 +0000
@@ -448,6 +448,10 @@
 # Linux on ARM
 "linux-elf-arm","gcc:-DL_ENDIAN -DTERMIO -O3 -fomit-frame-pointer -Wall::-D_REENTRANT:::BN_LLONG::::::::::dlfcn:linux-shared:-fPIC::.so.\$(SHLIB_MAJOR).\$(SHLIB_MINOR)",
 
+# RISC OS on ARM
+"riscos-aof-arm","gcc:-DL_ENDIAN -DTERMIOS -DNO_SYS_UN_H -O3 -fomit-frame-pointer -Wall::-D_REENTRANT:::BN_LLONG:::::::::::::::echo",
+
+
 # SCO/Caldera targets.
 #
 # Originally we had like unixware-*, unixware-*-pentium, unixware-*-p6, etc.
