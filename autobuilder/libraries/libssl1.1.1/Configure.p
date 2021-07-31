--- Configure.orig	2018-09-12 00:48:19.000000000 +1200
+++ Configure	2018-11-27 14:40:42.072261756 +1300
@@ -1117,7 +1117,7 @@
 
 $target{CXXFLAGS}//=$target{CFLAGS} if $target{CXX};
 $target{cxxflags}//=$target{cflags} if $target{CXX};
-$target{exe_extension}="";
+# $target{exe_extension}="";
 $target{exe_extension}=".exe" if ($config{target} eq "DJGPP"
                                   || $config{target} =~ /^(?:Cygwin|mingw)/);
 $target{exe_extension}=".pm"  if ($config{target} =~ /vos/);
