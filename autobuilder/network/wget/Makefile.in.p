--- doc/Makefile.in.orig	2009-12-03 18:46:10.000000000 -0800
+++ doc/Makefile.in	2009-12-03 18:30:26.000000000 -0800
@@ -1035,7 +1035,7 @@
 #wget.pod: $(srcdir)/wget.texi version.texi
 #	$(TEXI2POD) $(srcdir)/wget.texi $@
 wget.pod: wget.info
-	info2pod wget.info > $@
+	info2pod wget.info > $@ || true
 
 $(MAN): wget.pod
 	$(POD2MAN) --center="GNU Wget" --release="GNU Wget @VERSION@" $? > $@
