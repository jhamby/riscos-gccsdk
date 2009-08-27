--- src/icqconf.cc.orig	2009-02-22 07:41:47.000000000 -0800
+++ src/icqconf.cc	2009-08-27 09:09:32.000000000 -0700
@@ -1812,6 +1812,7 @@
 void icqconf::checkdiskspace() {
     fenoughdiskspace = true;
 
+#ifndef __riscos__
 #if !(defined(__sun__) || defined(__NetBSD__) || defined(__sgi__))
     struct statfs st;
     if(!statfs(conf->getdirname().c_str(), &st)) {
@@ -1821,6 +1822,7 @@
 #endif
 	fenoughdiskspace = ((double) st.f_bavail) * st.f_bsize >= 10240;
     }
+#endif
 }
 
 void icqconf::setcharsets(const string &from, const string &to) {
