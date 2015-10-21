--- ./openbsd-compat/Makefile.in.orig	2015-09-23 16:04:35.221552336 +0200
+++ ./openbsd-compat/Makefile.in	2015-09-23 16:04:35.233552336 +0200
@@ -16,9 +16,9 @@
 INSTALL=@INSTALL@
 LDFLAGS=-L. @LDFLAGS@
 
-OPENBSD=base64.o basename.o bcrypt_pbkdf.o bindresvport.o blowfish.o daemon.o dirname.o fmt_scaled.o getcwd.o getgrouplist.o getopt_long.o getrrsetbyname.o glob.o inet_aton.o inet_ntoa.o inet_ntop.o mktemp.o pwcache.o readpassphrase.o reallocarray.o realpath.o rresvport.o setenv.o setproctitle.o sha1.o sha2.o rmd160.o md5.o sigact.o strlcat.o strlcpy.o strmode.o strnlen.o strptime.o strsep.o strtonum.o strtoll.o strtoul.o strtoull.o timingsafe_bcmp.o vis.o blowfish.o bcrypt_pbkdf.o explicit_bzero.o
+OPENBSD=base64.o basename.o bcrypt_pbkdf.o bindresvport.o blowfish.o daemon.o dirname.o fmt_scaled.o getcwd.o getgrouplist.o getrrsetbyname.o glob.o inet_aton.o inet_ntoa.o inet_ntop.o mktemp.o pwcache.o readpassphrase.o reallocarray.o realpath.o rresvport.o setenv.o setproctitle.o sha1.o sha2.o rmd160.o md5.o sigact.o strlcat.o strlcpy.o strmode.o strnlen.o strptime.o strsep.o strtonum.o strtoll.o strtoul.o strtoull.o timingsafe_bcmp.o vis.o blowfish.o bcrypt_pbkdf.o explicit_bzero.o
 
-COMPAT=arc4random.o bsd-asprintf.o bsd-closefrom.o bsd-cray.o bsd-cygwin_util.o bsd-getpeereid.o getrrsetbyname-ldns.o bsd-misc.o bsd-nextstep.o bsd-openpty.o bsd-poll.o bsd-setres_id.o bsd-snprintf.o bsd-statvfs.o bsd-waitpid.o fake-rfc2553.o openssl-compat.o xmmap.o xcrypt.o kludge-fd_set.o
+COMPAT=arc4random.o bsd-asprintf.o bsd-closefrom.o bsd-cray.o bsd-cygwin_util.o bsd-getpeereid.o getrrsetbyname-ldns.o bsd-misc.o bsd-nextstep.o bsd-openpty.o bsd-poll.o bsd-setres_id.o bsd-snprintf.o bsd-waitpid.o fake-rfc2553.o openssl-compat.o xmmap.o xcrypt.o kludge-fd_set.o
 
 PORTS=port-aix.o port-irix.o port-linux.o port-solaris.o port-tun.o port-uw.o
 
