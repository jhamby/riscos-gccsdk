--- Makefile.orig	2012-11-01 20:53:41.000000000 +0000
+++ Makefile	2012-11-01 20:56:15.000000000 +0000
@@ -1,6 +1,7 @@
 prefix = /usr
 
 CFLAGS ?= -g -O2
+LIBS = -static
 
 PERL = perl
 INSTALL = install
@@ -50,19 +51,19 @@
 # owl and openSUSE have crypt_gensalt(3) in the libc's libcrypt
 DEFS += -DHAVE_LINUX_CRYPT_GENSALT
 endif
-mkpasswd_LDADD += -lcrypt
+#mkpasswd_LDADD += -lcrypt
 endif
 
 CPPFLAGS += $(DEFS) $(INCLUDES)
 
 ##############################################################################
-all: Makefile.depend whois mkpasswd pos
+all: Makefile.depend whois$(AB_EXEEXT) mkpasswd pos
 
 ##############################################################################
 %.o: %.c
 	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<
 
-whois: $(whois_OBJECTS)
+whois$(AB_EXEEXT): $(whois_OBJECTS)
 	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(whois_LDADD) $(LIBS)
 
 mkpasswd: $(mkpasswd_OBJECTS)
@@ -90,10 +91,10 @@
 ##############################################################################
 install: install-whois install-mkpasswd install-pos
 
-install-whois: whois
+install-whois: whois$(AB_EXEEXT)
 	$(INSTALL) -d $(BASEDIR)$(prefix)/bin/
 	$(INSTALL) -d $(BASEDIR)$(prefix)/share/man/man1/
-	$(INSTALL) -m 0755 whois $(BASEDIR)$(prefix)/bin/
+	$(INSTALL) -m 0755 whois$(AB_EXEEXT) $(BASEDIR)$(prefix)/bin/
 	$(INSTALL) -m 0644 whois.1 $(BASEDIR)$(prefix)/share/man/man1/
 
 install-mkpasswd: mkpasswd
@@ -110,7 +111,7 @@
 
 clean:
 	rm -f Makefile.depend as_del.h as32_del.h ip_del.h ip6_del.h \
-		tld_serv.h servers_charset.h *.o whois mkpasswd
+		tld_serv.h servers_charset.h *.o whois$(AB_EXEEXT) mkpasswd
 	rm -f po/*.mo
 
 pos:
