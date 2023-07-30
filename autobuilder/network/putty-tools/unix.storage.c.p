--- unix/storage.c.orig	2020-08-12 21:29:44.282950669 +0100
+++ unix/storage.c	2020-08-12 21:29:52.858788947 +0100
@@ -94,7 +94,7 @@
         if (env)
             return dupstr(env);
 
-        home = getenv("HOME");
+        home = dupstr("Boot:Choices");
         pwd = getpwuid(getuid());
         if (pwd && pwd->pw_dir) {
             pwd_home = pwd->pw_dir;
@@ -123,12 +123,12 @@
 
         old_dir = old_dir2 = old_dir3 = NULL;
         if (home) {
-            old_dir = dupprintf("%s/.putty", home);
+            old_dir = dupprintf("%s.Putty", home);
         }
         if (pwd_home) {
-            old_dir2 = dupprintf("%s/.putty", pwd_home);
+            old_dir2 = dupprintf("%s.Putty", pwd_home);
         }
-        old_dir3 = dupstr("/.putty");
+        old_dir3 = dupstr(".Putty");
 
         if (old_dir && access(old_dir, F_OK) == 0) {
             ret = old_dir;
@@ -183,13 +183,13 @@
         if (env)
             return dupstr(env);
         tmp = make_filename(INDEX_DIR, NULL);
-        ret = dupprintf("%s/sshhostkeys", tmp);
+        ret = dupprintf("%s.sshhostkeys", tmp);
         sfree(tmp);
         return ret;
     }
     if (index == INDEX_HOSTKEYS_TMP) {
         tmp = make_filename(INDEX_HOSTKEYS, NULL);
-        ret = dupprintf("%s.tmp", tmp);
+        ret = dupprintf("%s/tmp", tmp);
         sfree(tmp);
         return ret;
     }
@@ -198,7 +198,7 @@
         if (env)
             return dupstr(env);
         tmp = make_filename(INDEX_DIR, NULL);
-        ret = dupprintf("%s/randomseed", tmp);
+        ret = dupprintf("%s.randomseed", tmp);
         sfree(tmp);
         return ret;
     }
@@ -691,7 +691,7 @@
         char *dir, *errmsg;
 
         dir = make_filename(INDEX_DIR, NULL);
-        if ((errmsg = make_dir_path(dir, 0700)) != NULL) {
+        if ((errmsg = make_dir_path("Boot:Choices.Putty", 0700)) != NULL) {
             nonfatal("Unable to store host key: %s", errmsg);
             sfree(errmsg);
             sfree(dir);
