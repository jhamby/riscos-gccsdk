Index: pngread.c
===================================================================
--- pngread.c
+++ pngread.c
@@ -437,6 +437,11 @@
 #ifdef PNG_READ_zTXt_SUPPORTED
       PNG_CONST PNG_zTXt;
 #endif
+#ifdef PNG_READ_APNG_SUPPORTED
+      PNG_CONST PNG_acTL;
+      PNG_CONST PNG_fcTL;
+      PNG_CONST PNG_fdAT;
+#endif
 #endif /* PNG_USE_LOCAL_ARRAYS */
       png_uint_32 length = png_read_chunk_header(png_ptr);
       PNG_CONST png_bytep chunk_name = png_ptr->chunk_name;
@@ -481,6 +486,9 @@
                   !(png_ptr->mode & PNG_HAVE_PLTE))
             png_error(png_ptr, "Missing PLTE before IDAT");
 
+#ifdef PNG_READ_APNG_SUPPORTED
+         png_have_info(png_ptr, info_ptr);
+#endif
          png_ptr->idat_size = length;
          png_ptr->mode |= PNG_HAVE_IDAT;
          break;
@@ -553,12 +561,97 @@
       else if (!png_memcmp(chunk_name, png_iTXt, 4))
          png_handle_iTXt(png_ptr, info_ptr, length);
 #endif
+#ifdef PNG_READ_APNG_SUPPORTED
+      else if (!png_memcmp(chunk_name, png_acTL, 4))
+         png_handle_acTL(png_ptr, info_ptr, length);
+      else if (!png_memcmp(chunk_name, png_fcTL, 4))
+         png_handle_fcTL(png_ptr, info_ptr, length);
+      else if (!png_memcmp(chunk_name, png_fdAT, 4))
+         png_handle_fdAT(png_ptr, info_ptr, length);
+#endif
       else
          png_handle_unknown(png_ptr, info_ptr, length);
    }
 }
 #endif /* PNG_SEQUENTIAL_READ_SUPPORTED */
 
+#ifdef PNG_READ_APNG_SUPPORTED
+void PNGAPI
+png_read_frame_head(png_structp png_ptr, png_infop info_ptr)
+{
+    png_byte have_chunk_after_DAT; /* after IDAT or after fdAT */
+    
+    png_debug(0, "Reading frame head");
+    
+    if (!(png_ptr->mode & PNG_HAVE_acTL))
+        png_error(png_ptr, "attempt to png_read_frame_head() but "
+                           "no acTL present");
+    
+    /* do nothing for the main IDAT */
+    if (png_ptr->num_frames_read == 0)
+        return;
+    
+    png_crc_finish(png_ptr, 0); /* CRC from last IDAT or fdAT chunk */
+    
+    png_read_reset(png_ptr);
+    png_ptr->mode &= ~PNG_HAVE_fcTL;
+    
+    have_chunk_after_DAT = 0;
+    for (;;)
+    {
+#ifdef PNG_USE_LOCAL_ARRAYS
+        PNG_IDAT;
+        PNG_fdAT;
+        PNG_fcTL;
+#endif
+        png_byte chunk_length[4];
+        png_uint_32 length;
+        
+        png_read_data(png_ptr, chunk_length, 4);
+        length = png_get_uint_31(png_ptr, chunk_length);
+        
+        png_reset_crc(png_ptr);
+        png_crc_read(png_ptr, png_ptr->chunk_name, 4);
+        
+        if (!png_memcmp(png_ptr->chunk_name, png_IDAT, 4))
+        {
+            /* discard trailing IDATs for the first frame */
+            if (have_chunk_after_DAT || png_ptr->num_frames_read > 1)
+                png_error(png_ptr, "png_read_frame_head(): out of place IDAT");
+            png_crc_finish(png_ptr, length);
+        }
+        else if (!png_memcmp(png_ptr->chunk_name, png_fcTL, 4))
+        {
+            png_handle_fcTL(png_ptr, info_ptr, length);
+            have_chunk_after_DAT = 1;
+        }
+        else if (!png_memcmp(png_ptr->chunk_name, png_fdAT, 4))
+        {
+            png_ensure_sequence_number(png_ptr, length);
+            
+            /* discard trailing fdATs for frames other than the first */
+            if (!have_chunk_after_DAT && png_ptr->num_frames_read > 1)
+                png_crc_finish(png_ptr, length - 4);
+            else if(png_ptr->mode & PNG_HAVE_fcTL)
+            {
+                png_ptr->idat_size = length - 4;
+                png_ptr->mode |= PNG_HAVE_IDAT;
+                
+                break;
+            }
+            else
+                png_error(png_ptr, "png_read_frame_head(): out of place fdAT");
+        }
+        else
+        {
+            png_warning(png_ptr, "Skipped (ignored) a chunk "
+                                 "between APNG chunks");
+            png_crc_finish(png_ptr, length);
+        }
+    }
+}
+#endif /* PNG_READ_APNG_SUPPORTED */
+
 /* Optional call to update the users info_ptr structure */
 void PNGAPI
 png_read_update_info(png_structp png_ptr, png_infop info_ptr)
@@ -599,6 +692,10 @@
 png_read_row(png_structp png_ptr, png_bytep row, png_bytep dsp_row)
 {
    PNG_CONST PNG_IDAT;
+#ifdef PNG_READ_APNG_SUPPORTED
+   PNG_CONST PNG_fdAT;
+   PNG_CONST PNG_IEND;
+#endif
    PNG_CONST int png_pass_dsp_mask[7] = {0xff, 0x0f, 0xff, 0x33, 0xff, 0x55,
       0xff};
    PNG_CONST int png_pass_mask[7] = {0x80, 0x08, 0x88, 0x22, 0xaa, 0x55, 0xff};
@@ -734,13 +831,39 @@
    {
       if (!(png_ptr->zstream.avail_in))
       {
-         while (!png_ptr->idat_size)
+         png_uint_32 bytes_to_skip = 0;
+
+         while (!png_ptr->idat_size || bytes_to_skip != 0)
          {
-            png_crc_finish(png_ptr, 0);
+            png_crc_finish(png_ptr, bytes_to_skip);
+            bytes_to_skip = 0;
 
             png_ptr->idat_size = png_read_chunk_header(png_ptr);
+#ifdef PNG_READ_APNG_SUPPORTED
+            if (png_ptr->num_frames_read == 0)
+            {
+#endif
             if (png_memcmp(png_ptr->chunk_name, png_IDAT, 4))
                png_error(png_ptr, "Not enough image data");
+#ifdef PNG_READ_APNG_SUPPORTED
+            }
+            else
+            {
+               if (!png_memcmp(png_ptr->chunk_name, png_IEND, 4))
+                  png_error(png_ptr, "Not enough image data");
+               if (png_memcmp(png_ptr->chunk_name, png_fdAT, 4))
+               {
+                  png_warning(png_ptr, "Skipped (ignored) a chunk "
+                                       "between APNG chunks");
+                  bytes_to_skip = png_ptr->idat_size;
+                  continue;
+               }
+               
+               png_ensure_sequence_number(png_ptr, png_ptr->idat_size);
+               
+               png_ptr->idat_size -= 4;
+            }
+#endif
          }
          png_ptr->zstream.avail_in = (uInt)png_ptr->zbuf_size;
          png_ptr->zstream.next_in = png_ptr->zbuf;
@@ -758,6 +881,9 @@
             png_error(png_ptr, "Extra compressed data");
          png_ptr->mode |= PNG_AFTER_IDAT;
          png_ptr->flags |= PNG_FLAG_ZLIB_FINISHED;
+#ifdef PNG_READ_APNG_SUPPORTED
+         png_ptr->num_frames_read++;
+#endif
          break;
       }
       if (ret != Z_OK)
@@ -1015,6 +1141,11 @@
 #ifdef PNG_READ_zTXt_SUPPORTED
       PNG_CONST PNG_zTXt;
 #endif
+#ifdef PNG_READ_APNG_SUPPORTED
+      PNG_CONST PNG_acTL;
+      PNG_CONST PNG_fcTL;
+      PNG_CONST PNG_fdAT;
+#endif
 #endif /* PNG_USE_LOCAL_ARRAYS */
       png_uint_32 length = png_read_chunk_header(png_ptr);
       PNG_CONST png_bytep chunk_name = png_ptr->chunk_name;
@@ -1115,6 +1246,14 @@
       else if (!png_memcmp(chunk_name, png_iTXt, 4))
          png_handle_iTXt(png_ptr, info_ptr, length);
 #endif
+#ifdef PNG_READ_APNG_SUPPORTED
+      else if (!png_memcmp(chunk_name, png_acTL, 4))
+         png_handle_acTL(png_ptr, info_ptr, length);
+      else if (!png_memcmp(chunk_name, png_fcTL, 4))
+         png_handle_fcTL(png_ptr, info_ptr, length);
+      else if (!png_memcmp(chunk_name, png_fdAT, 4))
+         png_handle_fdAT(png_ptr, info_ptr, length);
+#endif
       else
          png_handle_unknown(png_ptr, info_ptr, length);
    } while (!(png_ptr->mode & PNG_HAVE_IEND));
Index: pngget.c
===================================================================
--- pngget.c
+++ pngget.c
@@ -842,6 +842,167 @@
 }
 #endif
 
