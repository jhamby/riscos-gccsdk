Index: libstdc++-v3/acinclude.m4
===================================================================
--- libstdc++-v3/acinclude.m4	(revision 170835)
+++ libstdc++-v3/acinclude.m4	(working copy)
@@ -48,7 +48,7 @@
   # Keep these sync'd with the list in Makefile.am.  The first provides an
   # expandable list at autoconf time; the second provides an expandable list
   # (i.e., shell variable) at configure time.
-  m4_define([glibcxx_SUBDIRS],[include libsupc++ python src doc po testsuite])
+  m4_define([glibcxx_SUBDIRS],[include python src doc po testsuite])
   SUBDIRS='glibcxx_SUBDIRS'
 
   # These need to be absolute paths, yet at the same time need to
@@ -1696,7 +1696,7 @@
 
 m4_pushdef([n_syserr], [1])dnl
 m4_foreach([syserr], [EOWNERDEAD, ENOTRECOVERABLE, ENOLINK, EPROTO, ENODATA,
-		      ENOSR, ENOSTR, ETIME, EBADMSG, ECANCELED,
+		      ENOMSG, ENOSR, ENOSTR, ETIME, EBADMSG, ECANCELED,
 		      EOVERFLOW, ENOTSUP, EIDRM, ETXTBSY],
 [m4_pushdef([SYSERR], m4_toupper(syserr))dnl
 AC_MSG_CHECKING([for syserr])
