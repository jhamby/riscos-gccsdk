--- test/Makefile.orig	2015-12-29 15:34:20.478636523 +0000
+++ test/Makefile	2015-12-29 15:34:41.466636545 +0000
@@ -738,7 +738,7 @@
 hmactest.o: ../include/openssl/opensslv.h ../include/openssl/ossl_typ.h
 hmactest.o: ../include/openssl/safestack.h ../include/openssl/stack.h
 hmactest.o: ../include/openssl/symhacks.h hmactest.c
-ideatest.o: ../e_os.h ../include/openssl/e_os2.h ../include/openssl/idea.h
+ideatest.o: ../e_os.h ../include/openssl/e_os2.h
 ideatest.o: ../include/openssl/opensslconf.h ideatest.c
 igetest.o: ../include/openssl/aes.h ../include/openssl/e_os2.h
 igetest.o: ../include/openssl/opensslconf.h ../include/openssl/ossl_typ.h
@@ -770,7 +770,7 @@
 mdc2test.o: ../e_os.h ../include/openssl/asn1.h ../include/openssl/bio.h
 mdc2test.o: ../include/openssl/crypto.h ../include/openssl/des.h
 mdc2test.o: ../include/openssl/des_old.h ../include/openssl/e_os2.h
-mdc2test.o: ../include/openssl/evp.h ../include/openssl/mdc2.h
+mdc2test.o: ../include/openssl/evp.h
 mdc2test.o: ../include/openssl/obj_mac.h ../include/openssl/objects.h
 mdc2test.o: ../include/openssl/opensslconf.h ../include/openssl/opensslv.h
 mdc2test.o: ../include/openssl/ossl_typ.h ../include/openssl/safestack.h
