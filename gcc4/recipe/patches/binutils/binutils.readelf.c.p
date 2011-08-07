--- binutils/readelf.c.orig	2011-07-16 19:17:00.000000000 +0100
+++ binutils/readelf.c	2011-07-16 17:22:12.000000000 +0100
@@ -1739,6 +1739,10 @@ get_dynamic_type (unsigned long type)
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
