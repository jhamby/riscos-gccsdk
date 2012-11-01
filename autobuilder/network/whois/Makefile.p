--- Makefile.orig	2008-12-08 16:46:06.000000000 -0800
+++ Makefile	2009-08-03 08:07:36.000000000 -0700
@@ -1,6 +1,7 @@
 prefix = /usr
 
 CFLAGS = -g -O2
+LIBS = -static
 
 PERL = perl
 INSTALL = install
@@ -50,7 +51,7 @@
 # owl and openSUSE have crypt_gensalt(3) in the libc's libcrypt
 DEFS += -DHAVE_LINUX_CRYPT_GENSALT
 endif
-mkpasswd_LDADD += -lcrypt
+#mkpasswd_LDADD += -lcrypt
 endif
 
 CPPFLAGS += $(DEFS) $(INCLUDES)
