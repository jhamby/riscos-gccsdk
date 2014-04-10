--- engine/hexen2/Makefile.orig	2012-06-20 14:03:05.000000000 +0100
+++ engine/hexen2/Makefile	2012-06-20 14:10:38.000000000 +0100
@@ -43,7 +43,7 @@
 # GENERAL OPTIONS (customize as required)

 # X directory
-X11BASE    =/usr/X11R6
+X11BASE    =$(GCCSDK_INSTALL_ENV)
 # the sdl-config command
 SDL_CONFIG?=sdl-config
 SDL_CFLAGS = $(shell $(SDL_CONFIG) --cflags 2> /dev/null)
@@ -54,7 +54,7 @@

 # use fast x86 assembly on ia32 machines? (auto-disabled for
 # any other cpu.)
-USE_X86_ASM=yes
+USE_X86_ASM=no

 # link to the opengl libraries at compile time? (defaults
 # to no, so the binaries will dynamically load the necessary
@@ -70,21 +70,21 @@
 # ALSA audio support? (req: alsa-lib and alsa-kernel modules
 # >= 1.0.1.  v0.9.8 and v1.0.0 might work, but not supported.
 # If not Linux, ALSA will be automatically be disabled.)
-USE_ALSA=yes
+USE_ALSA=no
 # OSS audio support? (for Unix. enabled on Linux and FreeBSD.
 # automatically disabled on other platforms: see snd_sys.h)
 USE_OSS=yes
 # SUN audio support? (enabled on OpenBSD, NetBSD and SUN.
 # automatically disabled on others: see snd_sys.h)
-USE_SUNAUDIO=yes
+USE_SUNAUDIO=no
 # SDL audio support? (enabled on all unix-like platforms.)
 USE_SDLAUDIO=yes

 # include target's MIDI driver if available?
-USE_MIDI=yes
+USE_MIDI=no

 # CDAudio support?
-USE_CDAUDIO=yes
+USE_CDAUDIO=no
 # use SDL cdaudio? (otherwise platform specific cdrom code will
 # be used.  The only problem with SDL_cdrom is that it lacks
 # proper volume controls.  See cd_unix.h for the availability of
@@ -104,7 +104,7 @@
 # use SciTech MGL for Win32 software video driver? (Default to yes
 # for x86, auto-disabled for Win64.  if disabled, the DIB/GDI-only
 # compatibility software video driver will be used.)
-USE_MGL=yes
+USE_MGL=no

 # use WinSock2 instead of WinSock-1.1? (disabled for w32 for compat.
 # with old Win95 machines.) (enabled for Win64 in the win64 section.)
@@ -125,7 +125,7 @@
 USE_CODEC_VORBIS=yes
 # either timidity or wildmidi (both possible
 # but not needed nor meaningful)
-USE_CODEC_TIMIDITY=yes
+USE_CODEC_TIMIDITY=no
 USE_CODEC_WILDMIDI=no
 # compile timidity with DLS instruments support? (no:
 # the dls code isn't good enough and isn't used in unix
@@ -185,7 +185,7 @@
 endif

 CPPFLAGS=
-LDFLAGS =
+LDFLAGS =-static

 # compiler includes
 INCLUDES= -I. -I$(COMMONDIR) -I$(LIBS_DIR)/common
@@ -233,7 +233,7 @@
 #############################################################
 # OpenGL settings
 #############################################################
-GL_DEFS = -DGLQUAKE
+GL_DEFS =
 GL_LIBS =

 #############################################################
