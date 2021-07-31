--- README.orig	2014-07-13 23:09:52.000000000 +0100
+++ README	2015-05-04 11:50:08.868713697 +0100
@@ -58,6 +58,21 @@
 files (man/*.x) are welcome.  However, the authoritative documentation
 is in texinfo form in the doc directory.
 
+*****************************************
+RISC OS Notes
+*****************************************
+The RISC OS port was compiled by the GCCSDK autobuilder and does not
+implement the cat or dir commands because they clash with RISC OS
+operating system commands.
+stdbuf was also omitted as it couldn't be made to compile.
+
+The programs are compiled with UnixLib so use Linux style filenames.
+
+See:
+http://www.riscos.info/index.php/RISC_OS_Filename_Translation#File_handling_in_RISC_OS_ports_and_source_files
+
+Most of the commands here respond to the "--help" argument to give help on
+their usage.
 
 *****************************************
 On Mac OS X 10.5.1 (Darwin 9.1), test failure
