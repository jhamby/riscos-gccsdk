Index: libiberty/configure.ac
===================================================================
--- libiberty/configure.ac	(revision 178499)
+++ libiberty/configure.ac	(working copy)
@@ -176,6 +176,15 @@
   enable_shared=no
 fi
 
+case "${host}" in
+  arm*-*-riscos)
+    # Don't build a shared library for SCL:
+    if [[ -n "`echo $CC | grep -E "(-mlibscl)|(-mmodule)"`" ]]; then
+      enable_shared=no
+    fi
+    ;;
+esac
+
 frag=
 case "${host}" in
   rs6000-ibm-aix3.1 | rs6000-ibm-aix)
@@ -466,6 +475,97 @@
     setobjs=yes
     ;;
 
+  arm*-*-riscos)
+    # If we are being configured for RISC OS, we know which functions
+    # UnixLib/SCL provides and which ones we will expected to provide.
+
+    if [[ -n "`echo $CC | grep -E "(-mlibscl)|(-mmodule)"`" ]]; then
+      # SCL support for functions mentioned in $funcs
+      AC_LIBOBJ([asprintf])
+      AC_LIBOBJ([basename])
+      AC_LIBOBJ([bcopy])
+      AC_LIBOBJ([bzero])
+      AC_LIBOBJ([getcwd])
+      AC_LIBOBJ([getpagesize])
+      AC_LIBOBJ([index])
+      AC_LIBOBJ([insque])
+      AC_LIBOBJ([memmem])
+      AC_LIBOBJ([mempcpy])
+      AC_LIBOBJ([mkstemps])
+      AC_LIBOBJ([random])
+      AC_LIBOBJ([rindex])
+      AC_LIBOBJ([sigsetmask])
+      AC_LIBOBJ([strcasecmp])
+      AC_LIBOBJ([strncasecmp])
+      AC_LIBOBJ([strndup])
+      AC_LIBOBJ([strverscmp])
+      AC_LIBOBJ([vasprintf])
+      AC_LIBOBJ([waitpid])
+      AC_LIBOBJ([setproctitle])
+      for f in $funcs; do
+        case "$f" in
+          asprintf | basename | bcopy | bzero | getcwd | getpagesize | index \
+            | insque | memmem | mempcpy | mkstemps | random | rindex \
+            | sigsetmask | strcasecmp | strncasecmp | strndup | strverscmp \
+            | vasprintf | waitpid | setproctitle)
+            ;;
+	  *)
+	    n=HAVE_`echo $f | tr 'abcdefghijklmnopqrstuvwxyz' 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'`
+	    AC_DEFINE_UNQUOTED($n)
+	    ;;
+        esac
+      done
+
+      # SCL support of the variables in $vars:
+      AC_DEFINE(HAVE_SYS_ERRLIST)
+      AC_DEFINE(HAVE_SYS_NERR)
+      AC_DEFINE(HAVE_SYS_SIGLIST)
+
+      # Of the functions in $checkfuncs, SCL only has:
+      AC_DEFINE(HAVE_GETTIMEOFDAY)
+      AC_DEFINE(HAVE_STRERROR)
+      AC_DEFINE(HAVE_STRSIGNAL)
+      AC_DEFINE(HAVE_TIMES)
+    else
+      # UnixLib support for functions mentioned in $funcs
+      AC_LIBOBJ([memmem])
+      AC_LIBOBJ([mempcpy])
+      AC_LIBOBJ([mkstemps])
+      AC_LIBOBJ([strverscmp])
+      AC_LIBOBJ([setproctitle])
+      for f in $funcs; do
+        case "$f" in
+          memmem | mempcpy | mkstemps | strverscmp | setproctitle)
+            ;;
+	  *)
+	    n=HAVE_`echo $f | tr 'abcdefghijklmnopqrstuvwxyz' 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'`
+	    AC_DEFINE_UNQUOTED($n)
+	    ;;
+        esac
+      done
+
+      # UnixLib support of the variables in $vars:
+      AC_DEFINE(HAVE_SYS_ERRLIST)
+      AC_DEFINE(HAVE_SYS_NERR)
+      AC_DEFINE(HAVE_SYS_SIGLIST)
+
+      # Of the functions in $checkfuncs, UnixLib only has:
+      AC_DEFINE(HAVE_GETRUSAGE)
+      AC_DEFINE(HAVE_GETTIMEOFDAY)
+      AC_DEFINE(HAVE_PSIGNAL)
+      AC_DEFINE(HAVE_REALPATH)
+      AC_DEFINE(HAVE_SBRK)
+      AC_DEFINE(HAVE_STRERROR)
+      AC_DEFINE(HAVE_STRSIGNAL)
+      AC_DEFINE(HAVE_SYSCONF)
+      AC_DEFINE(HAVE_TIMES)
+      AC_DEFINE(HAVE_WAIT3)
+      AC_DEFINE(HAVE_WAIT4)
+    fi
+
+    setobjs=yes
+    ;;
+
   *-*-msdosdjgpp)
     AC_LIBOBJ([vasprintf])
     AC_LIBOBJ([vsnprintf])