+#ifdef PNG_APNG_SUPPORTED
+png_uint_32 PNGAPI
+png_get_acTL(png_structp png_ptr, png_infop info_ptr,
+             png_uint_32 *num_frames, png_uint_32 *num_plays)
+{
+    png_debug1(1, "in %s retrieval function", "acTL");
+    
+    if (png_ptr != NULL && info_ptr != NULL &&
+        (info_ptr->valid & PNG_INFO_acTL) &&
+        num_frames != NULL && num_plays != NULL)
+    {
+        *num_frames = info_ptr->num_frames;
+        *num_plays = info_ptr->num_plays;
+        return (1);
+    }
+    
+    return (0);
+}
+
+png_uint_32 PNGAPI
+png_get_num_frames(png_structp png_ptr, png_infop info_ptr)
+{
+    png_debug(1, "in png_get_num_frames()");
+    
+    if (png_ptr != NULL && info_ptr != NULL)
+        return (info_ptr->num_frames);
+    return (0);
+}
+
+png_uint_32 PNGAPI
+png_get_num_plays(png_structp png_ptr, png_infop info_ptr)
+{
+    png_debug(1, "in png_get_num_plays()");
+    
+    if (png_ptr != NULL && info_ptr != NULL)
+        return (info_ptr->num_plays);
+    return (0);
+}
+
+png_uint_32 PNGAPI
+png_get_next_frame_fcTL(png_structp png_ptr, png_infop info_ptr,
+             png_uint_32 *width, png_uint_32 *height,
+             png_uint_32 *x_offset, png_uint_32 *y_offset,
+             png_uint_16 *delay_num, png_uint_16 *delay_den,
+             png_byte *dispose_op, png_byte *blend_op)
+{
+    png_debug1(1, "in %s retrieval function", "fcTL");
+    
+    if (png_ptr != NULL && info_ptr != NULL &&
+        (info_ptr->valid & PNG_INFO_fcTL) &&
+        width != NULL && height != NULL && 
+        x_offset != NULL && x_offset != NULL && 
+        delay_num != NULL && delay_den != NULL &&
+        dispose_op != NULL && blend_op != NULL)
+    {
+        *width = info_ptr->next_frame_width;
+        *height = info_ptr->next_frame_height;
+        *x_offset = info_ptr->next_frame_x_offset;
+        *y_offset = info_ptr->next_frame_y_offset;
+        *delay_num = info_ptr->next_frame_delay_num;
+        *delay_den = info_ptr->next_frame_delay_den;
+        *dispose_op = info_ptr->next_frame_dispose_op;
+        *blend_op = info_ptr->next_frame_blend_op;
+        return (1);
+    }
+    
+    return (0);
+}
+
+png_uint_32 PNGAPI
+png_get_next_frame_width(png_structp png_ptr, png_infop info_ptr)
+{
+    png_debug(1, "in png_get_next_frame_width()");
+    
+    if (png_ptr != NULL && info_ptr != NULL)
+        return (info_ptr->next_frame_width);
+    return (0);
+}
+
+png_uint_32 PNGAPI
+png_get_next_frame_height(png_structp png_ptr, png_infop info_ptr)
+{
+    png_debug(1, "in png_get_next_frame_height()");
+    
+    if (png_ptr != NULL && info_ptr != NULL)
+        return (info_ptr->next_frame_height);
+    return (0);
+}
+
+png_uint_32 PNGAPI
+png_get_next_frame_x_offset(png_structp png_ptr, png_infop info_ptr)
+{
+    png_debug(1, "in png_get_next_frame_x_offset()");
+    
+    if (png_ptr != NULL && info_ptr != NULL)
+        return (info_ptr->next_frame_x_offset);
+    return (0);
+}
+
+png_uint_32 PNGAPI
+png_get_next_frame_y_offset(png_structp png_ptr, png_infop info_ptr)
+{
+    png_debug(1, "in png_get_next_frame_y_offset()");
+    
+    if (png_ptr != NULL && info_ptr != NULL)
+        return (info_ptr->next_frame_y_offset);
+    return (0);
+}
+
+png_uint_16 PNGAPI
+png_get_next_frame_delay_num(png_structp png_ptr, png_infop info_ptr)
+{
+    png_debug(1, "in png_get_next_frame_delay_num()");
+    
+    if (png_ptr != NULL && info_ptr != NULL)
+        return (info_ptr->next_frame_delay_num);
+    return (0);
+}
+
+png_uint_16 PNGAPI
+png_get_next_frame_delay_den(png_structp png_ptr, png_infop info_ptr)
+{
+    png_debug(1, "in png_get_next_frame_delay_den()");
+    
+    if (png_ptr != NULL && info_ptr != NULL)
+        return (info_ptr->next_frame_delay_den);
+    return (0);
+}
+
+png_byte PNGAPI
+png_get_next_frame_dispose_op(png_structp png_ptr, png_infop info_ptr)
+{
+    png_debug(1, "in png_get_next_frame_dispose_op()");
+    
+    if (png_ptr != NULL && info_ptr != NULL)
+        return (info_ptr->next_frame_dispose_op);
+    return (0);
+}
+
+png_byte PNGAPI
+png_get_next_frame_blend_op(png_structp png_ptr, png_infop info_ptr)
+{
+    png_debug(1, "in png_get_next_frame_blend_op()");
+    
+    if (png_ptr != NULL && info_ptr != NULL)
+        return (info_ptr->next_frame_blend_op);
+    return (0);
+}
+
+png_byte PNGAPI
+png_get_first_frame_is_hidden(png_structp png_ptr, png_infop info_ptr)
+{
+    png_debug(1, "in png_first_frame_is_hidden()");
+    
+    if (png_ptr != NULL)
+       return (png_byte)(png_ptr->apng_flags & PNG_FIRST_FRAME_HIDDEN);
+    
+    return 0;
+}
+#endif /* PNG_APNG_SUPPORTED */
+
 #ifdef PNG_UNKNOWN_CHUNKS_SUPPORTED
 png_uint_32 PNGAPI
 png_get_unknown_chunks(png_structp png_ptr, png_infop info_ptr,
Index: png.c
===================================================================
--- png.c
+++ png.c
@@ -56,6 +56,11 @@
 PNG_tIME;
 PNG_tRNS;
 PNG_zTXt;
+#ifdef PNG_APNG_SUPPORTED
+PNG_acTL;
+PNG_fcTL;
+PNG_fdAT;
+#endif
 
 #ifdef PNG_READ_SUPPORTED
 /* Arrays to facilitate easy interlacing - use pass (0 - 6) as index */
Index: png.h
===================================================================
--- png.h
+++ png.h
@@ -1038,6 +1038,19 @@
    png_fixed_point int_y_blue PNG_DEPSTRUCT;
 #endif
 
+#ifdef PNG_APNG_SUPPORTED
+   png_uint_32 num_frames; /* including default image */
+   png_uint_32 num_plays;
+   png_uint_32 next_frame_width;
+   png_uint_32 next_frame_height;
+   png_uint_32 next_frame_x_offset;
+   png_uint_32 next_frame_y_offset;
+   png_uint_16 next_frame_delay_num;
+   png_uint_16 next_frame_delay_den;
+   png_byte next_frame_dispose_op;
+   png_byte next_frame_blend_op;
+#endif
+
 } png_info;
 
 typedef png_info FAR * png_infop;
@@ -1139,6 +1152,10 @@
 #define PNG_INFO_sPLT 0x2000   /* ESR, 1.0.6 */
 #define PNG_INFO_sCAL 0x4000   /* ESR, 1.0.6 */
 #define PNG_INFO_IDAT 0x8000L  /* ESR, 1.0.6 */
