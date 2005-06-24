? nsprpub/touched
? nsprpub/config/now,ff8
? nsprpub/config/nsinstall,ff8
? nsprpub/pr/include/md/_riscos.cfg
? nsprpub/pr/include/md/_riscos.h
? nsprpub/pr/src/md/unix/riscos.c
Index: nsprpub/configure.in
===================================================================
RCS file: /cvsroot/mozilla/nsprpub/configure.in,v
retrieving revision 1.189
diff -u -r1.189 configure.in
--- nsprpub/configure.in	2 Jun 2005 04:21:17 -0000	1.189
+++ nsprpub/configure.in	24 Jun 2005 16:25:43 -0000
@@ -1688,6 +1688,19 @@
     PR_MD_CSRCS=qnx.c
     ;;
 
+*-riscos*)
+    OS_TARGET=RISCOS
+    OS_ARCH=RISCOS
+    AC_DEFINE(XP_UNIX)
+    AC_DEFINE(RISCOS)
+    AC_DEFINE(_PR_NEED_H_ERRNO)
+    USE_PTHREADS=1
+    MDCPUCFG_H=_riscos.cfg
+    PR_MD_CSRCS=riscos.c
+    DLL_SUFFIX=a
+    LD="/home/riscos/env/ro-ar cr"
+    ;;
+
 *-*-sco*)
     AC_DEFINE(XP_UNIX)
     AC_DEFINE(SCO)
Index: nsprpub/pr/include/prthread.h
===================================================================
RCS file: /cvsroot/mozilla/nsprpub/pr/include/prthread.h,v
retrieving revision 3.11
diff -u -r3.11 prthread.h
--- nsprpub/pr/include/prthread.h	25 Apr 2004 15:00:47 -0000	3.11
+++ nsprpub/pr/include/prthread.h	24 Jun 2005 16:25:44 -0000
@@ -44,7 +44,7 @@
 ** is not guaranteed, so programming using priority based synchronization
 ** is a no-no.
 **
-** NSPR threads are scheduled based loosly on their client set priority.
+** NSPR threads are scheduled based loosely on their client set priority.
 ** In general, a thread of a higher priority has a statistically better
 ** chance of running relative to threads of lower priority. However,
 ** NSPR uses multiple strategies to provide execution vehicles for thread
Index: nsprpub/pr/include/md/_pth.h
===================================================================
RCS file: /cvsroot/mozilla/nsprpub/pr/include/md/_pth.h,v
retrieving revision 3.28
diff -u -r3.28 _pth.h
--- nsprpub/pr/include/md/_pth.h	22 Nov 2004 21:24:53 -0000	3.28
+++ nsprpub/pr/include/md/_pth.h	24 Jun 2005 16:25:48 -0000
@@ -146,7 +146,7 @@
 	|| defined(HPUX) || defined(LINUX) || defined(FREEBSD) \
 	|| defined(NETBSD) || defined(OPENBSD) || defined(BSDI) \
 	|| defined(VMS) || defined(NTO) || defined(DARWIN) \
-	|| defined(UNIXWARE)
+	|| defined(UNIXWARE) || defined(RISCOS)
 #define _PT_PTHREAD_INVALIDATE_THR_HANDLE(t)  (t) = 0
 #define _PT_PTHREAD_THR_HANDLE_IS_INVALID(t)  (t) == 0
 #define _PT_PTHREAD_COPY_THR_HANDLE(st, dt)   (dt) = (st)
@@ -259,7 +259,7 @@
 #define PT_PRIO_MIN            0
 #define PT_PRIO_MAX            31
 #elif defined(NETBSD) \
-	|| defined(BSDI) || defined(DARWIN) || defined(UNIXWARE) /* XXX */
+	|| defined(BSDI) || defined(DARWIN) || defined(UNIXWARE) || defined(RISCOS) /* XXX */
 #define PT_PRIO_MIN            0
 #define PT_PRIO_MAX            126
 #else
@@ -293,7 +293,7 @@
 #elif defined(HPUX) || defined(LINUX) || defined(SOLARIS) \
 	|| defined(FREEBSD) || defined(NETBSD) || defined(OPENBSD) \
 	|| defined(BSDI) || defined(NTO) || defined(DARWIN) \
-	|| defined(UNIXWARE)
+	|| defined(UNIXWARE) || defined(RISCOS)
 #define _PT_PTHREAD_YIELD()            	sched_yield()
 #else
 #error "Need to define _PT_PTHREAD_YIELD for this platform"
