--- README	2004-11-18 14:12:58.000000000 +0000
+++ Help	2005-01-04 22:16:34.000000000 +0000
@@ -1,5 +1,6 @@
 DOSBox v0.63
 
+See end of this file for notes specific to RISC OS.
 
 =====
 NOTE: 
@@ -33,6 +34,7 @@
 11. Building your own version of DOSBox
 12. Special thanks
 13. Contact
+14. RISC OS notes
 
 
 ==============
@@ -763,3 +765,72 @@
 See the site: 
 http://dosbox.sourceforge.net
 for an emailaddress (The Crew-page).
+
+=================
+14. RISC OS Notes
+=================
+
+This file is a copy of the README file from within the application
+directory with the addition of these RISC OS version notes.
+
+Ported to RISC OS by Alan Buckley March 2004.
+Version 0.62 ported October 2004.
+
+This program is 32 bit compatible and requires RISCOS 3.6+.
+
+It requires 48MB of memory (as it emulates a DOS machine with 16MB
+of ram).
+
+Digital Renderer module (available from http://home.t-online.de/~zarquon)
+may be installed in System:Modules if sound is required. If it is not
+installed DosBox will run without sound.
+
+The !UnixHome application (available from http://www.chocky.org/unix/) must
+have been seen by the filer.
+
+You will also need to have installed the Shared Unix Library 1.0.2 which
+is available from the Unix Porting Project (http://www.chocky.org/unix/) 
+and the GCC web pages.
+
+Part of the Unix Porting Project.  Support for this program under RISC OS
+is only offered to subscribers.  See http://www.chocky.org/unix/
+for more information.
+
+The source code provided here has had minor changes to the original to
+support RISC OS.
+
+
+Directory names for the MOUNT command and command line
+------------------------------------------------------
+
+When using the mount command or specifying a file name on the command
+line you need to use "/" instead of "." as path separators.
+
+e.g.
+
+MOUNT C /ADFS::HardDisk4/$/Misc/PCGames
+
+
+Running a DOS game from a dummy application directory
+------------------------------------------------------
+
+The following is an example of how to set up a RISC OS application
+directory to run the game Arkanoid in the dosbox.
+
+Create a new directory !Ark
+
+Copy the files of the game to this directory.
+Create a !Run obey file with the following contents:
+
+| Run file for Arkanoid
+
+Set Ark$Dir <Obey$Dir>
+Set Ark$Path <Ark$Dir>.
+<DosBox$Dir>.!Run "Ark:/Arkanoid.com"
+
+This will mount the directory in as C in Dosbox and then run
+Arkanoid.com.
+
+See the other command line options in the main notes above.
+
+The DosBox application must have been seen by the filer.
