--- bfd/elf32-arm.c.orig	2018-01-13 13:31:15.000000000 +0000
+++ bfd/elf32-arm.c	2019-01-21 22:38:00.484254229 +0000
@@ -2140,6 +2140,11 @@
 
 #define CMSE_PREFIX "__acle_se_"
 
+#ifdef __RISCOS_TARGET__
+#define RISCOS_ABI_SECTION_NAME ".riscos.abi.version"
+#define RISCOS_PIC_SECTION_NAME ".riscos.pic"
+#endif
+
 /* The name of the dynamic interpreter.  This is put in the .interp
    section.  */
 #define ELF_DYNAMIC_INTERPRETER     "/usr/lib/ld.so.1"
@@ -2340,6 +2345,88 @@
   0xea000000,		/* b	.Lplt_tail			*/
 };
 
+#ifdef __RISCOS_TARGET__
+#define RISCOS_OPTIMISE_PLT
+
+#ifdef RISCOS_OPTIMISE_PLT
+#define PLT_RISCOS_EXEC_HEADER_SIZE 20
+#define PLT_RISCOS_SHARED_HEADER_SIZE 20
+#define PLT_RISCOS_EXEC_ENTRY_SIZE 12
+#define PLT_RISCOS_SHARED_ENTRY_SIZE 20
+#else
+#define PLT_RISCOS_EXEC_HEADER_SIZE 20
+#define PLT_RISCOS_SHARED_HEADER_SIZE 24
+#define PLT_RISCOS_EXEC_ENTRY_SIZE 12
+#define PLT_RISCOS_SHARED_ENTRY_SIZE 24
+#endif
+
+#define SRELPIC_VERSION 1
+#define SRELPIC_HEADER_SIZE 8 /* in bytes */
+  
+/* The first entry in a procedure linkage table looks like
+   this.  It is set up so that any shared library function that is
+   called before the relocation has been set up calls the dynamic
+   linker first.  */
+static const bfd_vma elf32_arm_riscos_exec_plt0_entry [] =
+  {
+    0xe52de004,		/* str   lr, [sp, #-4]! */
+    0xe59fe004,		/* ldr   lr, [pc, #4]   */
+    0xe08fe00e,		/* add   lr, pc, lr     */
+    0xe5bef008,		/* ldr   pc, [lr, #8]! */
+    0x00000000		/* &GOT[0] - .          */
+  };
+
+#ifdef RISCOS_OPTIMISE_PLT
+static const bfd_vma elf32_arm_riscos_shared_plt0_entry [] =
+  {
+    0xe52de004,		/* str   lr, [sp, #-4]!            */
+    0xe3a0e902,		/* mov   lr, #0x8000               */
+    0xe59ee038,		/* ldr   lr, [lr, #0x38]           */
+    0xe59ee000,		/* ldr   lr, [lr, #__GOTT_INDEX__] */
+    0xe5bef008,		/* ldr   pc, [lr, #8]! */
+  };
+
+static const bfd_vma elf32_arm_riscos_shared_plt_entry [] =
+  {
+    0xe3a0c902,		/* mov  ip, #0x8000               */
+    0xe59cc038,		/* ldr  ip, [ip, #0x38]           */
+    0xe59cc000,		/* ldr  ip, [ip, #__GOTT_INDEX__] */
+    0xe28cca00,		/* add	ip, ip, #0x00XX000 */
+    0xe5bcf000,		/* ldr  pc,[ip, #0x0000XXX]!  @ call function */
+  };
+#else
+static const bfd_vma elf32_arm_riscos_shared_plt0_entry [] =
+  {
+    0xe52de004,		/* str   lr, [sp, #-4]! */
+    0xe59fe008,		/* ldr   lr, [pc, #8]   */
+    0xe59ee000,		/* ldr   lr, [lr, #0]   */
+    0xe59ee000,		/* ldr   lr, [lr, #__GOTT_INDEX__]   */
+    0xe5bef008,		/* ldr   pc, [lr, #8]! */
+    0x00000000		/* __GOTT_BASE__        */
+  };
+
+static const bfd_vma elf32_arm_riscos_shared_plt_entry [] =
+  {
+    0xe59fc00C,		/* ldr  ip, [pc, #12]      */
+    0xe59cc000,		/* ldr  ip, [ip, #0]       */
+    0xe59cc000,		/* ldr  ip, [ip, #__GOTT_INDEX__] */
+    0xe28cca00,		/* add	ip, ip, #0x00XX000 */
+    0xe5bcf000,		/* ldr  pc,[ip, #0x0000XXX]!  @ call function */
+    0x00000000		/* __GOTT_BASE__           */
+  };
+#endif
+
+/* Subsequent entries in a procedure linkage table look like
+   this.  */
+static const bfd_vma elf32_arm_riscos_exec_plt_entry [] =
+  {
+    0xe28fc600,		/* add   ip, pc, #0xXX00000 */
+    0xe28cca00,		/* add   ip, ip, #0x00XX000 */
+    0xe5bcf000		/* ldr   pc,[ip, #0x0000XXX]! */
+  };
+
+#endif
+
 #define ARM_MAX_FWD_BRANCH_OFFSET  ((((1 << 23) - 1) << 2) + 8)
 #define ARM_MAX_BWD_BRANCH_OFFSET  ((-((1 << 23) << 2)) + 8)
 #define THM_MAX_FWD_BRANCH_OFFSET  ((1 << 22) -2 + 4)
