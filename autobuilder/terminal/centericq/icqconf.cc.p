--- src/icqconf.cc.old	2003-02-03 20:52:00.000000000 +0000
+++ src/icqconf.cc	2003-02-03 20:52:54.000000000 +0000
@@ -968,6 +968,7 @@
 void icqconf::checkdiskspace() {
     fenoughdiskspace = true;
 
+#ifndef __riscos__
 #ifndef __sun__
     struct statfs st;
     if(!statfs(conf.getdirname().c_str(), &st)) {
@@ -977,6 +978,7 @@
 #endif
 	fenoughdiskspace = ((double) st.f_bavail) * st.f_bsize >= 10240;
     }
+#endif    
 }
 
 // ----------------------------------------------------------------------------