+#ifdef PNG_APNG_SUPPORTED
+#define PNG_INFO_acTL 0x10000L
+#define PNG_INFO_fcTL 0x20000L
+#endif
 
 /* This is used for the transformation routines, as some of them
  * change these values for the row.  It also should enable using
@@ -1179,6 +1196,10 @@
 typedef void (PNGAPI *png_progressive_end_ptr) PNGARG((png_structp, png_infop));
 typedef void (PNGAPI *png_progressive_row_ptr) PNGARG((png_structp, png_bytep,
    png_uint_32, int));
+#ifdef PNG_APNG_SUPPORTED
+typedef void (PNGAPI *png_progressive_frame_ptr) PNGARG((png_structp, 
+   png_uint_32));
+#endif
 #endif
 
 #if defined(PNG_READ_USER_TRANSFORM_SUPPORTED) || \
@@ -1524,6 +1545,39 @@
    png_uint_32 user_height_max PNG_DEPSTRUCT;
 #endif
 
+#ifdef PNG_APNG_SUPPORTED
+   png_uint_32 apng_flags;
+   png_uint_32 next_seq_num;         /* next fcTL/fdAT chunk sequence number */
+   png_uint_32 first_frame_width;
+   png_uint_32 first_frame_height;
+
+#ifdef PNG_READ_APNG_SUPPORTED
+   png_uint_32 num_frames_read;      /* incremented after all image data of */
+                                     /* a frame is read */
+#ifdef PNG_PROGRESSIVE_READ_SUPPORTED
+   png_progressive_frame_ptr frame_info_fn; /* frame info read callback */
+   png_progressive_frame_ptr frame_end_fn;  /* frame data read callback */
+#endif
+#endif
+
+#ifdef PNG_WRITE_APNG_SUPPORTED
+   png_uint_32 num_frames_to_write;
+   png_uint_32 num_frames_written;
+#endif
+
+/* For png_struct.apng_flags: */
+#define PNG_FIRST_FRAME_HIDDEN       0x0001
+
+/* dispose_op flags from inside fcTL */
+#define PNG_DISPOSE_OP_NONE        0x00
+#define PNG_DISPOSE_OP_BACKGROUND  0x01
+#define PNG_DISPOSE_OP_PREVIOUS    0x02
+
+/* blend_op flags from inside fcTL */
+#define PNG_BLEND_OP_SOURCE        0x00
+#define PNG_BLEND_OP_OVER          0x01
+#endif /* PNG_APNG_SUPPORTED */
+
 /* New member added in libpng-1.0.25 and 1.2.17 */
 #ifdef PNG_UNKNOWN_CHUNKS_SUPPORTED
    /* Storage for unknown chunk that the library doesn't recognize. */
@@ -1858,6 +1912,18 @@
 extern PNG_EXPORT(void,png_write_image) PNGARG((png_structp png_ptr,
    png_bytepp image));
 
+#ifdef PNG_WRITE_APNG_SUPPORTED
+extern PNG_EXPORT (void,png_write_frame_head) PNGARG((png_structp png_ptr,
+   png_infop png_info, png_bytepp row_pointers,
+   png_uint_32 width, png_uint_32 height,
+   png_uint_32 x_offset, png_uint_32 y_offset, 
+   png_uint_16 delay_num, png_uint_16 delay_den, png_byte dispose_op,
+   png_byte blend_op));
+
+extern PNG_EXPORT (void,png_write_frame_tail) PNGARG((png_structp png_ptr,
+   png_infop png_info));
+#endif
+   
 /* Writes the end of the PNG file. */
 extern PNG_EXPORT(void,png_write_end) PNGARG((png_structp png_ptr,
    png_infop info_ptr));
@@ -2111,6 +2177,11 @@
    png_voidp progressive_ptr,
    png_progressive_info_ptr info_fn, png_progressive_row_ptr row_fn,
    png_progressive_end_ptr end_fn));
+#ifdef PNG_READ_APNG_SUPPORTED
+extern PNG_EXPORT(void,png_set_progressive_frame_fn) PNGARG((png_structp png_ptr,
+   png_progressive_frame_ptr frame_info_fn,
+   png_progressive_frame_ptr frame_end_fn));
+#endif
 
 /* Returns the user pointer associated with the push read functions */
 extern PNG_EXPORT(png_voidp,png_get_progressive_ptr)
@@ -2551,6 +2622,59 @@
 #endif
 #endif /* PNG_sCAL_SUPPORTED || PNG_WRITE_sCAL_SUPPORTED */
 
+#ifdef PNG_APNG_SUPPORTED
+extern PNG_EXPORT(png_uint_32,png_get_acTL) PNGARG((png_structp png_ptr,
+   png_infop info_ptr, png_uint_32 *num_frames, png_uint_32 *num_plays));
+extern PNG_EXPORT(png_uint_32,png_set_acTL) PNGARG((png_structp png_ptr, 
+   png_infop info_ptr, png_uint_32 num_frames, png_uint_32 num_plays));
+extern PNG_EXPORT(png_uint_32,png_get_num_frames) PNGARG((png_structp png_ptr,
+   png_infop info_ptr));
+extern PNG_EXPORT(png_uint_32,png_get_num_plays) 
+   PNGARG((png_structp png_ptr, png_infop info_ptr));
+
+extern PNG_EXPORT(png_uint_32,png_get_next_frame_fcTL) 
+   PNGARG((png_structp png_ptr, png_infop info_ptr, png_uint_32 *width, 
+   png_uint_32 *height, png_uint_32 *x_offset, png_uint_32 *y_offset, 
+   png_uint_16 *delay_num, png_uint_16 *delay_den, png_byte *dispose_op,
+   png_byte *blend_op));
+extern PNG_EXPORT(png_uint_32,png_set_next_frame_fcTL) 
+   PNGARG((png_structp png_ptr, png_infop info_ptr, png_uint_32 width, 
+   png_uint_32 height, png_uint_32 x_offset, png_uint_32 y_offset, 
+   png_uint_16 delay_num, png_uint_16 delay_den, png_byte dispose_op,
+   png_byte blend_op));
+extern PNG_EXPORT(void,png_ensure_fcTL_is_valid)
+   PNGARG((png_structp png_ptr,
+   png_uint_32 width, png_uint_32 height,
+   png_uint_32 x_offset, png_uint_32 y_offset,
+   png_uint_16 delay_num, png_uint_16 delay_den,
+   png_byte dispose_op, png_byte blend_op));
+extern PNG_EXPORT(png_uint_32,png_get_next_frame_width)
+   PNGARG((png_structp png_ptr, png_infop info_ptr));
+extern PNG_EXPORT(png_uint_32,png_get_next_frame_height)
+   PNGARG((png_structp png_ptr, png_infop info_ptr));
+extern PNG_EXPORT(png_uint_32,png_get_next_frame_x_offset)
+   PNGARG((png_structp png_ptr, png_infop info_ptr));
+extern PNG_EXPORT(png_uint_32,png_get_next_frame_y_offset)
+   PNGARG((png_structp png_ptr, png_infop info_ptr));
+extern PNG_EXPORT(png_uint_16,png_get_next_frame_delay_num)
+   PNGARG((png_structp png_ptr, png_infop info_ptr));
+extern PNG_EXPORT(png_uint_16,png_get_next_frame_delay_den)
+   PNGARG((png_structp png_ptr, png_infop info_ptr));
+extern PNG_EXPORT(png_byte,png_get_next_frame_dispose_op)
+   PNGARG((png_structp png_ptr, png_infop info_ptr));
+extern PNG_EXPORT(png_byte,png_get_next_frame_blend_op)
+   PNGARG((png_structp png_ptr, png_infop info_ptr));
+extern PNG_EXPORT(png_byte,png_get_first_frame_is_hidden)
+   PNGARG((png_structp png_ptr, png_infop info_ptr));
+extern PNG_EXPORT(png_uint_32,png_set_first_frame_is_hidden)
+   PNGARG((png_structp png_ptr, png_infop info_ptr, png_byte is_hidden));
+#endif /* PNG_APNG_SUPPORTED */
+
+#ifdef PNG_READ_APNG_SUPPORTED
+extern PNG_EXPORT(void,png_read_frame_head) PNGARG((png_structp png_ptr,
+   png_infop info_ptr));
+#endif
+
 #ifdef PNG_HANDLE_AS_UNKNOWN_SUPPORTED
 /* Provide a list of chunks and how they are to be handled, if the built-in
    handling or default unknown chunk handling is not desired.  Any chunks not
@@ -2915,6 +3039,10 @@
 #define PNG_BACKGROUND_IS_GRAY     0x800
 #define PNG_HAVE_PNG_SIGNATURE    0x1000
 #define PNG_HAVE_CHUNK_AFTER_IDAT 0x2000 /* Have another chunk after IDAT */
+#ifdef PNG_APNG_SUPPORTED
+#define PNG_HAVE_acTL             0x4000
+#define PNG_HAVE_fcTL             0x8000L
+#endif
 
 /* Flags for the transformations the PNG library does on the image data */
 #define PNG_BGR                0x0001
