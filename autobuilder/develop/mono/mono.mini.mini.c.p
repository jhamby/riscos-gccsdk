--- mono/mini/mini.c.orig	2011-12-19 21:10:25.000000000 +0000
+++ mono/mini/mini.c	2012-02-13 19:30:51.000000000 +0000
@@ -2966,6 +2966,9 @@
 		target = patch_info->data.bb->native_offset + code;
 		break;
 	case MONO_PATCH_INFO_ABS:
+#ifdef __riscos__
+	case MONO_PATCH_INFO_RISCOS_STKOVF:
+#endif
 		target = patch_info->data.target;
 		break;
 	case MONO_PATCH_INFO_LABEL:
@@ -4790,6 +4793,19 @@
 					}
 				}
 			}
+#ifdef __riscos__
+			/* FIXME: So far this is the only way I have found to stop the JIT
+			 * from using r8 and r11 as general registers. r8 is used in PIC
+			 * PLT entries and is therefor corrupted across function calls. r11
+			 * is the stack frame pointer.
+			 */
+			for (l = regs; l; l = l->next) {
+				if (GPOINTER_TO_UINT (l->data) == 8 || GPOINTER_TO_UINT (l->data) == 11) {
+					regs = g_list_delete_link (regs, l);
+					break;
+				}
+			}
+#endif
 			mono_linear_scan (cfg, vars, regs, &cfg->used_int_regs);
 		}
 	}
