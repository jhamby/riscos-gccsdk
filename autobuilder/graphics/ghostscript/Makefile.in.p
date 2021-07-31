--- base/Makefile.in.orig	2009-12-18 07:04:10.000000000 +0000
+++ base/Makefile.in	2012-09-26 14:56:29.000000000 +0100
@@ -54,20 +54,20 @@

 prefix = @prefix@
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
 datarootdir = @datarootdir@

 # The following must be substituted using @datadir@ and @libdir@
 # to avoid adding RPM generation paths (CUPS STR #1112)
-gsdir = @datadir@/ghostscript
-gsdatadir = $(gsdir)/$(GS_DOT_VERSION)
+gsdir = '<GhostScript$$Dir>'
+gsdatadir = $(gsdir)/$(GS_VERSION_MAJOR)_$(GS_VERSION_MINOR)
 gssharedir = @libdir@/ghostscript/$(GS_DOT_VERSION)
 gsincludedir = @includedir@/ghostscript/

@@ -87,12 +87,12 @@
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
@@ -101,7 +101,7 @@
 # see the "File searching" section of Use.htm for full details.
 # Because of this, setting SEARCH_HERE_FIRST to 0 is not recommended.

-SEARCH_HERE_FIRST=1
+SEARCH_HERE_FIRST=0

 # Define the name of the interpreter initialization file.
 # (There is no reason to change this.)
@@ -375,7 +375,7 @@
 # Choose whether to compile the .ps initialization files into the executable.
 # See gs.mak for details.

-COMPILE_INITS=@COMPILE_INITS@
+COMPILE_INITS=1

 # Choose whether to store band lists on files or in memory.
 # The choices are 'file' or 'memory'.
@@ -399,11 +399,11 @@

 # List of default devices, in order of priority. They need not be
 # present in the actual build.
-GS_DEV_DEFAULT="x11alpha x11 bbox"
+GS_DEV_DEFAULT="spr"

 # Fallback default device.  This is set to 'display' by
 # unix-dll.mak when building a shared object.
-DISPLAY_DEV=$(DD)bbox.dev
+DISPLAY_DEV=$(DD)spr.dev

 # Choose the device(s) to include.  See devs.mak for details,
 # devs.mak and contrib.mak for the list of available devices.
