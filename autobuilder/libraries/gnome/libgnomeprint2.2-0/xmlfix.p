--- tests/tools/xmlfix.c.orig	2008-01-10 20:19:46.000000000 -0800
+++ tests/tools/xmlfix.c	2008-01-10 20:21:48.000000000 -0800
@@ -33,12 +33,12 @@
 }
 
 /* #undef xmlStringText */
-const xmlChar xmlStringText[] = { 't', 'e', 'x', 't', 0 };
+extern const xmlChar xmlStringText[]; /* = { 't', 'e', 'x', 't', 0 }; */
 /* #undef xmlStringTextNoenc */
-const xmlChar xmlStringTextNoenc[] =
-              { 't', 'e', 'x', 't', 'n', 'o', 'e', 'n', 'c', 0 };
+extern const xmlChar xmlStringTextNoenc[]; /* =
+              { 't', 'e', 'x', 't', 'n', 'o', 'e', 'n', 'c', 0 }; */
 /* #undef xmlStringComment */
-const xmlChar xmlStringComment[] = { 'c', 'o', 'm', 'm', 'e', 'n', 't', 0 };
+extern const xmlChar xmlStringComment[]; /* = { 'c', 'o', 'm', 'm', 'e', 'n', 't', 0 }; */
 
 static xmlNodePtr xmlStaticCopyNodeList (xmlNodePtr node, xmlDocPtr doc, xmlNodePtr parent);
 
