--- binutils/readelf.c.orig	2013-11-18 09:40:15.000000000 +0100
+++ binutils/readelf.c	2013-12-31 13:16:36.454126719 +0100
@@ -1865,6 +1865,12 @@ get_dynamic_type (unsigned long type)
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
