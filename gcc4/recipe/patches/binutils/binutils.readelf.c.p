--- binutils/readelf.c.orig	2011-09-21 22:49:13.000000000 +0200
+++ binutils/readelf.c	2011-11-21 19:42:18.630237517 +0100
@@ -1765,6 +1765,10 @@ get_dynamic_type (unsigned long type)
 	    case EM_IA_64:
 	      result = get_ia64_dynamic_type (type);
 	      break;
+	    case EM_ARM:
+	      if (type == DT_RISCOS_PIC)
+		return "RISCOS_PIC";
+	      /* Fall through.  */
 	    default:
 	      result = NULL;
 	      break;
