--- src/Congruence_System.defs.hh.orig	2015-03-01 19:57:52.887328139 +0100
+++ src/Congruence_System.defs.hh	2015-03-01 19:57:12.315328259 +0100
@@ -33,6 +33,7 @@ site: http://www.cs.unipr.it/ppl/ . */
 #include "Grid.types.hh"
 #include "Grid_Certificate.types.hh"
 #include <iosfwd>
+#include <cstddef>
 
 namespace Parma_Polyhedra_Library {
 
@@ -235,7 +236,7 @@ public:
   class const_iterator
     : public std::iterator<std::forward_iterator_tag,
 			   Congruence,
-			   ptrdiff_t,
+			   std::ptrdiff_t,
 			   const Congruence*,
 			   const Congruence&> {
   public:
