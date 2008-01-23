--- src/icqconf.cc.orig	2008-01-22 21:17:49.000000000 -0800
+++ src/icqconf.cc	2008-01-22 21:18:12.000000000 -0800
@@ -1714,6 +1714,7 @@
 void icqconf::checkdiskspace() {
     fenoughdiskspace = true;
 
+#if 0
 #ifndef __sun__
     struct statfs st;
     if(!statfs(conf.getdirname().c_str(), &st)) {
@@ -1723,6 +1724,7 @@
 #endif
 	fenoughdiskspace = ((double) st.f_bavail) * st.f_bsize >= 10240;
     }
+#endif
 }
 
 void icqconf::setcharsets(const string &from, const string &to) {
