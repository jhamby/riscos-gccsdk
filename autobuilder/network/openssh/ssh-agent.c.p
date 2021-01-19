--- ssh-agent.c.orig	2014-07-30 03:32:46.000000000 +0100
+++ ssh-agent.c	2021-01-19 18:59:51.839511697 +0000
@@ -86,6 +86,10 @@
 #include <sys/prctl.h>	/* For prctl() and PR_SET_DUMPABLE */
 #endif
 
+void debugRO(const char *fmt, ...) {
+        printf("debugRO: %s\n",fmt);
+}
+
 typedef enum {
 	AUTH_UNUSED,
 	AUTH_SOCKET,
@@ -145,47 +149,57 @@
 static void
 close_socket(SocketEntry *e)
 {
+	debugRO("Entered close_socket().");
 	close(e->fd);
 	e->fd = -1;
 	e->type = AUTH_UNUSED;
 	buffer_free(&e->input);
 	buffer_free(&e->output);
 	buffer_free(&e->request);
+	close(e->fd);
+	debugRO("Leaving close_socket().");
 }
 
 static void
 idtab_init(void)
 {
+	debugRO("Entered idtab_init().");
 	int i;
 
 	for (i = 0; i <=2; i++) {
 		TAILQ_INIT(&idtable[i].idlist);
 		idtable[i].nentries = 0;
 	}
+	debugRO("Leaving idtab_init().");
 }
 
 /* return private key table for requested protocol version */
 static Idtab *
 idtab_lookup(int version)
 {
+	debugRO("Entered idtab_lookup().");
 	if (version < 1 || version > 2)
 		fatal("internal error, bad protocol version %d", version);
+	debugRO("Leaving idtab_lookup().");
 	return &idtable[version];
 }
 
 static void
 free_identity(Identity *id)
 {
+	debugRO("Entered free_identity().");
 	key_free(id->key);
 	free(id->provider);
 	free(id->comment);
 	free(id);
+	debugRO("Leaving free_identity().");
 }
 
 /* return matching private key for given public key */
 static Identity *
 lookup_identity(Key *key, int version)
 {
+        debugRO("Entered lookup_identity().");
 	Identity *id;
 
 	Idtab *tab = idtab_lookup(version);
@@ -193,6 +207,7 @@
 		if (key_equal(key, id->key))
 			return (id);
 	}
+        debugRO("Leaving lookup_identity().");
 	return (NULL);
 }
 
@@ -200,6 +215,7 @@
 static int
 confirm_key(Identity *id)
 {
+        debugRO("Entered confirm_key().");
 	char *p;
 	int ret = -1;
 
@@ -208,6 +224,7 @@
 	    id->comment, p))
 		ret = 0;
 	free(p);
+        debugRO("Leaving confirm_key().");
 
 	return (ret);
 }
@@ -216,6 +233,7 @@
 static void
 process_request_identities(SocketEntry *e, int version)
 {
+        debugRO("Entered process_request_identities().");
 	Idtab *tab = idtab_lookup(version);
 	Identity *id;
 	Buffer msg;
@@ -243,6 +261,7 @@
 	buffer_put_int(&e->output, buffer_len(&msg));
 	buffer_append(&e->output, buffer_ptr(&msg), buffer_len(&msg));
 	buffer_free(&msg);
+        debugRO("leaving process_request_identities().");
 }
 
 #ifdef WITH_SSH1
@@ -250,6 +269,7 @@
 static void
 process_authentication_challenge1(SocketEntry *e)
 {
+        debugRO("Entered process_authentication_challenge1().");
 	u_char buf[32], mdbuf[16], session_id[16];
 	u_int response_type;
 	BIGNUM *challenge;
@@ -315,6 +335,7 @@
 	key_free(key);
 	BN_clear_free(challenge);
 	buffer_free(&msg);
+        debugRO("Leaving process_authentication_challenge1().");
 }
 #endif
 
@@ -322,6 +343,7 @@
 static void
 process_sign_request2(SocketEntry *e)
 {
+        debugRO("Entered process_sign_request2().");
 	u_char *blob, *data, *signature = NULL;
 	u_int blen, dlen, slen = 0;
 	extern int datafellows;
@@ -362,12 +384,14 @@
 	free(blob);
 	free(signature);
 	datafellows = odatafellows;
+        debugRO("Leaving process_sign_request2().");
 }
 
 /* shared */
 static void
 process_remove_identity(SocketEntry *e, int version)
 {
+        debugRO("Entered process_remove_identity().");
 	u_int blen;
 	int success = 0;
 	Key *key = NULL;
@@ -420,6 +444,7 @@
 	buffer_put_int(&e->output, 1);
 	buffer_put_char(&e->output,
 	    success ? SSH_AGENT_SUCCESS : SSH_AGENT_FAILURE);
+        debugRO("Leaving process_remove_identity().");
 }
 
 static void
