--- gtk-xmhtml/format.c.orig	2006-08-13 08:30:55.000000000 -0700
+++ gtk-xmhtml/format.c	2006-08-13 08:31:12.000000000 -0700
@@ -251,7 +251,7 @@
 #include <ctype.h>	/* isspace, tolower */
 
 #include <gdk/gdki18n.h>
-#if !defined(G_HAVE_BROKEN_WCTYPE) && (defined(G_HAVE_WCTYPE_H) || defined(G_HAVE_WCHAR_H)) && !defined(X_LOCALE)
+#if !defined(G_HAVE_BROKEN_WCTYPE) && (defined(G_HAVE_WCTYPE_H) || defined(G_HAVE_WCHAR_H)) && !defined(X_LOCALE) && 0
 #define is_ideograph(wc) \
 	(iswalpha (wc) && (!iswupper (wc) && !iswlower (wc) )) || \
 	(!(iswalnum(wc) || iswspace(wc) || iswpunct(wc) || iswcntrl(wc)))
