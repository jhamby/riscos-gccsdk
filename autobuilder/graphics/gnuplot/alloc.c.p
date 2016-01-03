--- alloc.c.orig	2001-06-26 07:31:48.000000000 +0200
+++ alloc.c	2015-11-21 15:24:52.000000000 +0100
@@ -2,6 +2,13 @@
 static char *RCSid = "$Id: alloc.c,v 1.1.1.2.2.1 2001/06/25 16:06:34 broeker Exp $";
 #endif
 
+/* Mod MRB 21.11.2015
+ * replaced two calls to malloc with calloc - guarantees that all bytes of
+ * the allocated memory block have been initialized to zero.
+ * avoids a segmentation fault when memory not zero'd.
+ */
+
+
 /* GNUPLOT - alloc.c */
 
 /*[
@@ -63,7 +70,7 @@
 #  define GP_FARMALLOC(size) farmalloc ((size))
 #  define GP_FARREALLOC(p,size) farrealloc ((p), (size))
 # else
-#  define GP_FARMALLOC(size) malloc ((size_t)(size))
+#  define GP_FARMALLOC(size) calloc ((size_t)(size),1)
 #  define GP_FARREALLOC(p,size) realloc ((p), (size_t)(size))
 # endif
 #endif
@@ -114,7 +121,7 @@
 }
 
 #define mark_free(p) ( ((struct frame_struct *)p)[-1].checksum = CHECKSUM_FREE)
-	
+
 static void validate(x)
 void *x;
 {
@@ -132,7 +139,7 @@
 		fprintf(stderr, "Heap corruption at end of block for %-60s\n", p->use);
 		int_error("Argh !", NO_CARET);
 	}
-}		
+}
 
 /* used to confirm that a pointer is inside an allocated region via
  * macro CHECK_POINTER. Nowhere near as good as using a bounds-checking
@@ -161,16 +168,16 @@
 {
 	struct frame_struct *p;
 	unsigned long total_size = size + RESERVED_SIZE + 1;
-	
+
 	TRACE_ALLOC(("gp_alloc %d for %s\n", (int) size, usage?usage:"<unknown>"));
-	
-	p=malloc(total_size);
+
+	p=calloc(total_size,1);
 	if (!p) int_error("Out of memory", NO_CARET);
 
 	bytes_allocated += size;
-	
+
 	mark(p,size,usage);
-	
+
 	return (char *)(p+1);
 }
 
@@ -184,7 +191,7 @@
 	mark_free(old); /* if block gets moved, old block is marked free */
 	                /* if not, we'll remark it later */
 
-	
+
 	{
 		struct frame_struct *p = (struct frame_struct *)old - 1;
 		unsigned long total = size + RESERVED_SIZE + 1;
@@ -197,7 +204,7 @@
 		  (int)size, usage?usage:"<unknown>", p->requested_size));
 
 		bytes_allocated += size - p->requested_size;
-		
+
 		mark(p,size,usage);
 
 		return (generic *)(p+1);
@@ -255,10 +262,10 @@
 #else  /* CHECK_HEAP_USE */
 
 /* gp_alloc:
- * allocate memory 
- * This is a protected version of malloc. It causes an int_error 
- * if there is not enough memory, but first it tries FreeHelp() 
- * to make some room, and tries again. If message is NULL, we 
+ * allocate memory
+ * This is a protected version of malloc. It causes an int_error
+ * if there is not enough memory, but first it tries FreeHelp()
+ * to make some room, and tries again. If message is NULL, we
  * allow NULL return. Otherwise, we handle the error, using the
  * message to create the int_error string. Note cp/sp_extend uses realloc,
  * so it depends on this using malloc().
