--- src/feature.h.orig	2021-03-13 14:08:02.379762639 +0000
+++ src/feature.h	2021-03-13 14:12:14.604795176 +0000
@@ -869,12 +869,12 @@
 /*
  * VIM_DEFAULTS_FILE	Name of the defaults.vim script file
  */
-/* #define VIM_DEFAULTS_FILE	"$VIMRUNTIME/defaults.vim" */
+#define VIM_DEFAULTS_FILE	"/<Vim$Dir>/runtime/defaults.vim"
 
 /*
  * EVIM_FILE		Name of the evim.vim script file
  */
-/* #define EVIM_FILE		"$VIMRUNTIME/evim.vim" */
+#define EVIM_FILE		"/<Vim$Dir>/runtime/evim.vim"
 
 /*
  * USR_EXRC_FILE	Name of the user .exrc file.
@@ -904,7 +904,7 @@
 /*
  * DFLT_HELPFILE	Name of the help file.
  */
-/* # define DFLT_HELPFILE	"$VIMRUNTIME/doc/help.txt.gz" */
+# define DFLT_HELPFILE	"/<Vim$Dir>/runtime/doc/help.txt.gz"
 
 /*
  * File names for:
@@ -925,13 +925,13 @@
 /*
  * SYS_MENU_FILE	Name of the default menu.vim file.
  */
-/* # define SYS_MENU_FILE	"$VIMRUNTIME/menu.vim" */
+# define SYS_MENU_FILE	"/<Vim$Dir>/runtime/menu.vim"
 
 /*
  * SYS_OPTWIN_FILE	Name of the default optwin.vim file.
  */
 #ifndef SYS_OPTWIN_FILE
-# define SYS_OPTWIN_FILE	"$VIMRUNTIME/optwin.vim"
+# define SYS_OPTWIN_FILE	"/<Vim$Dir>runtime/optwin.vim"
 #endif
 
 /*
