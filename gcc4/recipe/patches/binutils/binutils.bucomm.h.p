--- binutils/bucomm.h.orig	2009-02-01 17:13:41.000000000 +0100
+++ binutils/bucomm.h	2009-02-01 17:13:28.000000000 +0100
@@ -1,6 +1,6 @@
 /* bucomm.h -- binutils common include file.
    Copyright 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000,
-   2001, 2002, 2003, 2005 Free Software Foundation, Inc.
+   2001, 2002, 2003, 2005, 2006 Free Software Foundation, Inc.
 
    This file is part of GNU Binutils.
 
@@ -200,6 +200,7 @@ int display_info (void);
 void print_arelt_descr (FILE *, bfd *, bfd_boolean);
 
 char *make_tempname (char *);
+char *make_tempdir (char *);
 
 bfd_vma parse_vma (const char *, const char *);
 
