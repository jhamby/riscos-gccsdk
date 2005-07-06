diff -ur Makefile Makefile
--- Makefile	2004-05-28 09:58:32.000000000 +0100
+++ Makefile	2005-07-06 09:45:37.000000000 +0100
@@ -14,8 +14,8 @@
 # MAKE = make

 # make NetHack
-PREFIX	 = /usr
-GAME     = glhack
+PREFIX	 = /home/riscos/usr
+GAME     = glhack,ff8
 # GAME     = glhack.prg
 GAMEUID  = root
 GAMEGRP  = games
@@ -38,7 +38,7 @@
 # (if there is, you'll have to do the installation by hand or modify the
 # instructions)
 GAMEDIR  = $(PREFIX)/lib/games/$(GAME)
-VARDIR   = /var/lib/games/glhack
+VARDIR   = /home/riscos/var/lib/games/glhack
 SHELLDIR = $(PREFIX)/bin
 MANDIR   = $(PREFIX)/share/man
 
@@ -208,28 +208,29 @@
 		< win/gl/glhack.sh \
 		> $(SHELLDIR)/$(GAME)
 # set up their permissions
-	-( cd $(GAMEDIR) ; $(CHOWN) $(GAMEUID) $(GAME) recover_glhack ; \
-			$(CHGRP) $(GAMEGRP) $(GAME) recover_glhack )
-	chmod $(GAMEPERM) $(GAMEDIR)/$(GAME)
-	chmod $(EXEPERM) $(GAMEDIR)/recover_glhack
-	-$(CHOWN) $(GAMEUID) $(SHELLDIR)/$(GAME)
-	$(CHGRP) $(GAMEGRP) $(SHELLDIR)/$(GAME)
-	chmod $(EXEPERM) $(SHELLDIR)/$(GAME)
+#	-( cd $(GAMEDIR) ; $(CHOWN) $(GAMEUID) $(GAME) recover_glhack ; 
+#\
+#			$(CHGRP) $(GAMEGRP) $(GAME) recover_glhack )
+#	chmod $(GAMEPERM) $(GAMEDIR)/$(GAME)
+#	chmod $(EXEPERM) $(GAMEDIR)/recover_glhack
+#	-$(CHOWN) $(GAMEUID) $(SHELLDIR)/$(GAME)
+#	$(CHGRP) $(GAMEGRP) $(SHELLDIR)/$(GAME)
+#	chmod $(EXEPERM) $(SHELLDIR)/$(GAME)
 
 dofiles-dlb: check-dlb
 	( cd dat ; cp nhdat $(DATNODLB) $(GAMEDIR) )
 # set up their permissions
-	-( cd $(GAMEDIR) ; $(CHOWN) $(GAMEUID) nhdat $(DATNODLB) ; \
-			$(CHGRP) $(GAMEGRP) nhdat $(DATNODLB) ; \
-			chmod $(FILEPERM) nhdat $(DATNODLB) )
+#	-( cd $(GAMEDIR) ; $(CHOWN) $(GAMEUID) nhdat $(DATNODLB) ; \
+#			$(CHGRP) $(GAMEGRP) nhdat $(DATNODLB) ; \
+#			chmod $(FILEPERM) nhdat $(DATNODLB) )
 
 dofiles-nodlb:
 # copy over the game files
 	( cd dat ; cp $(DAT) $(GAMEDIR) )
 # set up their permissions
-	-( cd $(GAMEDIR) ; $(CHOWN) $(GAMEUID) $(DAT) ; \
-			$(CHGRP) $(GAMEGRP) $(DAT) ; \
-			chmod $(FILEPERM) $(DAT) )
+#	-( cd $(GAMEDIR) ; $(CHOWN) $(GAMEUID) $(DAT) ; \
+#			$(CHGRP) $(GAMEGRP) $(DAT) ; \
+#			chmod $(FILEPERM) $(DAT) )
 
 update: $(GAME) recover_glhack $(VARDAT) dungeon spec_levs
 #	(don't yank the old version out from under people who're playing it)
@@ -256,16 +257,16 @@
 	-rm -rf $(GAMEDIR) $(VARDIR)
 	-mkdir -p $(GAMEDIR) $(VARDIR) $(VARDIR)/save
 	-rmdir ./-p
