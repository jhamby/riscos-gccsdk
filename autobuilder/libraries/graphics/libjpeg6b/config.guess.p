--- config.guess.old	2003-04-04 14:28:29.000000000 +0100
+++ config.guess	2003-04-04 13:29:02.000000000 +0100
@@ -136,6 +136,9 @@
     arm32:NetBSD:*:*)
 	echo arm-unknown-netbsd`echo ${UNAME_RELEASE}|sed -e 's/[-_].*/\./'`
 	exit 0 ;;
+    arm:riscos:*:*)
+	echo arm-riscos-aof
+	exit 0 ;;
     SR2?01:HI-UX/MPP:*:*)
 	echo hppa1.1-hitachi-hiuxmpp
 	exit 0;;
