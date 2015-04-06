--- src/3rdparty/harfbuzz/src/harfbuzz-shaper.h.orig	2013-11-13 00:57:48.000000000 +0000
+++ src/3rdparty/harfbuzz/src/harfbuzz-shaper.h	2013-11-13 01:00:28.569379765 +0000
@@ -136,7 +136,7 @@
     hb_bitfield wordStart        : 1;
     hb_bitfield wordEnd          : 1;
     hb_bitfield mandatoryBreak   : 1;
-} HB_CharAttributes;
+} __attribute__((packed)) HB_CharAttributes;
 
 void HB_GetTailoredCharAttributes(const HB_UChar16 *string, hb_uint32 stringLength,
                                   const HB_ScriptItem *items, hb_uint32 numItems,
