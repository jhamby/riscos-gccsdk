--- tools/build/v2/tools/gcc.jam.orig	2007-06-12 05:47:49.000000000 +0100
+++ tools/build/v2/tools/gcc.jam	2008-04-09 08:57:03.809918300 +0100
@@ -440,22 +440,23 @@
 }
 
 .IMPLIB-COMMAND = ;
-if [ os.on-windows ]
-{
-    .IMPLIB-COMMAND = "-Wl,--out-implib," ;
-    generators.register
-        [ new gcc-linking-generator gcc.link
-            : OBJ SEARCHED_LIB STATIC_LIB IMPORT_LIB
-            : EXE
-            : <toolset>gcc ] ;
-    generators.register
-        [ new gcc-linking-generator gcc.link.dll
-            : OBJ SEARCHED_LIB STATIC_LIB IMPORT_LIB
-            : IMPORT_LIB SHARED_LIB
-            : <toolset>gcc ] ;
-}
-else
-{
+# Comment out for RISC OS cross compile on cygwin
+#if [ os.on-windows ]
+#{
+#    .IMPLIB-COMMAND = "-Wl,--out-implib," ;
+#    generators.register
+#        [ new gcc-linking-generator gcc.link
+#            : OBJ SEARCHED_LIB STATIC_LIB IMPORT_LIB
+#            : EXE
+#            : <toolset>gcc ] ;
+#    generators.register
+#        [ new gcc-linking-generator gcc.link.dll
+#            : OBJ SEARCHED_LIB STATIC_LIB IMPORT_LIB
+#            : IMPORT_LIB SHARED_LIB
+#            : <toolset>gcc ] ;
+#}
+#else
+#{
     generators.register
         [ new gcc-linking-generator gcc.link
             : LIB OBJ
@@ -466,7 +467,7 @@
             : LIB OBJ
             : SHARED_LIB
             : <toolset>gcc ] ;
-}
+#}
 
 # Declare flags for linking
 # First, the common flags
@@ -635,53 +636,54 @@
 # Set up threading support. It's somewhat contrived, so perform it at the end,
 # to avoid cluttering other code.
 
-if [ os.on-windows ] 
-{
-    flags gcc OPTIONS <threading>multi : -mthreads ;
-}
-else if [ modules.peek : UNIX ] 
-{
-    switch [ modules.peek : JAMUNAME ]
-    {
-    case SunOS* :
-        {
-        flags gcc OPTIONS <threading>multi : -pthreads ;
-        flags gcc FINDLIBS-SA <threading>multi : rt ;
-        }
-    case BeOS :
-        {
+# Comment out to try to get RISC OS cross compile to work on cygwin
+#if [ os.on-windows ] 
+#{
+#    flags gcc OPTIONS <threading>multi : -mthreads ;
+#}
+#else if [ modules.peek : UNIX ] 
+#{
+#    switch [ modules.peek : JAMUNAME ]
+#    {
+#    case SunOS* :
+#        {
+#        flags gcc OPTIONS <threading>multi : -pthreads ;
+#        flags gcc FINDLIBS-SA <threading>multi : rt ;
+#        }
+#    case BeOS :
+#        {
         # BeOS has no threading options, don't set anything here.
-        }
-    case *BSD :
-        {
-        flags gcc OPTIONS <threading>multi : -pthread ;
-        # there is no -lrt on BSD
-        }
-    case DragonFly :
-        {
-        flags gcc OPTIONS <threading>multi : -pthread ;
-        # there is no -lrt on BSD - DragonFly is a FreeBSD variant,
-        # which anoyingly doesn't say it's a *BSD.
-        }
-    case IRIX :
-        {
+#        }
+#    case *BSD :
+#        {
+#        flags gcc OPTIONS <threading>multi : -pthread ;
+#        # there is no -lrt on BSD
+#        }
+#    case DragonFly :
+#        {
+#        flags gcc OPTIONS <threading>multi : -pthread ;
+#        # there is no -lrt on BSD - DragonFly is a FreeBSD variant,
+#        # which anoyingly doesn't say it's a *BSD.
+#        }
+#    case IRIX :
+#        {
         # gcc on IRIX does not support multi-threading, don't set anything here.
-        }
-    case HP_UX :
-        {
+#        }
+#    case HP_UX :
+#        {
         # gcc on HP-UX does not support multi-threading, don't set anything here
-        }
-    case Darwin :
-        {
+#        }
+#    case Darwin :
+#        {
         # Darwin has no threading options, don't set anything here.
-        }
-    case * :
-        {
-        flags gcc OPTIONS <threading>multi : -pthread ;
-        flags gcc FINDLIBS-SA <threading>multi : rt ;
-        }
-    }
-}
+#        }
+#    case * :
+#        {
+#        flags gcc OPTIONS <threading>multi : -pthread ;
+#        flags gcc FINDLIBS-SA <threading>multi : rt ;
+#        }
+#    }
+#}
 
 local rule cpu-flags ( toolset variable : architecture : instruction-set + : values + : default ? )
 {
