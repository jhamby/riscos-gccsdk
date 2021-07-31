--- mono/utils/mono-sigcontext.h.orig	2013-01-08 18:41:09.000000000 +0000
+++ mono/utils/mono-sigcontext.h	2013-01-18 18:05:03.000000000 +0000
@@ -323,7 +323,15 @@
 	#define UCONTEXT_REG_R12(ctx) (((ucontext_t*)(ctx))->uc_mcontext->__ss.__r[12])
 	#define UCONTEXT_REG_CPSR(ctx) (((ucontext_t*)(ctx))->uc_mcontext->__ss.__cpsr)
 	#define UCONTEXT_REG_VFPREGS(ctx) (double*)(((ucontext_t*)(ctx))->uc_mcontext->__fs.__r)
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
@@ -337,6 +345,7 @@
 		* we don't use them for now...
 		*/
 	} arm_ucontext;
+#endif
 	#define UCONTEXT_REG_PC(ctx) (((arm_ucontext*)(ctx))->sig_ctx.arm_pc)
 	#define UCONTEXT_REG_SP(ctx) (((arm_ucontext*)(ctx))->sig_ctx.arm_sp)
 	#define UCONTEXT_REG_LR(ctx) (((arm_ucontext*)(ctx))->sig_ctx.arm_lr)