@@ -3220,6 +3307,27 @@
   unsigned int bfd_count;
   unsigned int top_index;
   asection **input_list;
+#ifdef __RISCOS_TARGET__
+  /* For RISC OS module __RelocCode & __RelocData support.  */
+  bfd * bfd_ro_module_owner;
+
+  asection * s_ro_module_reloccode;
+  bfd_vma ro_module_reloccode_offset;
+  bfd_vma ro_module_reloccode_size;
+
+  asection * s_ro_module_relocdata;
+  bfd_vma ro_module_relocdata_offset;
+  bfd_vma ro_module_relocdata_size;
+
+  bfd_vma ro_module_image_ro_base;
+  bfd_vma ro_module_image_rw_base;
+
+  /* For RISC OS PIC support.  */
+  asection * srelpic;
+  bfd_vma srelpic_offset;
+  int srelpic_size;
+  int plt_count;
+#endif
 };
 
 static inline int
@@ -3620,6 +3728,70 @@
 	  || arch >= TAG_CPU_ARCH_V7);
 }
 
+#ifdef __RISCOS_TARGET__
+bfd_boolean
+bfd_elf32_arm_create_riscos_pic_section (struct bfd_link_info *info)
+{
+  BFD_ASSERT (bfd_link_pic(info) && info->flag_pic == 2);
+
+  struct elf32_arm_link_hash_table *htab = elf32_arm_hash_table (info);
+  bfd *	dynobj = elf_hash_table (info)->dynobj;
+  const struct elf_backend_data *bed = get_elf_backend_data (dynobj);
+
+  asection *s = bfd_make_section_with_flags (dynobj,
+					     RISCOS_PIC_SECTION_NAME,
+					     SEC_ALLOC | SEC_LOAD | SEC_DATA
+					     | SEC_HAS_CONTENTS | SEC_IN_MEMORY
+					     | SEC_READONLY | SEC_LINKER_CREATED);
+  if (s == NULL
+      || !bfd_set_section_alignment (dynobj, s, bed->s->log_file_align))
+    return FALSE;
+  htab->srelpic = s;
+  htab->srelpic_size = SRELPIC_HEADER_SIZE;
+
+  return TRUE;
+}
+
+bfd_boolean
+bfd_elf32_arm_allocate_riscos_pic_section (struct bfd_link_info *info)
+{
+  BFD_ASSERT (bfd_link_pic(info) && info->flag_pic == 2);
+
+  struct elf32_arm_link_hash_table * htab;
+  bfd *	dynobj = elf_hash_table (info)->dynobj;
+
+  htab = elf32_arm_hash_table (info);
+
+  asection *s = htab->srelpic;
+
+#ifdef RISCOS_OPTIMISE_PLT
+  if (htab->plt_count)
+    htab->srelpic_size += (htab->plt_count + 1) * 4;
+#else
+  /* Add PLT relocs, if any. We add one to account for the first entry used
+     when lazy binding. We multiply by 8 because there are two 4 byte
+     relocations per PLT entry.  */
+  if (htab->plt_count)
+    htab->srelpic_size += (htab->plt_count + 1) * 8;
+#endif
+
+  s->contents = (bfd_byte *) bfd_zalloc (dynobj, htab->srelpic_size);
+  if (s->contents == NULL)
+    return FALSE;
+  s->size = htab->srelpic_size;
+
+  /* Write header into section:
+     +0 version number
+     +4 Number of relocs.  */
+  bfd_put_32 (dynobj, SRELPIC_VERSION, s->contents + 0);
+  bfd_put_32 (dynobj, (s->size - SRELPIC_HEADER_SIZE) >> 2, s->contents + 4);
+
+  htab->srelpic_offset = SRELPIC_HEADER_SIZE;
+  
+  return TRUE;
+}
+#endif
+
 /* Create .plt, .rel(a).plt, .got, .got.plt, .rel(a).got, .dynbss, and
    .rel(a).bss sections in DYNOBJ, and set up shortcuts to them in our
    hash table.  */
@@ -3639,6 +3811,23 @@
   if (!_bfd_elf_create_dynamic_sections (dynobj, info))
     return FALSE;
 
+#ifdef __RISCOS_TARGET__
+  if (info->riscos_abi)
+  {
+    const struct elf_backend_data *bed;
+    asection *sec;
+    flagword flags;
+
+    bed = get_elf_backend_data (dynobj);
+    flags = bed->dynamic_sec_flags;
+    sec = bfd_make_section_anyway_with_flags (dynobj,
+					      RISCOS_ABI_SECTION_NAME,
+					      flags | SEC_READONLY);
+    if (sec == NULL)
+      return FALSE;
+  }
+#endif
+
   if (htab->vxworks_p)
     {
       if (!elf_vxworks_create_dynamic_sections (dynobj, info, &htab->srelplt2))
@@ -3678,6 +3867,22 @@
       htab->obfd = saved_obfd;
     }
 
