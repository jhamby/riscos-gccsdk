--- mono/mini/mini.h.orig	2011-10-09 17:40:13.000000000 +0100
+++ mono/mini/mini.h	2011-11-06 16:26:14.000000000 +0000
@@ -88,7 +88,7 @@
 #define LLVM_CHECK_VERSION(major,minor) 0
 #endif
 
-/* 
+/*
  * Whenever we are using mono's LLVM branch.
  * This can be used in if statements, code which references new definitions from the branch
  * still needs an #ifdef LLVM_MONO_BRANCH.
@@ -295,7 +295,7 @@
  */
 typedef struct {
 	StackFrameType type;
-	/* 
+	/*
 	 * For FRAME_TYPE_MANAGED, otherwise NULL.
 	 */
 	MonoJitInfo *ji;
@@ -342,7 +342,7 @@
 	for (b = mono_bitset_find_start (set); b < n && b >= 0; b = mono_bitset_find_first (set, b))
 #define mono_bitset_foreach_bit_rev(set,b,n) \
 	for (b = mono_bitset_find_last (set, n - 1); b >= 0; b = b ? mono_bitset_find_last (set, b) : -1)
- 
+
 #endif
 
 /*
@@ -418,7 +418,7 @@
         NULLIFY_INS ((ins)); \
     } while (0)
 
-/* 
+/*
  * this is used to determine when some branch optimizations are possible: we exclude FP compares
  * because they have weird semantics with NaNs.
  */
@@ -529,7 +529,7 @@
 	int pc_offset;
 	/* The basic block containing the call site */
 	MonoBasicBlock *bb;
-	/* 
+	/*
 	 * The set of variables live at the call site.
 	 * Has length cfg->num_varinfo in bits.
 	 */
@@ -542,7 +542,7 @@
 } GCCallSite;
 
 /*
- * The IR-level extended basic block.  
+ * The IR-level extended basic block.
  *
  * A basic block can have multiple exits just fine, as long as the point of
  * 'departure' is the last instruction in the basic block. Extended basic
@@ -567,7 +567,7 @@
 
 	/* unique block number identification */
 	gint32 block_num;
-	
+
 	gint32 dfn;
 
 	/* Basic blocks: incoming and outgoing counts and pointers */
@@ -604,13 +604,13 @@
 	/* fast dominator algorithm */
 	MonoBasicBlock *df_parent, *ancestor, *child, *label;
 	int size, sdom, idomn;
-	
+
 	/* loop nesting and recognition */
 	GList *loop_blocks;
 	gint8  nesting;
 	gint8  loop_body_start;
 
-	/* 
+	/*
 	 * Whenever the bblock is rarely executed so it should be emitted after
 	 * the function epilog.
 	 */
@@ -633,7 +633,7 @@
 	 * call).
 	 */
 	guint extend_try_block : 1;
-	
+
 	/* use for liveness analysis */
 	MonoBitSet *gen_set;
 	MonoBitSet *kill_set;
@@ -666,7 +666,7 @@
 	 *
 	 *  Bits: |     0-3      |       4-7      |     8-31
 	 * 	  |		 |                |
-	 *        | clause-flags |   MONO_REGION  | clause-index 
+	 *        | clause-flags |   MONO_REGION  | clause-index
 	 *
 	 */
 	guint region;
@@ -683,7 +683,7 @@
 	BB_EXCEPTION_UNSAFE     = 1 << 3,
 	BB_EXCEPTION_HANDLER    = 1 << 4,
 	/* for Native Client, mark the blocks that can be jumped to indirectly */
-	BB_INDIRECT_JUMP_TARGET = 1 << 5 
+	BB_INDIRECT_JUMP_TARGET = 1 << 5
 };
 
 typedef struct MonoMemcpyArgs {
@@ -712,7 +712,7 @@
 	gboolean rgctx_arg;
 	/* Whenever there is an IMT argument */
 	gboolean imt_arg;
-	/* 
+	/*
 	 * The position of the vret arg in the argument list.
 	 * Only if ret->storage == ArgVtypeRetAddr.
 	 * Should be 0 or 1.
@@ -728,7 +728,7 @@
  	guint16 opcode;
 	guint8  type; /* stack type */
 	guint8  flags;
-	
+
 	/* used by the register allocator */
 	gint32 dreg, sreg1, sreg2, sreg3;
 
@@ -777,10 +777,10 @@
 		guint16 source_opcode; /*OP_XCONV_R8_TO_I4 needs to know which op was used to do proper widening*/
 		int pc_offset; /* OP_GC_LIVERANGE_START/END */
 	} backend;