Index: nsprpub/pr/include/md/prosdep.h
===================================================================
RCS file: /cvsroot/mozilla/nsprpub/pr/include/md/prosdep.h,v
retrieving revision 3.15
diff -u -r3.15 prosdep.h
--- nsprpub/pr/include/md/prosdep.h	25 Apr 2004 15:00:47 -0000	3.15
+++ nsprpub/pr/include/md/prosdep.h	24 Jun 2005 16:25:48 -0000
@@ -138,6 +138,9 @@
 #elif defined(NTO)
 #include "md/_nto.h"
 
+#elif defined(RISCOS)
+#include "md/_riscos.h"
+
 #else
 #error unknown Unix flavor
 
Index: nsprpub/pr/src/io/prprf.c
===================================================================
RCS file: /cvsroot/mozilla/nsprpub/pr/src/io/prprf.c,v
retrieving revision 3.17
diff -u -r3.17 prprf.c
--- nsprpub/pr/src/io/prprf.c	25 Apr 2004 15:00:58 -0000	3.17
+++ nsprpub/pr/src/io/prprf.c	24 Jun 2005 16:25:49 -0000
@@ -394,7 +394,7 @@
 }
 
 /*
-** BiuldArgArray stands for Numbered Argument list Sprintf
+** BuildArgArray stands for Numbered Argument list Sprintf
 ** for example,  
 **	fmp = "%4$i, %2$d, %3s, %1d";
 ** the number must start from 1, and no gap among them
@@ -410,7 +410,7 @@
 
     /*
     **	first pass:
-    **	detemine how many legal % I have got, then allocate space
+    **	determine how many legal % I have got, then allocate space
     */
 
     p = fmt;
@@ -448,7 +448,7 @@
 	return NULL;
     }
 
-    
+
     if( number > NAS_DEFAULT_NUM ){
 	nas = (struct NumArg*)PR_MALLOC( number * sizeof( struct NumArg ) );
 	if( !nas ){
Index: nsprpub/pr/src/linking/prlink.c
===================================================================
RCS file: /cvsroot/mozilla/nsprpub/pr/src/linking/prlink.c,v
retrieving revision 3.76
diff -u -r3.76 prlink.c
--- nsprpub/pr/src/linking/prlink.c	10 Mar 2005 01:29:42 -0000	3.76
+++ nsprpub/pr/src/linking/prlink.c	24 Jun 2005 16:25:50 -0000
@@ -331,7 +331,7 @@
 #endif /* HAVE_DLL */
 #endif /* XP_UNIX */
 
-#if !defined(XP_MAC) && !defined(XP_BEOS)
+#if !defined(XP_MAC) && !defined(XP_BEOS) && !defined(RISCOS)
     PR_LOG(_pr_linker_lm, PR_LOG_MIN, ("Loaded library %s (init)", lm?lm->name:"NULL"));
 #endif
 
Index: nsprpub/pr/src/malloc/prmem.c
===================================================================
RCS file: /cvsroot/mozilla/nsprpub/pr/src/malloc/prmem.c,v
retrieving revision 3.17
diff -u -r3.17 prmem.c
--- nsprpub/pr/src/malloc/prmem.c	16 Mar 2005 23:21:11 -0000	3.17
+++ nsprpub/pr/src/malloc/prmem.c	24 Jun 2005 16:25:50 -0000
@@ -148,7 +148,7 @@
     return sym;
 }
 
-#elif defined(USE_MACH_DYLD)
+#elif defined(USE_MACH_DYLD) || defined(RISCOS)
 
 static void *
 pr_FindSymbolInProg(const char *name)
Index: nsprpub/pr/src/md/unix/unix.c
===================================================================
RCS file: /cvsroot/mozilla/nsprpub/pr/src/md/unix/unix.c,v
retrieving revision 3.50
diff -u -r3.50 unix.c
--- nsprpub/pr/src/md/unix/unix.c	9 Mar 2005 22:57:05 -0000	3.50
+++ nsprpub/pr/src/md/unix/unix.c	24 Jun 2005 16:25:55 -0000
@@ -75,7 +75,7 @@
 #define _PRSockLen_t int
 #elif (defined(AIX) && !defined(AIX4_1)) || defined(FREEBSD) \
     || defined(NETBSD) || defined(OPENBSD) || defined(UNIXWARE) \
-    || defined(DGUX) || defined(VMS) || defined(NTO)
+    || defined(DGUX) || defined(VMS) || defined(NTO) || defined(RISCOS)
 #define _PRSockLen_t size_t
 #else
 #error "Cannot determine architecture"
Index: nsprpub/pr/src/md/unix/uxrng.c
===================================================================
RCS file: /cvsroot/mozilla/nsprpub/pr/src/md/unix/uxrng.c,v
retrieving revision 1.17
diff -u -r1.17 uxrng.c
--- nsprpub/pr/src/md/unix/uxrng.c	25 Apr 2004 15:00:59 -0000	1.17
+++ nsprpub/pr/src/md/unix/uxrng.c	24 Jun 2005 16:25:55 -0000
@@ -298,7 +298,7 @@
     return 0;
 }
 #elif defined(SCO) || defined(UNIXWARE) || defined(BSDI) || defined(NTO) \
