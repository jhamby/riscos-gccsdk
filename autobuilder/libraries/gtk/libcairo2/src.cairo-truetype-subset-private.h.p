--- src/cairo-truetype-subset-private.h.orig	2012-02-02 00:48:14.000000000 +0000
+++ src/cairo-truetype-subset-private.h	2013-08-09 15:18:55.197776115 +0100
@@ -120,7 +120,7 @@
     int16_t     font_direction_hint;
     int16_t     index_to_loc_format;
     int16_t     glyph_data_format;
-} tt_head_t;
+} __attribute__((packed)) tt_head_t;
 
 typedef struct _tt_hhea {
     int16_t     version_1;
@@ -137,7 +137,7 @@
     int16_t     reserved[5];
     int16_t     metric_data_format;
     uint16_t    num_hmetrics;
-} tt_hhea_t;
+} __attribute__((packed)) tt_hhea_t;
 
 typedef struct _tt_maxp {
     int16_t     version_1;
@@ -165,14 +165,14 @@
     uint16_t name;
     uint16_t length;
     uint16_t offset;
-} tt_name_record_t;
+} __attribute__((packed)) tt_name_record_t;
 
 typedef struct _tt_name {
     uint16_t   format;
     uint16_t   num_records;
     uint16_t   strings_offset;
     tt_name_record_t records[1];
-} tt_name_t;
+} __attribute__((packed)) tt_name_t;
 
 
 /* bitmask for fsSelection field */
@@ -205,7 +205,7 @@
     int16_t           num_contours;
     int8_t            data[8];
     tt_composite_glyph_t glyph;
-} tt_glyph_data_t;
+} __attribute__((packed)) tt_glyph_data_t;
 
 #endif /* CAIRO_HAS_FONT_SUBSET */
 
