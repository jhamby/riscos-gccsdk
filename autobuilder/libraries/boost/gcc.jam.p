--- tools/build/v2/tools/gcc.jam.orig	2009-10-28 07:47:51.000000000 +0000
+++ tools/build/v2/tools/gcc.jam	2011-02-09 14:53:53.647598700 +0000
@@ -903,6 +903,10 @@
             {
                 # Darwin has no threading options so do not set anything here.
             }
+            case riscos :
+            {
+                # RISC OS has no threading options so do not set anything here.
+            }
             case * :
             {
                 option = -pthread ;
