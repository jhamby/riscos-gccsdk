--- src/Makefile.in.org	2006-08-03 00:39:42.000000000 +0200
+++ src/Makefile.in	2006-08-03 00:47:35.000000000 +0200
@@ -482,7 +482,8 @@
 @XLOCALE_TRUE@USE_I18N_LIBS = $(I18N_LIBS)
 @XCMS_TRUE@USE_XCMS_LIBS = $(XCMS_LIBS)
 @XKB_TRUE@USE_XKB_LIBS = $(XKB_LIBS)
-libX11_la_LDFLAGS = -version-number 6:2:0 -no-undefined
+#libX11_la_LDFLAGS = -version-number 6:2:0 -no-undefined
+libX11_la_LDFLAGS =
 libX11_la_LIBADD = \
 	$(USE_I18N_LIBS) \
 	$(USE_XCMS_LIBS) \
