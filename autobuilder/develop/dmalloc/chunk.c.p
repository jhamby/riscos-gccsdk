--- ./chunk.c.org	2004-10-19 15:51:21.000000000 +0100
+++ ./chunk.c	2006-10-11 22:47:05.690000000 +0100
@@ -163,6 +163,9 @@
 static	unsigned long	func_free_c = 0;	/* count the frees */
 static	unsigned long	func_delete_c = 0;	/* count the deletes */
 
+static void
+get_callers(const char *callers[], unsigned int skip, unsigned int count);
+
 /**************************** skip list routines *****************************/
 
 /*
@@ -1003,6 +1006,7 @@
   char		where_buf[MAX_FILE_LENGTH + 64];
   char		where_buf2[MAX_FILE_LENGTH + 64];
   const char	*prev_file;
+  const char	**prev_callers;
   const void	*dump_pnt = user_pnt;
   const void	*start_user;
   unsigned int	prev_line, user_size;
@@ -1021,6 +1025,7 @@
   else {
     prev_file = slot_p->sa_file;
     prev_line = slot_p->sa_line;
+    prev_callers = slot_p->sa_callers;
     user_size = slot_p->sa_user_size;
     if (user_pnt == NULL) {
       get_pnt_info(slot_p, &pnt_info);
@@ -1039,17 +1044,19 @@
   if (start_user == NULL) {
     dmalloc_message("  from '%s' prev access '%s'",
 		    _dmalloc_chunk_desc_pnt(where_buf, sizeof(where_buf),
-					    now_file, now_line),
+					    now_file, now_line, 0),
 		    _dmalloc_chunk_desc_pnt(where_buf2, sizeof(where_buf2),
-					    prev_file, prev_line));
+					    prev_file, prev_line,
+					    prev_callers));
   }
   else {
     dmalloc_message("  pointer '%#lx' from '%s' prev access '%s'",
 		    (unsigned long)start_user,
 		    _dmalloc_chunk_desc_pnt(where_buf, sizeof(where_buf),
-					    now_file, now_line),
+					    now_file, now_line, 0),
 		    _dmalloc_chunk_desc_pnt(where_buf2, sizeof(where_buf2),
-					    prev_file, prev_line));
+					    prev_file, prev_line,
+					    prev_callers));
   }
   
   /*
@@ -1130,7 +1137,8 @@
 		      (unsigned long)other_p->sa_mem, other_p->sa_user_size,
 		      _dmalloc_chunk_desc_pnt(where_buf, sizeof(where_buf),
 					      other_p->sa_file,
-					      other_p->sa_line));
+					      other_p->sa_line,
+					      other_p->sa_callers));
     }
   }
   /* find the next pointer in case it ran under */
@@ -1144,7 +1152,8 @@
 		      (unsigned long)other_p->sa_mem, other_p->sa_user_size,
 		      _dmalloc_chunk_desc_pnt(where_buf, sizeof(where_buf),
 					      other_p->sa_file,
-					      other_p->sa_line));
+					      other_p->sa_line,
+					      other_p->sa_callers));
     }
   }
 }
@@ -1867,9 +1876,21 @@
  * line -> Line number or 0.
  */
 char	*_dmalloc_chunk_desc_pnt(char *buf, const int buf_size,
-				 const char *file, const unsigned int line)
+				 const char *file, const unsigned int line,
+				 const char *callers[])
 {
-  if (file == DMALLOC_DEFAULT_FILE && line == DMALLOC_DEFAULT_LINE) {
+  if (file == DMALLOC_DEFAULT_FILE && line == DMALLOC_DEFAULT_LINE &&
+      callers && callers[0]) {
+    unsigned int i;
+    size_t offset = 0;
+    for (i = 0; i != SA_CALLERS && callers[i]; i++) {
+      (void)loc_snprintf(buf + offset, buf_size - offset, "%s/", callers[i]);
+      offset += strlen(buf + offset);
+      if (buf_size <= offset)
+        break;
+    }
+  }
+  else if (file == DMALLOC_DEFAULT_FILE && line == DMALLOC_DEFAULT_LINE) {
     (void)loc_snprintf(buf, buf_size, "unknown");
   }
   else if (line == DMALLOC_DEFAULT_LINE) {
@@ -2394,6 +2415,7 @@
   
   slot_p->sa_file = file;
   slot_p->sa_line = line;
+  get_callers(slot_p->sa_callers, 3, SA_CALLERS);
   slot_p->sa_use_iter = _dmalloc_iter_c;
 #if LOG_PNT_SEEN_COUNT
   slot_p->sa_seen_c++;
@@ -2435,7 +2457,7 @@
     dmalloc_message("*** %s: at '%s' for %ld bytes, got '%s'",
 		    trans_log,
 		    _dmalloc_chunk_desc_pnt(where_buf, sizeof(where_buf),
-					    file, line),
+					    file, line, slot_p->sa_callers),
 		    size, display_pnt(pnt_info.pi_user_start, slot_p, disp_buf,
 				      sizeof(disp_buf)));
   }
@@ -2507,7 +2529,7 @@
     /* does the user want a specific message? */
     dmalloc_message("WARNING: tried to free(0) from '%s'",
 		    _dmalloc_chunk_desc_pnt(where_buf, sizeof(where_buf),
-					    file, line));
+					    file, line, 0));
 #endif
     
     /*
@@ -2563,11 +2585,12 @@
   if (BIT_IS_SET(_dmalloc_flags, DEBUG_LOG_TRANS)) {
     dmalloc_message("*** free: at '%s' pnt '%s': size %u, alloced at '%s'",
 		    _dmalloc_chunk_desc_pnt(where_buf, sizeof(where_buf), file,
-					    line),
+					    line, 0),
 		    display_pnt(user_pnt, slot_p, disp_buf, sizeof(disp_buf)),
 		    slot_p->sa_user_size,
 		    _dmalloc_chunk_desc_pnt(where_buf2, sizeof(where_buf2),
-					    slot_p->sa_file, slot_p->sa_line));
+					    slot_p->sa_file, slot_p->sa_line,
+					    slot_p->sa_callers));
   }
   
 #if MEMORY_TABLE_TOP_LOG
@@ -2578,6 +2601,7 @@
   /* update the file/line -- must be after _dmalloc_table_delete */
   slot_p->sa_file = file;
   slot_p->sa_line = line;
+  get_callers(slot_p->sa_callers, 3, SA_CALLERS);
   
   /* monitor current allocation level */
   alloc_current -= slot_p->sa_user_size;
@@ -2777,6 +2801,7 @@
      */
     slot_p->sa_file = file;
     slot_p->sa_line = line;
+    get_callers(slot_p->sa_callers, 3, SA_CALLERS);
   }
   
   if (BIT_IS_SET(_dmalloc_flags, DEBUG_LOG_TRANS)) {
@@ -2793,10 +2818,11 @@
     dmalloc_message("*** %s: at '%s' from '%#lx' (%u bytes) file '%s' to '%#lx' (%lu bytes)",
 		    trans_log,
 		    _dmalloc_chunk_desc_pnt(where_buf, sizeof(where_buf),
-					    file, line),
+					    file, line, 0),
 		    (unsigned long)old_user_pnt, old_size,
 		    _dmalloc_chunk_desc_pnt(where_buf2, sizeof(where_buf2),
-					    old_file, old_line),
+					    old_file, old_line,
+					    slot_p->sa_callers),
 		    (unsigned long)new_user_pnt, new_size);
   }
   
@@ -3012,7 +3038,8 @@
 			slot_p->sa_user_size,
 			_dmalloc_chunk_desc_pnt(where_buf, sizeof(where_buf),
 						slot_p->sa_file,
-						slot_p->sa_line));
+						slot_p->sa_line,
+						slot_p->sa_callers));
 	
 	if ((! freed_b)
 	    && BIT_IS_SET(_dmalloc_flags, DEBUG_LOG_NONFREE_SPACE)) {
@@ -3132,3 +3159,99 @@
   
   return mem_count;
 }
+
+
+#define __UNIXLIB_INTERNALS
+#include <unixlib/unix.h>
+
+
+/**
+ * Get the names of caller functions.
+ *
+ * callers[]   filled with callers, terminated by NULL, at least count long
+ * skip        number of frames to skip before filling callers
+ * count       number of names to get
+ */
+static void
+get_callers(const char *callers[], unsigned int skip, unsigned int count)
+{
+  int *fp = __backtrace_getfp(), *oldfp = NULL;
+  unsigned int frame;
+
+  for (frame = 0; frame != count; frame++)
+    callers[frame] = NULL;
+
+  /* fp[0]  => pc
+     fp[-1] => lr
+     fp[-2] => sp
+     fp[-3] => previous fp  */
+
+  /* Skip unwanted frames. */
+  for (frame = 0; fp != NULL && frame != skip; frame++)
+  {
+      /* Check that FP is different.  */
+      if (fp == oldfp)
+	return;
+
+      /* Validate FP address.  */
+      if (!__valid_address(fp - 3, fp))
+	return;
+
+      oldfp = fp;
+      fp = (int *)fp[-3];
+  }
+
+  for (frame = 0; fp != NULL && frame != count; frame++)
+  {
+      unsigned int *pc;
+
+      /* Check that FP is different.  */
+      if (fp == oldfp)
+	break;
+
+      /* Validate FP address.  */
+      if (!__valid_address(fp - 3, fp))
+	break;
+
+      /* Retrieve PC counter.  */
+      if (__32bit)
+	pc = (unsigned int *)(fp[0] & 0xfffffffc);
+      else
+	pc = (unsigned int *)(fp[0] & 0x03fffffc);
+
+      if (!__valid_address(pc, pc))
+	break;
+
+      /* Retrieve function name.  */
+      if (!__valid_address(pc - 7, pc))
+	break;
+      else
+	{
+	  int address;
+	  const char *name = NULL;
+
+	  for (address = -3; address > -8; address--)
+	    {
+	      if ((pc[address] & 0xffffff00) == 0xff000000)
+		{
+		  name = (const char *)(pc + address) - (pc[address] & 0xff);
+		  break;
+		}
+	    }
+
+	  /* Function name sanity check.  */
+	  if (name != NULL
+	      && (!__valid_address(name, (name + 256))
+		  || strnlen(name, 256) == 256))
+	    name = NULL;
+
+	  if (!name)
+	    callers[frame] = "?";
+	  else
+	    callers[frame] = name;
+	}
+
+      oldfp = fp;
+      fp = (int *)fp[-3];
+  }
+}
