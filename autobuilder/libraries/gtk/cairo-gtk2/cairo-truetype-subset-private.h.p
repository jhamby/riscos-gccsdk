--- src/cairo-truetype-subset-private.h.orig	2012-04-23 21:12:04.000000000 +0100
+++ src/cairo-truetype-subset-private.h	2012-04-23 21:15:23.000000000 +0100
@@ -119,7 +119,7 @@
     int16_t     font_direction_hint;
     int16_t     index_to_loc_format;
     int16_t     glyph_data_format;
-} tt_head_t;
+} __attribute__((packed)) tt_head_t;
 
 typedef struct _tt_hhea {
     int16_t     version_1;
@@ -171,7 +171,7 @@
     uint16_t   num_records;
     uint16_t   strings_offset;
     tt_name_record_t records[1];
-} tt_name_t;
+} __attribute__((packed)) tt_name_t;
 
 
 
@@ -192,7 +192,7 @@
     int16_t           num_contours;
     int8_t            data[8];
     tt_composite_glyph_t glyph;
-} tt_glyph_data_t;
+} __attribute__((packed)) tt_glyph_data_t;
 
 #endif /* CAIRO_HAS_FONT_SUBSET */
 