+#ifdef __RISCOS_TARGET__
+  if (info->flag_pic == 2)
+    {
+      if (bfd_link_pic(info))
+        {
+	  htab->plt_header_size = PLT_RISCOS_SHARED_HEADER_SIZE;
+	  htab->plt_entry_size = PLT_RISCOS_SHARED_ENTRY_SIZE;
+	}
+      else
+	{
+	  htab->plt_header_size = PLT_RISCOS_EXEC_HEADER_SIZE;
+	  htab->plt_entry_size = PLT_RISCOS_EXEC_ENTRY_SIZE;
+	}
+    }
+#endif
+
   if (!htab->root.splt
       || !htab->root.srelplt
       || !htab->root.sdynbss
@@ -3807,9 +4012,212 @@
     }
   ret->root.root.hash_table_free = elf32_arm_link_hash_table_free;
 
+#ifdef __RISCOS_TARGET__
+  ret->bfd_ro_module_owner = NULL;
+  ret->s_ro_module_reloccode = NULL;
+  ret->ro_module_reloccode_offset = 0;
+  ret->ro_module_reloccode_size = 0;
+  ret->s_ro_module_relocdata = NULL;
+  ret->ro_module_relocdata_offset = 0;
+  ret->ro_module_relocdata_size = 0;
+  ret->ro_module_image_ro_base = 0;
+  ret->ro_module_image_rw_base = 0;
+
+  ret->srelpic = NULL;
+  ret->srelpic_offset = 0;
+  ret->plt_count = 0;
+#endif
+
   return &ret->root.root;
 }
 
