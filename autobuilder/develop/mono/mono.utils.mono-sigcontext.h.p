--- mono/utils/mono-sigcontext.h.orig	2011-09-14 04:32:19.000000000 +0100
+++ mono/utils/mono-sigcontext.h	2011-11-14 17:46:11.000000000 +0000
@@ -241,7 +241,15 @@
 	#define UCONTEXT_REG_R11(ctx) (((ucontext_t*)(ctx))->uc_mcontext->__ss.__r[11])
 	#define UCONTEXT_REG_R12(ctx) (((ucontext_t*)(ctx))->uc_mcontext->__ss.__r[12])
 	#define UCONTEXT_REG_CPSR(ctx) (((ucontext_t*)(ctx))->uc_mcontext->__ss.__cpsr)
-#elif defined(__linux__)
+#elif defined(__linux__) || defined(__riscos__)
+#ifdef __riscos__
+#include <ucontext.h>
+	/* I think this structure is supposed to match the 3rd parameter passed to
+	   a signal handler.  */
+	typedef struct arm_ucontext {
+		struct sigcontext sig_ctx;
+	} arm_ucontext;
+#else
 	typedef struct arm_ucontext {
 		unsigned long       uc_flags;
 		struct arm_ucontext *uc_link;
@@ -255,6 +263,7 @@
 		* we don't use them for now...
 		*/
 	} arm_ucontext;
+#endif
 	#define UCONTEXT_REG_PC(ctx) (((arm_ucontext*)(ctx))->sig_ctx.arm_pc)
 	#define UCONTEXT_REG_SP(ctx) (((arm_ucontext*)(ctx))->sig_ctx.arm_sp)
 	#define UCONTEXT_REG_LR(ctx) (((arm_ucontext*)(ctx))->sig_ctx.arm_lr)
