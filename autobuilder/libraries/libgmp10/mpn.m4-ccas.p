--- mpn/m4-ccas.org	2014-04-21 20:57:01.851797698 +0200
+++ mpn/m4-ccas	2014-04-21 20:58:06.314987286 +0200
@@ -98,7 +98,7 @@
 fi
 
 echo "$M4 $DEFS $ASM >$TMP"
-$M4 $DEFS $ASM >$TMP || exit
+$M4 $DEFS $ASM | sed s"/[[:space:]]bx[[:space:]]/ mov pc, /" >$TMP || exit
 
 echo "$CC"
 $CC || exit
