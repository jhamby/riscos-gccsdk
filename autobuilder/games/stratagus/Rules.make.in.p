--- Rules.make.in.orig	2005-04-20 12:40:30.796875000 +0100
+++ Rules.make.in	2005-04-21 12:01:45.640625000 +0100
@@ -29,13 +29,13 @@
 COMP_CFLAGS		= -DUSE_ZLIB @BZ2_CFLAGS@
 COMP_LIBS		= -lz @BZ2_LIBS@
 
-TOOLLIBS=$(XLDFLAGS) -lpng -lz -lm @EXTRA_LIBS@ @THREAD_LIBS@ -L/usr/local/lib \
+TOOLLIBS=$(XLDFLAGS) -lpng -lz -lm @EXTRA_LIBS@ @THREAD_LIBS@ -L/home/riscos/env/lib \
 	@STATIC_LDFLAGS@
 
 STRATAGUS_LIBS= -lpng -lz -lm \
 	@EXTRA_LIBS@ @VIDEO_LIBS@ @THREAD_LIBS@ $(CCLLIB) $(COMP_LIBS) \
 	@FLAC_LIBS@ @OGG_LIBS@ @MAD_LIBS@ @MIKMOD_LIBS@ @CDAUDIO_LIBS@ \
-	@LUA_LIBS@ @STATIC_LDFLAGS@ -lz -lm -L/usr/local/lib
+	@LUA_LIBS@ @STATIC_LDFLAGS@ -lz -lm -L/home/riscos/env/lib
 
 DISTLIST=$(TOPDIR)/distlist
 TAGS=$(TOPDIR)/src/tags
@@ -55,7 +55,7 @@
     @OGG_CFLAGS@ @MAD_CFLAGS@ @FLAC_CFLAGS@ @CDAUDIO_CFLAGS@ \
     @MIKMOD_CFLAGS@ @LUA_CFLAGS@ $(CCL) \
     $(COMP_CFLAGS) @PLATFORM@ \
-    -I/usr/local/include
+    -I/home/riscos/env/include
 
 CTAGSFLAGS=-i defptvS -a -f 
 
