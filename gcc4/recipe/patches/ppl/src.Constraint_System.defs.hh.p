--- src/Constraint_System.defs.hh.orig	2015-03-01 19:58:03.831328106 +0100
+++ src/Constraint_System.defs.hh	2015-03-01 19:57:12.315328259 +0100
@@ -33,6 +33,7 @@ site: http://www.cs.unipr.it/ppl/ . */
 #include "Congruence_System.types.hh"
 #include <iterator>
 #include <iosfwd>
+#include <cstddef>
 
 namespace Parma_Polyhedra_Library {
 
@@ -204,7 +205,7 @@ public:
   class const_iterator
     : public std::iterator<std::forward_iterator_tag,
 			   Constraint,
-			   ptrdiff_t,
+			   std::ptrdiff_t,
 			   const Constraint*,
 			   const Constraint&> {
   public:
