--- config.lib.orig	2018-04-01 12:16:47.000000000 +0100
+++ config.lib	2023-03-18 11:24:35.498910704 +0000
@@ -1458,12 +1458,12 @@
 			fi
 		fi
 
-		has_rdynamic=`$1 -dumpspecs | grep rdynamic`
-		if [ -n "$has_rdynamic" ]; then
-			# rdynamic is used to get useful stack traces from crash reports.
-			flags="$flags -rdynamic"
-			ldflags="$ldflags -rdynamic"
-		fi
+#		has_rdynamic=`$1 -dumpspecs | grep rdynamic`
+#		if [ -n "$has_rdynamic" ]; then
+#			# rdynamic is used to get useful stack traces from crash reports.
+#			flags="$flags -rdynamic"
+#			ldflags="$ldflags -rdynamic"
+#		fi
 	fi
 
 	eval "$2=\"$flags\""
@@ -1486,7 +1486,7 @@
 	# Special CXXFlags for HOST
 	CXXFLAGS="$CXXFLAGS"
 	# Libs to compile. In fact this is just LDFLAGS
-	LIBS="-lstdc++"
+	LIBS="-lstdc++ -lSDL"
 	# LDFLAGS used for HOST
 	LDFLAGS="$LDFLAGS"
 	# FEATURES for HOST (lto)
