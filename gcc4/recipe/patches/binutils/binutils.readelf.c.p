--- binutils/readelf.c.orig	2012-11-05 17:27:35.000000000 +0100
+++ binutils/readelf.c	2012-11-17 14:55:35.865614483 +0100
@@ -1798,6 +1798,12 @@ get_dynamic_type (unsigned long type)
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