@@ -3057,6 +3185,11 @@
 #define PNG_tIME png_byte png_tIME[5] = {116,  73,  77,  69, '\0'}
 #define PNG_tRNS png_byte png_tRNS[5] = {116,  82,  78,  83, '\0'}
 #define PNG_zTXt png_byte png_zTXt[5] = {122,  84,  88, 116, '\0'}
+#ifdef PNG_APNG_SUPPORTED
+#define PNG_acTL png_byte png_acTL[5] = { 97,  99,  84,  76, '\0'}
+#define PNG_fcTL png_byte png_fcTL[5] = {102,  99,  84,  76, '\0'}
+#define PNG_fdAT png_byte png_fdAT[5] = {102, 100,  65,  84, '\0'}
+#endif
 
 #ifdef PNG_USE_GLOBAL_ARRAYS
 PNG_EXPORT_VAR (png_byte FARDATA) png_IHDR[5];
@@ -3080,6 +3213,11 @@
 PNG_EXPORT_VAR (png_byte FARDATA) png_tIME[5];
 PNG_EXPORT_VAR (png_byte FARDATA) png_tRNS[5];
 PNG_EXPORT_VAR (png_byte FARDATA) png_zTXt[5];
+#ifdef PNG_APNG_SUPPORTED
+PNG_EXPORT_VAR (png_byte FARDATA) png_acTL[5];
+PNG_EXPORT_VAR (png_byte FARDATA) png_fcTL[5];
+PNG_EXPORT_VAR (png_byte FARDATA) png_fdAT[5];
+#endif
 #endif /* PNG_USE_GLOBAL_ARRAYS */
 
 #if defined(PNG_1_0_X) || defined (PNG_1_2_X)
@@ -3362,6 +3500,17 @@
 #endif
 #endif
 
+#ifdef PNG_WRITE_APNG_SUPPORTED
+PNG_EXTERN void png_write_acTL PNGARG((png_structp png_ptr,
+   png_uint_32 num_frames, png_uint_32 num_plays));
+
+PNG_EXTERN void png_write_fcTL PNGARG((png_structp png_ptr, 
+   png_uint_32 width, png_uint_32 height, 
+   png_uint_32 x_offset, png_uint_32 y_offset, 
+   png_uint_16 delay_num, png_uint_16 delay_den,
+   png_byte dispose_op, png_byte blend_op));
+#endif
+
 /* Called when finished processing a row of data */
 PNG_EXTERN void png_write_finish_row PNGARG((png_structp png_ptr)) PNG_PRIVATE;
 
@@ -3414,6 +3563,20 @@
 PNG_EXTERN void png_read_transform_info PNGARG((png_structp png_ptr,
    png_infop info_ptr)) PNG_PRIVATE;
 
+#ifdef PNG_READ_APNG_SUPPORTED
+/* Private, reset some things to become ready for reading next frame */
+PNG_EXTERN void png_read_reset PNGARG((png_structp png_ptr));
+PNG_EXTERN void png_read_reinit PNGARG((png_structp png_ptr,
+   png_infop info_ptr));
+PNG_EXTERN void png_progressive_read_reset PNGARG((png_structp png_ptr));
+#endif
+#ifdef PNG_WRITE_APNG_SUPPORTED
+/* Private, reset some things to become ready for writing next frame */
+PNG_EXTERN void png_write_reset PNGARG((png_structp png_ptr));
+PNG_EXTERN void png_write_reinit PNGARG((png_structp png_ptr, 
+   png_infop info_ptr, png_uint_32 width, png_uint_32 height));
+#endif
+
 /* These are the functions that do the transformations */
 #ifdef PNG_READ_FILLER_SUPPORTED
 PNG_EXTERN void png_do_read_filler PNGARG((png_row_infop row_info,
@@ -3637,6 +3800,18 @@
    png_uint_32 length)) PNG_PRIVATE;
 #endif
 
+#ifdef PNG_READ_APNG_SUPPORTED
+PNG_EXTERN void png_handle_acTL PNGARG((png_structp png_ptr, png_infop info_ptr,
+   png_uint_32 length));
+PNG_EXTERN void png_handle_fcTL PNGARG((png_structp png_ptr, png_infop info_ptr,
+   png_uint_32 length));
+PNG_EXTERN void png_have_info PNGARG((png_structp png_ptr, png_infop info_ptr));
+PNG_EXTERN void png_handle_fdAT PNGARG((png_structp png_ptr, png_infop info_ptr,
+   png_uint_32 length));
+PNG_EXTERN void png_ensure_sequence_number PNGARG((png_structp png_ptr, 
+   png_uint_32 length));
+#endif
+
 PNG_EXTERN void png_handle_unknown PNGARG((png_structp png_ptr,
    png_infop info_ptr, png_uint_32 length)) PNG_PRIVATE;
 
Index: pngwrite.c
===================================================================
--- pngwrite.c
+++ pngwrite.c
@@ -57,6 +57,10 @@
    /* The rest of these check to see if the valid field has the appropriate
     * flag set, and if it does, writes the chunk.
     */
+#ifdef PNG_WRITE_APNG_SUPPORTED
+   if (info_ptr->valid & PNG_INFO_acTL)
+      png_write_acTL(png_ptr, info_ptr->num_frames, info_ptr->num_plays);
+#endif
 #ifdef PNG_WRITE_gAMA_SUPPORTED
    if (info_ptr->valid & PNG_INFO_gAMA)
    {
@@ -319,6 +323,10 @@
       return;
    if (!(png_ptr->mode & PNG_HAVE_IDAT))
       png_error(png_ptr, "No IDATs written into file");
+#ifdef PNG_WRITE_APNG_SUPPORTED
+   if (png_ptr->num_frames_written != png_ptr->num_frames_to_write)
+      png_error(png_ptr, "Not enough frames written");
+#endif
 
    /* See if user wants us to write information chunks */
    if (info_ptr != NULL)
@@ -1589,4 +1597,39 @@
    params = params;
 }
 #endif
+
+#ifdef PNG_WRITE_APNG_SUPPORTED
+void PNGAPI
+png_write_frame_head(png_structp png_ptr, png_infop info_ptr,
+    png_bytepp row_pointers, png_uint_32 width, png_uint_32 height, 
+    png_uint_32 x_offset, png_uint_32 y_offset, 
+    png_uint_16 delay_num, png_uint_16 delay_den, png_byte dispose_op,
+    png_byte blend_op)
+{
+    png_debug(1, "in png_write_frame_head");
+    
+    /* there is a chance this has been set after png_write_info was called,
+    * so it would be set but not written. is there a way to be sure? */
+    if (!(info_ptr->valid & PNG_INFO_acTL))
+        png_error(png_ptr, "png_write_frame_head(): acTL not set");
+    
+    png_write_reset(png_ptr);
+    
+    png_write_reinit(png_ptr, info_ptr, width, height);
+    
+    if ( !(png_ptr->num_frames_written == 0 && 
+           (png_ptr->apng_flags & PNG_FIRST_FRAME_HIDDEN) ) )
+        png_write_fcTL(png_ptr, width, height, x_offset, y_offset, 
+                       delay_num, delay_den, dispose_op, blend_op);
+}
+
+void PNGAPI
+png_write_frame_tail(png_structp png_ptr, png_infop png_info)
+{
+    png_debug(1, "in png_write_frame_tail");
+    
+    png_ptr->num_frames_written++;
+}
+#endif /* PNG_WRITE_APNG_SUPPORTED */
+
 #endif /* PNG_WRITE_SUPPORTED */
Index: pngconf.h
===================================================================
--- pngconf.h
+++ pngconf.h
@@ -951,6 +951,10 @@
 #  define PNG_NO_READ_tEXt
 #  define PNG_NO_READ_zTXt
 #endif
+#ifndef PNG_NO_READ_APNG
+#  define PNG_READ_APNG_SUPPORTED
+#  define PNG_APNG_SUPPORTED
+#endif
 #ifndef PNG_NO_READ_bKGD
 #  define PNG_READ_bKGD_SUPPORTED
 #  define PNG_bKGD_SUPPORTED
@@ -1177,6 +1181,14 @@
 #    define PNG_TEXT_SUPPORTED
 #  endif
 #endif
+#ifndef PNG_NO_WRITE_APNG
+#  ifndef PNG_WRITE_APNG_SUPPORTED
+#    define PNG_WRITE_APNG_SUPPORTED
+#  endif
+#  ifndef PNG_APNG_SUPPORTED
+#    define PNG_APNG_SUPPORTED
+#  endif
+#endif
 
 #ifdef PNG_WRITE_tIME_SUPPORTED
 #  ifndef PNG_NO_CONVERT_tIME
