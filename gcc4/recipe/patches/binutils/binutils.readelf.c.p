--- binutils/readelf.c.orig	2012-03-17 18:03:10.000000000 +0000
+++ binutils/readelf.c	2012-03-17 18:02:54.000000000 +0000
@@ -1765,6 +1765,12 @@ get_dynamic_type (unsigned long type)
 	    case EM_IA_64:
 	      result = get_ia64_dynamic_type (type);
 	      break;
+	    case EM_ARM:
+	      if (type == DT_RISCOS_PIC)
+		return "RISCOS_PIC";
+	      else if (type == DT_RISCOS_ABI_VERSION)
+		return "RISCOS_ABI_VERSION";
+	      /* Fall through.  */
 	    default:
 	      result = NULL;
 	      break;
