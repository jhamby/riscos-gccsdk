--- configure.ac.orig	2022-11-24 15:14:28.832602000 +0000
+++ configure.ac	2022-11-24 15:16:08.945753720 +0000
@@ -1831,6 +1831,9 @@
             if test x$ARCH = xnetbsd; then
                 RPI_CFLAGS="-I/usr/pkg/include -I/usr/pkg/include/interface/vcos/pthreads -I/usr/pkg/include/interface/vmcs_host/linux"
                 RPI_LIBS="-Wl,-R/usr/pkg/lib -L/usr/pkg/lib -lbcm_host"
+            elif test x$ARCH = xriscos; then
+                RPI_CFLAGS="-I$GCCSDK_INSTALL_ENV/vfp/include -I$GCCSDK_INSTALL_ENV/vfp/include/interface/vmcs_host/linux"
+                RPI_LIBS="-L$GCCSDK_INSTALL_ENV/vfp/lib -lbcm_host -ldl"
             else
                 RPI_CFLAGS="-I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux"
                 RPI_LIBS="-Wl,-rpath,/opt/vc/lib -L/opt/vc/lib -lbcm_host"
@@ -4550,10 +4553,15 @@
         CheckDiskAudio
         CheckDummyAudio
         CheckDLOPEN
+        CheckRPATH
         CheckO_CLOEXEC
         CheckOSS
         CheckPTHREAD
         CheckClockGettime
+        # Need to check for Raspberry PI first and add platform specific compiler flags, otherwise the test for GLES fails!
+        CheckRPI
+        CheckEGL
+        CheckOpenGLES
 
         # Set up files for the misc library
         if test x$enable_misc = xyes; then
@@ -4839,6 +4847,9 @@
     LDFLAGS="$save_LDFLAGS"
     AC_MSG_RESULT($have_enable_new_dtags)
   fi
+  if test $ARCH = riscos; then
+    SDL_RLD_FLAGS="-Wl,-rpath-link,\${libdir}"
+  fi
   if test $ARCH = solaris; then
     SDL_RLD_FLAGS="-R\${libdir}"
   fi
