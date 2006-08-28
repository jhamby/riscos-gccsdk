--- configure.org	2006-07-22 15:54:30.000000000 +0200
+++ configure	2006-07-22 15:55:05.000000000 +0200
@@ -4435,60 +4435,10 @@
 
 echo "$as_me:$LINENO: checking mmap usage" >&5
 echo $ECHO_N "checking mmap usage... $ECHO_C" >&6
-if test "$cross_compiling" = yes; then
-   cat >>confdefs.h <<\_ACEOF
-#define USE_MMAP 0
-_ACEOF
- ac_cv_use_mmap=no
-
-else
-  cat >conftest.$ac_ext <<_ACEOF
-#line $LINENO "configure"
-#include "confdefs.h"
-
-#if HAVE_SYS_TYPES_H
-#  include <sys/types.h>
-#endif
-#if HAVE_SYS_MMAN_H
-#  include <sys/mman.h>
-#endif
-main() {
-  /* so we know here that we could open /dev/zero and map to that but I
-     did not want the library to open another file-descriptor */
-  void *ret = mmap(0L, 10, PROT_READ | PROT_WRITE | PROT_EXEC,
-	           MAP_PRIVATE | MAP_ANON, -1 /* no fd */, 0 /* no offset */);
-  if (ret == MAP_FAILED) { exit(1); }
-  else { exit(0); }
-}
-
-_ACEOF
-rm -f conftest$ac_exeext
-if { (eval echo "$as_me:$LINENO: \"$ac_link\"") >&5
-  (eval $ac_link) 2>&5
-  ac_status=$?
-  echo "$as_me:$LINENO: \$? = $ac_status" >&5
-  (exit $ac_status); } && { ac_try='./conftest$ac_exeext'
-  { (eval echo "$as_me:$LINENO: \"$ac_try\"") >&5
-  (eval $ac_try) 2>&5
-  ac_status=$?
-  echo "$as_me:$LINENO: \$? = $ac_status" >&5
-  (exit $ac_status); }; }; then
    cat >>confdefs.h <<\_ACEOF
 #define USE_MMAP 1
 _ACEOF
  ac_cv_use_mmap=yes
-else
-  echo "$as_me: program exited with status $ac_status" >&5
-echo "$as_me: failed program was:" >&5
-cat conftest.$ac_ext >&5
-( exit $ac_status )
- cat >>confdefs.h <<\_ACEOF
-#define USE_MMAP 0
-_ACEOF
- ac_cv_use_mmap=no
-fi
-rm -f core core.* *.core conftest$ac_exeext conftest.$ac_objext conftest.$ac_ext
-fi
 echo "$as_me:$LINENO: result: $ac_cv_use_mmap" >&5
 echo "${ECHO_T}$ac_cv_use_mmap" >&6
 
@@ -4573,129 +4523,7 @@
 
 echo "$as_me:$LINENO: checking basic-block size" >&5
 echo $ECHO_N "checking basic-block size... $ECHO_C" >&6
