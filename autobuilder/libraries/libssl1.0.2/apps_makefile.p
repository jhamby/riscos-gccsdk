--- apps/Makefile.org	2005-07-05 20:41:28.000000000 +0200
+++ apps/Makefile	2006-04-19 18:15:32.820000000 +0200
@@ -102,20 +102,14 @@
 	@set -e; for i in $(EXE); \
 	do  \
 	(echo installing $$i; \
-	 cp $$i $(INSTALL_PREFIX)$(INSTALLTOP)/bin/$$i.new; \
-	 chmod 755 $(INSTALL_PREFIX)$(INSTALLTOP)/bin/$$i.new; \
-	 mv -f $(INSTALL_PREFIX)$(INSTALLTOP)/bin/$$i.new $(INSTALL_PREFIX)$(INSTALLTOP)/bin/$$i ); \
+	 cp $$i $(INSTALL_PREFIX)$(INSTALLTOP)/bin/$$i; ); \
 	 done;
 	@set -e; for i in $(SCRIPTS); \
 	do  \
 	(echo installing $$i; \
-	 cp $$i $(INSTALL_PREFIX)$(OPENSSLDIR)/misc/$$i.new; \
-	 chmod 755 $(INSTALL_PREFIX)$(OPENSSLDIR)/misc/$$i.new; \
-	 mv -f $(INSTALL_PREFIX)$(OPENSSLDIR)/misc/$$i.new $(INSTALL_PREFIX)$(OPENSSLDIR)/misc/$$i ); \
+	 cp $$i $(INSTALL_PREFIX)$(OPENSSLDIR)/misc/$$i ); \
 	 done
-	@cp openssl.cnf $(INSTALL_PREFIX)$(OPENSSLDIR)/openssl.cnf.new; \
-	chmod 644 $(INSTALL_PREFIX)$(OPENSSLDIR)/openssl.cnf.new; \
-	mv -f  $(INSTALL_PREFIX)$(OPENSSLDIR)/openssl.cnf.new $(INSTALL_PREFIX)$(OPENSSLDIR)/openssl.cnf
+	@cp openssl.cnf $(INSTALL_PREFIX)$(OPENSSLDIR)/openssl.cnf

 tags:
 	ctags $(SRC)
@@ -941,9 +935,9 @@
 speed.o: ../include/openssl/ec.h ../include/openssl/ecdh.h
 speed.o: ../include/openssl/ecdsa.h ../include/openssl/engine.h
 speed.o: ../include/openssl/err.h ../include/openssl/evp.h
-speed.o: ../include/openssl/hmac.h ../include/openssl/idea.h
+speed.o: ../include/openssl/hmac.h
 speed.o: ../include/openssl/lhash.h ../include/openssl/md4.h
-speed.o: ../include/openssl/md5.h ../include/openssl/mdc2.h
+speed.o: ../include/openssl/md5.h
 speed.o: ../include/openssl/modes.h ../include/openssl/obj_mac.h
 speed.o: ../include/openssl/objects.h ../include/openssl/ocsp.h
 speed.o: ../include/openssl/opensslconf.h ../include/openssl/opensslv.h
@@ -1029,7 +1023,7 @@
 version.o: ../include/openssl/des_old.h ../include/openssl/e_os2.h
 version.o: ../include/openssl/ec.h ../include/openssl/ecdh.h
 version.o: ../include/openssl/ecdsa.h ../include/openssl/engine.h
-version.o: ../include/openssl/evp.h ../include/openssl/idea.h
+version.o: ../include/openssl/evp.h
 version.o: ../include/openssl/lhash.h ../include/openssl/obj_mac.h
 version.o: ../include/openssl/objects.h ../include/openssl/ocsp.h
 version.o: ../include/openssl/opensslconf.h ../include/openssl/opensslv.h
