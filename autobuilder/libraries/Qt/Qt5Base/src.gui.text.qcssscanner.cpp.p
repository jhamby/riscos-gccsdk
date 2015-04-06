--- src/gui/text/qcssscanner.cpp.orig	2013-08-25 19:03:32.000000000 +0100
+++ src/gui/text/qcssscanner.cpp	2013-11-18 11:40:34.656463502 +0000
@@ -76,165 +76,165 @@
 
     // initial state
         ch = next();
-        if (ch.unicode() >= 9 && ch.unicode() <= 10)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 9 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 10)
             goto state_1;
-        if (ch.unicode() >= 12 && ch.unicode() <= 13)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 12 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 13)
             goto state_1;
-        if (ch.unicode() == 32)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 32)
             goto state_1;
-        if (ch.unicode() == 33) {
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 33) {
             token = QCss::EXCLAMATION_SYM;
             goto found;
         }
-        if (ch.unicode() == 34)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 34)
             goto state_3;
-        if (ch.unicode() == 35)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 35)
             goto state_4;
-        if (ch.unicode() == 39)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 39)
             goto state_5;
-        if (ch.unicode() == 40) {
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 40) {
             token = QCss::LPAREN;
             goto found;
         }
-        if (ch.unicode() == 41) {
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 41) {
             token = QCss::RPAREN;
             goto found;
         }
-        if (ch.unicode() == 42) {
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 42) {
             token = QCss::STAR;
             goto found;
         }
-        if (ch.unicode() == 43)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 43)
             goto state_9;
-        if (ch.unicode() == 44)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 44)
             goto state_10;
-        if (ch.unicode() == 45)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 45)
             goto state_11;
-        if (ch.unicode() == 46)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 46)
             goto state_12;
-        if (ch.unicode() == 47)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 47)
             goto state_13;
-        if (ch.unicode() >= 48 && ch.unicode() <= 57)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 48 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 57)
             goto state_14;
-        if (ch.unicode() == 58) {
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 58) {
             token = QCss::COLON;
             goto found;
         }
-        if (ch.unicode() == 59) {
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 59) {
             token = QCss::SEMICOLON;
             goto found;
         }
-        if (ch.unicode() == 60)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 60)
             goto state_17;
-        if (ch.unicode() == 61) {
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 61) {
             token = QCss::EQUAL;
             goto found;
         }
-        if (ch.unicode() == 62)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 62)
             goto state_19;
-        if (ch.unicode() == 64)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 64)
             goto state_20;
-        if (ch.unicode() == 91) {
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 91) {
             token = QCss::LBRACKET;
             goto found;
         }
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_22;
-        if (ch.unicode() == 93) {
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 93) {
             token = QCss::RBRACKET;
             goto found;
         }
-        if (ch.unicode() == 95)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 95)
             goto state_24;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_24;
-        if (ch.unicode() == 123)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 123)
             goto state_25;
-        if (ch.unicode() == 124)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 124)
             goto state_26;
-        if (ch.unicode() == 125) {
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 125) {
             token = QCss::RBRACE;
             goto found;
         }
-        if (ch.unicode() == 126)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 126)
             goto state_28;
         goto out;
     state_1:
         lastAcceptingPos = pos;
         token = QCss::S;
         ch = next();
-        if (ch.unicode() >= 9 && ch.unicode() <= 10)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 9 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 10)
             goto state_29;
-        if (ch.unicode() >= 12 && ch.unicode() <= 13)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 12 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 13)
             goto state_29;
-        if (ch.unicode() == 32)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 32)
             goto state_29;
-        if (ch.unicode() == 43)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 43)
             goto state_9;
-        if (ch.unicode() == 44)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 44)
             goto state_10;
-        if (ch.unicode() == 62)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 62)
             goto state_19;
-        if (ch.unicode() == 123)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 123)
             goto state_25;
         goto out;
     state_3:
         lastAcceptingPos = pos;
         token = QCss::INVALID;
         ch = next();
-        if (ch.unicode() >= 1 && ch.unicode() <= 9)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 1 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 9)
             goto state_30;
-        if (ch.unicode() == 11)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 11)
             goto state_30;
-        if (ch.unicode() >= 14 && ch.unicode() <= 33)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 14 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 33)
             goto state_30;
