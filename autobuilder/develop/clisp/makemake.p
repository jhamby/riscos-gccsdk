--- src/makemake_base.in	2006-07-15 09:11:28.000000000 +0100
+++ src/makemake.in	2006-09-12 21:41:40.000000000 +0100
@@ -1607,6 +1607,7 @@
   CPARTS=$CPARTS' zthread'
 fi
 CPARTS=$CPARTS' built'
+CPARTS=$CPARTS' riscos'
 
 EVERY_INCLUDES='lispbibl fsubr subr pseudofun constsym constobj '$OS_INCLUDES
 
@@ -1772,7 +1773,7 @@
 LPARTS=$LPARTS' clos-dependent clos-print clos-custom documentation'
 LPARTS=$LPARTS' fill-out disassem condition loadform gstream xcharin keyboard'
 LPARTS=$LPARTS' screen runprog query reploop dribble complete'
-LPARTS=$LPARTS' pprint describe room edit macros3 clhs inspect gray'
+LPARTS=$LPARTS' pprint describe room edit macros3 clhs inspect gray riscos'
 if [ "${with_threads}" != no ]; then
   LPARTS=$LPARTS' threads'
 fi
