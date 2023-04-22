--- Configure.orig	2022-12-17 09:03:07.077418340 +0000
+++ Configure	2022-12-17 09:06:13.684243756 +0000
@@ -1329,6 +1329,7 @@
 $target{cxxflags}//=$target{cflags} if $target{CXX};
 $target{exe_extension}=".exe" if ($config{target} eq "DJGPP");
 $target{exe_extension}=".pm"  if ($config{target} =~ /vos/);
+$target{exe_extension}=",e1f" if ($config{target} eq "arm-unknown-riscos");
 
 # Fill %config with values from %user, and in case those are undefined or
 # empty, use values from %target (acting as a default).