-	-$(CHOWN) $(GAMEUID) $(GAMEDIR) $(VARDIR) $(VARDIR)/save
-	$(CHGRP) $(GAMEGRP) $(GAMEDIR) $(VARDIR) $(VARDIR)/save
-	chmod $(VARDIRPERM) $(GAMEDIR) $(VARDIR) $(VARDIR)/save
+#	-$(CHOWN) $(GAMEUID) $(GAMEDIR) $(VARDIR) $(VARDIR)/save
+#	$(CHGRP) $(GAMEGRP) $(GAMEDIR) $(VARDIR) $(VARDIR)/save
+#	chmod $(VARDIRPERM) $(GAMEDIR) $(VARDIR) $(VARDIR)/save
 # set up the game files
 	( $(MAKE) dofiles )
 # set up some additional files
 	touch $(VARDIR)/perm $(VARDIR)/record $(VARDIR)/logfile
-	-( cd $(VARDIR) ; $(CHOWN) $(GAMEUID) perm record logfile ; \
-			$(CHGRP) $(GAMEGRP) perm record logfile ; \
-			chmod $(VARFILEPERM) perm record logfile )
+#	-( cd $(VARDIR) ; $(CHOWN) $(GAMEUID) perm record logfile ; \
+#			$(CHGRP) $(GAMEGRP) perm record logfile ; \
+#			chmod $(VARFILEPERM) perm record logfile )
 # do the documentation [AJA]
 	( $(MAKE) manpages )
 # and a reminder
Only in dat: Arc-fila.lev
Only in dat: Arc-filb.lev
Only in dat: Arc-goal.lev
Only in dat: Arc-loca.lev
Only in dat: Arc-strt.lev
Only in dat: Bar-fila.lev
Only in dat: Bar-filb.lev
Only in dat: Bar-goal.lev
Only in dat: Bar-loca.lev
Only in dat: Bar-strt.lev
Only in dat: Cav-fila.lev
Only in dat: Cav-filb.lev
Only in dat: Cav-goal.lev
Only in dat: Cav-loca.lev
Only in dat: Cav-strt.lev
Only in dat: Hea-fila.lev
Only in dat: Hea-filb.lev
Only in dat: Hea-goal.lev
Only in dat: Hea-loca.lev
Only in dat: Hea-strt.lev
Only in dat: Kni-fila.lev
Only in dat: Kni-filb.lev
Only in dat: Kni-goal.lev
Only in dat: Kni-loca.lev
Only in dat: Kni-strt.lev
Only in dat: Mon-fila.lev
Only in dat: Mon-filb.lev
Only in dat: Mon-goal.lev
Only in dat: Mon-loca.lev
Only in dat: Mon-strt.lev
Only in dat: Pri-fila.lev
Only in dat: Pri-filb.lev
Only in dat: Pri-goal.lev
Only in dat: Pri-loca.lev
Only in dat: Pri-strt.lev
Only in dat: Ran-fila.lev
Only in dat: Ran-filb.lev
Only in dat: Ran-goal.lev
Only in dat: Ran-loca.lev
Only in dat: Ran-strt.lev
Only in dat: Rog-fila.lev
Only in dat: Rog-filb.lev
Only in dat: Rog-goal.lev
Only in dat: Rog-loca.lev
Only in dat: Rog-strt.lev
Only in dat: Sam-fila.lev
Only in dat: Sam-filb.lev
Only in dat: Sam-goal.lev
Only in dat: Sam-loca.lev
Only in dat: Sam-strt.lev
Only in dat: Tou-fila.lev
Only in dat: Tou-filb.lev
Only in dat: Tou-goal.lev
Only in dat: Tou-loca.lev
Only in dat: Tou-strt.lev
Only in dat: Val-fila.lev
Only in dat: Val-filb.lev
Only in dat: Val-goal.lev
Only in dat: Val-loca.lev
Only in dat: Val-strt.lev
Only in dat: Wiz-fila.lev
Only in dat: Wiz-filb.lev
Only in dat: Wiz-goal.lev
Only in dat: Wiz-loca.lev
Only in dat: Wiz-strt.lev
Only in dat: air.lev
Only in dat: asmodeus.lev
Only in dat: astral.lev
Only in dat: baalz.lev
Only in dat: bigrm-1.lev
Only in dat: bigrm-2.lev
Only in dat: bigrm-3.lev
Only in dat: bigrm-4.lev
Only in dat: bigrm-5.lev
Only in dat: castle.lev
Only in dat: data
Only in dat: dungeon
Only in dat: dungeon.pdf
Only in dat: earth.lev
Only in dat: fakewiz1.lev
Only in dat: fakewiz2.lev
Only in dat: fire.lev
Only in dat: juiblex.lev
Only in dat: knox.lev
Only in dat: medusa-1.lev
Only in dat: medusa-2.lev
Only in dat: minefill.lev
Only in dat: minend-1.lev
Only in dat: minend-2.lev
Only in dat: minend-3.lev
Only in dat: minetn-1.lev
Only in dat: minetn-2.lev
Only in dat: minetn-3.lev
Only in dat: minetn-4.lev
Only in dat: minetn-5.lev
Only in dat: minetn-6.lev
Only in dat: minetn-7.lev
Only in dat: nhdat
Only in dat: options
Only in dat: oracle.lev
Only in dat: oracles
Only in dat: orcus.lev
Only in dat: quest.dat
Only in dat: quest_levs
Only in dat: rumors
Only in dat: sanctum.lev
Only in dat: soko1-1.lev
Only in dat: soko1-2.lev
Only in dat: soko2-1.lev
Only in dat: soko2-2.lev
Only in dat: soko3-1.lev
Only in dat: soko3-2.lev
Only in dat: soko4-1.lev
Only in dat: soko4-2.lev
Only in dat: spec_levs
Only in dat: tower1.lev
Only in dat: tower2.lev
Only in dat: tower3.lev
Only in dat: valley.lev
Only in dat: water.lev
Only in dat: wizard1.lev
Only in dat: wizard2.lev
Only in dat: wizard3.lev
Only in doc: Guidebook
diff -ur doc/Makefile doc/Makefile
--- doc/Makefile	2002-06-28 07:57:30.000000000 +0100
+++ doc/Makefile	2005-07-06 09:45:37.000000000 +0100
@@ -38,7 +38,7 @@
 
 
 GAME	= nethack
