--- src/widgets/styles/qcommonstyle.cpp.orig	2014-05-15 18:12:12.000000000 +0100
+++ src/widgets/styles/qcommonstyle.cpp	2014-05-27 19:55:16.350522842 +0100
@@ -4920,7 +4920,7 @@
         const QPlatformTheme *theme = QGuiApplicationPrivate::platformTheme();
         const QPlatformTheme::ThemeHint hintType = QPlatformTheme::PasswordMaskCharacter;
         const QVariant hint = theme ? theme->themeHint(hintType) : QPlatformTheme::defaultThemeHint(hintType);
-        ret = hint.toChar().unicode();
+        ret = RISCOS_DEREF_QCHAR(hint.toChar().unicode());
         break;
     }
     case SH_ToolBox_SelectedPageTitleBold:
