Index: gcc/config/arm/unwind-arm.h
===================================================================
--- gcc/config/arm/unwind-arm.h	(revision 172858)
+++ gcc/config/arm/unwind-arm.h	(working copy)
@@ -117,6 +117,9 @@
 	  _uw reserved1;
 	}
       pr_cache;
+#ifdef __riscos__
+      _uw alloca_fp;
+#endif
       long long int :0;	/* Force alignment to 8-byte boundary */
     };
 
@@ -228,7 +231,7 @@
       if (!tmp)
 	return 0;
 
-#if (defined(linux) && !defined(__uClinux__)) || defined(__NetBSD__)
+#if (defined(linux) && !defined(__uClinux__)) || defined(__NetBSD__) || (defined(__riscos__) && !defined(__TARGET_MODULE__))
       /* Pc-relative indirect.  */
       tmp += ptr;
       tmp = *(_Unwind_Word *) tmp;