-MANDIR	= /usr/man/man6
+MANDIR	= /home/riscos/usr/man/man6
 MANEXT	= 6
 
 # manual installation for most BSD-style systems
diff -ur include/config.h include/config.h
--- include/config.h	2004-05-28 11:06:59.000000000 +0100
+++ include/config.h	2005-07-06 09:45:37.000000000 +0100
@@ -48,7 +48,7 @@
 /* #define GNOME_GRAPHICS */	/* Gnome interface */
 /* #define MSWIN_GRAPHICS */	/* Windows NT, CE, Graphics */
 #define SDL_GRAPHICS	/* Software SDL graphics (glHack) */
-#define GL_GRAPHICS	/* OpenGL graphics (glHack) */
+/*#define GL_GRAPHICS */	/* OpenGL graphics (glHack) */
 /* #define GL3D_GRAPHICS */	/* OpenGL real 3D renderer (glHack) */
 
 #define VANILLA_GLHACK	/* Differentiates glHack from Slash'EM */
@@ -241,7 +241,7 @@
  * otherwise it will be the current directory.
  */
 # ifndef HACKDIR
-#  define HACKDIR "/usr/lib/games/glhack"
+#  define HACKDIR "<GLHack$Dir>/usr"
 # endif
 
 /*
Only in include: date.h
Only in include: dgn_comp.h
Only in include: lev_comp.h
Only in include: onames.h
Only in include: pm.h
diff -ur include/unixconf.h include/unixconf.h
--- include/unixconf.h	2004-05-28 09:00:02.000000000 +0100
+++ include/unixconf.h	2005-07-06 09:45:37.000000000 +0100
@@ -95,14 +95,14 @@
  *		Ralf Brown, 7/26/89 (from v2.3 hack of 10/10/88)
  */
 
