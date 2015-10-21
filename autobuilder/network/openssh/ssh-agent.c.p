--- ./ssh-agent.c.orig	2015-09-23 16:04:35.509552334 +0200
+++ ./ssh-agent.c	2015-09-23 16:28:32.413538799 +0200
@@ -95,6 +95,10 @@
 #include <sys/prctl.h>	/* For prctl() and PR_SET_DUMPABLE */
 #endif
 
+void debugRO(const char *fmt, ...) {
+        printf("debugRO: %s\n",fmt);
+}
+
 typedef enum {
 	AUTH_UNUSED,
 	AUTH_SOCKET,
@@ -160,47 +164,57 @@
 static void
 close_socket(SocketEntry *e)
 {
+        debugRO("Entered close_socket().");
 	close(e->fd);
 	e->fd = -1;
 	e->type = AUTH_UNUSED;
 	sshbuf_free(e->input);
 	sshbuf_free(e->output);
 	sshbuf_free(e->request);
+	close(e->fd);
+        debugRO("Leaving close_socket().");
 }
 
 static void
 idtab_init(void)
 {
+        debugRO("Entered idtab_init().");
 	int i;
 
 	for (i = 0; i <=2; i++) {
 		TAILQ_INIT(&idtable[i].idlist);
 		idtable[i].nentries = 0;
 	}
+        debugRO("Leaving idtab_init().");
 }
 
 /* return private key table for requested protocol version */
 static Idtab *
 idtab_lookup(int version)
 {
+        debugRO("Entered idtab_lookup().");
 	if (version < 1 || version > 2)
 		fatal("internal error, bad protocol version %d", version);
+        debugRO("Leaving idtab_lookup().");
 	return &idtable[version];
 }
 
 static void
 free_identity(Identity *id)
 {
+        debugRO("Entered free_identity().");
 	sshkey_free(id->key);
 	free(id->provider);
 	free(id->comment);
 	free(id);
+        debugRO("Leaving free_identity().");
 }
 
 /* return matching private key for given public key */
 static Identity *
 lookup_identity(struct sshkey *key, int version)
 {
+        debugRO("Entered lookup_identity().");
 	Identity *id;
 
 	Idtab *tab = idtab_lookup(version);
@@ -208,6 +222,7 @@
 		if (sshkey_equal(key, id->key))
 			return (id);
 	}
+        debugRO("Leaving lookup_identity().");
 	return (NULL);
 }
 
@@ -215,6 +230,7 @@
 static int
 confirm_key(Identity *id)
 {
+        debugRO("Entered confirm_key().");
 	char *p;
 	int ret = -1;
 
@@ -224,6 +240,7 @@
 	    id->comment, p))
 		ret = 0;
 	free(p);
+        debugRO("Leaving confirm_key().");
 
 	return (ret);
 }
@@ -231,18 +248,21 @@
 static void
 send_status(SocketEntry *e, int success)
 {
+        debugRO("Entered send_status().");
 	int r;
 
 	if ((r = sshbuf_put_u32(e->output, 1)) != 0 ||
 	    (r = sshbuf_put_u8(e->output, success ?
 	    SSH_AGENT_SUCCESS : SSH_AGENT_FAILURE)) != 0)
 		fatal("%s: buffer error: %s", __func__, ssh_err(r));
+        debugRO("Leaving send_status().");
 }
 
 /* send list of supported public keys to 'client' */
 static void
 process_request_identities(SocketEntry *e, int version)
 {
+        debugRO("Entered process_request_identities().");
 	Idtab *tab = idtab_lookup(version);
 	Identity *id;
 	struct sshbuf *msg;
@@ -287,6 +307,7 @@
 	if ((r = sshbuf_put_stringb(e->output, msg)) != 0)
 		fatal("%s: buffer error: %s", __func__, ssh_err(r));
 	sshbuf_free(msg);
+        debugRO("Leaving process_request_identities().");
 }
 
 #ifdef WITH_SSH1
@@ -294,6 +315,7 @@
 static void
 process_authentication_challenge1(SocketEntry *e)
 {
+        debugRO("Entered process_authentication_challenge1().");
 	u_char buf[32], mdbuf[16], session_id[16];
 	u_int response_type;
 	BIGNUM *challenge;
@@ -368,6 +390,7 @@
 	sshkey_free(key);
 	BN_clear_free(challenge);
 	sshbuf_free(msg);
+        debugRO("Leaving process_authentication_challenge1().");
 }
 #endif
 
@@ -375,6 +398,7 @@
 static void
 process_sign_request2(SocketEntry *e)
 {
+        debugRO("Entered process_sign_request2().");
 	u_char *blob, *data, *signature = NULL;
 	size_t blen, dlen, slen = 0;
 	u_int compat = 0, flags;
@@ -426,12 +450,14 @@
 	free(data);
 	free(blob);
 	free(signature);
+        debugRO("Leaving process_sign_request2().");
 }
 
 /* shared */
 static void
 process_remove_identity(SocketEntry *e, int version)
 {
+        debugRO("Entered process_remove_identity().");
 	size_t blen;
 	int r, success = 0;
 	struct sshkey *key = NULL;
@@ -490,6 +516,7 @@
 		sshkey_free(key);
 	}
 	send_status(e, success);
+        debugRO("Leaving process_remove_identity().");
 }
 
 static void
@@ -497,6 +524,7 @@
 {
 	Idtab *tab = idtab_lookup(version);
 	Identity *id;
+        debugRO("Entered process_remove_all_identities().");
 
 	/* Loop over all identities and clear the keys. */
 	for (id = TAILQ_FIRST(&tab->idlist); id;
@@ -510,6 +538,7 @@
 
 	/* Send success. */
 	send_status(e, 1);
+        debugRO("Left process_remove_all_identities().");
 }
 
 /* removes expired keys and returns number of seconds until the next expiry */
@@ -520,6 +549,7 @@
 	Identity *id, *nxt;
 	int version;
 	Idtab *tab;
+        debugRO("Entered reaper().");
 
 	for (version = 1; version < 3; version++) {
 		tab = idtab_lookup(version);
@@ -537,6 +567,7 @@
 				    MIN(deadline, id->death);
 		}
 	}
+        debugRO("Leaving reaper().");
 	if (deadline == 0 || deadline <= now)
 		return 0;
 	else
@@ -553,6 +584,7 @@
 {
 	struct sshkey *k = NULL;
 	int r = SSH_ERR_INTERNAL_ERROR;
+        debugRO("Entering agent_decode_rsa1().");
 
 	*kp = NULL;
 	if ((k = sshkey_new_private(KEY_RSA1)) == NULL)
@@ -579,6 +611,7 @@
 
 	r = 0; /* success */
  out:
+        debugRO("Leaving agent_decode_rsa1().");
 	if (r == 0)
 		*kp = k;
 	else
@@ -590,6 +623,7 @@
 static void
 process_add_identity(SocketEntry *e, int version)
 {
+        debugRO("Entered process_add_identity().");
 	Idtab *tab = idtab_lookup(version);
 	Identity *id;
 	int success = 0, confirm = 0;
@@ -661,6 +695,7 @@
 	id->confirm = confirm;
 send:
 	send_status(e, success);
+        debugRO("Leaving process_add_identity().");
 }
 
 /* XXX todo: encrypt sensitive data with passphrase */
@@ -671,6 +706,7 @@
 	char *passwd, passwdhash[LOCK_SIZE];
 	static u_int fail_count = 0;
 	size_t pwlen;
+        debugRO("Entered process_add_identity().");
 
 	if ((r = sshbuf_get_cstring(e->request, &passwd, &pwlen)) != 0)
 		fatal("%s: buffer error: %s", __func__, ssh_err(r));
@@ -708,6 +744,7 @@
 	explicit_bzero(passwd, pwlen);
 	free(passwd);
 	send_status(e, success);
+        debugRO("Leaving process_add_identity().");
 }
 
 static void
@@ -715,6 +752,7 @@
 {
 	struct sshbuf *msg;
 	int r;
+        debugRO("Entered no_identities().");
 
 	if ((msg = sshbuf_new()) == NULL)
 		fatal("%s: sshbuf_new failed", __func__);
@@ -726,6 +764,7 @@
 	    (r = sshbuf_put_stringb(e->output, msg)) != 0)
 		fatal("%s: buffer error: %s", __func__, ssh_err(r));
 	sshbuf_free(msg);
+        debugRO("Leaving no_identities().");
 }
 
 #ifdef ENABLE_PKCS11
@@ -740,6 +779,7 @@
 	struct sshkey **keys = NULL, *k;
 	Identity *id;
 	Idtab *tab;
+        debugRO("Entered process_add_smartcard_key().");
 
 	if ((r = sshbuf_get_cstring(e->request, &provider, NULL)) != 0 ||
 	    (r = sshbuf_get_cstring(e->request, &pin, NULL)) != 0)
@@ -792,6 +832,7 @@
 	free(provider);
 	free(keys);
 	send_status(e, success);
+        debugRO("Leaving process_add_smartcard_key().");
 }
 
 static void
@@ -801,6 +842,7 @@
 	int r, version, success = 0;
 	Identity *id, *nxt;
 	Idtab *tab;
+        debugRO("Entered process_remove_smartcard_key().");
 
 	if ((r = sshbuf_get_cstring(e->request, &provider, NULL)) != 0 ||
 	    (r = sshbuf_get_cstring(e->request, &pin, NULL)) != 0)
