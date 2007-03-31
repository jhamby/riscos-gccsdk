*** src/Makefile.in.old	2007-03-26 12:22:49.000000000 +0100
--- src/Makefile.in	2007-03-26 12:22:36.000000000 +0100
***************
*** 188,194 ****
  sharedstatedir = @sharedstatedir@
  sysconfdir = @sysconfdir@
  target_alias = @target_alias@
! SUBDIRS = cppunit DllPlugInTester
  all: all-recursive
  
  .SUFFIXES:
--- 188,194 ----
  sharedstatedir = @sharedstatedir@
  sysconfdir = @sysconfdir@
  target_alias = @target_alias@
! SUBDIRS = cppunit # DllPlugInTester
  all: all-recursive
  
  .SUFFIXES:
