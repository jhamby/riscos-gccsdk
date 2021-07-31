--- libgnome/gnome-score.c.orig	2009-08-01 11:24:12.000000000 -0700
+++ libgnome/gnome-score.c	2009-08-01 11:24:41.000000000 -0700
@@ -306,7 +306,7 @@
 static void
 drop_perms (void)
 {
-#ifndef G_OS_WIN32
+#if !defined(G_OS_WIN32) && !defined(__riscos__)
    gid_t gid = getegid ();
 
    setregid (getgid (), getgid ());	/* on some os'es (eg linux) this
