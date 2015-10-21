--- ./readconf.c.orig	2015-09-23 16:04:35.097552338 +0200
+++ ./readconf.c	2015-09-23 20:57:13.782749226 +0200
@@ -1818,19 +1818,19 @@
 		options->protocol = SSH_PROTO_2;
 	if (options->num_identity_files == 0) {
 		if (options->protocol & SSH_PROTO_1) {
-			add_identity_file(options, "~/",
+			add_identity_file(options, "Boot:Choices.",
 			    _PATH_SSH_CLIENT_IDENTITY, 0);
 		}
 		if (options->protocol & SSH_PROTO_2) {
-			add_identity_file(options, "~/",
+			add_identity_file(options, "Boot:Choices.",
 			    _PATH_SSH_CLIENT_ID_RSA, 0);
-			add_identity_file(options, "~/",
+			add_identity_file(options, "Boot:Choices.",
 			    _PATH_SSH_CLIENT_ID_DSA, 0);
 #ifdef OPENSSL_HAS_ECC
-			add_identity_file(options, "~/",
+			add_identity_file(options, "Boot:Choices.",
 			    _PATH_SSH_CLIENT_ID_ECDSA, 0);
 #endif
-			add_identity_file(options, "~/",
+			add_identity_file(options, "Boot:Choices.",
 			    _PATH_SSH_CLIENT_ID_ED25519, 0);
 		}
 	}
