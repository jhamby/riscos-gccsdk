--- src/Grid_Generator_System.defs.hh.orig	2015-03-01 19:57:31.127328203 +0100
+++ src/Grid_Generator_System.defs.hh	2015-03-01 19:57:12.315328259 +0100
@@ -30,6 +30,7 @@ site: http://www.cs.unipr.it/ppl/ . */
 #include "Variables_Set.types.hh"
 #include "Grid.types.hh"
 #include <iosfwd>
+#include <cstddef>
 
 namespace Parma_Polyhedra_Library {
 
@@ -267,7 +268,7 @@ public:
   class const_iterator
     : public std::iterator<std::forward_iterator_tag,
 			   Grid_Generator,
-			   ptrdiff_t,
+			   std::ptrdiff_t,
 			   const Grid_Generator*,
 			   const Grid_Generator&>,
       private Generator_System::const_iterator {
