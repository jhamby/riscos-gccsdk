--- Tgif.tmpl.orig	2005-05-12 13:01:24.000000000 +0100
+++ Tgif.tmpl	2005-05-12 13:06:46.000000000 +0100
@@ -41,7 +41,7 @@
 XCOMM BINDIR	= /u/halfmoon/bin
 XCOMM MANPATH	= /u/halfmoon/man
 XCOMM TGIFDIR	= /u/halfmoon/lib/X11/tgif
-XCOMM MOREDEFINES	= -D_HAS_STREAMS_SUPPORT -DENABLE_NLS -D_TGIF_DBG
+XCOMM MOREDEFINES	= -D_HAS_STREAMS_SUPPORT -D_TGIF_DBG
 LOCALEDIR	= /usr/share/locale
 DIRDEFINES	= -DTGIF_PATH=\"$(TGIFDIR)\" -DPSFILE_MOD=0664 \@@\
                   -DLOCALEDIR=\"$(LOCALEDIR)\" \@@\
@@ -55,12 +55,12 @@
 MISCDEFINES	= -D_BACKGROUND_DONT_FORK -D_USE_XDRAWPOINT_TO_PUT_A_POINT \@@\
                   -D_USE_PS_ADOBE_STRING=\"3.0/3.0\" -D_DONT_USE_MKTEMP \@@\
 		  -D_DONT_REENCODE=\"FFDingbests:ZapfDingbats\" \@@\
-		  -DPRINT_CMD=\"lpr\" -DUSE_XT_INITIALIZE -D_ENABLE_NLS \@@\
+		  -DPRINT_CMD=\"lpr\" -DUSE_XT_INITIALIZE \@@\
 		  -D_TMP_DIR_IN_HOME_DIR
 XCOMM           -DUSE_XAPPLRESDIR \@@\
 XCOMM           -DXAPPLOADDIR_DEF=\"/usr/X11R6/lib/X11/app-defaults\" \@@\
 
-MOREDEFINES	= -D_XPM3TOPPM
+MOREDEFINES	= -D_XPM3TOPPM -D_NO_DL_SUPPORT
 
 XCOMM ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 XCOMM Please see Imakefile for more defines to be added to MOREDEFINES above
