--- Makefile.orig	2008-12-08 16:46:06.000000000 -0800
+++ Makefile	2009-08-03 08:07:36.000000000 -0700
@@ -1,6 +1,7 @@
 prefix = /usr
 
 CFLAGS = -g -O2
+LIBS = -static
 
 PERL = perl
 INSTALL = install
@@ -33,7 +34,7 @@
 mkpasswd_LDADD += -lxcrypt
 DEFS += -DHAVE_XCRYPT
 else
-mkpasswd_LDADD += -lcrypt
+#mkpasswd_LDADD += -lcrypt
 endif
 
 all: Makefile.depend whois mkpasswd #pos
