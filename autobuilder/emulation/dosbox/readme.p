--- README.orig	2010-05-10 19:58:07.000000000 +0100
+++ README	2010-05-25 20:45:27.000000000 +0100
@@ -1,5 +1,6 @@
 DOSBox v0.74 Manual (always use the latest version from www.dosbox.com)

+See end of this file for notes specific to RISC OS.


 =====
@@ -35,6 +36,7 @@
 15. Building your own version of DOSBox
 16. Special thanks
 17. Contact
+18. RISC OS notes



@@ -1466,4 +1468,73 @@
 http://www.dosbox.com
 for an email address (The Crew-page).

+=================
+18. RISC OS Notes
+=================
+
+This file is a copy of the README file from within the application
+directory with the addition of these RISC OS version notes.
+
+Ported to RISC OS by Alan Buckley March 2004.
+Version 0.62 ported October 2004.
+Version 0.65 ported November 2006.
+Version 0.71 ported August 2007.
+Version 0.74 ported November 2010
+
+This program is 32 bit compatible and requires RISC OS 5.
+
+It requires 48MB of memory (as it emulates a DOS machine with 16MB
+of ram). Hence the requirement for RISC OS 5.
+
+The SharedUnixLibrary module is requried and should be merged with
+your system modules. It is available from:
+http://www.riscos.info/downloads/gccsdk/sharedunixlib/system.zip
+
+Digital Renderer module must be installed for sound support. It is included
+in the above system.zip file.
+
+The !UnixHome application must have been seen by the filer.
+It is available from:
+http://www.riscos.info/unix/downloads/support/UnixHome-1.03.zip
+
+
+The source code provided here has had minor changes to the original to
+support RISC OS and can be located by searching for __riscos__ in
+the source code.
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

