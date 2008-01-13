--- Makefile.orig	2007-11-24 19:40:54.000000000 -0800
+++ Makefile	2007-11-24 19:41:08.000000000 -0800
@@ -30,7 +30,7 @@
 
 whois: whois.c whois.h config.h data.h \
 		as_del.h as32_del.h ip_del.h ip6_del.h tld_serv.h
-	$(CC) $(CFLAGS) $(whois_CFLAGS) $(OPTS) whois.c -o whois \
+	$(CC) $(CFLAGS) $(whois_CFLAGS) $(OPTS) whois.c -o whois$(AB_EXEEXT) -static \
 		$(LDFLAGS) $(whois_LDADD)
 
 mkpasswd: mkpasswd.c
