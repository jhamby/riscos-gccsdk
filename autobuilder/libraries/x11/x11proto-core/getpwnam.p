--- Xos_r.h	2006-12-03 12:22:18.000000000 -0800
+++ Xos_r.h	2006-12-03 14:45:18.000000000 -0800
@@ -260,7 +260,7 @@
  */
 
 #if defined(__NetBSD__) || defined(__FreeBSD__) || defined(__OpenBSD__) || \
-    defined(__APPLE__) || defined(__DragonFly__)
+    defined(__APPLE__) || defined(__DragonFly__) || defined(__riscos__)
 static __inline__ void _Xpw_copyPasswd(_Xgetpwparams p)
 {
    memcpy(&(p).pws, (p).pwp, sizeof(struct passwd));
@@ -273,11 +273,15 @@
    (p).len = strlen((p).pwp->pw_passwd);
    strcpy((p).pws.pw_passwd,(p).pwp->pw_passwd);
 
+#ifdef __riscos__
+   (p).pws.pw_gecos = (p).pws.pw_passwd + (p).len + 1;
+#else
    (p).pws.pw_class = (p).pws.pw_passwd + (p).len + 1;
    (p).len = strlen((p).pwp->pw_class);
    strcpy((p).pws.pw_class, (p).pwp->pw_class);
 
    (p).pws.pw_gecos = (p).pws.pw_class + (p).len + 1;
+#endif
    (p).len = strlen((p).pwp->pw_gecos);
    strcpy((p).pws.pw_gecos, (p).pwp->pw_gecos);
 
