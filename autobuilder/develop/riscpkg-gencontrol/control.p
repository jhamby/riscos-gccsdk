--- RiscPkg/Control.orig	2015-02-03 12:55:29.663705000 +0000
+++ RiscPkg/Control	2015-02-06 13:15:02.634322456 +0000
@@ -1,11 +1,13 @@
 Source: RiscPkg-GenControl
-Section: Programming
+Section: Development
 Priority: Standard
 Licence: Free
 Maintainer: Graham Shaw <gdshaw@riscpkg.org>
-Standards-Version: 0.0.0
-Version: 0.1.0
+Standards-Version: 0.4.0
+Version: 0.1.0-ab-1
 Build-Depends: LibPkg
 Description: A tool for generating RiscPkg binary control files.
 
 Package: RiscPkg-GenControl
+Components: Apps.Development.!RiscPkg-GenControl (Movable LookAt)
+
