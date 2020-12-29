--- include/CMakeLists.txt.orig	2007-05-30 18:55:02.000000000 +0100
+++ include/CMakeLists.txt	2018-01-07 11:45:24.154296818 +0000
@@ -59,7 +59,6 @@
 CHECK_INCLUDE_FILES("utime.h" HAVE_UTIME_H)
 CHECK_INCLUDE_FILES("sys/ioctl.h" HAVE_SYS_IOCTL_H)
 CHECK_INCLUDE_FILES("sys/param.h" HAVE_SYS_PARAM_H)
-CHECK_INCLUDE_FILES("wait.h" HAVE_WAIT_H)
 CHECK_INCLUDE_FILES("sys/wait.h" HAVE_SYS_WAIT_H)
 CHECK_INCLUDE_FILES("sys/resource.h" HAVE_SYS_RESOURCE_H)
 CHECK_INCLUDE_FILES("sys/utsname.h" HAVE_SYS_UTSNAME_H)
@@ -67,12 +66,8 @@
 CHECK_INCLUDE_FILES("sys/poll.h" HAVE_SYS_POLL_H)
 CHECK_INCLUDE_FILES("netdb.h" HAVE_NETDB_H)
 CHECK_INCLUDE_FILES("sys/socket.h" HAVE_SYS_SOCKET_H)
-CHECK_INCLUDE_FILES("linux/pg.h" HAVE_LINUX_PG_H)
 CHECK_INCLUDE_FILES("arpa/inet.h" HAVE_ARPA_INET_H)
-CHECK_INCLUDE_FILES("sys/ipc.h" HAVE_SYS_IPC_H)
 CHECK_INCLUDE_FILES("sys/types.h;sys/mman.h" HAVE_SYS_MMAN_H)
-CHECK_INCLUDE_FILES("sys/types.h;sys/dkio.h" HAVE_SYS_DKIO_H)
-CHECK_INCLUDE_FILES("sys/types.h;sun/dkio.h" HAVE_SUN_DKIO_H)
 CHECK_INCLUDE_FILES("iconv.h" HAVE_ICONV_H)
 
 
@@ -114,11 +109,6 @@
 ")
 CHECK_C_SOURCE_COMPILES("${TESTSRC}" HAVE_DRAND48)
 SET(TESTSRC " 
-#include <stdlib.h>
-int main() { return ( (int) drand()  )  ; }
-")
-CHECK_C_SOURCE_COMPILES("${TESTSRC}" HAVE_DRAND)
-SET(TESTSRC " 
 #include <sys/socket.h>
 #include <netdb.h>
 int main(int argc, char **argv) {
@@ -140,25 +130,6 @@
 CHECK_C_SOURCE_COMPILES("${TESTSRC}" HAVE_INET_NTOA)
 
 SET(TESTSRC "
-#include <sys/types.h>
-#include <wait.h>
-int main() {
-union wait w;
-return 0; 
-}
-")
-# FIXME: try the same with #include <sys/wait.h>... may work, if yes, integrate properly
-CHECK_C_SOURCE_COMPILES("${TESTSRC}" HAVE_UNION_WAIT)
-SET(TESTSRC "
-#include <sys/types.h>
-#include <sys/mman.h>
-int main() {
-if (mlockall(MCL_CURRENT|MCL_FUTURE) < 0)  
-   return(0);
-   }
-")
-CHECK_C_SOURCE_COMPILES("${TESTSRC}" HAVE_MLOCKALL)
-SET(TESTSRC "
 int main(int argc, char **argv) {
    int a[argc];
    return 0;
