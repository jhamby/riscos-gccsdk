--- Makefile.orig	2004-06-24 18:25:50.000000000 +0100
+++ Makefile	2004-06-24 19:29:30.000000000 +0100
@@ -8,7 +8,8 @@
 endif
 
 ifndef datadir
-  datadir=$(prefix)/share
+  #datadir=$(prefix)/share
+  datadir=\<IceBreaker\$$Dir\>/data
 endif
 
 ifndef mandir
@@ -16,15 +17,16 @@
 endif
 
 ifndef highscoredir
-  highscoredir=/var/local/lib/games
+  #highscoredir=/var/local/lib/games
   #highscoredir=/var/lib/games
+  highscoredir=\<IceBreaker$$Dir\>/data
 endif
 
 ifndef OPTIMIZE
   OPTIMIZE=-O3
 endif
 
-CFLAGS=-Wall $(OPTIMIZE) $(SDL_CFLAGS) -DDATAPREFIX=\"$(datadir)/icebreaker\" -DHISCOREPREFIX=\"$(highscoredir)\"
+CFLAGS=-Wall $(OPTIMIZE) $(SDL_CFLAGS) -DDATAPREFIX=\"$(datadir)\" -DHISCOREPREFIX=\"$(highscoredir)\"
 
 SRC=icebreaker.c cursor.c grid.c laundry.c line.c penguin.c sound.c \
     level.c intro.c text.c status.c transition.c hiscore.c dialog.c \
