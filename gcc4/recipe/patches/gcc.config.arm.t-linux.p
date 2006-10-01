Index: gcc/config/arm/t-linux
===================================================================
RCS file: gcc/config/arm/t-linux,v
retrieving revision 1.10
diff -u -r1.10 t-linux
--- gcc/config/arm/t-linux	15 May 2004 12:41:35 -0000	1.10
+++ gcc/config/arm/t-linux	17 May 2006 21:59:32 -0000
@@ -4,6 +4,13 @@
 LIBGCC2_DEBUG_CFLAGS = -g0
 
 LIB1ASMSRC = arm/lib1funcs.asm
+#LIB1ASMFUNCS = _udivsi3 _divsi3 _umodsi3 _modsi3 _dvmd_lnx _bb_init_func \
+#	_call_via_rX _interwork_call_via_rX \
+#	_lshrdi3 _ashrdi3 _ashldi3 \
+#	_negdf2 _addsubdf3 _muldivdf3 _cmpdf2 _unorddf2 _fixdfsi _fixunsdfsi \
+#	_truncdfsf2 _negsf2 _addsubsf3 _muldivsf3 _cmpsf2 _unordsf2 \
+#	_fixsfsi _fixunssfsi _floatdidf _floatdisf
+
 LIB1ASMFUNCS = _udivsi3 _divsi3 _umodsi3 _modsi3 _dvmd_lnx
 
 # MULTILIB_OPTIONS = mhard-float/msoft-float
