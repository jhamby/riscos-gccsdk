--- pdf/UnicodeToAcornMapData.cc.orig	2021-08-11 12:26:03.268366018 -0700
+++ pdf/UnicodeToAcornMapData.cc	2021-08-11 12:35:07.252555324 -0700
@@ -256,7 +256,7 @@
   {0x00FD,0xFD},
   {0x00FE,0xFE},
   {0x00FF,0xFF},
-  {-1,-1}
+  {(unsigned int)-1,(unsigned int)-1}
 };
 
 UnicodeToAcornMapData unicodeToAcornSymbol[] =
@@ -455,7 +455,7 @@
   {0xF8FC,0xFC},
   {0xF8FD,0xFD},
   {0xF8FE,0xFE},
-  {-1,-1}
+  {(unsigned int)-1,(unsigned int)-1}
 };
 
 
@@ -664,5 +664,5 @@
   {0x27BC,0xFC},
   {0x27BD,0xFD},
   {0x27BE,0xFE},
-  {-1,-1}
+  {(unsigned int)-1,(unsigned int)-1}
 };