-	
+
 	MonoClass *klass;
 };
-	
+
 struct MonoCallInst {
 	MonoInst inst;
 	MonoMethodSignature *signature;
@@ -795,7 +795,7 @@
 	/* If this is TRUE, 'fptr' points to a MonoJumpInfo instead of an address. */
 	guint fptr_is_patch : 1;
 	/*
-	 * If this is true, then the call returns a vtype in a register using the same 
+	 * If this is true, then the call returns a vtype in a register using the same
 	 * calling convention as OP_CALL.
 	 */
 	guint vret_in_reg : 1;
@@ -821,7 +821,7 @@
 	gint32 offPrm;
 };
 
-/* 
+/*
  * flags for MonoInst
  * Note: some of the values overlap, because they can't appear
  * in the same MonoInst.
@@ -913,7 +913,7 @@
 		guint16 tid; /* tree number */
 		guint16 bid; /* block number */
 	} pos ;
-	guint32 abs_pos; 
+	guint32 abs_pos;
 } MonoPosition;
 
 typedef struct {
@@ -949,8 +949,8 @@
 	char            cpstate;  /* used by SSA conditional  constant propagation */
 	/* The native offsets corresponding to the live range of the variable */
 	gint32         live_range_start, live_range_end;
-	/* 
-	 * cfg->varinfo [idx]->dreg could be replaced for OP_REGVAR, this contains the 
+	/*
+	 * cfg->varinfo [idx]->dreg could be replaced for OP_REGVAR, this contains the
 	 * original vreg.
 	 */
 	gint32         vreg;
@@ -1001,7 +1001,7 @@
 
 	/* context to be used by the guard trampoline when resuming interruption.*/
 	MonoContext handler_block_context;
-	/* 
+	/*
 	 * Stores the state at the exception throw site to be used by mono_stack_walk ()
 	 * when it is called from profiler functions during exception handling.
 	 */
@@ -1081,7 +1081,7 @@
 		MonoJumpInfoImtTramp *imt_tramp;
 	} data;
 };
- 
+
 /* Contains information describing an rgctx entry */
 struct MonoJumpInfoRgctxEntry {
 	MonoMethod *method;
@@ -1193,7 +1193,7 @@
 	MonoMethod      *method_to_register; /* The method to register in JIT info tables */
 	MonoGenericContext *generic_context;
 
-	/* 
+	/*
 	 * This variable represents the hidden argument holding the vtype
 	 * return address. If the method returns something other than a vtype, or
 	 * the vtype is returned in registers this is NULL.
@@ -1204,7 +1204,7 @@
 	 * This is used to initialize the cil_code field of MonoInst's.
 	 */
 	const unsigned char *ip;
-	
+
 	struct MonoAliasingInformation *aliasing_info;
 
 	/* A hashtable of region ID-> SP var mappings */
@@ -1216,7 +1216,7 @@
 	GHashTable      *exvars;
 
 	GList           *ldstr_list; /* used by AOT */
-	
+
 	MonoDomain      *domain;
 
 	guint            real_offset;
@@ -1326,7 +1326,7 @@
 	/* Size of above array */
 	guint32 vreg_is_mp_len;
 
-	/* 
+	/*
 	 * The original method to compile, differs from 'method' when doing generic
 	 * sharing.
 	 */
@@ -1383,7 +1383,7 @@
 	GSList *try_block_holes;
 
 	/* GC Maps */
-   
+
 	/* The offsets of the locals area relative to the frame pointer */
 	gint locals_min_stack_offset, locals_max_stack_offset;
 
@@ -1656,12 +1656,12 @@
 	MINI_TOKEN_SOURCE_FIELD
 };
 
- /* 
+ /*
   * Information about a trampoline function.
   */
  typedef struct
  {
-	/* 
+	/*
 	 * The native code of the trampoline. Not owned by this structure.
 	 */
  	guint8 *code;
@@ -1671,7 +1671,7 @@
 	 * structure.
 	 */
  	char *name;
-	/* 
+	/*
 	 * Patches required by the trampoline when aot-ing. Owned by this structure.
 	 */
 	MonoJumpInfo *ji;
@@ -1905,8 +1905,8 @@
 void              mono_trampolines_cleanup (void) MONO_INTERNAL;
 guint8 *          mono_get_trampoline_code (MonoTrampolineType tramp_type) MONO_INTERNAL;
 gpointer          mono_create_specific_trampoline (gpointer arg1, MonoTrampolineType tramp_type, MonoDomain *domain, guint32 *code_len) MONO_INTERNAL;
-gpointer          mono_create_jump_trampoline (MonoDomain *domain, 
-											   MonoMethod *method, 
+gpointer          mono_create_jump_trampoline (MonoDomain *domain,
+											   MonoMethod *method,
 											   gboolean add_sync_wrapper) MONO_INTERNAL;
 gpointer          mono_create_class_init_trampoline (MonoVTable *vtable) MONO_INTERNAL;
 gpointer          mono_create_generic_class_init_trampoline (void) MONO_INTERNAL;
@@ -1924,9 +1924,9 @@
 gpointer          mono_magic_trampoline (mgreg_t *regs, guint8 *code, gpointer arg, guint8* tramp) MONO_INTERNAL;
 gpointer          mono_generic_virtual_remoting_trampoline (mgreg_t *regs, guint8 *code, MonoMethod *m, guint8 *tramp) MONO_INTERNAL;
 gpointer          mono_delegate_trampoline (mgreg_t *regs, guint8 *code, gpointer *tramp_data, guint8* tramp) MONO_INTERNAL;
-gpointer          mono_aot_trampoline (mgreg_t *regs, guint8 *code, guint8 *token_info, 
+gpointer          mono_aot_trampoline (mgreg_t *regs, guint8 *code, guint8 *token_info,
 									   guint8* tramp) MONO_INTERNAL;
-gpointer          mono_aot_plt_trampoline (mgreg_t *regs, guint8 *code, guint8 *token_info, 
+gpointer          mono_aot_plt_trampoline (mgreg_t *regs, guint8 *code, guint8 *token_info,
 										   guint8* tramp) MONO_INTERNAL;
 void              mono_class_init_trampoline (mgreg_t *regs, guint8 *code, MonoVTable *vtable, guint8 *tramp) MONO_INTERNAL;
 void              mono_generic_class_init_trampoline (mgreg_t *regs, guint8 *code, MonoVTable *vtable, guint8 *tramp) MONO_INTERNAL;
@@ -1956,17 +1956,17 @@
 int               mono_op_imm_to_op (int opcode) MONO_INTERNAL;
 void              mono_decompose_op_imm (MonoCompile *cfg, MonoBasicBlock *bb, MonoInst *ins) MONO_INTERNAL;
 void              mono_peephole_ins (MonoBasicBlock *bb, MonoInst *ins) MONO_INTERNAL;
-MonoUnwindOp     *mono_create_unwind_op (int when, 
-										 int tag, int reg, 
+MonoUnwindOp     *mono_create_unwind_op (int when,
+										 int tag, int reg,
 										 int val) MONO_INTERNAL;
-void              mono_emit_unwind_op (MonoCompile *cfg, int when, 
-									   int tag, int reg, 
+void              mono_emit_unwind_op (MonoCompile *cfg, int when,
+									   int tag, int reg,
 									   int val) MONO_INTERNAL;
 MonoTrampInfo*    mono_tramp_info_create (const char *name, guint8 *code, guint32 code_size, MonoJumpInfo *ji, GSList *unwind_ops) MONO_INTERNAL;
 void              mono_tramp_info_free (MonoTrampInfo *info) MONO_INTERNAL;
 
-int               mono_method_to_ir (MonoCompile *cfg, MonoMethod *method, MonoBasicBlock *start_bblock, MonoBasicBlock *end_bblock, 
-									 MonoInst *return_var, GList *dont_inline, MonoInst **inline_args, 
+int               mono_method_to_ir (MonoCompile *cfg, MonoMethod *method, MonoBasicBlock *start_bblock, MonoBasicBlock *end_bblock,
+									 MonoInst *return_var, GList *dont_inline, MonoInst **inline_args,
 									 guint inline_offset, gboolean is_virtual_call) MONO_INTERNAL;
 
 MonoInst         *mono_decompose_opcode (MonoCompile *cfg, MonoInst *ins) MONO_INTERNAL;
@@ -2056,8 +2056,8 @@
 #endif
 
 gboolean
-mono_arch_find_jit_info (MonoDomain *domain, MonoJitTlsData *jit_tls, 
-						 MonoJitInfo *ji, MonoContext *ctx, 
+mono_arch_find_jit_info (MonoDomain *domain, MonoJitTlsData *jit_tls,
+						 MonoJitInfo *ji, MonoContext *ctx,
 						 MonoContext *new_ctx, MonoLMF **lmf,
 						 mgreg_t **save_locations,
 						 StackFrameInfo *frame_info) MONO_INTERNAL;
@@ -2143,7 +2143,7 @@
 MonoJitInfo * mono_find_jit_info                (MonoDomain *domain, MonoJitTlsData *jit_tls, MonoJitInfo *res, MonoJitInfo *prev_ji, MonoContext *ctx, MonoContext *new_ctx, char **trace, MonoLMF **lmf, int *native_offset, gboolean *managed) MONO_INTERNAL;
 
 gboolean
-mono_find_jit_info_ext (MonoDomain *domain, MonoJitTlsData *jit_tls, 
+mono_find_jit_info_ext (MonoDomain *domain, MonoJitTlsData *jit_tls,
 						MonoJitInfo *prev_ji, MonoContext *ctx,
 						MonoContext *new_ctx, char **trace, MonoLMF **lmf,
 						mgreg_t **save_locations,
@@ -2157,8 +2157,8 @@
 gpointer mono_get_throw_corlib_exception        (void) MONO_INTERNAL;
 
 MonoArray *ves_icall_get_trace                  (MonoException *exc, gint32 skip, MonoBoolean need_file_info) MONO_INTERNAL;
-MonoBoolean ves_icall_get_frame_info            (gint32 skip, MonoBoolean need_file_info, 
-						 MonoReflectionMethod **method, 
+MonoBoolean ves_icall_get_frame_info            (gint32 skip, MonoBoolean need_file_info,
+						 MonoReflectionMethod **method,
 						 gint32 *iloffset, gint32 *native_offset,
 						 MonoString **file, gint32 *line, gint32 *column) MONO_INTERNAL;
 MonoString *ves_icall_System_Exception_get_trace (MonoException *exc) MONO_INTERNAL;
@@ -2190,7 +2190,7 @@
 void      mono_debug_record_line_number         (MonoCompile *cfg, MonoInst *ins, guint32 address) MONO_INTERNAL;
 void      mono_debug_serialize_debug_info       (MonoCompile *cfg, guint8 **out_buf, guint32 *buf_len) MONO_INTERNAL;
 void      mono_debug_add_aot_method             (MonoDomain *domain,
-						 MonoMethod *method, guint8 *code_start, 
+						 MonoMethod *method, guint8 *code_start,
 						 guint8 *debug_info, guint32 debug_info_len) MONO_INTERNAL;
 void      mono_debug_add_icall_wrapper          (MonoMethod *method, MonoJitICallInfo* info) MONO_INTERNAL;
 void      mono_debug_print_vars                 (gpointer ip, gboolean only_arguments);
@@ -2344,10 +2344,10 @@
 			  SIMD_VERSION_SSE41 | SIMD_VERSION_SSE42 |
 			  SIMD_VERSION_SSE4a,
 
-	/* this value marks the end of the bit indexes used in 
+	/* this value marks the end of the bit indexes used in
 	 * this emum.
 	 */
-	SIMD_VERSION_INDEX_END = 6 
+	SIMD_VERSION_INDEX_END = 6
 };
 
 #define MASK(x) (1 << x)
