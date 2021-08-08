--- mk/extra.mk.in.orig	2021-08-08 08:43:20.010101615 +0100
+++ mk/extra.mk.in	2021-08-08 08:43:41.493649469 +0100
@@ -6,7 +6,7 @@
 ERLC = @ERLC@
 OBJC = @OBJC@
 AR = @AR@
-LD = ld
+LD = arm-unknown-riscos-ld
 CFLAGS = @CFLAGS@
 CXXFLAGS = @CXXFLAGS@
 CPPFLAGS = @CPPFLAGS@