Index: pngpread.c
===================================================================
--- pngpread.c
+++ pngpread.c
@@ -206,6 +206,11 @@
 #ifdef PNG_READ_zTXt_SUPPORTED
       PNG_CONST PNG_zTXt;
 #endif
+#ifdef PNG_READ_APNG_SUPPORTED
+      PNG_CONST PNG_acTL;
+      PNG_CONST PNG_fcTL;
+      PNG_CONST PNG_fdAT;
+#endif
 #endif /* PNG_USE_LOCAL_ARRAYS */
 
    /* First we make sure we have enough data for the 4 byte chunk name
@@ -232,6 +237,103 @@
       png_ptr->mode |= PNG_HAVE_CHUNK_HEADER;
    }
 
+#ifdef PNG_READ_APNG_SUPPORTED
+   if (png_ptr->num_frames_read > 0 && 
+       png_ptr->num_frames_read < info_ptr->num_frames)
+   {
+      if (!png_memcmp(png_ptr->chunk_name, png_IDAT, 4))
+      {
+         /* Discard trailing IDATs for the first frame */
+         if (png_ptr->mode & PNG_HAVE_fcTL || png_ptr->num_frames_read > 1)
+            png_error(png_ptr, "out of place IDAT");
+         
+         if (png_ptr->push_length + 4 > png_ptr->buffer_size)
+         {
+            png_push_save_buffer(png_ptr);
+            return;
+         }
+         png_push_crc_skip(png_ptr, png_ptr->push_length);
+         png_ptr->mode &= ~PNG_HAVE_CHUNK_HEADER;
+         return;
+      }
+      else if (!png_memcmp(png_ptr->chunk_name, png_fdAT, 4))
+      {
+         if (png_ptr->buffer_size < 4)
+         {
+            png_push_save_buffer(png_ptr);
+            return;
+         }
+         png_ensure_sequence_number(png_ptr, 4);
+         
+         if (!(png_ptr->mode & PNG_HAVE_fcTL))
+         {
+            /* Discard trailing fdATs for frames other than the first */
+            if (png_ptr->num_frames_read < 2)
+               png_error(png_ptr, "out of place fdAT");
+            
+            if (png_ptr->push_length + 4 > png_ptr->buffer_size)
+            {
+               png_push_save_buffer(png_ptr);
+               return;
+            }
+            png_push_crc_skip(png_ptr, png_ptr->push_length);
+            png_ptr->mode &= ~PNG_HAVE_CHUNK_HEADER;
+            return;
+         }
+         else
+         {
+            /* frame data follows */
+            png_ptr->idat_size = png_ptr->push_length - 4;
+            png_ptr->mode |= PNG_HAVE_IDAT;
+            png_ptr->process_mode = PNG_READ_IDAT_MODE;
+            
+            return;
+         }
+      }
+      else if(!png_memcmp(png_ptr->chunk_name, png_fcTL, 4))
+      {
+         if (png_ptr->push_length + 4 > png_ptr->buffer_size)
+         {
+            png_push_save_buffer(png_ptr);
+            return;
+         }
+         
+         png_read_reset(png_ptr);
+         png_ptr->mode &= ~PNG_HAVE_fcTL;
+         
+         png_handle_fcTL(png_ptr, info_ptr, png_ptr->push_length);
+         
+         if (!(png_ptr->mode & PNG_HAVE_fcTL))
+            png_error(png_ptr, "missing required fcTL chunk");
+         
+         png_read_reinit(png_ptr, info_ptr);
+         png_progressive_read_reset(png_ptr);
+         
+         if (png_ptr->frame_info_fn != NULL)
+            (*(png_ptr->frame_info_fn))(png_ptr, png_ptr->num_frames_read);
+         
+         png_ptr->mode &= ~PNG_HAVE_CHUNK_HEADER;
+         
+         return;
+      }
+      else
+      {
+         if (png_ptr->push_length + 4 > png_ptr->buffer_size)
+         {
+            png_push_save_buffer(png_ptr);
+            return;
+         }
+         png_warning(png_ptr, "Skipped (ignored) a chunk "
+                              "between APNG chunks");
+         png_push_crc_skip(png_ptr, png_ptr->push_length);
+         png_ptr->mode &= ~PNG_HAVE_CHUNK_HEADER;
+         return;
+      }
+      
+      return;
+   }
+#endif /* PNG_READ_APNG_SUPPORTED */
+
    if (!png_memcmp(png_ptr->chunk_name, png_IDAT, 4))
      if (png_ptr->mode & PNG_AFTER_IDAT)
         png_ptr->mode |= PNG_HAVE_CHUNK_AFTER_IDAT;
@@ -327,6 +429,9 @@
             png_error(png_ptr, "Too many IDAT's found");
       }
 
+#ifdef PNG_READ_APNG_SUPPORTED
+      png_have_info(png_ptr, info_ptr);
+#endif
       png_ptr->idat_size = png_ptr->push_length;
       png_ptr->mode |= PNG_HAVE_IDAT;
       png_ptr->process_mode = PNG_READ_IDAT_MODE;
@@ -559,6 +664,38 @@
    }
 
 #endif
