--- ./pathnames.h.orig	2015-09-23 16:04:35.393552335 +0200
+++ ./pathnames.h	2015-09-23 17:02:13.457519763 +0200
@@ -12,10 +12,10 @@
  * called by a name other than "ssh" or "Secure Shell".
  */
 
-#define ETCDIR				"/etc"
+#define ETCDIR				"Boot:Choices"
 
 #ifndef SSHDIR
-#define SSHDIR				ETCDIR "/ssh"
+#define SSHDIR				ETCDIR "ssh"
 #endif
 
 #ifndef _PATH_SSH_PIDDIR
@@ -26,27 +26,27 @@
  * System-wide file containing host keys of known hosts.  This file should be
  * world-readable.
  */
-#define _PATH_SSH_SYSTEM_HOSTFILE	SSHDIR "/ssh_known_hosts"
+#define _PATH_SSH_SYSTEM_HOSTFILE	SSHDIR ".ssh_known_hosts"
 /* backward compat for protocol 2 */
-#define _PATH_SSH_SYSTEM_HOSTFILE2	SSHDIR "/ssh_known_hosts2"
+#define _PATH_SSH_SYSTEM_HOSTFILE2	SSHDIR ".ssh_known_hosts2"
 
 /*
  * Of these, ssh_host_key must be readable only by root, whereas ssh_config
  * should be world-readable.
  */
-#define _PATH_SERVER_CONFIG_FILE	SSHDIR "/sshd_config"
-#define _PATH_HOST_CONFIG_FILE		SSHDIR "/ssh_config"
-#define _PATH_HOST_KEY_FILE		SSHDIR "/ssh_host_key"
-#define _PATH_HOST_DSA_KEY_FILE		SSHDIR "/ssh_host_dsa_key"
-#define _PATH_HOST_ECDSA_KEY_FILE	SSHDIR "/ssh_host_ecdsa_key"
-#define _PATH_HOST_ED25519_KEY_FILE	SSHDIR "/ssh_host_ed25519_key"
-#define _PATH_HOST_RSA_KEY_FILE		SSHDIR "/ssh_host_rsa_key"
-#define _PATH_DH_MODULI			SSHDIR "/moduli"
+#define _PATH_SERVER_CONFIG_FILE	SSHDIR ".sshd_config"
+#define _PATH_HOST_CONFIG_FILE		SSHDIR ".ssh_config"
+#define _PATH_HOST_KEY_FILE		SSHDIR ".ssh_host_key"
+#define _PATH_HOST_DSA_KEY_FILE		SSHDIR ".ssh_host_dsa_key"
+#define _PATH_HOST_ECDSA_KEY_FILE	SSHDIR ".ssh_host_ecdsa_key"
+#define _PATH_HOST_ED25519_KEY_FILE	SSHDIR ".ssh_host_ed25519_key"
+#define _PATH_HOST_RSA_KEY_FILE		SSHDIR ".ssh_host_rsa_key"
+#define _PATH_DH_MODULI			SSHDIR ".moduli"
 /* Backwards compatibility */
-#define _PATH_DH_PRIMES			SSHDIR "/primes"
+#define _PATH_DH_PRIMES			SSHDIR ".primes"
 
 #ifndef _PATH_SSH_PROGRAM
-#define _PATH_SSH_PROGRAM		"/usr/bin/ssh"
+#define _PATH_SSH_PROGRAM		"<SSH$Dir>.ssh"
 #endif
 
 /*
@@ -59,26 +59,26 @@
  * The directory in user's home directory in which the files reside. The
  * directory should be world-readable (though not all files are).
  */
-#define _PATH_SSH_USER_DIR		".ssh"
+#define _PATH_SSH_USER_DIR		"ssh"
 
 /*
  * Per-user file containing host keys of known hosts.  This file need not be
  * readable by anyone except the user him/herself, though this does not
  * contain anything particularly secret.
  */
-#define _PATH_SSH_USER_HOSTFILE		"~/" _PATH_SSH_USER_DIR "/known_hosts"
+#define _PATH_SSH_USER_HOSTFILE		"Boot:Choices." _PATH_SSH_USER_DIR ".known_hosts"
 /* backward compat for protocol 2 */
-#define _PATH_SSH_USER_HOSTFILE2	"~/" _PATH_SSH_USER_DIR "/known_hosts2"
+#define _PATH_SSH_USER_HOSTFILE2	"Boot:Choices." _PATH_SSH_USER_DIR ".known_hosts2"
 
 /*
  * Name of the default file containing client-side authentication key. This
  * file should only be readable by the user him/herself.
  */