-        if (ch.unicode() == 34)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 34)
             goto state_31;
-        if (ch.unicode() >= 35 && ch.unicode() <= 91)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 35 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 91)
             goto state_30;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_32;
-        if (ch.unicode() >= 93 && ch.unicode() <= 96)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 93 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 96)
             goto state_30;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_30;
-        if (ch.unicode() >= 123)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 123)
             goto state_30;
         goto out;
     state_4:
         ch = next();
-        if (ch.unicode() == 45)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 45)
             goto state_33;
-        if (ch.unicode() >= 48 && ch.unicode() <= 57)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 48 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 57)
             goto state_33;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_34;
-        if (ch.unicode() == 95)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 95)
             goto state_33;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_33;
         goto out;
     state_5:
         lastAcceptingPos = pos;
         token = QCss::INVALID;
         ch = next();
-        if (ch.unicode() >= 1 && ch.unicode() <= 9)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 1 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 9)
             goto state_35;
-        if (ch.unicode() == 11)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 11)
             goto state_35;
-        if (ch.unicode() >= 14 && ch.unicode() <= 38)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 14 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 38)
             goto state_35;
-        if (ch.unicode() == 39)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 39)
             goto state_36;
-        if (ch.unicode() >= 40 && ch.unicode() <= 91)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 40 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 91)
             goto state_35;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_37;
-        if (ch.unicode() >= 93 && ch.unicode() <= 96)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 93 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 96)
             goto state_35;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_35;
-        if (ch.unicode() >= 123)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 123)
             goto state_35;
         goto out;
     state_9:
@@ -249,27 +249,27 @@
         lastAcceptingPos = pos;
         token = QCss::MINUS;
         ch = next();
-        if (ch.unicode() == 45)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 45)
             goto state_38;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_22;
-        if (ch.unicode() == 95)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 95)
             goto state_24;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_24;
         goto out;
     state_12:
         lastAcceptingPos = pos;
         token = QCss::DOT;
         ch = next();
-        if (ch.unicode() >= 48 && ch.unicode() <= 57)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 48 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 57)
             goto state_39;
         goto out;
     state_13:
         lastAcceptingPos = pos;
         token = QCss::SLASH;
         ch = next();
-        if (ch.unicode() == 42) {
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 42) {
             token = handleCommentStart();
             goto found;
         }
@@ -278,24 +278,24 @@
         lastAcceptingPos = pos;
         token = QCss::NUMBER;
         ch = next();
-        if (ch.unicode() == 37)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 37)
             goto state_41;
-        if (ch.unicode() == 45)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 45)
             goto state_42;
-        if (ch.unicode() == 46)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 46)
             goto state_43;
-        if (ch.unicode() >= 48 && ch.unicode() <= 57)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 48 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 57)
             goto state_44;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_45;
-        if (ch.unicode() == 95)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 95)
             goto state_46;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_46;
         goto out;
     state_17:
         ch = next();
-        if (ch.unicode() == 33)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 33)
             goto state_47;
         goto out;
     state_19:
@@ -304,43 +304,43 @@
         goto out;
     state_20:
         ch = next();
-        if (ch.unicode() == 45)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 45)
             goto state_48;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_49;
-        if (ch.unicode() == 95)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 95)
             goto state_50;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_50;
         goto out;
     state_22:
         ch = next();
-        if (ch.unicode() >= 1 && ch.unicode() <= 9)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 1 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 9)
             goto state_51;
-        if (ch.unicode() == 11)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 11)
             goto state_51;
-        if (ch.unicode() >= 14 && ch.unicode() <= 47)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 14 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 47)
             goto state_51;
-        if (ch.unicode() >= 58 && ch.unicode() <= 96)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 58 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 96)
             goto state_51;
-        if (ch.unicode() >= 103)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 103)
             goto state_51;
         goto out;
     state_24:
         lastAcceptingPos = pos;
         token = QCss::IDENT;
         ch = next();
-        if (ch.unicode() == 40)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 40)
             goto state_52;
-        if (ch.unicode() == 45)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 45)
             goto state_53;
-        if (ch.unicode() >= 48 && ch.unicode() <= 57)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 48 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 57)
             goto state_53;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_54;
-        if (ch.unicode() == 95)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 95)
             goto state_53;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_53;
         goto out;
     state_25:
@@ -351,14 +351,14 @@
         lastAcceptingPos = pos;
         token = QCss::OR;
         ch = next();
-        if (ch.unicode() == 61) {
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 61) {
             token = QCss::DASHMATCH;
             goto found;
         }
         goto out;
     state_28:
         ch = next();
-        if (ch.unicode() == 61) {
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 61) {
             token = QCss::INCLUDES;
             goto found;
         }
@@ -367,42 +367,42 @@
         lastAcceptingPos = pos;
         token = QCss::S;
         ch = next();
-        if (ch.unicode() >= 9 && ch.unicode() <= 10)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 9 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 10)
             goto state_29;
-        if (ch.unicode() >= 12 && ch.unicode() <= 13)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 12 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 13)
             goto state_29;
-        if (ch.unicode() == 32)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 32)
             goto state_29;
-        if (ch.unicode() == 43)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 43)
             goto state_9;
-        if (ch.unicode() == 44)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 44)
             goto state_10;
-        if (ch.unicode() == 62)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 62)
             goto state_19;
-        if (ch.unicode() == 123)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 123)
             goto state_25;
         goto out;
     state_30:
         lastAcceptingPos = pos;
         token = QCss::INVALID;
         ch = next();
-        if (ch.unicode() >= 1 && ch.unicode() <= 9)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 1 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 9)
             goto state_30;
-        if (ch.unicode() == 11)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 11)
             goto state_30;
-        if (ch.unicode() >= 14 && ch.unicode() <= 33)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 14 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 33)
             goto state_30;
-        if (ch.unicode() == 34)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 34)
             goto state_31;
-        if (ch.unicode() >= 35 && ch.unicode() <= 91)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 35 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 91)
             goto state_30;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_32;
-        if (ch.unicode() >= 93 && ch.unicode() <= 96)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 93 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 96)
             goto state_30;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_30;
-        if (ch.unicode() >= 123)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 123)
             goto state_30;
         goto out;
     state_31:
@@ -411,72 +411,72 @@
         goto out;
     state_32:
         ch = next();
-        if (ch.unicode() >= 1 && ch.unicode() <= 9)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 1 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 9)
             goto state_57;
-        if (ch.unicode() == 10)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 10)
             goto state_58;
-        if (ch.unicode() == 11)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 11)
             goto state_57;
-        if (ch.unicode() == 12)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 12)
             goto state_59;
-        if (ch.unicode() == 13)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 13)
             goto state_60;
-        if (ch.unicode() >= 14 && ch.unicode() <= 47)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 14 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 47)
             goto state_57;
-        if (ch.unicode() >= 58 && ch.unicode() <= 96)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 58 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 96)
             goto state_57;
-        if (ch.unicode() >= 103)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 103)
             goto state_57;
         goto out;
     state_33:
         lastAcceptingPos = pos;
         token = QCss::HASH;
         ch = next();
-        if (ch.unicode() == 45)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 45)
             goto state_61;
-        if (ch.unicode() >= 48 && ch.unicode() <= 57)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 48 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 57)
             goto state_61;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_62;
-        if (ch.unicode() == 95)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 95)
             goto state_61;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_61;
         goto out;
     state_34:
         ch = next();
-        if (ch.unicode() >= 1 && ch.unicode() <= 9)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 1 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 9)
             goto state_63;
-        if (ch.unicode() == 11)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 11)
             goto state_63;
-        if (ch.unicode() >= 14 && ch.unicode() <= 47)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 14 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 47)
             goto state_63;
-        if (ch.unicode() >= 58 && ch.unicode() <= 96)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 58 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 96)
             goto state_63;
-        if (ch.unicode() >= 103)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 103)
             goto state_63;
         goto out;
     state_35:
         lastAcceptingPos = pos;
         token = QCss::INVALID;
         ch = next();
-        if (ch.unicode() >= 1 && ch.unicode() <= 9)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 1 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 9)
             goto state_35;
-        if (ch.unicode() == 11)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 11)
             goto state_35;
-        if (ch.unicode() >= 14 && ch.unicode() <= 38)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 14 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 38)
             goto state_35;
-        if (ch.unicode() == 39)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 39)
             goto state_36;
-        if (ch.unicode() >= 40 && ch.unicode() <= 91)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 40 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 91)
             goto state_35;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_37;
