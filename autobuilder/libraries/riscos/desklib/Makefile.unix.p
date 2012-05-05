--- !DLSources/Makefile.unix.orig	2012-04-23 17:36:02.000000000 +0100
+++ !DLSources/Makefile.unix	2012-04-23 17:36:18.000000000 +0100
@@ -37,7 +37,7 @@
 WARNINGS = -std=c99 -pedantic -Wall -Wundef -Wpointer-arith \
            -Wcast-align -Wwrite-strings -Wstrict-prototypes \
            -Wmissing-prototypes -Wmissing-declarations -Wnested-externs \
-           -Winline -Wno-unused -Wno-long-long -W -Wcast-qual -Wshadow -Werror
+           -Winline -Wno-unused -Wno-long-long -W -Wcast-qual -Wshadow
 
 # We do ELF based builds only
 CC = $(GCCSDK_INSTALL_CROSSBIN)/arm-unknown-riscos-gcc
