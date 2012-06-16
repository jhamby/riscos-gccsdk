Index: gcc/ginclude/unwind-arm-common.h
===================================================================
--- gcc/ginclude/unwind-arm-common.h	(revision 178826)
+++ gcc/ginclude/unwind-arm-common.h	(working copy)
@@ -117,6 +117,9 @@
 	  _uw reserved1;
 	}
       pr_cache;
+#ifdef __riscos__
+      _uw alloca_fp;
+#endif
       long long int :0;	/* Force alignment to 8-byte boundary */
     };
 