-        if (ch.unicode() >= 93 && ch.unicode() <= 96)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 93 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 96)
             goto state_35;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_35;
-        if (ch.unicode() >= 123)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 123)
             goto state_35;
         goto out;
     state_36:
@@ -485,26 +485,26 @@
         goto out;
     state_37:
         ch = next();
-        if (ch.unicode() >= 1 && ch.unicode() <= 9)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 1 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 9)
             goto state_64;
-        if (ch.unicode() == 10)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 10)
             goto state_65;
-        if (ch.unicode() == 11)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 11)
             goto state_64;
-        if (ch.unicode() == 12)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 12)
             goto state_66;
-        if (ch.unicode() == 13)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 13)
             goto state_67;
-        if (ch.unicode() >= 14 && ch.unicode() <= 47)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 14 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 47)
             goto state_64;
-        if (ch.unicode() >= 58 && ch.unicode() <= 96)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 58 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 96)
             goto state_64;
-        if (ch.unicode() >= 103)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 103)
             goto state_64;
         goto out;
     state_38:
         ch = next();
-        if (ch.unicode() == 62) {
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 62) {
             token = QCss::CDC;
             goto found;
         }
@@ -513,17 +513,17 @@
         lastAcceptingPos = pos;
         token = QCss::NUMBER;
         ch = next();
-        if (ch.unicode() == 37)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 37)
             goto state_41;
-        if (ch.unicode() == 45)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 45)
             goto state_42;
-        if (ch.unicode() >= 48 && ch.unicode() <= 57)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 48 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 57)
             goto state_69;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_45;
-        if (ch.unicode() == 95)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 95)
             goto state_46;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_46;
         goto out;
     state_41:
@@ -532,122 +532,122 @@
         goto out;
     state_42:
         ch = next();
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_45;
-        if (ch.unicode() == 95)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 95)
             goto state_46;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_46;
         goto out;
     state_43:
         ch = next();
-        if (ch.unicode() >= 48 && ch.unicode() <= 57)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 48 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 57)
             goto state_39;
         goto out;
     state_44:
         lastAcceptingPos = pos;
         token = QCss::NUMBER;
         ch = next();
-        if (ch.unicode() == 37)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 37)
             goto state_41;
-        if (ch.unicode() == 45)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 45)
             goto state_42;
-        if (ch.unicode() == 46)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 46)
             goto state_43;
-        if (ch.unicode() >= 48 && ch.unicode() <= 57)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 48 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 57)
             goto state_44;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_45;
-        if (ch.unicode() == 95)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 95)
             goto state_46;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_46;
         goto out;
     state_45:
         ch = next();
-        if (ch.unicode() >= 1 && ch.unicode() <= 9)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 1 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 9)
             goto state_70;
-        if (ch.unicode() == 11)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 11)
             goto state_70;
-        if (ch.unicode() >= 14 && ch.unicode() <= 47)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 14 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 47)
             goto state_70;
-        if (ch.unicode() >= 58 && ch.unicode() <= 96)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 58 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 96)
             goto state_70;
-        if (ch.unicode() >= 103)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 103)
             goto state_70;
         goto out;
     state_46:
         lastAcceptingPos = pos;
         token = QCss::LENGTH;
         ch = next();
-        if (ch.unicode() == 45)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 45)
             goto state_71;
-        if (ch.unicode() >= 48 && ch.unicode() <= 57)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 48 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 57)
             goto state_71;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_72;
-        if (ch.unicode() == 95)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 95)
             goto state_71;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_71;
         goto out;
     state_47:
         ch = next();
-        if (ch.unicode() == 45)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 45)
             goto state_73;
         goto out;
     state_48:
         ch = next();
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_49;
-        if (ch.unicode() == 95)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 95)
             goto state_50;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_50;
         goto out;
     state_49:
         ch = next();
-        if (ch.unicode() >= 1 && ch.unicode() <= 9)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 1 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 9)
             goto state_74;
-        if (ch.unicode() == 11)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 11)
             goto state_74;
-        if (ch.unicode() >= 14 && ch.unicode() <= 47)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 14 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 47)
             goto state_74;
-        if (ch.unicode() >= 58 && ch.unicode() <= 96)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 58 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 96)
             goto state_74;
-        if (ch.unicode() >= 103)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 103)
             goto state_74;
         goto out;
     state_50:
         lastAcceptingPos = pos;
         token = QCss::ATKEYWORD_SYM;
         ch = next();
-        if (ch.unicode() == 45)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 45)
             goto state_75;
-        if (ch.unicode() >= 48 && ch.unicode() <= 57)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 48 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 57)
             goto state_75;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_76;
-        if (ch.unicode() == 95)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 95)
             goto state_75;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_75;
         goto out;
     state_51:
         lastAcceptingPos = pos;
         token = QCss::IDENT;
         ch = next();
-        if (ch.unicode() == 40)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 40)
             goto state_52;
-        if (ch.unicode() == 45)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 45)
             goto state_53;
-        if (ch.unicode() >= 48 && ch.unicode() <= 57)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 48 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 57)
             goto state_53;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_54;
-        if (ch.unicode() == 95)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 95)
             goto state_53;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_53;
         goto out;
     state_52:
@@ -658,326 +658,326 @@
         lastAcceptingPos = pos;
         token = QCss::IDENT;
         ch = next();
-        if (ch.unicode() == 40)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 40)
             goto state_52;
-        if (ch.unicode() == 45)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 45)
             goto state_53;
-        if (ch.unicode() >= 48 && ch.unicode() <= 57)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 48 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 57)
             goto state_53;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_54;
-        if (ch.unicode() == 95)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 95)
             goto state_53;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_53;
         goto out;
     state_54:
         ch = next();
-        if (ch.unicode() >= 1 && ch.unicode() <= 9)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 1 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 9)
             goto state_77;
-        if (ch.unicode() == 11)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 11)
             goto state_77;
-        if (ch.unicode() >= 14 && ch.unicode() <= 47)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 14 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 47)
             goto state_77;
-        if (ch.unicode() >= 58 && ch.unicode() <= 96)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 58 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 96)
             goto state_77;
-        if (ch.unicode() >= 103)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 103)
             goto state_77;
         goto out;
     state_57:
         lastAcceptingPos = pos;
         token = QCss::INVALID;
         ch = next();
-        if (ch.unicode() >= 1 && ch.unicode() <= 9)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 1 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 9)
             goto state_30;
-        if (ch.unicode() == 11)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 11)
             goto state_30;
-        if (ch.unicode() >= 14 && ch.unicode() <= 33)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 14 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 33)
             goto state_30;
-        if (ch.unicode() == 34)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 34)
             goto state_31;
-        if (ch.unicode() >= 35 && ch.unicode() <= 91)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 35 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 91)
             goto state_30;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_32;
-        if (ch.unicode() >= 93 && ch.unicode() <= 96)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 93 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 96)
             goto state_30;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_30;
-        if (ch.unicode() >= 123)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 123)
             goto state_30;
         goto out;
     state_58:
         lastAcceptingPos = pos;
         token = QCss::INVALID;
         ch = next();
-        if (ch.unicode() >= 1 && ch.unicode() <= 9)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 1 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 9)
             goto state_30;
-        if (ch.unicode() == 11)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 11)
             goto state_30;
-        if (ch.unicode() >= 14 && ch.unicode() <= 33)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 14 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 33)
             goto state_30;
-        if (ch.unicode() == 34)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 34)
             goto state_31;
-        if (ch.unicode() >= 35 && ch.unicode() <= 91)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 35 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 91)
             goto state_30;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_32;
-        if (ch.unicode() >= 93 && ch.unicode() <= 96)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 93 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 96)
             goto state_30;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_30;
-        if (ch.unicode() >= 123)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 123)
             goto state_30;
         goto out;
     state_59:
         lastAcceptingPos = pos;
         token = QCss::INVALID;
         ch = next();
-        if (ch.unicode() >= 1 && ch.unicode() <= 9)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 1 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 9)
             goto state_30;
-        if (ch.unicode() == 11)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 11)
             goto state_30;
-        if (ch.unicode() >= 14 && ch.unicode() <= 33)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 14 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 33)
             goto state_30;
-        if (ch.unicode() == 34)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 34)
             goto state_31;
-        if (ch.unicode() >= 35 && ch.unicode() <= 91)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 35 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 91)
             goto state_30;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_32;
-        if (ch.unicode() >= 93 && ch.unicode() <= 96)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 93 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 96)
             goto state_30;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_30;
-        if (ch.unicode() >= 123)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 123)
             goto state_30;
         goto out;
     state_60:
         lastAcceptingPos = pos;
         token = QCss::INVALID;
         ch = next();
-        if (ch.unicode() >= 1 && ch.unicode() <= 9)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 1 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 9)
             goto state_30;
-        if (ch.unicode() == 10)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 10)
             goto state_78;
-        if (ch.unicode() == 11)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 11)
             goto state_30;
-        if (ch.unicode() >= 14 && ch.unicode() <= 33)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 14 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 33)
             goto state_30;
-        if (ch.unicode() == 34)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 34)
             goto state_31;
-        if (ch.unicode() >= 35 && ch.unicode() <= 91)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 35 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 91)
             goto state_30;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_32;
-        if (ch.unicode() >= 93 && ch.unicode() <= 96)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 93 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 96)
             goto state_30;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_30;
-        if (ch.unicode() >= 123)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 123)
             goto state_30;
         goto out;
     state_61:
         lastAcceptingPos = pos;
         token = QCss::HASH;
         ch = next();
-        if (ch.unicode() == 45)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 45)
             goto state_61;
-        if (ch.unicode() >= 48 && ch.unicode() <= 57)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 48 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 57)
             goto state_61;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_62;
-        if (ch.unicode() == 95)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 95)
             goto state_61;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_61;
         goto out;
     state_62:
         ch = next();
-        if (ch.unicode() >= 1 && ch.unicode() <= 9)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 1 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 9)
             goto state_79;
-        if (ch.unicode() == 11)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 11)
             goto state_79;
-        if (ch.unicode() >= 14 && ch.unicode() <= 47)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 14 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 47)
             goto state_79;
-        if (ch.unicode() >= 58 && ch.unicode() <= 96)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 58 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 96)
             goto state_79;
-        if (ch.unicode() >= 103)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 103)
             goto state_79;
         goto out;
     state_63:
         lastAcceptingPos = pos;
         token = QCss::HASH;
         ch = next();
-        if (ch.unicode() == 45)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 45)
             goto state_61;
-        if (ch.unicode() >= 48 && ch.unicode() <= 57)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 48 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 57)
             goto state_61;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_62;
-        if (ch.unicode() == 95)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 95)
             goto state_61;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_61;
         goto out;
     state_64:
         lastAcceptingPos = pos;
         token = QCss::INVALID;
         ch = next();
-        if (ch.unicode() >= 1 && ch.unicode() <= 9)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 1 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 9)
             goto state_35;
-        if (ch.unicode() == 11)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 11)
             goto state_35;
-        if (ch.unicode() >= 14 && ch.unicode() <= 38)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 14 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 38)
             goto state_35;
-        if (ch.unicode() == 39)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 39)
             goto state_36;
-        if (ch.unicode() >= 40 && ch.unicode() <= 91)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 40 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 91)
             goto state_35;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_37;
-        if (ch.unicode() >= 93 && ch.unicode() <= 96)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 93 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 96)
             goto state_35;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_35;
-        if (ch.unicode() >= 123)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 123)
             goto state_35;
         goto out;
     state_65:
         lastAcceptingPos = pos;
         token = QCss::INVALID;
         ch = next();
-        if (ch.unicode() >= 1 && ch.unicode() <= 9)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 1 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 9)
             goto state_35;
-        if (ch.unicode() == 11)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 11)
             goto state_35;
-        if (ch.unicode() >= 14 && ch.unicode() <= 38)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 14 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 38)
             goto state_35;
-        if (ch.unicode() == 39)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 39)
             goto state_36;
-        if (ch.unicode() >= 40 && ch.unicode() <= 91)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 40 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 91)
             goto state_35;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_37;
-        if (ch.unicode() >= 93 && ch.unicode() <= 96)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 93 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 96)
             goto state_35;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_35;
-        if (ch.unicode() >= 123)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 123)
             goto state_35;
         goto out;
     state_66:
         lastAcceptingPos = pos;
         token = QCss::INVALID;
         ch = next();
-        if (ch.unicode() >= 1 && ch.unicode() <= 9)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 1 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 9)
             goto state_35;