+#ifdef PNG_READ_APNG_SUPPORTED
+   else if (!png_memcmp(png_ptr->chunk_name, png_acTL, 4))
+   {
+      if (png_ptr->push_length + 4 > png_ptr->buffer_size)
+      {
+         png_push_save_buffer(png_ptr);
+         return;
+      }
+
+      png_handle_acTL(png_ptr, info_ptr, png_ptr->push_length);
+   }
+   else if (!png_memcmp(png_ptr->chunk_name, png_fcTL, 4))
+   {
+      if (png_ptr->push_length + 4 > png_ptr->buffer_size)
+      {
+         png_push_save_buffer(png_ptr);
+         return;
+      }
+
+      png_handle_fcTL(png_ptr, info_ptr, png_ptr->push_length);
+   }
+   else if (!png_memcmp(png_ptr->chunk_name, png_fdAT, 4))
+   {
+      if (png_ptr->push_length + 4 > png_ptr->buffer_size)
+      {
+         png_push_save_buffer(png_ptr);
+         return;
+      }
+
+      png_handle_fdAT(png_ptr, info_ptr, png_ptr->push_length);
+   }
+#endif /* PNG_READ_APNG_SUPPORTED */
    else
    {
       if (png_ptr->push_length + 4 > png_ptr->buffer_size)
@@ -738,13 +875,17 @@
 png_push_read_IDAT(png_structp png_ptr)
 {
 #ifdef PNG_USE_LOCAL_ARRAYS
-   PNG_CONST PNG_IDAT;
+   PNG_IDAT;
+#ifdef PNG_READ_APNG_SUPPORTED
+   PNG_fdAT;
+   PNG_IEND;
+#endif
 #endif
    if (!(png_ptr->mode & PNG_HAVE_CHUNK_HEADER))
    {
       png_byte chunk_length[4];
 
-      if (png_ptr->buffer_size < 8)
+      if (png_ptr->buffer_size < 12)
       {
          png_push_save_buffer(png_ptr);
          return;
@@ -756,15 +897,62 @@
       png_crc_read(png_ptr, png_ptr->chunk_name, 4);
       png_ptr->mode |= PNG_HAVE_CHUNK_HEADER;
 
-      if (png_memcmp(png_ptr->chunk_name, png_IDAT, 4))
+#ifdef PNG_READ_APNG_SUPPORTED
+      if (png_memcmp(png_ptr->chunk_name, (png_bytep)png_fdAT, 4)
+          && png_ptr->num_frames_read > 0)
+      {
+          if (png_ptr->flags & PNG_FLAG_ZLIB_FINISHED)
+          {
+              png_ptr->process_mode = PNG_READ_CHUNK_MODE;
+              if (png_ptr->frame_end_fn != NULL)
+                 (*(png_ptr->frame_end_fn))(png_ptr, png_ptr->num_frames_read);
+              png_ptr->num_frames_read++;
+              return;
+          }
+          else
+          {
+              if (!png_memcmp(png_ptr->chunk_name, png_IEND, 4))
+                  png_error(png_ptr, "Not enough image data");
+              if (png_ptr->push_length + 4 > png_ptr->buffer_size)
+              {
+                 png_push_save_buffer(png_ptr);
+                 return;
+              }
+              png_warning(png_ptr, "Skipping (ignoring) a chunk between "
+                                   "APNG chunks");
+              png_crc_finish(png_ptr, png_ptr->push_length);
+              png_ptr->mode &= ~PNG_HAVE_CHUNK_HEADER;
+              return;
+          }
+      }
+      else 
+#endif
+      if (png_memcmp(png_ptr->chunk_name, png_IDAT, 4)
+#ifdef PNG_READ_APNG_SUPPORTED
+                && (png_ptr->num_frames_read == 0)
+#endif
+         )
       {
          png_ptr->process_mode = PNG_READ_CHUNK_MODE;
          if (!(png_ptr->flags & PNG_FLAG_ZLIB_FINISHED))
             png_error(png_ptr, "Not enough compressed data");
+#ifdef PNG_READ_APNG_SUPPORTED
+         if (png_ptr->frame_end_fn != NULL)
+            (*(png_ptr->frame_end_fn))(png_ptr, png_ptr->num_frames_read);
+         png_ptr->num_frames_read++;
+#endif
          return;
       }
 
       png_ptr->idat_size = png_ptr->push_length;
+
+#ifdef PNG_READ_APNG_SUPPORTED
+      if (png_ptr->num_frames_read > 0)
+      {
+         png_ensure_sequence_number(png_ptr, 4);
+         png_ptr->idat_size -= 4;
+      }
+#endif
    }
    if (png_ptr->idat_size && png_ptr->save_buffer_size)
    {
@@ -1725,6 +1913,17 @@
    png_set_read_fn(png_ptr, progressive_ptr, png_push_fill_buffer);
 }
 
+#ifdef PNG_READ_APNG_SUPPORTED
+void PNGAPI
+png_set_progressive_frame_fn(png_structp png_ptr,
+   png_progressive_frame_ptr frame_info_fn,
+   png_progressive_frame_ptr frame_end_fn)
+{
+   png_ptr->frame_info_fn = frame_info_fn;
+   png_ptr->frame_end_fn = frame_end_fn;
+}
+#endif
+
 png_voidp PNGAPI
 png_get_progressive_ptr(png_structp png_ptr)
 {
Index: pngset.c
===================================================================
--- pngset.c
+++ pngset.c
@@ -266,6 +266,11 @@
       info_ptr->rowbytes = (png_size_t)0;
    else
       info_ptr->rowbytes = PNG_ROWBYTES(info_ptr->pixel_depth, width);
+
+#ifdef PNG_APNG_SUPPORTED
+   /* for non-animated png. this may be overritten from an acTL chunk later */
+   info_ptr->num_frames = 1;
+#endif
 }
 
 #ifdef PNG_oFFs_SUPPORTED
@@ -960,6 +965,142 @@
 }
 #endif /* PNG_sPLT_SUPPORTED */
 
+#ifdef PNG_APNG_SUPPORTED
+png_uint_32 PNGAPI
+png_set_acTL(png_structp png_ptr, png_infop info_ptr, 
+    png_uint_32 num_frames, png_uint_32 num_plays)
+{
+    png_debug1(1, "in %s storage function", "acTL");
+
+    if (png_ptr == NULL || info_ptr == NULL)
+    {
+        png_warning(png_ptr,
+                    "Call to png_set_acTL() with NULL png_ptr "
+                    "or info_ptr ignored");
+        return (0);
+    }
+    if (num_frames == 0)
+    {
+        png_warning(png_ptr,
+                    "Ignoring attempt to set acTL with num_frames zero");
+        return (0);
+    }
+    if (num_frames > PNG_UINT_31_MAX)
+    {
+        png_warning(png_ptr,
+                    "Ignoring attempt to set acTL with num_frames > 2^31-1");
+        return (0);
+    }
+    if (num_plays > PNG_UINT_31_MAX)
+    {
+        png_warning(png_ptr,
+                    "Ignoring attempt to set acTL with num_plays "
+                    "> 2^31-1");
+        return (0);
+    }
+    
+    info_ptr->num_frames = num_frames;
+    info_ptr->num_plays = num_plays;
+    
+    info_ptr->valid |= PNG_INFO_acTL;
+    
+    return (1);
+}
+
+/* delay_num and delay_den can hold any 16-bit values including zero */
+png_uint_32 PNGAPI
+png_set_next_frame_fcTL(png_structp png_ptr, png_infop info_ptr, 
+    png_uint_32 width, png_uint_32 height,
+    png_uint_32 x_offset, png_uint_32 y_offset,
+    png_uint_16 delay_num, png_uint_16 delay_den,
+    png_byte dispose_op, png_byte blend_op)
+{
+    png_debug1(1, "in %s storage function", "fcTL");
+
+    if (png_ptr == NULL || info_ptr == NULL)
+    {
+        png_warning(png_ptr,
+                    "Call to png_set_fcTL() with NULL png_ptr or info_ptr "
+                    "ignored");
+        return (0);
+    }
+    
+    png_ensure_fcTL_is_valid(png_ptr, width, height, x_offset, y_offset, 
+                             delay_num, delay_den, dispose_op, blend_op);
+    
+    if (blend_op == PNG_BLEND_OP_OVER)
+    {
+        if (!(png_ptr->color_type & PNG_COLOR_MASK_ALPHA) &&
+            !(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)))
+        {
+          png_warning(png_ptr, "PNG_BLEND_OP_OVER is meaningless "
+                               "and wasteful for opaque images, ignored");
+          blend_op = PNG_BLEND_OP_SOURCE;
+        }
+    }
+
+    info_ptr->next_frame_width = width;
+    info_ptr->next_frame_height = height;
+    info_ptr->next_frame_x_offset = x_offset;
+    info_ptr->next_frame_y_offset = y_offset;
+    info_ptr->next_frame_delay_num = delay_num;
+    info_ptr->next_frame_delay_den = delay_den;
+    info_ptr->next_frame_dispose_op = dispose_op;
+    info_ptr->next_frame_blend_op = blend_op;
+    
+    info_ptr->valid |= PNG_INFO_fcTL;
+    
+    return (1);
+}
+
+void PNGAPI
+png_ensure_fcTL_is_valid(png_structp png_ptr, 
+    png_uint_32 width, png_uint_32 height,
+    png_uint_32 x_offset, png_uint_32 y_offset,
+    png_uint_16 delay_num, png_uint_16 delay_den,
+    png_byte dispose_op, png_byte blend_op)
+{
+    if (width + x_offset > png_ptr->first_frame_width || 
+        height + y_offset > png_ptr->first_frame_height)
+        png_error(png_ptr, "dimensions of a frame are greater than"
+                           "the ones in IHDR");
+    if (width > PNG_UINT_31_MAX)
+        png_error(png_ptr, "invalid width in fcTL (> 2^31-1)");
+    if (height > PNG_UINT_31_MAX)
+        png_error(png_ptr, "invalid height in fcTL (> 2^31-1)");
+    if (x_offset > PNG_UINT_31_MAX)
+        png_error(png_ptr, "invalid x_offset in fcTL (> 2^31-1)");
+    if (y_offset > PNG_UINT_31_MAX)
+        png_error(png_ptr, "invalid y_offset in fcTL (> 2^31-1)");
+
+    if (dispose_op != PNG_DISPOSE_OP_NONE &&
+        dispose_op != PNG_DISPOSE_OP_BACKGROUND &&
+        dispose_op != PNG_DISPOSE_OP_PREVIOUS)
+        png_error(png_ptr, "invalid dispose_op in fcTL");
+
+    if (blend_op != PNG_BLEND_OP_SOURCE &&
+        blend_op != PNG_BLEND_OP_OVER)
+        png_error(png_ptr, "invalid blend_op in fcTL");
+}
+
+png_uint_32 PNGAPI
+png_set_first_frame_is_hidden(png_structp png_ptr, png_infop info_ptr,
+                              png_byte is_hidden)
+{
+    png_debug(1, "in png_first_frame_is_hidden()");
+    
+    if (png_ptr == NULL)
+        return 0;
+    
+    if(is_hidden)
+        png_ptr->apng_flags |= PNG_FIRST_FRAME_HIDDEN;
+    else
+        png_ptr->apng_flags &= ~PNG_FIRST_FRAME_HIDDEN;
+    
+    return 1;
+}
+#endif /* PNG_APNG_SUPPORTED */
+
 #ifdef PNG_UNKNOWN_CHUNKS_SUPPORTED
 void PNGAPI
 png_set_unknown_chunks(png_structp png_ptr,
Index: pngrutil.c
===================================================================
--- pngrutil.c
+++ pngrutil.c
@@ -451,6 +451,11 @@
    filter_type = buf[11];
    interlace_type = buf[12];
 
+#ifdef PNG_READ_APNG_SUPPORTED
+   png_ptr->first_frame_width = width;
+   png_ptr->first_frame_height = height;
+#endif
+
    /* Set internal variables */
    png_ptr->width = width;
    png_ptr->height = height;
@@ -2323,6 +2328,172 @@
 }
 #endif
 
+#ifdef PNG_READ_APNG_SUPPORTED
+void /* PRIVATE */
+png_handle_acTL(png_structp png_ptr, png_infop info_ptr, png_uint_32 length)
+{
+    png_byte data[8];
+    png_uint_32 num_frames;
+    png_uint_32 num_plays;
+    png_uint_32 didSet;
+    
+    png_debug(1, "in png_handle_acTL");
+
+    if (!(png_ptr->mode & PNG_HAVE_IHDR))
+    {
+        png_error(png_ptr, "Missing IHDR before acTL");
+    }
+    else if (png_ptr->mode & PNG_HAVE_IDAT)
+    {
+        png_warning(png_ptr, "Invalid acTL after IDAT skipped");
+        png_crc_finish(png_ptr, length);
+        return;
+    }
+    else if (png_ptr->mode & PNG_HAVE_acTL)
+    {
+        png_warning(png_ptr, "Duplicate acTL skipped");
+        png_crc_finish(png_ptr, length);
+        return;
+    }
+    else if (length != 8)
+    {
+        png_warning(png_ptr, "acTL with invalid length skipped");
+        png_crc_finish(png_ptr, length);
+        return;
+    }
+    
+    png_crc_read(png_ptr, data, 8);
+    png_crc_finish(png_ptr, 0);
+    
+    num_frames = png_get_uint_31(png_ptr, data);
+    num_plays = png_get_uint_31(png_ptr, data + 4);
+    
+    /* the set function will do error checking on num_frames */
+    didSet = png_set_acTL(png_ptr, info_ptr, num_frames, num_plays);
+    if(didSet)
+        png_ptr->mode |= PNG_HAVE_acTL;
+}
+
+void /* PRIVATE */
+png_handle_fcTL(png_structp png_ptr, png_infop info_ptr, png_uint_32 length)
+{
+    png_byte data[22];
+    png_uint_32 width;
+    png_uint_32 height;
+    png_uint_32 x_offset;
+    png_uint_32 y_offset;
+    png_uint_16 delay_num;
+    png_uint_16 delay_den;
+    png_byte dispose_op;
+    png_byte blend_op;
+
+    png_debug(1, "in png_handle_fcTL");
+
+    png_ensure_sequence_number(png_ptr, length);
+
+    if (!(png_ptr->mode & PNG_HAVE_IHDR))
+    {
+        png_error(png_ptr, "Missing IHDR before fcTL");
+    }
+    else if (png_ptr->mode & PNG_HAVE_IDAT)
+    {
+        /* for any frames other then the first this message may be misleading,
+        * but correct. PNG_HAVE_IDAT is unset before the frame head is read
+        * i can't think of a better message */
+        png_warning(png_ptr, "Invalid fcTL after IDAT skipped");
+        png_crc_finish(png_ptr, length-4);
+        return;
+    }
+    else if (png_ptr->mode & PNG_HAVE_fcTL)
+    {
+        png_warning(png_ptr, "Duplicate fcTL within one frame skipped");
+        png_crc_finish(png_ptr, length-4);
+        return;
+    }
+    else if (length != 26)
+    {
+        png_warning(png_ptr, "fcTL with invalid length skipped");
+        png_crc_finish(png_ptr, length-4);
+        return;
+    }
+
+    png_crc_read(png_ptr, data, 22);
+    png_crc_finish(png_ptr, 0);
+
+    width = png_get_uint_31(png_ptr, data);
+    height = png_get_uint_31(png_ptr, data + 4);
+    x_offset = png_get_uint_31(png_ptr, data + 8);
+    y_offset = png_get_uint_31(png_ptr, data + 12);
+    delay_num = png_get_uint_16(data + 16);
+    delay_den = png_get_uint_16(data + 18);
+    dispose_op = data[20];
+    blend_op = data[21];
+
+    if (png_ptr->num_frames_read == 0 && (x_offset != 0 || y_offset != 0))
+        png_error(png_ptr, "fcTL for the first frame must have zero offset");
+
+    if (info_ptr != NULL)
+    {
+        if (png_ptr->num_frames_read == 0 &&
+            (width != info_ptr->width || height != info_ptr->height))
+            png_error(png_ptr, "size in first frame's fcTL must match "
+                               "the size in IHDR");
+
+        /* the set function will do more error checking */
+        png_set_next_frame_fcTL(png_ptr, info_ptr, width, height,
+                                x_offset, y_offset, delay_num, delay_den,
+                                dispose_op, blend_op);
+
+        png_read_reinit(png_ptr, info_ptr);
+    }
+
+    png_ptr->mode |= PNG_HAVE_fcTL;
+}
+
+void /* PRIVATE */
+png_have_info(png_structp png_ptr, png_infop info_ptr)
+{
+    if((info_ptr->valid & PNG_INFO_acTL) && !(info_ptr->valid & PNG_INFO_fcTL))
+    {
+        png_ptr->apng_flags |= PNG_FIRST_FRAME_HIDDEN;
+        info_ptr->num_frames++;
+    }
+}
+
+void /* PRIVATE */
+png_handle_fdAT(png_structp png_ptr, png_infop info_ptr, png_uint_32 length)
+{
+    png_ensure_sequence_number(png_ptr, length);
+    
+    /* This function is only called from png_read_end(), png_read_info(), 
+    * and png_push_read_chunk() which means that:
+    * - the user doesn't want to read this frame
+    * - or this is an out-of-place fdAT
+    * in either case it is safe to ignore the chunk with a warning */
+    png_warning(png_ptr, "ignoring fdAT chunk");
+    png_crc_finish(png_ptr, length - 4);
+}
+
+void /* PRIVATE */
+png_ensure_sequence_number(png_structp png_ptr, png_uint_32 length)
+{
+    png_byte data[4];
+    png_uint_32 sequence_number;
+    
+    if (length < 4)
+        png_error(png_ptr, "invalid fcTL or fdAT chunk found");
+    
+    png_crc_read(png_ptr, data, 4);
+    sequence_number = png_get_uint_31(png_ptr, data);
+    
+    if (sequence_number != png_ptr->next_seq_num)
+        png_error(png_ptr, "fcTL or fdAT chunk with out-of-order sequence "
+                           "number found");
+    
+    png_ptr->next_seq_num++;
+}
+#endif /* PNG_READ_APNG_SUPPORTED */
+
 /* This function is called when we haven't found a handler for a
    chunk.  If there isn't a problem with the chunk itself (ie bad
    chunk name, CRC, or a critical chunk), the chunk is silently ignored
@@ -3354,4 +3525,75 @@
 
    png_ptr->flags |= PNG_FLAG_ROW_INIT;
 }
+
+#ifdef PNG_READ_APNG_SUPPORTED
+/* This function is to be called after the main IDAT set has been read and
+ * before a new IDAT is read. It resets some parts of png_ptr
+ * to make them usable by the read functions again */
+void /* PRIVATE */
+png_read_reset(png_structp png_ptr)
+{
+    png_ptr->mode &= ~PNG_HAVE_IDAT;
+    png_ptr->mode &= ~PNG_AFTER_IDAT;
+    png_ptr->row_number = 0;
+    png_ptr->pass = 0;
+    png_ptr->flags &= ~PNG_FLAG_ROW_INIT;
+}
+
+void /* PRIVATE */
+png_read_reinit(png_structp png_ptr, png_infop info_ptr)
+{
+    png_ptr->width = info_ptr->next_frame_width;
+    png_ptr->height = info_ptr->next_frame_height;
+    png_ptr->rowbytes = PNG_ROWBYTES(png_ptr->pixel_depth,png_ptr->width);
+    if (png_ptr->prev_row)
+       png_memset_check(png_ptr, png_ptr->prev_row, 0, png_ptr->rowbytes + 1);
+}
+
+/* same as png_read_reset() but for the progressive reader */
+void /* PRIVATE */
+png_progressive_read_reset(png_structp png_ptr)
+{
+#ifdef PNG_USE_LOCAL_ARRAYS
+    /* start of interlace block */
+    const int FARDATA png_pass_start[] = {0, 4, 0, 2, 0, 1, 0};
+
+    /* offset to next interlace block */
+    const int FARDATA png_pass_inc[] = {8, 8, 4, 4, 2, 2, 1};
+
+    /* start of interlace block in the y direction */
+    const int FARDATA png_pass_ystart[] = {0, 0, 4, 0, 2, 0, 1};
+
+    /* offset to next interlace block in the y direction */
+    const int FARDATA png_pass_yinc[] = {8, 8, 8, 4, 4, 2, 2};
+#endif
+    
+    if (png_ptr->interlaced)
+    {
+        if (!(png_ptr->transformations & PNG_INTERLACE))
+            png_ptr->num_rows = (png_ptr->height + png_pass_yinc[0] - 1 -
+                                png_pass_ystart[0]) / png_pass_yinc[0];
+        else
+           png_ptr->num_rows = png_ptr->height;
+
+        png_ptr->iwidth = (png_ptr->width +
+                           png_pass_inc[png_ptr->pass] - 1 -
+                           png_pass_start[png_ptr->pass]) /
+                           png_pass_inc[png_ptr->pass];
+    }
+    else
+    {
+        png_ptr->num_rows = png_ptr->height;
+        png_ptr->iwidth = png_ptr->width;
+    }
+    png_ptr->flags &= ~PNG_FLAG_ZLIB_FINISHED;
+    if (inflateReset(&(png_ptr->zstream)) != Z_OK)
+        png_error(png_ptr, "inflateReset failed");
+    png_ptr->zstream.avail_in = 0;
+    png_ptr->zstream.next_in = 0;
+    png_ptr->zstream.next_out = png_ptr->row_buf;
+    png_ptr->zstream.avail_out = (uInt)PNG_ROWBYTES(png_ptr->pixel_depth,
+        png_ptr->iwidth) + 1;
+}
+#endif /* PNG_READ_APNG_SUPPORTED */
 #endif /* PNG_READ_SUPPORTED */
Index: pngwutil.c
===================================================================
--- pngwutil.c
+++ pngwutil.c
@@ -516,6 +516,11 @@
    /* Write the chunk */
    png_write_chunk(png_ptr, (png_bytep)png_IHDR, buf, (png_size_t)13);
 
+#ifdef PNG_WRITE_APNG_SUPPORTED
+   png_ptr->first_frame_width = width;
+   png_ptr->first_frame_height = height;
+#endif
+
    /* Initialize zlib with PNG info */
    png_ptr->zstream.zalloc = png_zalloc;
    png_ptr->zstream.zfree = png_zfree;
@@ -641,6 +646,9 @@
 {
 #ifdef PNG_USE_LOCAL_ARRAYS
    PNG_IDAT;
+#ifdef PNG_WRITE_APNG_SUPPORTED
+   PNG_fdAT;
+#endif
 #endif
 
    png_debug(1, "in png_write_IDAT");
@@ -686,7 +694,28 @@
             "Invalid zlib compression method or flags in IDAT");
    }
 
