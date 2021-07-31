--- Makefile.orig	2019-03-27 23:55:35.000000000 +0000
+++ Makefile	2021-01-19 18:05:52.151629049 +0000
@@ -1,6 +1,7 @@
 prefix = /usr
 
-CFLAGS ?= -g -O2
+CFLAGS ?= -g -O3
+LIBS = -static
 
 PKG_CONFIG ?= pkg-config
 PERL ?= perl
@@ -62,19 +63,19 @@
 DEFS += -DHAVE_CRYPT_H -DHAVE_LINUX_CRYPT_GENSALT -D_OW_SOURCE
 mkpasswd_LDADD += -lcrypt -lowcrypt
 else
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
@@ -119,11 +120,11 @@
 ##############################################################################
 install: install-whois install-mkpasswd install-pos
 
-install-whois: whois
+install-whois: whois$(AB_EXEEXT)
 	$(INSTALL) -d $(BASEDIR)$(prefix)/bin/
 	$(INSTALL) -d $(BASEDIR)$(prefix)/share/man/man1/
 	$(INSTALL) -d $(BASEDIR)$(prefix)/share/man/man5/
-	$(INSTALL) -m 0755 whois $(BASEDIR)$(prefix)/bin/
+	$(INSTALL) -m 0755 whois$(AB_EXEEXT) $(BASEDIR)$(prefix)/bin/
 	$(INSTALL) -m 0644 whois.1 $(BASEDIR)$(prefix)/share/man/man1/
 	$(INSTALL) -m 0644 whois.conf.5 $(BASEDIR)$(prefix)/share/man/man5/
 
@@ -141,7 +142,7 @@
 
 clean:
 	rm -f Makefile.depend as_del.h as32_del.h ip_del.h ip6_del.h \
-		new_gtlds.h tld_serv.h servers_charset.h *.o whois mkpasswd
+		new_gtlds.h tld_serv.h servers_charset.h *.o whois$(AB_EXEEXT) mkpasswd
 	rm -f po/*.mo
 
 pos:
