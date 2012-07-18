--- mono/metadata/locales.c.orig	2011-12-19 21:10:24.000000000 +0000
+++ mono/metadata/locales.c	2012-07-18 20:49:27.000000000 +0100
@@ -345,6 +345,97 @@
 	return entry;
 }
 
+#ifdef __riscos__
+
+typedef struct locale_entry
+{
+	const char *riscos_territory_name;
+	const char *culture_name;
+} locale_entry;
+
+static const locale_entry locale_table[] =
+{
+	{ "uk", "en-GB" },
+	{ "italy", "it-IT" },
+	{ "spain", "es-ES" },
+	{ "france", "fr-FR" },
+	{ "germany", "de-DE" },
+	{ "portugal", "pt-PT" },
+	{ "greece", "el-GR" },
+	{ "sweden", "sv-SE" },
+	{ "finland", "fi-FI" },
+	{ "denmark", "da-DK" },
+	{ "norway", "no" },
+	{ "iceland", "is-IS" },
+	{ "canada", "en-CA" },
+	{ "turkey", "tr-TR" },
+	{ "arabic", "ar" },
+	{ "ireland", "en-IE" },
+	{ "hongkong", "zh-HK" },
+	{ "russia", "ru-RU" },
+	{ "israel", "he-IL" },
+	{ "mexico", "es-MX" },
+	{ "australia", "en-AU" },
+	{ "austria", "de-AT" },
+	{ "belgium", "nl-BE" },
+	{ "japan", "ja-JP" },
+	{ "netherlands", "nl-NL" },
+	{ "switzerland", "de-CH" }, /* Could also be italian or french */
+	{ "usa", "en-US" },
+	{ "china", "zh-CN" },
+	{ "brazil", "pt-BR" },
+	{ "safrica", "af-ZA" },
+	{ "korea", "ko-KR" },
+	{ "taiwan", "zh-TW" },
+	{ "faroe", "fo-FO" },
+	{ "albania", "sq-AL" },
+	{ "bulgaria", "bg-BG" },
+	{ "czech", "cs-CZ" },
+	{ "farsi", "fa" },
+	{ "gujarati", "gu" },
+	{ "estonia", "et-EE" },
+	{ "hungary", "hu-HU" },
+	{ "latvia", "lv-LV" },
+	{ "lithuania", "lt-LT" },
+	{ "macedonia", "mk" },
+	{ "poland", "pl-PL" },
+	{ "punjabi", "pa" },
+	{ "romania", "ro-RO" },
+	{ "serbocroat", "hr" },
+	{ "slovak", "sk" },
+	{ "slovene", "sl" },
+	{ "tamil", "ta" },
+	{ "ukraine", "uk" },
+	{ "swiss1", "fr-CH" },
+	{ "swiss2", "de-CH" },
+	{ "swiss3", "it-CH" }
+};
+
+/* Translate a RISC OS country as returned by setlocale in UnixLib and
+ * listed here:
+ * 
+ * http://www.riscosopen.org/wiki/documentation/show/Territory%20Numbers
+ * 
+ * into a culture name as listed here:
+ * 
+ * http://msdn.microsoft.com/en-us/library/system.globalization.cultureinfo(v=vs.71).aspx
+ * 
+ * Without this translation, Mono interprets UK as Ukraine rather than United Kingdom.
+ * In fact, for United Kingdom, Mono expects "en-GB".
+ */
+static const char *riscos_country_to_culture (const char *riscos_country)
+{
+	int i;
+
+	for (i = 0; i < sizeof (locale_table) / sizeof (locale_entry); i++)
+		if (stricmp (locale_table[i].riscos_territory_name, riscos_country) == 0)
+			return locale_table[i].culture_name;
+
+	return NULL;
+}
+
+#endif
+
 /*
  * The following two methods are modified from the ICU source code. (http://oss.software.ibm.com/icu)
  * Copyright (c) 1995-2003 International Business Machines Corporation and others
@@ -360,6 +451,10 @@
 		posix_locale = g_getenv("LANG");
 		if (posix_locale == 0) {
 			posix_locale = setlocale(LC_ALL, NULL);
+#ifdef __riscos__
+			if (posix_locale != NULL)
+				posix_locale = riscos_country_to_culture (posix_locale);
+#endif
 		}
 	}
 