-#define _PATH_SSH_CLIENT_IDENTITY	_PATH_SSH_USER_DIR "/identity"
-#define _PATH_SSH_CLIENT_ID_DSA		_PATH_SSH_USER_DIR "/id_dsa"
-#define _PATH_SSH_CLIENT_ID_ECDSA	_PATH_SSH_USER_DIR "/id_ecdsa"
-#define _PATH_SSH_CLIENT_ID_RSA		_PATH_SSH_USER_DIR "/id_rsa"
-#define _PATH_SSH_CLIENT_ID_ED25519	_PATH_SSH_USER_DIR "/id_ed25519"
+#define _PATH_SSH_CLIENT_IDENTITY	_PATH_SSH_USER_DIR ".identity"
+#define _PATH_SSH_CLIENT_ID_DSA		_PATH_SSH_USER_DIR ".id_dsa"
+#define _PATH_SSH_CLIENT_ID_ECDSA	_PATH_SSH_USER_DIR ".id_ecdsa"
+#define _PATH_SSH_CLIENT_ID_RSA		_PATH_SSH_USER_DIR ".id_rsa"
+#define _PATH_SSH_CLIENT_ID_ED25519	_PATH_SSH_USER_DIR ".id_ed25519"
 
 /*
  * Configuration file in user's home directory.  This file need not be
@@ -86,7 +86,7 @@
  * particularly secret.  If the user's home directory resides on an NFS
  * volume where root is mapped to nobody, this may need to be world-readable.
  */
-#define _PATH_SSH_USER_CONFFILE		_PATH_SSH_USER_DIR "/config"
+#define _PATH_SSH_USER_CONFFILE		_PATH_SSH_USER_DIR ".config"
 
 /*
  * File containing a list of those rsa keys that permit logging in as this
@@ -96,10 +96,10 @@
  * may need to be world-readable.  (This file is read by the daemon which is
  * running as root.)
  */
-#define _PATH_SSH_USER_PERMITTED_KEYS	_PATH_SSH_USER_DIR "/authorized_keys"
+#define _PATH_SSH_USER_PERMITTED_KEYS	_PATH_SSH_USER_DIR ".authorized_keys"
 
 /* backward compat for protocol v2 */
-#define _PATH_SSH_USER_PERMITTED_KEYS2	_PATH_SSH_USER_DIR "/authorized_keys2"
+#define _PATH_SSH_USER_PERMITTED_KEYS2	_PATH_SSH_USER_DIR ".authorized_keys2"
 
 /*
  * Per-user and system-wide ssh "rc" files.  These files are executed with
@@ -107,41 +107,31 @@
  * passed "proto cookie" as arguments if X11 forwarding with spoofing is in
  * use.  xauth will be run if neither of these exists.
  */
-#define _PATH_SSH_USER_RC		_PATH_SSH_USER_DIR "/rc"
-#define _PATH_SSH_SYSTEM_RC		SSHDIR "/sshrc"
+#define _PATH_SSH_USER_RC		_PATH_SSH_USER_DIR ".rc"
+#define _PATH_SSH_SYSTEM_RC		SSHDIR ".sshrc"
 
 /*
  * Ssh-only version of /etc/hosts.equiv.  Additionally, the daemon may use
- * ~/.rhosts and /etc/hosts.equiv if rhosts authentication is enabled.
+ * Boot:Choices/.rhosts and /etc/hosts.equiv if rhosts authentication is enabled.
  */
-#define _PATH_SSH_HOSTS_EQUIV		SSHDIR "/shosts.equiv"
+#define _PATH_SSH_HOSTS_EQUIV		SSHDIR ".shosts-equiv"
 #define _PATH_RHOSTS_EQUIV		"/etc/hosts.equiv"
 
 /*
  * Default location of askpass
  */
 #ifndef _PATH_SSH_ASKPASS_DEFAULT
-#define _PATH_SSH_ASKPASS_DEFAULT	"/usr/X11R6/bin/ssh-askpass"
+#define _PATH_SSH_ASKPASS_DEFAULT	"<SSH$Dir>.ssh-askpass"
 #endif
 
 /* Location of ssh-keysign for hostbased authentication */
 #ifndef _PATH_SSH_KEY_SIGN
-#define _PATH_SSH_KEY_SIGN		"/usr/libexec/ssh-keysign"
+#define _PATH_SSH_KEY_SIGN		"<SSH$Dir>.ssh-keysign"
 #endif
 
 /* Location of ssh-pkcs11-helper to support keys in tokens */
 #ifndef _PATH_SSH_PKCS11_HELPER
-#define _PATH_SSH_PKCS11_HELPER		"/usr/libexec/ssh-pkcs11-helper"
-#endif
-
-/* xauth for X11 forwarding */
-#ifndef _PATH_XAUTH
-#define _PATH_XAUTH			"/usr/X11R6/bin/xauth"
-#endif
-
-/* UNIX domain socket for X11 server; displaynum will replace %u */
-#ifndef _PATH_UNIX_X
-#define _PATH_UNIX_X "/tmp/.X11-unix/X%u"
+#define _PATH_SSH_PKCS11_HELPER		"<SSH$Dir>.ssh-pkcs11-helper"
 #endif
 
 /* for scp */
@@ -179,5 +169,5 @@
 
 /* Askpass program define */
 #ifndef ASKPASS_PROGRAM
-#define ASKPASS_PROGRAM         "/usr/lib/ssh/ssh-askpass"
+#define ASKPASS_PROGRAM         "<SSH$Dir>.ssh-askpass"
 #endif /* ASKPASS_PROGRAM */
