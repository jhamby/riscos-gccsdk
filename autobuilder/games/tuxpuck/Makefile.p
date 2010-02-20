--- Makefile.orig	2010-02-20 19:31:25.000000000 +0000
+++ Makefile	2010-02-20 19:31:01.000000000 +0000
@@ -1,7 +1,7 @@
 # Makefile for TuxPuck , Copyright Jacob Kroon 2001-2002
 NAME		= tuxpuck
 VERSION		= 0.8.2
-CC		= gcc
+CC		= gcc -static
 CFLAGS		+= -D_VERSION=\"$(VERSION)\" -g -Wall
 CSOURCES	= tuxpuck.c video.c audio.c menu.c sprite.c font.c timer.c \
 		  board.c entity.c glass.c scoreboard.c player.c zoom.c png.c \
