--- src/Makefile.in.org	2004-12-01 09:17:20.000000000 +0100
+++ src/Makefile.in	2006-05-15 18:59:06.900000000 +0200
@@ -54,14 +54,14 @@

 prefix = @prefix@
 exec_prefix = @exec_prefix@
-bindir = @bindir@
+bindir = '<GhostScript$$Dir>/bin'
 scriptdir = $(bindir)
 libdir = @libdir@
 mandir = @mandir@
 man1ext = 1
-datadir = @datadir@
-gsdir = $(datadir)/ghostscript
-gsdatadir = $(gsdir)/$(GS_DOT_VERSION)
+datadir = '<GhostScript$$Dir>'
+gsdir = '<GhostScript$$Dir>'
+gsdatadir = $(gsdir)/$(GS_VERSION_MAJOR)_$(GS_VERSION_MINOR)

 docdir=$(gsdatadir)/doc
 exdir=$(gsdatadir)/examples
@@ -75,7 +75,7 @@
 # Define the default directory for cached data files
 # this must be a single path.

-GS_CACHE_DIR="~/.ghostscript/cache/"
+GS_CACHE_DIR='<GhostScript$$ScrapDir>'

 # Define whether or not searching for initialization files should always
 # look in the current directory first.  This leads to well-known security
@@ -84,7 +84,7 @@
 # see the "File searching" section of Use.htm for full details.
 # Because of this, setting SEARCH_HERE_FIRST to 0 is not recommended.

-SEARCH_HERE_FIRST=1
+SEARCH_HERE_FIRST=0

 # Define the name of the interpreter initialization file.
 # (There is no reason to change this.)
@@ -301,7 +301,7 @@
 #	FPU_TYPE=-1 means that floating point is always much slower than
 # fixed point.

-FPU_TYPE=1
+FPU_TYPE=0

 # Define the .dev module that implements thread and synchronization
 # primitives for this platform.
@@ -317,7 +317,7 @@

 # Choose the language feature(s) to include.  See gs.mak for details.

-FEATURE_DEVS=$(PSD)psl3.dev $(PSD)pdf.dev $(PSD)dpsnext.dev $(PSD)ttfont.dev $(PSD)epsf.dev $(GLD)pipe.dev $(PSD)fapi.dev @JBIG2DEVS@ @JPXDEVS@
+FEATURE_DEVS=$(PSD)psl3.dev $(PSD)pdf.dev $(PSD)ttfont.dev $(PSD)epsf.dev $(PSD)jbig2.dev
 #FEATURE_DEVS=$(PSD)psl3.dev $(PSD)pdf.dev
 #FEATURE_DEVS=$(PSD)psl3.dev $(PSD)pdf.dev $(PSD)dpsnext.dev $(PSD)ttfont.dev $(PSD)rasterop.dev $(GLD)pipe.dev
 # The following is strictly for testing.
@@ -327,7 +327,7 @@
 # Choose whether to compile the .ps initialization files into the executable.
 # See gs.mak for details.

-COMPILE_INITS=@COMPILE_INITS@
+COMPILE_INITS=0

 # Choose whether to store band lists on files or in memory.
 # The choices are 'file' or 'memory'.
@@ -362,7 +362,7 @@
 # devs.mak and contrib.mak for the list of available devices.

 #DEVICE_DEVS=$(DISPLAY_DEV) $(DD)x11.dev $(DD)x11alpha.dev $(DD)x11cmyk.dev $(DD)x11gray2.dev $(DD)x11gray4.dev $(DD)x11mono.dev
-DEVICE_DEVS=$(DISPLAY_DEV) @X11DEVS@
+DEVICE_DEVS=$(DD)spr.dev $(DD)spr1.dev $(DD)spr8m.dev $(DD)sprm.dev $(DD)spr8c.dev

 DEVICE_DEVS1=$(DD)bmpmono.dev $(DD)bmpgray.dev $(DD)bmpsep1.dev $(DD)bmpsep8.dev $(DD)bmp16.dev $(DD)bmp256.dev $(DD)bmp16m.dev $(DD)bmp32b.dev
 DEVICE_DEVS2=#$(DD)rinkj.dev
@@ -380,7 +380,7 @@
 DEVICE_DEVS13=@PNGDEVS@
 DEVICE_DEVS14=$(DD)jpeg.dev $(DD)jpeggray.dev
 DEVICE_DEVS15=$(DD)pdfwrite.dev $(DD)pswrite.dev $(DD)epswrite.dev $(DD)pxlmono.dev $(DD)pxlcolor.dev
-DEVICE_DEVS16=$(DD)bbox.dev
+DEVICE_DEVS16=$(DD)gdi.dev

 DEVICE_DEVS17=
 DEVICE_DEVS18=
