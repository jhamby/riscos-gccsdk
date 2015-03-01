--- src/Generator_System.defs.hh.orig	2015-03-01 19:58:13.511328077 +0100
+++ src/Generator_System.defs.hh	2015-03-01 19:57:12.319328259 +0100
@@ -33,6 +33,7 @@ site: http://www.cs.unipr.it/ppl/ . */
 #include "Polyhedron.types.hh"
 #include "Poly_Con_Relation.defs.hh"
 #include <iosfwd>
+#include <cstddef>
 
 namespace Parma_Polyhedra_Library {
 
@@ -250,7 +251,7 @@ public:
   class const_iterator
     : public std::iterator<std::forward_iterator_tag,
 			   Generator,
-			   ptrdiff_t,
+			   std::ptrdiff_t,
 			   const Generator*,
 			   const Generator&> {
   public:
