--- crypto/evp/Makefile.orig	2015-12-29 15:14:33.178635310 +0000
+++ crypto/evp/Makefile	2015-12-29 15:14:46.478635323 +0000
@@ -289,7 +289,7 @@
 e_idea.o: ../../e_os.h ../../include/openssl/asn1.h ../../include/openssl/bio.h
 e_idea.o: ../../include/openssl/buffer.h ../../include/openssl/crypto.h
 e_idea.o: ../../include/openssl/e_os2.h ../../include/openssl/err.h
-e_idea.o: ../../include/openssl/evp.h ../../include/openssl/idea.h
+e_idea.o: ../../include/openssl/evp.h
 e_idea.o: ../../include/openssl/lhash.h ../../include/openssl/obj_mac.h
 e_idea.o: ../../include/openssl/objects.h ../../include/openssl/opensslconf.h
 e_idea.o: ../../include/openssl/opensslv.h ../../include/openssl/ossl_typ.h
@@ -538,7 +538,7 @@
 m_mdc2.o: ../../include/openssl/e_os2.h ../../include/openssl/ec.h
 m_mdc2.o: ../../include/openssl/ecdh.h ../../include/openssl/ecdsa.h
 m_mdc2.o: ../../include/openssl/err.h ../../include/openssl/evp.h
-m_mdc2.o: ../../include/openssl/lhash.h ../../include/openssl/mdc2.h
+m_mdc2.o: ../../include/openssl/lhash.h
 m_mdc2.o: ../../include/openssl/obj_mac.h ../../include/openssl/objects.h
 m_mdc2.o: ../../include/openssl/opensslconf.h ../../include/openssl/opensslv.h
 m_mdc2.o: ../../include/openssl/ossl_typ.h ../../include/openssl/pkcs7.h
