--- ./src/polkit/polkit-authorization-db-dummy.c.orig	2009-07-31 12:25:42.000000000 -0700
+++ ./src/polkit/polkit-authorization-db-dummy.c	2009-07-31 12:25:54.000000000 -0700
@@ -105,7 +105,7 @@
 polkit_authorization_db_debug (PolKitAuthorizationDB *authdb)
 {
         kit_return_if_fail (authdb != NULL);
-        _pk_debug ("PolKitAuthorizationDB: refcount=%d", authdb->refcount);
+//        _pk_debug ("PolKitAuthorizationDB: refcount=%d", authdb->refcount);
 }
 
 polkit_bool_t
