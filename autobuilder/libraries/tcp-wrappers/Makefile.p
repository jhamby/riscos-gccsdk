--- Makefile.old	2003-01-09 14:35:51.000000000 +0000
+++ Makefile	2003-01-09 16:35:06.000000000 +0000
@@ -99,6 +99,13 @@
 	LIBS= RANLIB=ranlib ARFLAGS=rv AUX_OBJ=setenv.o \
 	NETGROUP=-DNETGROUP TLI= IPV6="$(IPV6)" all
 
+riscos:
+	make REAL_DAEMON_DIR=$(REAL_DAEMON_DIR) STYLE=$(STYLE) \
+	LIBS= RANLIB=ranlib ARFLAGS=rv AUX_OBJ=setenv.o \
+	NETGROUP=-DNETGROUP TLI= IPV6= \
+	EXTRA_CFLAGS=-DSYS_ERRLIST_DEFINED libwrap.a
+	cp libwrap.a /home/riscos/env/lib
+
 # Ditto, with vsyslog
 sunos4:
 	@make REAL_DAEMON_DIR=$(REAL_DAEMON_DIR) STYLE=$(STYLE) \
