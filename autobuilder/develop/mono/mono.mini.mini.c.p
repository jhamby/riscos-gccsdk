--- mono/mini/mini.c.orig	2013-01-08 18:41:05.000000000 +0000
+++ mono/mini/mini.c	2013-01-16 19:41:47.000000000 +0000
@@ -3002,6 +3002,9 @@
 		target = patch_info->data.bb->native_offset + code;
 		break;
 	case MONO_PATCH_INFO_ABS:
+#ifdef __riscos__
+	case MONO_PATCH_INFO_RISCOS_STKOVF:
+#endif
 		target = patch_info->data.target;
 		break;
 	case MONO_PATCH_INFO_LABEL:
@@ -4914,6 +4917,19 @@
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
@@ -6164,7 +6180,7 @@
 static void
 mini_parse_debug_options (void)
 {
-	char *options = getenv ("MONO_DEBUG");
+	char *options = getenv ("Mono$Debug");
 	gchar **args, **ptr;
 	
 	if (!options)
@@ -6208,7 +6224,7 @@
 		else if (!strcmp (arg, "soft-breakpoints"))
 			debug_options.soft_breakpoints = TRUE;
 		else {
-			fprintf (stderr, "Invalid option for the MONO_DEBUG env variable: %s\n", arg);
+			fprintf (stderr, "Invalid option for the Mono$Debug env variable: %s\n", arg);
 			fprintf (stderr, "Available options: 'handle-sigint', 'keep-delegates', 'reverse-pinvoke-exceptions', 'collect-pagefault-stats', 'break-on-unverified', 'no-gdb-backtrace', 'dont-free-domains', 'suspend-on-sigsegv', 'suspend-on-unhandled', 'dyn-runtime-invoke', 'gdb', 'explicit-null-checks', 'init-stacks'\n");
 			exit (1);
 		}
@@ -6437,7 +6453,7 @@
 	mono_threads_runtime_init (&ticallbacks);
 
 
-	if (getenv ("MONO_DEBUG") != NULL)
+	if (getenv ("Mono$Debug") != NULL)
 		mini_parse_debug_options ();
 	
 	mono_arch_cpu_init ();