+#ifdef PNG_WRITE_APNG_SUPPORTED
+   if(png_ptr->num_frames_written == 0)
+#endif
    png_write_chunk(png_ptr, (png_bytep)png_IDAT, data, length);
+#ifdef PNG_WRITE_APNG_SUPPORTED
+   else
+   {
+      png_byte buf[4];
+      
+      png_write_chunk_start(png_ptr, (png_bytep)png_fdAT, 4 + length);
+      
+      png_save_uint_32(buf, png_ptr->next_seq_num);
+      png_write_chunk_data(png_ptr, buf, 4);
+      
+      png_write_chunk_data(png_ptr, data, length);
+      
+      png_write_chunk_end(png_ptr);
+      
+      png_ptr->next_seq_num++;
+   }
+#endif
+
    png_ptr->mode |= PNG_HAVE_IDAT;
 }
 
@@ -1754,6 +1783,70 @@
 }
 #endif
 
+#ifdef PNG_WRITE_APNG_SUPPORTED
+void /* PRIVATE */
+png_write_acTL(png_structp png_ptr,
+   png_uint_32 num_frames, png_uint_32 num_plays)
+{
+#ifdef PNG_USE_LOCAL_ARRAYS
+    PNG_acTL;
+#endif
+    png_byte data[16];
+    
+    png_debug(1, "in png_write_acTL");
+    
+    png_ptr->num_frames_to_write = num_frames;
+    
+    if (png_ptr->apng_flags & PNG_FIRST_FRAME_HIDDEN)
+        num_frames--;
+    
+    png_save_uint_32(data, num_frames);
+    png_save_uint_32(data + 4, num_plays);
+    
+    png_write_chunk(png_ptr, (png_bytep)png_acTL, data, (png_size_t)8);
+}
+
+void /* PRIVATE */
+png_write_fcTL(png_structp png_ptr, png_uint_32 width, png_uint_32 height, 
+    png_uint_32 x_offset, png_uint_32 y_offset,
+    png_uint_16 delay_num, png_uint_16 delay_den, png_byte dispose_op, 
+    png_byte blend_op)
+{
+#ifdef PNG_USE_LOCAL_ARRAYS
+    PNG_fcTL;
+#endif
+    png_byte data[26];
+    
+    png_debug(1, "in png_write_fcTL");
+    
+    if (png_ptr->num_frames_written == 0 && (x_offset != 0 || y_offset != 0))
+        png_error(png_ptr, "x and/or y offset for the first frame aren't 0");
+    if (png_ptr->num_frames_written == 0 && 
+        (width != png_ptr->first_frame_width || 
+         height != png_ptr->first_frame_height))
+        png_error(png_ptr, "width and/or height in the first frame's fcTL "
+                           "don't match the ones in IHDR");
+    
+    /* more error checking */
+    png_ensure_fcTL_is_valid(png_ptr, width, height, x_offset, y_offset, 
+                             delay_num, delay_den, dispose_op, blend_op);
+    
+    png_save_uint_32(data, png_ptr->next_seq_num);
+    png_save_uint_32(data + 4, width);
+    png_save_uint_32(data + 8, height);
+    png_save_uint_32(data + 12, x_offset);
+    png_save_uint_32(data + 16, y_offset);
+    png_save_uint_16(data + 20, delay_num);
+    png_save_uint_16(data + 22, delay_den);
+    data[24] = dispose_op;
+    data[25] = blend_op;
+    
+    png_write_chunk(png_ptr, (png_bytep)png_fcTL, data, (png_size_t)26);
+    
+    png_ptr->next_seq_num++;
+}
+#endif /* PNG_WRITE_APNG_SUPPORTED */
+
 /* Initializes the row writing capability of libpng */
 void /* PRIVATE */
 png_write_start_row(png_structp png_ptr)