-    || defined(QNX) || defined(DARWIN)
+    || defined(QNX) || defined(DARWIN) || defined(RISCOS)
 #include <sys/times.h>
 
 static size_t
Index: nsprpub/pr/src/misc/prsystem.c
===================================================================
RCS file: /cvsroot/mozilla/nsprpub/pr/src/misc/prsystem.c,v
retrieving revision 3.25
diff -u -r3.25 prsystem.c
--- nsprpub/pr/src/misc/prsystem.c	6 May 2005 20:35:58 -0000	3.25
+++ nsprpub/pr/src/misc/prsystem.c	24 Jun 2005 16:25:55 -0000
@@ -255,6 +255,8 @@
     }
 #elif defined(IRIX)
     numCpus = sysconf( _SC_NPROC_ONLN );
+#elif defined(RISCOS)
+    numCpus = 1;
 #elif defined(XP_UNIX)
     numCpus = sysconf( _SC_NPROCESSORS_ONLN );
 #else
Index: nsprpub/pr/src/pthreads/ptio.c
===================================================================
RCS file: /cvsroot/mozilla/nsprpub/pr/src/pthreads/ptio.c,v
retrieving revision 3.100
diff -u -r3.100 ptio.c
--- nsprpub/pr/src/pthreads/ptio.c	23 Jun 2005 00:13:04 -0000	3.100
+++ nsprpub/pr/src/pthreads/ptio.c	24 Jun 2005 16:25:57 -0000
@@ -208,7 +208,7 @@
     || defined(HPUX10_30) || defined(HPUX11) || defined(LINUX) \
     || defined(FREEBSD) || defined(NETBSD) || defined(OPENBSD) \
     || defined(BSDI) || defined(VMS) || defined(NTO) || defined(DARWIN) \
-    || defined(UNIXWARE)
+    || defined(UNIXWARE) || defined(RISCOS)
 #define _PRSelectFdSetArg_t fd_set *
 #else
 #error "Cannot determine architecture"
@@ -3232,7 +3232,7 @@
 #if defined(HPUX) || defined(OSF1) || defined(SOLARIS) || defined (IRIX) \
     || defined(AIX) || defined(LINUX) || defined(FREEBSD) || defined(NETBSD) \
     || defined(OPENBSD) || defined(BSDI) || defined(VMS) || defined(NTO) \
-    || defined(DARWIN) || defined(UNIXWARE)
+    || defined(DARWIN) || defined(UNIXWARE) || defined(RISCOS)
 #define _PR_FCNTL_FLAGS O_NONBLOCK
 #else
 #error "Can't determine architecture"
Index: nsprpub/pr/src/pthreads/ptthread.c
===================================================================
RCS file: /cvsroot/mozilla/nsprpub/pr/src/pthreads/ptthread.c,v
retrieving revision 3.64
diff -u -r3.64 ptthread.c
--- nsprpub/pr/src/pthreads/ptthread.c	26 May 2005 02:27:51 -0000	3.64
+++ nsprpub/pr/src/pthreads/ptthread.c	24 Jun 2005 16:26:02 -0000
@@ -248,11 +248,11 @@
 
     /*
     * Here we set the pthread's backpointer to the PRThread to NULL.
-    * Otherwise the desctructor would get called eagerly as the thread
+    * Otherwise the destructor would get called eagerly as the thread
     * returns to the pthread runtime. The joining thread would them be
     * the proud possessor of a dangling reference. However, this is the
     * last chance to delete the object if the thread is detached, so
-    * just let the destuctor do the work.
+    * just let the destructor do the work.
     */
     if (PR_FALSE == detached)
     {
@@ -1233,7 +1233,7 @@
 	while (me->suspend & PT_THREAD_SUSPENDED)
 	{
 #if !defined(FREEBSD) && !defined(NETBSD) && !defined(OPENBSD) \
-    && !defined(BSDI) && !defined(VMS) && !defined(UNIXWARE) && !defined(DARWIN)  /*XXX*/
+    && !defined(BSDI) && !defined(VMS) && !defined(UNIXWARE) && !defined(DARWIN) && !defined(RISCOS) /*XXX*/
         PRIntn rv;
 	    sigwait(&sigwait_set, &rv);
 #endif
