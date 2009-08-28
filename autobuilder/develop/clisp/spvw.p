--- src/spvw.d.orig	2008-06-16 14:28:19.000000000 -0700
+++ src/spvw.d	2009-08-27 20:43:00.000000000 -0700
@@ -22,6 +22,44 @@
  version */
 #include "lispbibl.c"
 
+# ---- RISC OS patch - application of OS_GSTrans to filename command line arguments -----------------
+#include <swis.h>
+#include <unixlib/local.h>
+
+
+nonreturning_function(local, fehler_riscos_gstrans, (const char *fname, const char *error_message)) {
+	pushSTACK(asciz_to_string(error_message, Symbol_value(S(ascii))));
+	pushSTACK(asciz_to_string(fname, Symbol_value(S(ascii))));
+	fehler(error, GETTEXT("Failed to translate filename ~S: ~S"));
+}
+
+
+void riscos_gstrans( const char * asciz, char * buffer, uintL bufferSize )
+{
+	_kernel_oserror *error_block;
+	uintL n;
+
+	error_block = _swix(OS_GSTrans, _IN(0) | _IN(1) | _IN(2) | _OUT(2), asciz, buffer, bufferSize-1, &n);
+
+	if (error_block != NULL)
+	{
+		fehler_riscos_gstrans( asciz, error_block->errmess );
+	}
+
+	// terminate string output by OS_GSTrans
+	buffer[n] = 0;
+}
+
+#define with_riscos_gstrans(fname,namevar,statement) \
+  { \
+	char namevar[512]; \
+	riscos_gstrans( fname, namevar, sizeof(namevar) ); \
+	statement \
+  }
+
+# -----------------------------------------------------------------------------
+
+
 #include <string.h> /* declares strchr() and possibly memset() */
 #ifdef MULTITHREAD
   #define bzero(ptr,len)  memset(ptr,0,len)
@@ -2951,7 +2989,7 @@
   init_subr_tab_1();            /* initialize subr_tab */
   markwatchset = NULL; markwatchset_allocated = markwatchset_size = 0;
   if (p->argv_memfile)
-    loadmem(p->argv_memfile);   /* load memory file */
+    with_riscos_gstrans( p->argv_memfile, unix_fname, { loadmem(unix_fname); } )   /* load memory file */
   else if (!loadmem_from_executable())
     p->argv_memfile = get_executable_name();
   else initmem();               /* manual initialization */
@@ -3026,7 +3064,8 @@
       skipSTACK(1);
     }
   } else {                      /* set it */
-    pushSTACK(asciz_to_string(p->argv_lisplibdir,O(pathname_encoding)));
+    with_riscos_gstrans( p->argv_lisplibdir, unix_pname,
+               { pushSTACK(asciz_to_string(unix_pname,O(pathname_encoding))); } )
     funcall(L(set_lib_directory),1);
   }
   if (p->argv_batchmode_p) {
@@ -3123,7 +3162,8 @@
     var uintL count = p->argv_init_filecount;
     finish_entry_frame(DRIVER,returner,,goto done_driver_init_files;);
     do {
-      pushSTACK(asciz_to_string(*fileptr++,O(misc_encoding)));
+      with_riscos_gstrans( *fileptr++, unix_fname,
+	    { pushSTACK(asciz_to_string(unix_fname,O(misc_encoding))); } )
       funcall(S(load),1);
     } while (--count);
    done_driver_init_files:
@@ -3143,7 +3183,9 @@
       finish_entry_frame(DRIVER,returner,,goto done_driver_compile_files;);
       do {
         var uintC argcount = 1;
-        var object filename = asciz_to_string(fileptr->input_file,O(misc_encoding));
+        var object filename;
+		with_riscos_gstrans( fileptr->input_file, unix_fname,
+		  { filename = asciz_to_string(unix_fname,O(misc_encoding)); } )
         pushSTACK(filename);
         pushSTACK(O(source_file_type));      /* #".lisp" */
         funcall(L(cd),0); pushSTACK(value1); /* (CD) */
@@ -3152,7 +3194,8 @@
         funcall(L(merge_pathnames),2); /* (MERGE-PATHNAMES file ...) */
         pushSTACK(value1);
         if (fileptr->output_file) {
-          filename = asciz_to_string(fileptr->output_file,O(misc_encoding));
+          with_riscos_gstrans( fileptr->output_file, unix_fname,
+            { filename = asciz_to_string(unix_fname,O(misc_encoding)); } )
           pushSTACK(S(Koutput_file));
           pushSTACK(filename);
           pushSTACK(O(compiled_file_type));    /* #".fas" */
@@ -3220,7 +3263,8 @@
     if (asciz_equal(p->argv_execute_file,"-")) {
       pushSTACK(Symbol_value(S(standard_input))); /* *STANDARD-INPUT* */
     } else {
-      pushSTACK(asciz_to_string(p->argv_execute_file,O(misc_encoding)));
+      with_riscos_gstrans( p->argv_execute_file, unix_fname,
+        { pushSTACK(asciz_to_string(unix_fname,O(misc_encoding))); } )
     }
    #ifdef WIN32_NATIVE
     pushSTACK(S(Kextra_file_types));
