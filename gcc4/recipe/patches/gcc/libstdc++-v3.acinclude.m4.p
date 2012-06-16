Index: libstdc++-v3/acinclude.m4
===================================================================
--- libstdc++-v3/acinclude.m4	(revision 188171)
+++ libstdc++-v3/acinclude.m4	(working copy)
@@ -49,7 +49,7 @@
   # Keep these sync'd with the list in Makefile.am.  The first provides an
   # expandable list at autoconf time; the second provides an expandable list
   # (i.e., shell variable) at configure time.
-  m4_define([glibcxx_SUBDIRS],[include libsupc++ python src src/c++98 src/c++11 doc po testsuite])
+  m4_define([glibcxx_SUBDIRS],[include libsupc++ python src src/c++98 src/c++11 src/riscos doc po testsuite])
   SUBDIRS='glibcxx_SUBDIRS'
 
   # These need to be absolute paths, yet at the same time need to
@@ -1844,7 +1844,7 @@
 
 m4_pushdef([n_syserr], [1])dnl
 m4_foreach([syserr], [EOWNERDEAD, ENOTRECOVERABLE, ENOLINK, EPROTO, ENODATA,
-		      ENOSR, ENOSTR, ETIME, EBADMSG, ECANCELED,
+		      ENOMSG, ENOSR, ENOSTR, ETIME, EBADMSG, ECANCELED,
 		      EOVERFLOW, ENOTSUP, EIDRM, ETXTBSY,
 		      ECHILD, ENOSPC, EPERM,
 		      ETIMEDOUT, EWOULDBLOCK],
