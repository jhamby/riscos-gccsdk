--- Makefile.in.orig	2018-09-13 10:59:15.000000000 +0100
+++ Makefile.in	2018-11-19 10:05:16.539107718 +0000
@@ -84,21 +84,21 @@
 prefix = @prefix@
 datarootdir = @datarootdir@
 exec_prefix = @exec_prefix@
-bindir = @bindir@
+bindir = '<GhostScript$$Dir>/bin'
 scriptdir = $(bindir)
 includedir = @includedir@
 libdir = @libdir@
 mandir = @mandir@
 man1ext = 1
 man1dir = $(mandir)/man$(man1ext)
-datadir = @datadir@
+datadir = '<GhostScript$$Dir>'
 docdir = @docdir@@VERSIONED_PATH@
 
 # The following must be substituted using @datadir@ and @libdir@
 # to avoid adding RPM generation paths (CUPS STR #1112)
-gsdir = @datadir@/ghostscript
-gsdatadir = $(gsdir)@VERSIONED_PATH@
-gssharedir = @libdir@/ghostscript@VERSIONED_PATH@
+gsdir = '<GhostScript$$Dir>'
+gsdatadir = '<GhostScript$$Dir>/$(GS_VERSION_MAJOR)_$(GS_VERSION_MINOR)'
+gssharedir = @VERSIONED_PATH@
 gsincludedir = @includedir@/ghostscript/
 
 exdir=$(gsdatadir)/examples
@@ -107,17 +107,17 @@
 # Choose whether to compile the .ps initialization files into the executable.
 # See gs.mak for details.
 
-COMPILE_INITS=@COMPILE_INITS@
+COMPILE_INITS=1
 
 # Define the default directory/ies for the runtime
 # initialization and font files.  Separate multiple directories with a :.
 
-GS_LIB_DEFAULT=$(gsdatadir)/Resource/Init:$(gsdatadir)/lib:$(gsdatadir)/Resource/Font:$(gsdir)/fonts:@fontpath@
+GS_LIB_DEFAULT=$(gsdatadir)/Resource/Init,$(gsdatadir)/lib,$(gsdatadir)/Resource/Font
 
 # Define the default directory for cached data files
 # this must be a single path.
 
-GS_CACHE_DIR="~/.ghostscript/cache/"
+GS_CACHE_DIR='<GhostScript$$ScrapDir>'
 
 # Define whether or not searching for initialization files should always
 # look in the current directory first.  This leads to well-known security
@@ -337,7 +337,7 @@
 # Define the directory where the ijs source is stored,
 # and the process forking method to use for the server.
 # See ijs.mak for more information.
- 
+
 SHARE_IJS=@SHAREIJS@
 IJS_NAME=@IJSLIB@
 IJSSRCDIR=@IJSDIR@
@@ -378,7 +378,7 @@
 # Define the name of the C compiler (target and host (AUX))
 
 CC=@CC@
-CCAUX=@CCAUX@
+CCAUX=/usr/bin/gcc
 
 # Define the name of the linker for the final link step.
 # Normally this is the same as the C compiler.
@@ -390,7 +390,7 @@
 GCFLAGS=@CPPFLAGS@ @GCFLAGS@ @CFLAGS@ @HAVE_POPEN_PROTO@
 GCFLAGSAUX=@GCFLAGSAUX@ @HAVE_POPEN_PROTO@
 
-# Define the added flags for standard, debugging, profiling 
+# Define the added flags for standard, debugging, profiling
 # and shared object builds.
 
 CFLAGS_STANDARD=@OPT_CFLAGS@
@@ -496,7 +496,7 @@
 #XLIBS=Xt SM ICE Xext X11
 
 # We use the autoconf macro AC_PATH_XTRA which defines X_LIBS with
-# the -L (or whatever). It also defines X_PRE_LIBS and X_EXTRA_LIBS 
+# the -L (or whatever). It also defines X_PRE_LIBS and X_EXTRA_LIBS
 # all three of which are stripped and slotted into XLIBS below.
 # Usually however, all but X_LIBS are empty on modern platforms.
 XLIBDIRS=@X_LDFLAGS@
@@ -583,13 +583,13 @@
 
 STDIO_IMPLEMENTATION=c
 
-# List of default devices, in order of priority. They need not be 
+# List of default devices, in order of priority. They need not be
 # present in the actual build.
-GS_DEV_DEFAULT="x11alpha bbox"
+GS_DEV_DEFAULT="spr"
 
-# Fallback default device.  This is set to 'display' by 
+# Fallback default device.  This is set to 'display' by
 # unix-dll.mak when building a shared object.
-DISPLAY_DEV=$(DD)bbox.dev
+DISPLAY_DEV=$(DD)spr.dev
 
 # Choose the device(s) to include.  See devs.mak for details,
 # devs.mak and contrib.mak for the list of available devices.