@@ -427,6 +452,7 @@
 {
 	Idtab *tab = idtab_lookup(version);
 	Identity *id;
+        debugRO("Entered process_remove_all_identities().");
 
 	/* Loop over all identities and clear the keys. */
 	for (id = TAILQ_FIRST(&tab->idlist); id;
@@ -441,6 +467,7 @@
 	/* Send success. */
 	buffer_put_int(&e->output, 1);
 	buffer_put_char(&e->output, SSH_AGENT_SUCCESS);
+        debugRO("Left process_remove_all_identities().");
 }
 
 /* removes expired keys and returns number of seconds until the next expiry */
@@ -451,6 +478,7 @@
 	Identity *id, *nxt;
 	int version;
 	Idtab *tab;
+        debugRO("Entered reaper().");
 
 	for (version = 1; version < 3; version++) {
 		tab = idtab_lookup(version);
@@ -468,6 +496,7 @@
 				    MIN(deadline, id->death);
 		}
 	}
+        debugRO("Leaving reaper().");
 	if (deadline == 0 || deadline <= now)
 		return 0;
 	else
@@ -477,6 +506,7 @@
 static void
 process_add_identity(SocketEntry *e, int version)
 {
+        debugRO("Entered process_add_identity().");
 	Idtab *tab = idtab_lookup(version);
 	Identity *id;
 	int type, success = 0, confirm = 0;
@@ -559,6 +589,7 @@
 	buffer_put_int(&e->output, 1);
 	buffer_put_char(&e->output,
 	    success ? SSH_AGENT_SUCCESS : SSH_AGENT_FAILURE);
+        debugRO("Leaving process_add_identity().");
 }
 
 /* XXX todo: encrypt sensitive data with passphrase */
@@ -591,6 +622,7 @@
 static void
 no_identities(SocketEntry *e, u_int type)
 {
+        debugRO("Entered no_identities().");
 	Buffer msg;
 
 	buffer_init(&msg);
@@ -601,6 +633,7 @@
 	buffer_put_int(&e->output, buffer_len(&msg));
 	buffer_append(&e->output, buffer_ptr(&msg), buffer_len(&msg));
 	buffer_free(&msg);
+        debugRO("Leaving no_identities().");
 }
 
 #ifdef ENABLE_PKCS11
@@ -613,6 +646,7 @@
 	Key **keys = NULL, *k;
 	Identity *id;
 	Idtab *tab;
+        debugRO("Entered process_add_smartcard_key().");
 
 	provider = buffer_get_string(&e->request, NULL);
 	pin = buffer_get_string(&e->request, NULL);
@@ -661,6 +695,7 @@
 	buffer_put_int(&e->output, 1);
 	buffer_put_char(&e->output,
 	    success ? SSH_AGENT_SUCCESS : SSH_AGENT_FAILURE);
+        debugRO("Leaving process_add_smartcard_key().");
 }
 
 static void
@@ -670,6 +705,7 @@
 	int version, success = 0;
 	Identity *id, *nxt;
 	Idtab *tab;
+        debugRO("Entered process_remove_smartcard_key().");
 
 	provider = buffer_get_string(&e->request, NULL);
 	pin = buffer_get_string(&e->request, NULL);
@@ -708,6 +744,7 @@
 {
 	u_int msg_len, type;
 	u_char *cp;
+        debugRO("Entered process_message().");
 
 	if (buffer_len(&e->input) < 5)
 		return;		/* Incomplete message. */
@@ -803,12 +840,14 @@
 		buffer_put_char(&e->output, SSH_AGENT_FAILURE);
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
 
@@ -822,6 +861,7 @@
 			buffer_init(&sockets[i].output);
 			buffer_init(&sockets[i].request);
 			sockets[i].type = type;
+                        debugRO("Leaving new_socket() inside for... if...");
 			return;
 		}
 	old_alloc = sockets_alloc;
@@ -835,6 +875,7 @@
 	buffer_init(&sockets[old_alloc].output);
 	buffer_init(&sockets[old_alloc].request);
 	sockets[old_alloc].type = type;
+        debugRO("Leaving new_socket().");
 }
 
 static int
@@ -845,6 +886,7 @@
 	int n = 0;
 	static struct timeval tv;
 	time_t deadline;
+        debugRO("Entered prepare_select().");
 
 	for (i = 0; i < sockets_alloc; i++) {
 		switch (sockets[i].type) {
@@ -897,12 +939,14 @@
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
@@ -975,11 +1019,13 @@
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
@@ -987,11 +1033,13 @@
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
@@ -1000,16 +1048,19 @@
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
@@ -1019,6 +1070,7 @@
 		cleanup_socket();
 		_exit(2);
 	}
+        debugRO("Leaving check_parent_exists().");
 }
 
 static void
@@ -1069,7 +1121,7 @@
 	__progname = ssh_get_progname(av[0]);
 	seed_rng();
 
-	while ((ch = getopt(ac, av, "cdksa:t:")) != -1) {
+	while ((ch = getopt(ac, av, "cdksa:t:v")) != -1) {
 		switch (ch) {
 		case 'c':
 			if (s_flag)
