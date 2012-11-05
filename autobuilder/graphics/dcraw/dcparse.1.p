--- dcparse.1.orig	2012-11-04 21:50:54.000000000 +0000
+++ dcparse.1	2012-04-29 11:59:46.000000000 +0100
@@ -0,0 +1,25 @@
+.\"
+.\" Manpage for dcparse
+.\" Copyright 2004, Steve King
+.\" This file may be distributed without restriction.
+.\"
+.TH dcparse 1 "Mar 17, 2005"
+.SH NAME
+dcparse - Extract embeded thumbnail image and print CIFF/TIFF data to screen
+.br
+.SH SYNOPSIS
+.B dcparse
+file1.crw file2.crw ...
+.br
+.SH DESCRIPTION
+This utility reads data directly from RAW digital photo files.
+
+.B dcparse
+prints internal CIFF/TIFF data, and creates a file called
+.B <filename>.thumb
+which is generally a JPEG file. The CIFF/TIFF data is technical in nature.
+
+.SH "SEE ALSO"
+dcraw(1)
+.SH AUTHOR
+Written by Steve King for debian.
