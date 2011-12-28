--- ostable.orig	2011-12-28 12:19:50.000000000 +0000
+++ ostable	2011-12-28 12:20:13.000000000 +0000
@@ -20,7 +20,7 @@
 gnueabi-linux		linux-gnueabi		linux[^-]*-gnueabi
 gnuspe-linux		linux-gnuspe		linux[^-]*-gnuspe
 gnulp-linux		linux-gnulp		linux[^-]*-gnulp
-gnu-linux		linux-gnu		linux[^-]*(-gnu.*)?
+gnu-linux		linux-gnu		[^-]*(-riscos.*)?
 gnu-kfreebsd		kfreebsd-gnu		kfreebsd[^-]*(-gnu.*)?
 gnu-knetbsd		knetbsd-gnu		knetbsd[^-]*(-gnu.*)?
 gnu-kopensolaris	kopensolaris-gnu	kopensolaris[^-]*(-gnu.*)?