-        if (ch.unicode() == 11)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 11)
             goto state_35;
-        if (ch.unicode() >= 14 && ch.unicode() <= 38)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 14 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 38)
             goto state_35;
-        if (ch.unicode() == 39)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 39)
             goto state_36;
-        if (ch.unicode() >= 40 && ch.unicode() <= 91)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 40 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 91)
             goto state_35;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_37;
-        if (ch.unicode() >= 93 && ch.unicode() <= 96)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 93 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 96)
             goto state_35;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_35;
-        if (ch.unicode() >= 123)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 123)
             goto state_35;
         goto out;
     state_67:
         lastAcceptingPos = pos;
         token = QCss::INVALID;
         ch = next();
-        if (ch.unicode() >= 1 && ch.unicode() <= 9)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 1 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 9)
             goto state_35;
-        if (ch.unicode() == 10)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 10)
             goto state_80;
-        if (ch.unicode() == 11)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 11)
             goto state_35;
-        if (ch.unicode() >= 14 && ch.unicode() <= 38)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 14 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 38)
             goto state_35;
-        if (ch.unicode() == 39)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 39)
             goto state_36;
-        if (ch.unicode() >= 40 && ch.unicode() <= 91)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 40 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 91)
             goto state_35;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_37;
-        if (ch.unicode() >= 93 && ch.unicode() <= 96)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 93 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 96)
             goto state_35;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_35;
-        if (ch.unicode() >= 123)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 123)
             goto state_35;
         goto out;
     state_69:
         lastAcceptingPos = pos;
         token = QCss::NUMBER;
         ch = next();
-        if (ch.unicode() == 37)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 37)
             goto state_41;
-        if (ch.unicode() == 45)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 45)
             goto state_42;
-        if (ch.unicode() >= 48 && ch.unicode() <= 57)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 48 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 57)
             goto state_69;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_45;
-        if (ch.unicode() == 95)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 95)
             goto state_46;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_46;
         goto out;
     state_70:
         lastAcceptingPos = pos;
         token = QCss::LENGTH;
         ch = next();
-        if (ch.unicode() == 45)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 45)
             goto state_71;
-        if (ch.unicode() >= 48 && ch.unicode() <= 57)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 48 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 57)
             goto state_71;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_72;
-        if (ch.unicode() == 95)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 95)
             goto state_71;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_71;
         goto out;
     state_71:
         lastAcceptingPos = pos;
         token = QCss::LENGTH;
         ch = next();
-        if (ch.unicode() == 45)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 45)
             goto state_71;
-        if (ch.unicode() >= 48 && ch.unicode() <= 57)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 48 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 57)
             goto state_71;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_72;
-        if (ch.unicode() == 95)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 95)
             goto state_71;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_71;
         goto out;
     state_72:
         ch = next();
-        if (ch.unicode() >= 1 && ch.unicode() <= 9)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 1 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 9)
             goto state_81;
-        if (ch.unicode() == 11)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 11)
             goto state_81;
-        if (ch.unicode() >= 14 && ch.unicode() <= 47)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 14 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 47)
             goto state_81;
-        if (ch.unicode() >= 58 && ch.unicode() <= 96)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 58 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 96)
             goto state_81;
-        if (ch.unicode() >= 103)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 103)
             goto state_81;
         goto out;
     state_73:
         ch = next();
-        if (ch.unicode() == 45) {
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 45) {
             token = QCss::CDO;
             goto found;
         }
@@ -986,151 +986,151 @@
         lastAcceptingPos = pos;
         token = QCss::ATKEYWORD_SYM;
         ch = next();
-        if (ch.unicode() == 45)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 45)
             goto state_75;
-        if (ch.unicode() >= 48 && ch.unicode() <= 57)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 48 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 57)
             goto state_75;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_76;
-        if (ch.unicode() == 95)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 95)
             goto state_75;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_75;
         goto out;
     state_75:
         lastAcceptingPos = pos;
         token = QCss::ATKEYWORD_SYM;
         ch = next();
-        if (ch.unicode() == 45)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 45)
             goto state_75;
-        if (ch.unicode() >= 48 && ch.unicode() <= 57)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 48 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 57)
             goto state_75;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_76;