+#ifdef __RISCOS_TARGET__
+/* Relocates 32-bit words because the ARM code/data is not loaded in memory
+   at Image$$RO$$Base (used by the linker) but at module RMA space.  */
+static const bfd_vma riscos_module_reloccode[] =
+{
+		/* __RelocCode:                   */
+  0xE59F1034,	/*	LDR	R1, RelocCode     */
+  0xE24FB00C,	/*	ADR	R11, __RelocCode  */
+  0xE05B1001,	/*	SUBS	R1, R11, R1       */
+  0x01A0F00E,	/*	MOVEQ	PC, R14           */
+  0xE59FB028,	/*	LDR	R11, ImageROBase  */
+  0xE08BB001,	/*	ADD	R11, R11,R1       */
+  0xE28F2024,	/*	ADR	R2, RelocCodeList */
+		/* RelocCodeLoop:                 */
+  0xE4920004,	/*	LDR	R0, [R2], #4      */
+  0xE3700001,	/*	CMP	R0, #-1           */
+  0x01A0F00E,	/*	MOVEQ	PC, R14           */
+  0xE79B3000,	/*	LDR	R3, [R11, R0]     */
+  0xE3330000,	/*	TEQ	R3, #0            */
+  0x10833001,	/*	ADDNE	R3, R3, R1        */
+  0x178B3000,	/*	STRNE	R3, [R11, R0]     */
+  0xEAFFFFF7,	/*	B	RelocCodeLoop     */
+#define RISCOS_MODULE_RELOCCODE_OFFSET_RELOCCODE (15*4)
+  0x00000000,	/* RelocCode:                     */
+		/*	.word	__RelocCode       */
+#define RISCOS_MODULE_RELOCCODE_OFFSET_IMAGEROBASE (16*4)
+  0x00000000	/* ImageROBase:                   */
+		/*	.word	Image$$RO$$Base   */
+		/* RelocCodeList:                 */
+		/* ... addresses to words which needs to be relocated
+		   terminated by a -1 value. It should also include the
+		   2 RISCOS_MODULE_RELOCCODE_OFFSET_* offsets.  */
+};
+
+/* Relocates 32-bit words because the RW data can contain RW data pointers
+   which should point to module RMA workspace instead of to module
+   code/data.  */
+static const bfd_vma riscos_module_relocdata[] =
+{
+		/*	Entry: R0 = client static relocation offset  */
+		/*	       R2 = Image$$RW$$Base                  */
+		/*	       R13 = SVC stack                       */
+		/*	Exit: All registers reserved                 */
+		/* __RelocData:                                      */
+  0xE92D400E,	/*	STMFD	R13!, {R1-R3, R14}                   */
+  0xE0822000,	/*	ADD	R2, R2, R0                           */
+  0xE28FE018,	/*	ADR	R14, RelocDataList                   */
+		/* RelocDataLoop:                                    */
+  0xE49E1004,	/*	LDR	R1, [R14], #4                        */
+  0xE3710001,	/*	CMP	R1, #-1                              */
+  0x17923001,	/*	LDRNE	R3, [R2, R1]                         */
+  0x10833000,	/*	ADDNE	R3, R3, R0                           */
+  0x17823001,	/*	STRNE	R3, [R2, R1]                         */
+  0x1AFFFFF9,	/*	BNE	RelocDataLoop                        */
+  0xE8BD800E	/*	LDMFD	R13!, {R1-R3, PC}                    */
+		/* Offsets in the RW data block which need relocated */
+		/* over R12[8] terminated with -1 value.             */
+		/* RelocDataList:                                    */
+};
+
+bfd_boolean
+bfd_elf32_arm_create_riscos_module_reloc (bfd *abfd, struct bfd_link_info *link_info)
+{
+  asection *s;
+  struct bfd_link_hash_entry *bh;
+  struct elf32_arm_link_hash_table * globals;
+
+  globals = elf32_arm_hash_table (link_info);
+  if (globals->bfd_ro_module_owner != NULL
+      || globals->s_ro_module_reloccode != NULL
+      || globals->s_ro_module_relocdata != NULL)
+    return FALSE; /* Multiple times called bfd_elf32_arm_create_riscos_module_reloc ? */
+
+  globals->bfd_ro_module_owner = abfd;
+
+  /* __RelocCode */
+  s = bfd_get_section_by_name (abfd, ".riscos.module.reloccode");
+  if (s != NULL)
+    return FALSE; /* Section already exists ?  */
+
+  s = bfd_make_section_with_flags (abfd,
+				   ".riscos.module.reloccode",
+				   (SEC_ALLOC
+				    | SEC_LOAD
+				    /* | SEC_RELOC */
+			            | SEC_CODE
+				    | SEC_HAS_CONTENTS
+				    | SEC_LINKER_CREATED
+				    | SEC_READONLY
+				    | SEC_IN_MEMORY));
+  if (s == NULL
+      || !bfd_set_section_alignment (abfd, s, 2))
+    return FALSE;
+
+  globals->s_ro_module_reloccode = s;
+
+  bh = NULL;
+  if (!_bfd_generic_link_add_one_symbol (link_info, abfd, "__RelocCode",
+      BSF_GLOBAL | BSF_FUNCTION, s, 0, (const char *) NULL, FALSE, FALSE, &bh))
+    return FALSE;
+
+  /* Set size which is the size of __RelocCode code + its data (including
+     RISCOS_MODULE_RELOCCODE_OFFSET_* and -1 sentinel value.  */
+  globals->ro_module_reloccode_size += 4*ARRAY_SIZE (riscos_module_reloccode) + 3*4;
+  s->size = globals->ro_module_reloccode_size;
+
+  /* __RelocData */
+  s = bfd_get_section_by_name (abfd, ".riscos.module.relocdata");
+  if (s != NULL)
+    return FALSE; /* Section already exists ?  */
+
+  s = bfd_make_section_with_flags (abfd,
+				   ".riscos.module.relocdata",
+				   (SEC_ALLOC
+				    | SEC_LOAD
+				    /* | SEC_RELOC */
+			            | SEC_CODE
+				    | SEC_HAS_CONTENTS
+				    | SEC_LINKER_CREATED
+				    | SEC_READONLY
+				    | SEC_IN_MEMORY));
+  if (s == NULL
+      || !bfd_set_section_alignment (abfd, s, 2))
+    return FALSE;
+
+  globals->s_ro_module_relocdata = s;
+
+  bh = NULL;
+  if (!_bfd_generic_link_add_one_symbol (link_info, abfd, "__RelocData",
+      BSF_GLOBAL | BSF_FUNCTION, s, 0, (const char *) NULL, FALSE, FALSE, &bh))
+    return FALSE;
+
+  /* Set size which is the size of __RelocData code + its data and -1 sentinel
+     value.  */
+  globals->ro_module_relocdata_size += 4*ARRAY_SIZE (riscos_module_relocdata) + 1*4;
+  s->size = globals->ro_module_relocdata_size;
+
+  return TRUE;
+}
+
+bfd_boolean
+bfd_elf32_arm_allocate_riscos_module_reloc (struct bfd_link_info *link_info)
+{
+  asection *s;
+  size_t i;
+  struct elf32_arm_link_hash_table * globals;
+
+  globals = elf32_arm_hash_table (link_info);
+  if (globals->bfd_ro_module_owner == NULL
+      || globals->s_ro_module_reloccode == NULL
+      || globals->s_ro_module_relocdata == NULL)
+    return FALSE;
+
+  /* __RelocCode */
+  s = globals->s_ro_module_reloccode;
+
+  if (s->output_section == NULL)
+    return TRUE; /* Apparently not in the output bfd, so bail out.  */
+
+  s->contents = (bfd_byte *) bfd_zalloc (globals->bfd_ro_module_owner, s->size);
+  if (s->contents == NULL)
+    return FALSE;
+
+  for (i = 0; i < ARRAY_SIZE (riscos_module_reloccode); ++i)
+    bfd_put_32 (globals->bfd_ro_module_owner, riscos_module_reloccode[i], s->contents + i*4);
+
+  globals->ro_module_reloccode_offset = 4*ARRAY_SIZE (riscos_module_reloccode);
+
+  /* __RelocData */
+  s = globals->s_ro_module_relocdata;
+
+  if (s->output_section == NULL)
+    return TRUE; /* Apparently not in the output bfd, so bail out.  */
+
+  s->contents = (bfd_byte *) bfd_zalloc (globals->bfd_ro_module_owner, s->size);
+  if (s->contents == NULL)
+    return FALSE;
+
+  for (i = 0; i < ARRAY_SIZE (riscos_module_relocdata); ++i)
+    bfd_put_32 (globals->bfd_ro_module_owner, riscos_module_relocdata[i], s->contents + i*4);
+
+  globals->ro_module_relocdata_offset = 4*ARRAY_SIZE (riscos_module_relocdata);
+
+  return TRUE;
+}
+#endif
+
 /* Determine what kind of NOPs are available.  */
 
 static bfd_boolean
@@ -9504,6 +9912,60 @@
 			      elf32_arm_plt_thumb_stub[1], ptr - 2);
 	    }
 