-/* #define NO_FILE_LINKS */	/* if no hard links */
-/* #define LOCKDIR "/usr/games/lib/nethackdir" */	/* where to put locks */
+#define NO_FILE_LINKS 	/* if no hard links */
+#define LOCKDIR "<GLHack$Dir>/locks"	/* where to put locks */
 
 /*
  * If you want the static parts of your playground on a read-only file
  * system, define VAR_PLAYGROUND to be where the variable parts are kept.
  */
-#define VAR_PLAYGROUND "/var/lib/games/glhack"
+#define VAR_PLAYGROUND "<GLHack$Dir>/var"
 
 
 /*
Only in include: vis_tab.h
diff -ur src/Makefile src/Makefile
--- src/Makefile	2004-05-28 09:00:02.000000000 +0100
+++ src/Makefile	2005-07-06 10:08:18.000000000 +0100
@@ -2,7 +2,7 @@
 #	SCCS Id: @(#)Makefile.src	3.4	2002/03/02
 
 CC = gcc
-LFLAGS = -L/usr/X11R6/lib
+LFLAGS = -L/home/riscos/env/lib
 WINSRC = $(WINTTYSRC) $(WINX11SRC)
 WINOBJ = $(WINTTYOBJ) $(WINX11OBJ)
 WINTTYLIB = /usr/lib/libncurses.a
@@ -159,7 +159,7 @@
 # flags for debugging:
 # CFLAGS = -g -I../include
 
-CFLAGS = -O -I../include
+CFLAGS = -O2 -I../include -I/home/riscos/env/include
 LFLAGS = 
 
 # The Qt and Be window systems are written in C++, while the rest of
@@ -281,7 +281,7 @@
 WINBELIB = -lbe
 #
 # libraries for GL (SDL)
-WINGLLIB = $(shell sdl-config --libs) -L/usr/X11R6/lib -lGL -lz -lpng
+WINGLLIB = $(shell sdl-config --libs) -lz -lpng
  
 # ---->
 #
@@ -485,10 +485,12 @@
 # below in the 'make depend' output.
 monst.o:
 	$(CC) $(CFLAGS) -c monst.c
+	/usr/bin/gcc $(CFLAGS) -c -o monstnative.o monst.c
 	@rm -f $(MAKEDEFS)
 
 objects.o:
 	$(CC) $(CFLAGS) -c objects.c
+	/usr/bin/gcc $(CFLAGS) -c -o objectsnative.o objects.c
 	@rm -f $(MAKEDEFS)
 
 # Qt windowport meta-object-compiler output
Only in src: Sysunix
Only in src: allmain.o
Only in src: alloc.o
Only in src: allocnative.o
Only in src: apply.o
Only in src: artifact.o
Only in src: attrib.o
Only in src: ball.o
Only in src: bones.o
Only in src: botl.o
Only in src: cmd.o
Only in src: config.h-t
Only in src: dbridge.o
Only in src: decl.o
Only in src: declnative.o
Only in src: detect.o
Only in src: dig.o
Only in src: display.o
Only in src: dlb.o
Only in src: dlbnative.o
Only in src: do.o
Only in src: do_name.o
Only in src: do_wear.o
Only in src: dog.o
Only in src: dogmove.o
Only in src: dokick.o
Only in src: dothrow.o
Only in src: drawing.o
Only in src: drawingnative.o
Only in src: dungeon.o
Only in src: eat.o
Only in src: end.o
Only in src: engrave.o
Only in src: exper.o
Only in src: explode.o
Only in src: extralev.o
diff -ur src/files.c src/files.c
--- src/files.c	2004-05-28 09:00:03.000000000 +0100
+++ src/files.c	2005-07-06 09:45:37.000000000 +0100
@@ -1424,11 +1424,11 @@
 #endif
 
 #if defined(UNIX) || defined(VMS)
-		if (unlink(lockname) < 0)
-			HUP raw_printf("Can't unlink %s.", lockname);
 # ifdef NO_FILE_LINKS
 		(void) close(lockfd);
 # endif
+		if (unlink(lockname) < 0)
+			HUP raw_printf("Can't unlink %s.", lockname);
 
 #endif  /* UNIX || VMS */
 
