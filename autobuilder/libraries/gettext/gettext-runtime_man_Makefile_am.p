--- gettext-runtime/man/Makefile.am.org	2005-05-20 22:13:40.000000000 +0200
+++ gettext-runtime/man/Makefile.am	2006-02-14 01:44:09.000000000 +0100
@@ -41,7 +41,7 @@
 textdomain.3 bindtextdomain.3 bind_textdomain_codeset.3
 man_MAN3IN = gettext.3.in ngettext.3.in \
 textdomain.3.in bindtextdomain.3.in bind_textdomain_codeset.3.in
+man_MAN3LINK =
-man_MAN3LINK = dgettext.3 dcgettext.3 dngettext.3 dcngettext.3
 man_MANS = $(man_MAN1) $(man_MAN3) $(man_MAN3LINK)

 man_HTMLGEN = gettext.1.html ngettext.1.html