+#ifdef __RISCOS_TARGET__
+	  if (info->flag_pic == 2)
+	    {
+	      if (bfd_link_pic(info))
+		{
+		  /* Insert __GOTT_INDEX__ reloc for this plt entry.  */
+		  bfd_put_32 (output_bfd,
+			      ((plt_address + 8) & 0x3fffffff) | (1 << 31),
+			      htab->srelpic->contents + htab->srelpic_offset);
+		  htab->srelpic_offset += 4;
+#ifndef RISCOS_OPTIMISE_PLT
+		  /* Insert __GOTT_BASE__ reloc for this plt entry.  */
+		  bfd_put_32 (output_bfd,
+			      plt_address + 20,
+			      htab->srelpic->contents + htab->srelpic_offset);
+		  htab->srelpic_offset += 4;
+#endif
+
+		  put_arm_insn (htab, output_bfd,
+				elf32_arm_riscos_shared_plt_entry[0],
+				ptr + 0);
+		  put_arm_insn (htab, output_bfd,
+				elf32_arm_riscos_shared_plt_entry[1],
+				ptr + 4);
+		  put_arm_insn (htab, output_bfd,
+				elf32_arm_riscos_shared_plt_entry[2],
+				ptr + 8);
+		  put_arm_insn (htab, output_bfd,
+				elf32_arm_riscos_shared_plt_entry[3]
+				| ((got_offset & 0x000ff000) >> 12),
+				ptr + 12);
+		  put_arm_insn (htab, output_bfd,
+				elf32_arm_riscos_shared_plt_entry[4]
+				| (got_offset & 0x00000fff),
+				ptr + 16);
+		}
+	      else
+		{
+		  put_arm_insn (htab, output_bfd,
+				elf32_arm_riscos_exec_plt_entry[0]
+				| ((got_displacement & 0x0ff00000) >> 20),
+				ptr + 0);
+		  put_arm_insn (htab, output_bfd,
+				elf32_arm_riscos_exec_plt_entry[1]
+				| ((got_displacement & 0x000ff000) >> 12),
+				ptr + 4);
+		  put_arm_insn (htab, output_bfd,
+				elf32_arm_riscos_exec_plt_entry[2]
+				| (got_displacement & 0x00000fff),
+				ptr + 8);
+		}
+	    }
+	  else
+#endif
 	  if (!elf32_arm_use_long_plt_entry)
 	    {
 	      BFD_ASSERT ((got_displacement & 0xf0000000) == 0);
@@ -9948,6 +10410,69 @@
   else
     addend = signed_addend = rel->r_addend;
 
+#ifdef __RISCOS_TARGET__
+  if (bfd_link_pic(info) && info->flag_pic == 2
+      && sym_name[0] == '_' && sym_name[1] == '_'
+      && (strcmp (sym_name, "__GOTT_INDEX__") == 0
+	  ||  strcmp (sym_name, "__GOTT_BASE__") == 0))
+    {
+      const bfd_vma osection_addr = input_section->output_section->vma + input_section->output_offset + rel->r_offset;
+
+      BFD_ASSERT (globals->srelpic_offset < globals->srelpic_size);
+
+      /* Set bit 31 of the offset for a __GOTT_INDEX__ reloc.  */
+      bfd_put_32 (input_bfd,
+		  (osection_addr & 0x3fffffff) | ((sym_name[7] == 'I') << 31),
+		  globals->srelpic->contents + globals->srelpic_offset);
+      globals->srelpic_offset += 4;
+    }
+
+  if (globals->bfd_ro_module_owner != NULL)
+    {
+      /* __RelocData entry:
+         Any ABS32 reloc in a rw data section pointing to RW data (ie module workspace), need to have a __RelocData entry.  */
+#define ro_module_is_datareloc(type, secname) \
+  ((type) == R_ARM_ABS32 \
+   && (!strncmp (secname, ".data", sizeof (".data")-1)))
+      if (ro_module_is_datareloc (r_type, input_section->name)
+	  && eh != NULL
+	  && (unsigned int)value + (unsigned int)addend >= (unsigned int)globals->ro_module_image_rw_base /* comparison needs to happen in 32-bit space.  */)
+	{
+	  /* Relocation inside RW data, i.e. in module RMA workspace.  */
+	  const bfd_vma osection_addr = input_section->output_section->vma + input_section->output_offset - globals->ro_module_image_rw_base;
+
+	  /* Just to make sure our dodgy allocation approach is not underallocating.  */
+	  BFD_ASSERT (globals->ro_module_relocdata_offset < globals->ro_module_relocdata_size);
+
+	  bfd_put_32 (input_bfd, osection_addr + rel->r_offset, globals->s_ro_module_relocdata->contents + globals->ro_module_relocdata_offset);
+	  globals->ro_module_relocdata_offset += 4;
+	}
+
+      /* __RelocCode entry:
+         An ABS32 reloc in any .data* and .rodata* section needs to result in an __RelocCode entry.
+         Any GOT32/GOTOFF32 reloc needs to be mentioned in __RelocCode entry.  */
+#define ro_module_is_codereloc(type, secname) \
+  (((type) == R_ARM_ABS32 \
+    && (!strncmp (secname, ".data", sizeof (".data")-1) \
+	|| !strncmp (secname, ".rodata", sizeof (".rodata")-1) \
+	|| !strcmp (secname, ".ctors") \
+	|| !strcmp (secname, ".dtors"))) \
+   || (type) == R_ARM_GOT32 \
+   || (type) == R_ARM_GOTOFF32)
+      if (ro_module_is_codereloc (r_type, input_section->name))
+	{
+	  /* Relocation because of placing Module in RMA instead of application space.  */
+	  const bfd_vma osection_addr = input_section->output_section->vma + input_section->output_offset - globals->ro_module_image_ro_base;
+
+	  /* Just to make sure our dodgy allocation approach is not underallocating.  */
+	  BFD_ASSERT (globals->ro_module_reloccode_offset < globals->ro_module_reloccode_size);
+
+          bfd_put_32 (input_bfd, osection_addr + rel->r_offset, globals->s_ro_module_reloccode->contents + globals->ro_module_reloccode_offset);
+          globals->ro_module_reloccode_offset += 4;
+	}
+    }
+#endif
+
   /* ST_BRANCH_TO_ARM is nonsense to thumb-only targets when we
      are resolving a function call relocation.  */
   if (using_thumb_only (globals)
@@ -11024,6 +11549,9 @@
 	 define _GLOBAL_OFFSET_TABLE in a different way, as is
 	 permitted by the ABI, we might have to change this
 	 calculation.  */
+#ifdef __RISCOS_TARGET__
+      if (globals->s_ro_module_reloccode == NULL)
+#endif
       value -= sgot->output_section->vma;
       return _bfd_final_link_relocate (howto, input_bfd, input_section,
 				       contents, rel->r_offset, value,
@@ -12876,10 +13404,93 @@
 {
   struct elf32_arm_link_hash_table *globals = elf32_arm_hash_table (info);
   asection *sec, *osec;
+#ifdef __RISCOS_TARGET__
+  asection *o;
+  struct bfd_link_order *p;
+  asection *ro_module_reloccode_section = NULL;
+  asection *ro_module_relocdata_section = NULL;
+  asection *srelpic = NULL;
+#endif
 
   if (globals == NULL)
     return FALSE;
 
+#ifdef __RISCOS_TARGET__
+  for (o = abfd->sections; o != NULL; o = o->next)
+    {
+      if (strcmp (o->name, ".riscos.module.reloccode") == 0)
+        {
+	  for (p = o->map_head.link_order; p != NULL; p = p->next)
+	    {
+	      asection *input_section;
+
+	      if (p->type != bfd_indirect_link_order)
+		continue;
+	      input_section = p->u.indirect.section;
+	      /* Hack: reset the SEC_HAS_CONTENTS flag so that
+		 elf_link_input_bfd ignores this section.  */
+	      input_section->flags &= ~SEC_HAS_CONTENTS;
+	    }
+
+          ro_module_reloccode_section = o;
+	  /* Skip this section later on.  */
+	  o->map_head.link_order = NULL;
+        }
+      else if (strcmp (o->name, ".riscos.module.relocdata") == 0)
+        {
+	  for (p = o->map_head.link_order; p != NULL; p = p->next)
+	    {
+	      asection *input_section;
+
+	      if (p->type != bfd_indirect_link_order)
+		continue;
+	      input_section = p->u.indirect.section;
+	      /* Hack: reset the SEC_HAS_CONTENTS flag so that
+		 elf_link_input_bfd ignores this section.  */
+	      input_section->flags &= ~SEC_HAS_CONTENTS;
+	    }
+
+          ro_module_relocdata_section = o;
+	  /* Skip this section later on.  */
+	  o->map_head.link_order = NULL;
+        }
+      else if (strcmp (o->name, RISCOS_PIC_SECTION_NAME) == 0)
+	srelpic = o;
+    }
+  if (globals->s_ro_module_reloccode != NULL
+      || globals->s_ro_module_relocdata != NULL)
+    {
+      struct elf_link_hash_entry *hash;
+      struct elf32_arm_link_hash_table *hash_table;
+
+      /* We need a pointer to the armelf specific hash table.  */
+      hash_table = elf32_arm_hash_table (info);
+
+      hash = elf_link_hash_lookup
+        (&(hash_table)->root, "Image$$RO$$Base", FALSE, FALSE, TRUE);
+
+      if (hash == NULL)
+        (*_bfd_error_handler) (_("Unable to find Image$$RO$$Base"));
+      else
+        globals->ro_module_image_ro_base = hash->root.u.def.value
+	  + hash->root.u.def.section->output_section->vma
+	  + hash->root.u.def.section->output_offset;
+
+      hash = elf_link_hash_lookup
+        (&(hash_table)->root, "Image$$RW$$Base", FALSE, FALSE, TRUE);
+
+      if (hash == NULL)
+        (*_bfd_error_handler) (_("Unable to find Image$$RW$$Base"));
+      else
+        globals->ro_module_image_rw_base = hash->root.u.def.value
+	  + hash->root.u.def.section->output_section->vma
+	  + hash->root.u.def.section->output_offset;
+
+      BFD_ASSERT (ro_module_reloccode_section != NULL);
+      BFD_ASSERT (ro_module_relocdata_section != NULL);
+    }
+#endif
+
   /* Invoke the regular ELF backend linker to do all the work.  */
   if (!bfd_elf_final_link (abfd, info))
     return FALSE;
@@ -12930,6 +13541,64 @@
 					   ARM_BX_GLUE_SECTION_NAME))
 	return FALSE;
     }
+#ifdef __RISCOS_TARGET__
+  if (ro_module_reloccode_section != NULL)
+    {
+      asection *ro_section = globals->s_ro_module_reloccode;
+      bfd_vma addr_reloccode = bfd_get_section_vma (globals->bfd_ro_module_owner, ro_section->output_section) + ro_section->output_offset;
+
+      /* Write start of __RelocCode address which will be used by __RelocCode itself.  */
+      bfd_put_32 (globals->bfd_ro_module_owner,
+                  addr_reloccode,
+                  ro_section->contents + RISCOS_MODULE_RELOCCODE_OFFSET_RELOCCODE);
+
+      /* Write imagerobase Image$$RO$$Base.  */
+      bfd_put_32 (globals->bfd_ro_module_owner,
+                  globals->ro_module_image_ro_base,
+                  ro_section->contents + RISCOS_MODULE_RELOCCODE_OFFSET_IMAGEROBASE);
+
+      BFD_ASSERT (globals->ro_module_reloccode_offset == globals->ro_module_reloccode_size - 3*4);
+
+      /* Write relocs for RISCOS_MODULE_RELOCCODE_OFFSET_*.  */
+      bfd_put_32 (globals->bfd_ro_module_owner,
+                  addr_reloccode + RISCOS_MODULE_RELOCCODE_OFFSET_RELOCCODE - globals->ro_module_image_ro_base,
+                  ro_section->contents + globals->ro_module_reloccode_offset + 0*4);
+      bfd_put_32 (globals->bfd_ro_module_owner,
+                  addr_reloccode + RISCOS_MODULE_RELOCCODE_OFFSET_IMAGEROBASE - globals->ro_module_image_ro_base,
+                  ro_section->contents + globals->ro_module_reloccode_offset + 1*4);
+
+      /* Write sentinel.  */
+      bfd_put_32 (globals->bfd_ro_module_owner,
+                  -1,
+                  ro_section->contents + globals->ro_module_reloccode_offset + 2*4);
+
+      /* Write __RelocCode section. */
+      if (!bfd_set_section_contents (abfd, ro_module_reloccode_section, ro_section->contents, 0, ro_section->size))
+        return FALSE;
+    }
+
+  if (ro_module_relocdata_section != NULL)
+    {
+      asection *ro_section = globals->s_ro_module_relocdata;
+
+      BFD_ASSERT (globals->ro_module_relocdata_offset <= globals->ro_module_relocdata_size - 4);
+
+      /* Write sentinel.  */
+      bfd_put_32 (globals->bfd_ro_module_owner,
+                  -1,
+                  ro_section->contents + globals->ro_module_relocdata_offset + 0*4);
+
+      /* Write __RelocData section.  */
+      if (!bfd_set_section_contents (abfd, ro_module_relocdata_section, ro_section->contents, 0, ro_section->size))
+        return FALSE;
+    }
+
+  if (srelpic != NULL)
+    {
+      if (!bfd_set_section_contents (abfd, srelpic, globals->srelpic->contents, 0, globals->srelpic_size))
+	return FALSE;
+    }
+#endif
 
   return TRUE;
 }
