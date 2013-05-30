--- Makefile.in.orig	2012-09-23 11:06:01.000000000 +0100
+++ Makefile.in	2012-09-23 11:06:15.000000000 +0100
@@ -370,12 +370,9 @@
 	imported				\
 	libgnomevfs				\
 	modules					\
-	daemon					\
 	schemas					\
 	devel-docs				\
 	doc					\
-	programs				\
-	test					\
 	po
 
 EXTRA_DIST = \
--- libgnomevfs/Makefile.in.orig	2013-05-29 21:31:00.712107548 +0100
+++ libgnomevfs/Makefile.in	2013-05-29 21:31:17.528078275 +0100
@@ -430,7 +430,6 @@
 	-D_LARGEFILE64_SOURCE                   \
 	-D_POSIX_PTHREAD_SEMANTICS		\
 	-D_REENTRANT				\
-	-DG_DISABLE_DEPRECATED			\
 	-DGNOME_VFS_PREFIX=\"$(prefix)\"	\
 	-DGNOME_VFS_BINDIR=\"$(bindir)\"	\
 	-DGNOME_VFS_DATADIR=\"$(datadir)\"	\