Only in src: files.o
Only in src: fountain.o
Only in src: gl_3d.o
Only in src: gl_conf.o
Only in src: gl_emul.o
Only in src: gl_font.o
Only in src: gl_hardw.o
Only in src: gl_image.o
Only in src: gl_main.o
Only in src: gl_map.o
Only in src: gl_menu.o
Only in src: gl_opt.o
Only in src: gl_rendu.o
Only in src: gl_role.o
Only in src: gl_softw.o
Only in src: gl_stat.o
Only in src: gl_text.o
Only in src: gl_tile.o
Only in src: gl_unit.o
Only in src: gl_win.o
Only in src: glhack,ff8
Only in src: hack.h-t
Only in src: hack.o
diff -ur src/hacklib.c src/hacklib.c
--- src/hacklib.c	2003-12-07 23:39:13.000000000 +0000
+++ src/hacklib.c	2005-07-06 09:45:37.000000000 +0100
@@ -5,6 +5,8 @@
 
 /* We could include only config.h, except for the overlay definitions... */
 #include "hack.h"
+#include <sys/time.h>
+#include <unixlib/time.h>
 /*=
     Assorted 'small' utility routines.	They're virtually independent of
 NetHack, except that rounddiv may call panic().
Only in src: hacklib.o
Only in src: invent.o
Only in src: ioctl.o
Only in src: light.o
Only in src: lock.o
Only in src: mail.o
Only in src: makemon.o
Only in src: mapglyph.o
Only in src: mcastu.o
Only in src: mhitm.o
Only in src: mhitu.o
Only in src: minion.o
Only in src: mklev.o
Only in src: mkmap.o
Only in src: mkmaze.o
Only in src: mkobj.o
Only in src: mkroom.o
Only in src: mon.o
Only in src: mondata.o
Only in src: monmove.o
Only in src: monst.o
Only in src: monstnative.o
Only in src: monstr.c
Only in src: monstr.o
Only in src: mplayer.o
Only in src: mthrowu.o
Only in src: muse.o
Only in src: music.o
Only in src: o_init.o
Only in src: objects.o
Only in src: objectsnative.o
Only in src: objnam.o
Only in src: options.o
Only in src: pager.o
Only in src: pickup.o
Only in src: pline.o
Only in src: polyself.o
Only in src: potion.o
Only in src: pray.o
Only in src: priest.o
Only in src: quest.o
Only in src: questpgr.o
Only in src: read.o
Only in src: rect.o
Only in src: region.o
Only in src: restore.o
Only in src: rip.o
Only in src: rnd.o
Only in src: role.o
Only in src: rumors.o
Only in src: save.o
Only in src: shk.o
Only in src: shknam.o
Only in src: sit.o
Only in src: sounds.o
Only in src: sp_lev.o
Only in src: spell.o
Only in src: steal.o
Only in src: steed.o
Only in src: teleport.o
Only in src: tile.c
Only in src: tile.o
Only in src: timeout.o
Only in src: topten.o
Only in src: track.o
Only in src: trap.o
Only in src: u_init.o
Only in src: uhitm.o
Only in src: unixmain.o
Only in src: unixres.o
Only in src: unixtty.o
Only in src: unixunix.o
Only in src: vault.o
Only in src: version.o
Only in src: vis_tab.c
Only in src: vis_tab.o
Only in src: vision.o
Only in src: weapon.o
Only in src: were.o
Only in src: wield.o
Only in src: windows.o
Only in src: wizard.o
Only in src: worm.o
Only in src: worn.o
Only in src: write.o
Only in src: zap.o
diff -ur sys/share/unixtty.c sys/share/unixtty.c
--- sys/share/unixtty.c	2003-12-07 23:39:13.000000000 +0000
+++ sys/share/unixtty.c	2005-07-06 10:14:31.000000000 +0100
@@ -38,7 +38,7 @@
 # ifdef LINUX
 #  include <sys/ioctl.h>
 #  undef delay_output	/* curses redefines this */
-#  include <curses.h>
+#  include <ncurses.h>
 # endif
 # define kill_sym	c_cc[VKILL]
 # define erase_sym	c_cc[VERASE]
diff -ur util/Makefile util/Makefile
--- util/Makefile	2004-05-28 09:00:06.000000000 +0100
+++ util/Makefile	2005-07-06 09:45:37.000000000 +0100
@@ -1,7 +1,7 @@
 #	Makefile for NetHack's utility programs.
 #	SCCS Id: @(#)Makefile.utl	3.4	1997/04/19
 
