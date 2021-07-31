--- Makerules.orig	2019-11-07 18:59:57.582030928 +0000
+++ Makerules	2019-11-07 19:00:31.690030963 +0000
@@ -25,7 +25,7 @@
   LDFLAGS += -g
 else ifeq ($(build),release)
   CFLAGS += -pipe -O2 -DNDEBUG -fomit-frame-pointer
-  LDFLAGS += $(LDREMOVEUNREACH) -Wl,-s
+  LDFLAGS += -static -Wl,-s -lglib-2.0 -lintl
 else ifeq ($(build),small)
   CFLAGS += -pipe -Os -DNDEBUG -fomit-frame-pointer
   LDFLAGS += $(LDREMOVEUNREACH) -Wl,-s