@@ -14392,6 +15061,23 @@
 	}
 
       eh = (struct elf32_arm_link_hash_entry *) h;
+#ifdef __RISCOS_TARGET__
+      /* For RISC OS module __RelocCode / __RelocData support.  */
+      if (ro_module_is_datareloc (r_type, sec->name)
+	  && eh != NULL)
+        {
+	  /* This can allocate too much but we can only be sure of the
+	     exact size after linking, e.g. after we know that the relocs
+	     are in RO RMA space or in RW RMA workspace.  */
+	  htab->ro_module_relocdata_size += 4;
+        }
+      if (ro_module_is_codereloc (r_type, sec->name))
+        htab->ro_module_reloccode_size += 4;
+
+      if (h && (strcmp (h->root.root.string, "__GOTT_INDEX__") == 0
+		|| strcmp (h->root.root.string, "__GOTT_BASE__") == 0))
+	htab->srelpic_size += 4;
+#endif
 
       call_reloc_p = FALSE;
       may_become_dynamic_p = FALSE;
@@ -15300,6 +15986,10 @@
 	{
 	  elf32_arm_allocate_plt_entry (info, eh->is_iplt, &h->plt, &eh->plt);
 
+#ifdef __RISCOS_TARGET__
+	  htab->plt_count++;
+#endif
+
 	  /* If this symbol is not defined in a regular file, and we are
 	     not generating a shared library, then set the symbol to this
 	     location in the .plt.  This is required to make function
@@ -15688,6 +16378,15 @@
 	}
     }
 
+#ifdef __RISCOS_TARGET__
+  if (info->riscos_abi)
+    {
+      s = bfd_get_linker_section (dynobj, RISCOS_ABI_SECTION_NAME);
+      s->size = strlen (info->riscos_abi) + 1;
+      s->contents = info->riscos_abi;
+    }
+#endif
+
   /* Set up .got offsets for local syms, and space for local dynamic
      relocs.  */
   for (ibfd = info->input_bfds; ibfd != NULL; ibfd = ibfd->link.next)
@@ -15992,7 +16691,23 @@
 #define add_dynamic_entry(TAG, VAL) \
   _bfd_elf_add_dynamic_entry (info, TAG, VAL)
 
-     if (bfd_link_executable (info))
+#ifdef __RISCOS_TARGET__
+      if (bfd_link_pic(info) && info->flag_pic == 2)
+	{
+	  if (!add_dynamic_entry (DT_RISCOS_PIC, 0))
+	    return FALSE;
+	}
+
+      if (info->riscos_abi)
+	if (!add_dynamic_entry (DT_RISCOS_ABI_VERSION, 0))
+	  return FALSE;
+
+      if (!add_dynamic_entry (DT_PLTGOT, 0))
+        return FALSE;
+
+#endif
+
+      if (bfd_link_executable (info))
 	{
 	  if (!add_dynamic_entry (DT_DEBUG, 0))
 	    return FALSE;
@@ -16305,6 +17020,14 @@
 	      name = ".gnu.version_r";
 	      goto get_vma_if_bpabi;
 
+#ifdef __RISCOS_TARGET__
+	    case DT_RISCOS_PIC:
+	      name = RISCOS_PIC_SECTION_NAME;
+	      goto get_vma;
+	    case DT_RISCOS_ABI_VERSION:
+	      name = RISCOS_ABI_SECTION_NAME;
+	      goto get_vma;
+#endif
 	    case DT_PLTGOT:
 	      name = htab->symbian_p ? ".got" : ".got.plt";
 	      goto get_vma;
@@ -16471,7 +17194,44 @@
 	  else
 	    {
 	      got_displacement = got_address - (plt_address + 16);
+#ifdef __RISCOS_TARGET__
+	      if (info->flag_pic == 2)
+	        {
+		  if (bfd_link_pic(info))
+		    {
+		      /* Insert __GOTT_INDEX__ reloc for this plt entry.  */
+		      bfd_put_32 (output_bfd,
+				  ((plt_address + 12) & 0x3fffffff) | (1 << 31),
+				  htab->srelpic->contents + htab->srelpic_offset);
+		      htab->srelpic_offset += 4;
+#ifndef RISCOS_OPTIMISE_PLT
+		      /* Insert __GOTT_BASE__ reloc for this plt entry.  */
+		      bfd_put_32 (output_bfd,
+				  plt_address + 20,
+				  htab->srelpic->contents + htab->srelpic_offset);
+		      htab->srelpic_offset += 4;
+#endif
 
+		      plt0_entry = elf32_arm_riscos_shared_plt0_entry;
+		      put_arm_insn (htab, output_bfd, plt0_entry[0], splt->contents + 0);
+		      put_arm_insn (htab, output_bfd, plt0_entry[1], splt->contents + 4);
+		      put_arm_insn (htab, output_bfd, plt0_entry[2], splt->contents + 8);
+		      put_arm_insn (htab, output_bfd, plt0_entry[3], splt->contents + 12);
+		      put_arm_insn (htab, output_bfd, plt0_entry[4], splt->contents + 16);
+		    }
+		  else
+		    {
+		      plt0_entry = elf32_arm_riscos_exec_plt0_entry;
+		      put_arm_insn (htab, output_bfd, plt0_entry[0], splt->contents +  0);
+		      put_arm_insn (htab, output_bfd, plt0_entry[1], splt->contents +  4);
+		      put_arm_insn (htab, output_bfd, plt0_entry[2], splt->contents +  8);
+		      put_arm_insn (htab, output_bfd, plt0_entry[3], splt->contents +  12);
+		      bfd_put_32 (output_bfd, got_displacement, splt->contents + 16);
+		    }
+	        }
+	      else
+#endif
+	        {
 	      plt0_entry = elf32_arm_plt0_entry;
 	      put_arm_insn (htab, output_bfd, plt0_entry[0],
 			    splt->contents + 0);
@@ -16489,6 +17249,7 @@
 #else
 	      bfd_put_32 (output_bfd, got_displacement, splt->contents + 16);
 #endif
+		}
 	    }
 	}
 
@@ -18800,13 +19561,34 @@
 				       flagsp, secp, valp))
     return FALSE;
 