-CC = gcc
+CC = /usr/bin/gcc
 LFLAGS = -L/usr/X11R6/lib
 YACC = bison -y
 LEX = flex
@@ -141,13 +141,13 @@
 
 # files that define all monsters and objects
 CMONOBJ = ../src/monst.c ../src/objects.c
-OMONOBJ = $(OBJDIR)/monst.o $(OBJDIR)/objects.o
+OMONOBJ = $(OBJDIR)/monstnative.o $(OBJDIR)/objectsnative.o
 # files that provide access to NetHack's names
 CNAMING = ../src/drawing.c ../src/decl.c $(CMONOBJ)
-ONAMING = $(OBJDIR)/drawing.o $(OBJDIR)/decl.o $(OMONOBJ)
+ONAMING = $(OBJDIR)/drawingnative.o $(OBJDIR)/declnative.o $(OMONOBJ)
 # dynamic memory allocation
 CALLOC = ../src/alloc.c panic.c
-OALLOC = $(OBJDIR)/alloc.o panic.o
+OALLOC = $(OBJDIR)/allocnative.o panic.o
 
 # object files for makedefs
 MAKEOBJS = makedefs.o $(OMONOBJ)
@@ -162,7 +162,7 @@
 RECOVOBJS = recover.o
 
 # object files for the data librarian
-DLBOBJS = dlb_main.o $(OBJDIR)/dlb.o $(OALLOC)
+DLBOBJS = dlb_main.o $(OBJDIR)/dlbnative.o $(OALLOC)
 
 # flags for creating distribution versions of sys/share/*_lex.c, using
 # a more portable flex skeleton, which is not included in the distribution.
@@ -371,17 +371,17 @@
 
 # make sure object files from src are available when needed
 #
-$(OBJDIR)/alloc.o: ../src/alloc.c $(CONFIG_H)
+$(OBJDIR)/allocnative.o: ../src/alloc.c $(CONFIG_H)
 	$(CC) $(CFLAGS) -c ../src/alloc.c -o $@
-$(OBJDIR)/drawing.o: ../src/drawing.c $(CONFIG_H)
+$(OBJDIR)/drawingnative.o: ../src/drawing.c $(CONFIG_H)
 	$(CC) $(CFLAGS) -c ../src/drawing.c -o $@
-$(OBJDIR)/decl.o: ../src/decl.c $(CONFIG_H)
+$(OBJDIR)/declnative.o: ../src/decl.c $(CONFIG_H)
 	$(CC) $(CFLAGS) -c ../src/decl.c -o $@
-$(OBJDIR)/monst.o: ../src/monst.c $(CONFIG_H)
+$(OBJDIR)/monstnative.o: ../src/monst.c $(CONFIG_H)
 	$(CC) $(CFLAGS) -c ../src/monst.c -o $@
-$(OBJDIR)/objects.o: ../src/objects.c $(CONFIG_H)
+$(OBJDIR)/objectsnative.o: ../src/objects.c $(CONFIG_H)
 	$(CC) $(CFLAGS) -c ../src/objects.c -o $@
-$(OBJDIR)/dlb.o: ../src/dlb.c $(HACK_H) ../include/dlb.h
+$(OBJDIR)/dlbnative.o: ../src/dlb.c $(HACK_H) ../include/dlb.h
 	$(CC) $(CFLAGS) -c ../src/dlb.c -o $@
 
 # make sure hack.h dependencies get transitive information
Only in util: dgn_comp
Only in util: dgn_lex.c
Only in util: dgn_lex.o
Only in util: dgn_main.o
Only in util: dgn_yacc.c
Only in util: dgn_yacc.o
Only in util: dlb
Only in util: dlb_main.o
Only in util: lev_comp
Only in util: lev_lex.c
Only in util: lev_lex.o
Only in util: lev_main.o
Only in util: lev_yacc.c
Only in util: lev_yacc.o
Only in util: makedefs
Only in util: makedefs.o
Only in util: panic.o
Only in util: recover.o
Only in util: recover_glhack
Only in util: tilemap
