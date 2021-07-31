--- engine/hexen2/Makefile.orig	2017-04-01 11:44:22.037227162 +0100
+++ engine/hexen2/Makefile	2017-04-01 11:46:16.610000000 +0100
@@ -47,7 +47,7 @@
 # GENERAL OPTIONS (customize as required)
 
 # X directory
-X11BASE    =/usr/X11R6
+X11BASE    =$(GCCSDK_INSTALL_ENV)
 # the sdl-config command
 SDL_CONFIG =sdl-config
 SDL_CFLAGS = $(shell $(SDL_CONFIG) --cflags 2> /dev/null)
@@ -55,7 +55,7 @@
 
 # use fast x86 assembly on ia32 machines? (auto-disabled for
 # any other cpu.)
-USE_X86_ASM=yes
+USE_X86_ASM=no
 
 # link to the opengl libraries at compile time? (defaults
 # to no, so the binaries will dynamically load the necessary
@@ -71,21 +71,21 @@
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
@@ -133,7 +133,7 @@
 USE_CODEC_UMX=no
 # either timidity (preferred) or wildmidi (both possible
 # but not needed nor meaningful)
-USE_CODEC_TIMIDITY=yes
+USE_CODEC_TIMIDITY=no
 USE_CODEC_WILDMIDI=no
 # which library to use for mp3 decoding: mad or mpg123
 MP3LIB=mad
@@ -202,7 +202,7 @@
 endif
 
 CPPFLAGS=
-LDFLAGS =
+LDFLAGS =-static
 # linkage may be sensitive to order: add SYSLIBS after all others.
 SYSLIBS =
 
@@ -239,7 +239,7 @@
 #############################################################
 # OpenGL settings
 #############################################################
-GL_DEFS = -DGLQUAKE
+GL_DEFS =
 GL_LIBS =
 
 #############################################################