-        if (ch.unicode() == 95)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 95)
             goto state_75;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_75;
         goto out;
     state_76:
         ch = next();
-        if (ch.unicode() >= 1 && ch.unicode() <= 9)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 1 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 9)
             goto state_83;
-        if (ch.unicode() == 11)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 11)
             goto state_83;
-        if (ch.unicode() >= 14 && ch.unicode() <= 47)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 14 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 47)
             goto state_83;
-        if (ch.unicode() >= 58 && ch.unicode() <= 96)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 58 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 96)
             goto state_83;
-        if (ch.unicode() >= 103)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 103)
             goto state_83;
         goto out;
     state_77:
         lastAcceptingPos = pos;
         token = QCss::IDENT;
         ch = next();
-        if (ch.unicode() == 40)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 40)
             goto state_52;
-        if (ch.unicode() == 45)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 45)
             goto state_53;
-        if (ch.unicode() >= 48 && ch.unicode() <= 57)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 48 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 57)
             goto state_53;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_54;
-        if (ch.unicode() == 95)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 95)
             goto state_53;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_53;
         goto out;
     state_78:
         lastAcceptingPos = pos;
         token = QCss::INVALID;
         ch = next();
-        if (ch.unicode() >= 1 && ch.unicode() <= 9)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 1 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 9)
             goto state_30;
-        if (ch.unicode() == 11)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 11)
             goto state_30;
-        if (ch.unicode() >= 14 && ch.unicode() <= 33)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 14 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 33)
             goto state_30;
-        if (ch.unicode() == 34)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 34)
             goto state_31;
-        if (ch.unicode() >= 35 && ch.unicode() <= 91)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 35 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 91)
             goto state_30;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_32;
-        if (ch.unicode() >= 93 && ch.unicode() <= 96)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 93 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 96)
             goto state_30;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_30;
-        if (ch.unicode() >= 123)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 123)
             goto state_30;
         goto out;
     state_79:
         lastAcceptingPos = pos;
         token = QCss::HASH;
         ch = next();
-        if (ch.unicode() == 45)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 45)
             goto state_61;
-        if (ch.unicode() >= 48 && ch.unicode() <= 57)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 48 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 57)
             goto state_61;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_62;
-        if (ch.unicode() == 95)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 95)
             goto state_61;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_61;
         goto out;
     state_80:
         lastAcceptingPos = pos;
         token = QCss::INVALID;
         ch = next();
-        if (ch.unicode() >= 1 && ch.unicode() <= 9)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 1 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 9)
             goto state_35;
-        if (ch.unicode() == 11)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 11)
             goto state_35;
-        if (ch.unicode() >= 14 && ch.unicode() <= 38)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 14 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 38)
             goto state_35;
-        if (ch.unicode() == 39)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 39)
             goto state_36;
-        if (ch.unicode() >= 40 && ch.unicode() <= 91)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 40 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 91)
             goto state_35;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_37;
-        if (ch.unicode() >= 93 && ch.unicode() <= 96)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 93 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 96)
             goto state_35;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_35;
-        if (ch.unicode() >= 123)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 123)
             goto state_35;
         goto out;
     state_81:
         lastAcceptingPos = pos;
         token = QCss::LENGTH;
         ch = next();
-        if (ch.unicode() == 45)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 45)
             goto state_71;
-        if (ch.unicode() >= 48 && ch.unicode() <= 57)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 48 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 57)
             goto state_71;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_72;
-        if (ch.unicode() == 95)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 95)
             goto state_71;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_71;
         goto out;
     state_83:
         lastAcceptingPos = pos;
         token = QCss::ATKEYWORD_SYM;
         ch = next();
-        if (ch.unicode() == 45)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 45)
             goto state_75;
-        if (ch.unicode() >= 48 && ch.unicode() <= 57)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) >= 48 && RISCOS_DEREF_QCHAR(ch.unicode()) <= 57)
             goto state_75;
-        if (ch.unicode() == 92)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 92)
             goto state_76;
-        if (ch.unicode() == 95)
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) == 95)
             goto state_75;
-        if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') || (ch.unicode() >= 'A' && ch.unicode() <= 'Z') || ch.unicode() >= 256)
+        if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') || (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') || RISCOS_DEREF_QCHAR(ch.unicode()) >= 256)
             goto state_75;
         goto out;
     found:
