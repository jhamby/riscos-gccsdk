--- Makefile.in.orig	2004-12-22 11:12:38.000000000 +0000
+++ Makefile.in	2004-12-22 11:13:22.000000000 +0000
@@ -228,7 +228,7 @@
 
 xlincity_LDADD = \
 	${XLC_OBJ} \
-	@X_LIBS@ -lXext -lX11 -lm
+	@X_LIBS@ -lXext -lCX11 -lDesk -lm
 
 
 lincity_SOURCES = \