-ac_cv_page_size=0
-if test $ac_cv_page_size = 0; then
-   if test "$cross_compiling" = yes; then
-  { { echo "$as_me:$LINENO: error: cannot run test program while cross compiling" >&5
-echo "$as_me: error: cannot run test program while cross compiling" >&2;}
-   { (exit 1); exit 1; }; }
-else
-  cat >conftest.$ac_ext <<_ACEOF
-main() { if (getpagesize()<=2048) exit(0); else exit(1); }
-_ACEOF
-rm -f conftest$ac_exeext
-if { (eval echo "$as_me:$LINENO: \"$ac_link\"") >&5
-  (eval $ac_link) 2>&5
-  ac_status=$?
-  echo "$as_me:$LINENO: \$? = $ac_status" >&5
-  (exit $ac_status); } && { ac_try='./conftest$ac_exeext'
-  { (eval echo "$as_me:$LINENO: \"$ac_try\"") >&5
-  (eval $ac_try) 2>&5
-  ac_status=$?
-  echo "$as_me:$LINENO: \$? = $ac_status" >&5
-  (exit $ac_status); }; }; then
-   ac_cv_page_size=11
-else
-  echo "$as_me: program exited with status $ac_status" >&5
-echo "$as_me: failed program was:" >&5
-cat conftest.$ac_ext >&5
-fi
-rm -f core core.* *.core conftest$ac_exeext conftest.$ac_objext conftest.$ac_ext
-fi
-fi
-if test $ac_cv_page_size = 0; then
-   if test "$cross_compiling" = yes; then
-  { { echo "$as_me:$LINENO: error: cannot run test program while cross compiling" >&5
-echo "$as_me: error: cannot run test program while cross compiling" >&2;}
-   { (exit 1); exit 1; }; }
-else
-  cat >conftest.$ac_ext <<_ACEOF
-main() { if (getpagesize()<=4096) exit(0); else exit(1); }
-_ACEOF
-rm -f conftest$ac_exeext
-if { (eval echo "$as_me:$LINENO: \"$ac_link\"") >&5
-  (eval $ac_link) 2>&5
-  ac_status=$?
-  echo "$as_me:$LINENO: \$? = $ac_status" >&5
-  (exit $ac_status); } && { ac_try='./conftest$ac_exeext'
-  { (eval echo "$as_me:$LINENO: \"$ac_try\"") >&5
-  (eval $ac_try) 2>&5
-  ac_status=$?
-  echo "$as_me:$LINENO: \$? = $ac_status" >&5
-  (exit $ac_status); }; }; then
-   ac_cv_page_size=12
-else
-  echo "$as_me: program exited with status $ac_status" >&5
-echo "$as_me: failed program was:" >&5
-cat conftest.$ac_ext >&5
-fi
-rm -f core core.* *.core conftest$ac_exeext conftest.$ac_objext conftest.$ac_ext
-fi
-fi
-if test $ac_cv_page_size = 0; then
-   if test "$cross_compiling" = yes; then
-  { { echo "$as_me:$LINENO: error: cannot run test program while cross compiling" >&5
-echo "$as_me: error: cannot run test program while cross compiling" >&2;}
-   { (exit 1); exit 1; }; }
-else
-  cat >conftest.$ac_ext <<_ACEOF
-main() { if (getpagesize()<=8192) exit(0); else exit(1); }
-_ACEOF
-rm -f conftest$ac_exeext
-if { (eval echo "$as_me:$LINENO: \"$ac_link\"") >&5
-  (eval $ac_link) 2>&5
-  ac_status=$?
-  echo "$as_me:$LINENO: \$? = $ac_status" >&5
-  (exit $ac_status); } && { ac_try='./conftest$ac_exeext'
-  { (eval echo "$as_me:$LINENO: \"$ac_try\"") >&5
-  (eval $ac_try) 2>&5
-  ac_status=$?
-  echo "$as_me:$LINENO: \$? = $ac_status" >&5
-  (exit $ac_status); }; }; then
-   ac_cv_page_size=13
-else
-  echo "$as_me: program exited with status $ac_status" >&5
-echo "$as_me: failed program was:" >&5
-cat conftest.$ac_ext >&5
-fi
-rm -f core core.* *.core conftest$ac_exeext conftest.$ac_objext conftest.$ac_ext
-fi
-fi
-if test $ac_cv_page_size = 0; then
-   if test "$cross_compiling" = yes; then
-  { { echo "$as_me:$LINENO: error: cannot run test program while cross compiling" >&5
-echo "$as_me: error: cannot run test program while cross compiling" >&2;}
-   { (exit 1); exit 1; }; }
-else
-  cat >conftest.$ac_ext <<_ACEOF
-main() { if (getpagesize()<=16384) exit(0); else exit(1); }
-_ACEOF
-rm -f conftest$ac_exeext
-if { (eval echo "$as_me:$LINENO: \"$ac_link\"") >&5
-  (eval $ac_link) 2>&5
-  ac_status=$?
-  echo "$as_me:$LINENO: \$? = $ac_status" >&5
-  (exit $ac_status); } && { ac_try='./conftest$ac_exeext'
-  { (eval echo "$as_me:$LINENO: \"$ac_try\"") >&5
-  (eval $ac_try) 2>&5
-  ac_status=$?
-  echo "$as_me:$LINENO: \$? = $ac_status" >&5
-  (exit $ac_status); }; }; then
-   ac_cv_page_size=14
-else
-  echo "$as_me: program exited with status $ac_status" >&5
-echo "$as_me: failed program was:" >&5
-cat conftest.$ac_ext >&5
-fi
-rm -f core core.* *.core conftest$ac_exeext conftest.$ac_objext conftest.$ac_ext
-fi
-fi
-if test $ac_cv_page_size = 0; then
-    ac_cv_page_size=15
-fi
-cat >>confdefs.h <<_ACEOF
-#define BASIC_BLOCK $ac_cv_page_size
-_ACEOF
+ac_cv_page_size=12
 
 echo "$as_me:$LINENO: result: $ac_cv_page_size" >&5
 echo "${ECHO_T}$ac_cv_page_size" >&6