@@ -2159,8 +2252,8 @@
 #ifndef PNG_WRITE_WEIGHTED_FILTER_SUPPORTED
   if (png_ptr->row_number == 0 && filter_to_do == PNG_ALL_FILTERS)
   {
-      /* These will never be selected so we need not test them. */
-      filter_to_do &= ~(PNG_FILTER_UP | PNG_FILTER_PAETH);
+      /* These use previous row */
+      filter_to_do &= ~(PNG_FILTER_UP | PNG_FILTER_AVG | PNG_FILTER_PAETH);
   }
 #endif 
 
@@ -2829,4 +2922,39 @@
    }
 #endif
 }
+
+#ifdef PNG_WRITE_APNG_SUPPORTED
+void /* PRIVATE */
+png_write_reset(png_structp png_ptr)
+{
+    png_ptr->row_number = 0;
+    png_ptr->pass = 0;
+    png_ptr->mode &= ~PNG_HAVE_IDAT;
+}
+
+void /* PRIVATE */
+png_write_reinit(png_structp png_ptr, png_infop info_ptr, 
+                 png_uint_32 width, png_uint_32 height)
+{
+    if (png_ptr->num_frames_written == 0 && 
+        (width != png_ptr->first_frame_width || 
+         height != png_ptr->first_frame_height))
+        png_error(png_ptr, "width and/or height in the first frame's fcTL "
+                           "don't match the ones in IHDR");
+    if (width > png_ptr->first_frame_width || 
+        height > png_ptr->first_frame_height)
+        png_error(png_ptr, "width and/or height for a frame greater than"
+                           "the ones in IHDR");
+    
+    png_set_IHDR(png_ptr, info_ptr, width, height, 
+                 info_ptr->bit_depth, info_ptr->color_type, 
+                 info_ptr->interlace_type, info_ptr->compression_type,
+                 info_ptr->filter_type);
+   
+    png_ptr->width = width;
+    png_ptr->height = height;
+    png_ptr->rowbytes = PNG_ROWBYTES(png_ptr->pixel_depth, width);
+    png_ptr->usr_width = png_ptr->width;
+}
+#endif
 #endif /* PNG_WRITE_SUPPORTED */
Index: pngrtran.c
===================================================================
--- pngrtran.c
+++ pngrtran.c
@@ -1352,7 +1352,7 @@
        * pixels.  This check added to libpng-1.2.19
        */
 #if (PNG_WARN_UNINITIALIZED_ROW==1)
-      png_error(png_ptr, "Uninitialized row");
+      png_warning(png_ptr, "Uninitialized row");
 #else
       png_warning(png_ptr, "Uninitialized row");
 #endif