+#ifdef __RISCOS_TARGET__
+  if (bfd_link_pic(info)
+      && (*namep)[0] == '_' && (*namep)[1] == '_'
+      && (strcmp (*namep, "__GOTT_BASE__") == 0
+	  || strcmp (*namep, "__GOTT_INDEX__") == 0))
+    {
+      struct elf_link_hash_table *htab;
+      struct elf_link_hash_entry *h;
+
+      htab = elf_hash_table (info);
+
+      h = elf_link_hash_lookup (htab, *namep, FALSE, FALSE, TRUE);
+      if (h)
+	h->type = STT_OBJECT;
+    }
+#endif
+
   return TRUE;
 }
 
 /* We use this to override swap_symbol_in and swap_symbol_out.  */
 const struct elf_size_info elf32_arm_size_info =
 {
+#ifdef __RISCOS_TARGET__
+  sizeof (Elf32_External_Ehdr) + 20,
+#else
   sizeof (Elf32_External_Ehdr),
+#endif
   sizeof (Elf32_External_Phdr),
   sizeof (Elf32_External_Shdr),
   sizeof (Elf32_External_Rel),
@@ -19195,6 +19977,8 @@
 #define ELF_MACHINE_CODE		EM_ARM
 #ifdef __QNXTARGET__
 #define ELF_MAXPAGESIZE			0x1000
+#elif defined(__RISCOS_TARGET__)
+#define ELF_MAXPAGESIZE			0x8000
 #else
 #define ELF_MAXPAGESIZE			0x10000
 #endif
