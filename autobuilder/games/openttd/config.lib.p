--- config.lib.orig	2016-03-20 17:14:47.890080592 +0000
+++ config.lib	2016-03-20 17:14:36.978080580 +0000
@@ -1403,12 +1403,12 @@
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
