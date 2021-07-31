--- Makefile.orig	2021-03-07 11:02:32.243502356 +0000
+++ Makefile	2021-03-07 11:22:48.015168318 +0000
@@ -14,8 +14,8 @@
 # MAKE = make
 
 # make NetHack
-PREFIX	 = $(DESTDIR)/usr
-GAME     = glhack
+PREFIX	 = $GCCSDK_INSTALL_ENV
+GAME     = glhack,ff8
 # GAME     = glhack.prg
 GAMEUID  = root
 GAMEGRP  = games
@@ -38,7 +38,7 @@
 # (if there is, you'll have to do the installation by hand or modify the
 # instructions)
 GAMEDIR  = $(PREFIX)/lib/games/$(GAME)
-VARDIR   = $(DESTDIR)/var/games/$(GAME)
+VARDIR   = $GCCSDK_INSTALL_ENV/var/games/$(GAME)
 SHELLDIR = $(PREFIX)/games
 MANDIR   = $(PREFIX)/share/man
 
@@ -207,28 +207,29 @@
 	cp win/gl/glhack.sh $(SHELLDIR)/$(GAME)
 
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
@@ -255,16 +256,16 @@
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