@@ -828,6 +870,7 @@
 		    " pkcs11_del_provider failed");
 	free(provider);
 	send_status(e, success);
+        debugRO("Leaving process_remove_smartcard_key().");
 }
 #endif /* ENABLE_PKCS11 */
 
@@ -840,6 +883,7 @@
 	u_char type;
 	const u_char *cp;
 	int r;
+        debugRO("Entered process_message().");
 
 	if (sshbuf_len(e->input) < 5)
 		return;		/* Incomplete message. */
@@ -932,12 +976,14 @@
 		send_status(e, 0);
 		break;
 	}
+        debugRO("Leaving process_message().");
 }
 
 static void
 new_socket(sock_type type, int fd)
 {
 	u_int i, old_alloc, new_alloc;
+        debugRO("Entered new_socket().");
 
 	set_nonblock(fd);
 
@@ -954,6 +1000,7 @@
 			if ((sockets[i].request = sshbuf_new()) == NULL)
 				fatal("%s: sshbuf_new failed", __func__);
 			sockets[i].type = type;
+                        debugRO("Leaving new_socket() inside for... if...");
 			return;
 		}
 	old_alloc = sockets_alloc;
@@ -970,6 +1017,7 @@
 	if ((sockets[old_alloc].request = sshbuf_new()) == NULL)
 		fatal("%s: sshbuf_new failed", __func__);
 	sockets[old_alloc].type = type;
+        debugRO("Leaving new_socket().");
 }
 
 static int
@@ -980,6 +1028,7 @@
 	int n = 0;
 	static struct timeval tv;
 	time_t deadline;
+        debugRO("Entered prepare_select().");
 
 	for (i = 0; i < sockets_alloc; i++) {
 		switch (sockets[i].type) {
@@ -1032,12 +1081,14 @@
 		tv.tv_usec = 0;
 		*tvpp = &tv;
 	}
+        debugRO("Leaving prepare_select().");
 	return (1);
 }
 
 static void
 after_select(fd_set *readset, fd_set *writeset)
 {
+        debugRO("Entered after_select().");
 	struct sockaddr_un sunaddr;
 	socklen_t slen;
 	char buf[1024];
@@ -1116,11 +1167,13 @@
 		default:
 			fatal("Unknown type %d", sockets[i].type);
 		}
+        debugRO("Leaving after_select().");
 }
 
 static void
 cleanup_socket(void)
 {
+        debugRO("Entered cleanup_socket().");
 	if (cleanup_pid != 0 && getpid() != cleanup_pid)
 		return;
 	debug("%s: cleanup", __func__);
@@ -1128,11 +1181,13 @@
 		unlink(socket_name);
 	if (socket_dir[0])
 		rmdir(socket_dir);
+        debugRO("Leaving cleanup_socket().");
 }
 
 void
 cleanup_exit(int i)
 {
+        debugRO("Entered cleanup_exit(). Bye!");
 	cleanup_socket();
 	_exit(i);
 }
@@ -1141,16 +1196,19 @@
 static void
 cleanup_handler(int sig)
 {
+        debugRO("Entered cleanup_handler().");
 	cleanup_socket();
 #ifdef ENABLE_PKCS11
 	pkcs11_terminate();
 #endif
+        debugRO("Leaving cleanup_handler(). Bye!");
 	_exit(2);
 }
 
 static void
 check_parent_exists(void)
 {
+        debugRO("Entered check_parent_exists().");
 	/*
 	 * If our parent has exited then getppid() will return (pid_t)1,
 	 * so testing for that should be safe.
@@ -1160,6 +1218,7 @@
 		cleanup_socket();
 		_exit(2);
 	}
+        debugRO("Leaving check_parent_exists().");
 }
 
 static void
@@ -1175,7 +1234,7 @@
 int
 main(int ac, char **av)
 {
-	int c_flag = 0, d_flag = 0, D_flag = 0, k_flag = 0, s_flag = 0;
+        int c_flag = 0, d_flag = 0, D_flag = 0, k_flag = 0, s_flag = 0;
 	int sock, fd, ch, result, saved_errno;
 	u_int nalloc;
 	char *shell, *format, *pidstr, *agentsocket = NULL;
@@ -1210,7 +1269,7 @@
 	__progname = ssh_get_progname(av[0]);
 	seed_rng();
 
-	while ((ch = getopt(ac, av, "cDdksE:a:t:")) != -1) {
+	while ((ch = getopt(ac, av, "cDdksE:a:t:v")) != -1) {
 		switch (ch) {
 		case 'E':
 			fingerprint_hash = ssh_digest_alg_by_name(optarg);
@@ -1231,7 +1290,7 @@
 			s_flag++;
 			break;
 		case 'd':
-			if (d_flag || D_flag)
+			if (D_flag)
 				usage();
 			d_flag++;
 			break;
