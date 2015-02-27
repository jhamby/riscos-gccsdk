--- RiscPkg/Control.orig	2015-02-25 12:40:02.822761000 +0000
+++ RiscPkg/Control	2015-02-25 12:50:01.722051213 +0000
@@ -1,11 +1,12 @@
 Source: FixDeps
 Priority: Optional
-Section: Programming
+Section: Development
 Maintainer: Graham Shaw <gdshaw@sagitta.demon.co.uk>
-Version: 1.0.2-0
+Version: 1.0.2-ab-1
 Description: A filter for processing makefile dependencies
  !FixDeps is a filter for converting source code dependencies from
  the form produced by GCC to a form usable by GNU Make when using
  RISC OS as the host operating system.
 
 Package: FixDeps
+Components: Apps.Development.!FixDeps (Movable LookAt)
