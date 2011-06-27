--- bfd/elf32-arm.c.orig	2011-05-11 09:29:12.000000000 +0200
+++ bfd/elf32-arm.c	2011-06-27 22:54:09.574436744 +0200
@@ -1910,7 +1910,7 @@ typedef unsigned short int insn16;
 
 /* The name of the dynamic interpreter.  This is put in the .interp
    section.  */
-#define ELF_DYNAMIC_INTERPRETER     "/usr/lib/ld.so.1"
+#define ELF_DYNAMIC_INTERPRETER     "ld-riscos/so/1"
 
 #ifdef FOUR_WORD_PLT
 
@@ -2010,6 +2010,46 @@ static const bfd_vma elf32_arm_symbian_p
     0x00000000,         /* dcd   R_ARM_GLOB_DAT(X) */
   };
 
+#define PLT_RISCOS_EXEC_HEADER_SIZE 20
+#define PLT_RISCOS_SHARED_HEADER_SIZE 12
+#define PLT_RISCOS_ENTRY_SIZE 12
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
+    0xe5bef010,		/* ldr   pc, [lr, #16]! */
+    0x00000000		/* &GOT[0] - .          */
+  };
+
+static const bfd_vma elf32_arm_riscos_shared_plt0_entry [] =
+  {
+    0xe52de004,		/* str   lr, [sp, #-4]! */
+    0xe1a0e007,		/* mov   lr, r7         */
+    0xe5bef010,		/* ldr   pc, [lr, #16]! */
+  };
+
+/* Subsequent entries in a procedure linkage table look like
+   this.  */
+static const bfd_vma elf32_arm_riscos_exec_plt_entry [] =
+  {
+    0xe28f8600,		/* add   r8, pc, #0xXX00000 */
+    0xe2888a00,		/* add   r8, r8, #0x00XX000 */
+    0xe5b8f000		/* ldr   pc,[r8, #0x0000XXX]! */
+  };
+
+static const bfd_vma elf32_arm_riscos_shared_plt_entry [] =
+  {
+    0xe2878600,		/* add  r8, r7, #0xXX00000 */
+    0xe2888a00,		/* add	r8, r8, #0x00XX000 */
+    0xe5b8f000		/* ldr  pc,[r8, #0x0000XXX]!  @ call function */
+  };
+
 #define ARM_MAX_FWD_BRANCH_OFFSET  ((((1 << 23) - 1) << 2) + 8)
 #define ARM_MAX_BWD_BRANCH_OFFSET  ((-((1 << 23) << 2)) + 8)
 #define THM_MAX_FWD_BRANCH_OFFSET  ((1 << 22) -2 + 4)
@@ -2660,6 +2700,20 @@ struct elf32_arm_link_hash_table
   unsigned int bfd_count;
   int top_index;
   asection **input_list;
+
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
 };
 
 /* Create an entry in an ARM ELF linker hash table.  */
@@ -2820,6 +2874,15 @@ elf32_arm_create_dynamic_sections (bfd *
 	}
     }
 
+  if (info->flag_pic == 2)
+    {
+      if (info->shared)
+	htab->plt_header_size = PLT_RISCOS_SHARED_HEADER_SIZE;
+      else
+	htab->plt_header_size = PLT_RISCOS_EXEC_HEADER_SIZE;
+      htab->plt_entry_size = PLT_RISCOS_ENTRY_SIZE;
+    }
+
   if (!htab->splt
       || !htab->srelplt
       || !htab->sdynbss
@@ -2962,9 +3025,204 @@ elf32_arm_link_hash_table_create (bfd *a
       return NULL;
     }
 
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
   return &ret->root.root;
 }
 
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
+
 /* Free the derived linker hash table.  */
 
 static void
@@ -6939,6 +7197,51 @@ elf32_arm_final_link_relocate (reloc_how
   else
     addend = signed_addend = rel->r_addend;
 
+  if (globals->bfd_ro_module_owner != NULL)
+    {
+      /* __RelocData entry:
+         Any ABS32 reloc in a rw data section pointing to RW data (ie module workspace), need to have a __RelocData entry.  */
+#define ro_module_is_datareloc(type, secname) \
+  ((type) == R_ARM_ABS32 \
+   && (!strncmp (secname, ".data", sizeof (".data")-1)))
+      if (ro_module_is_datareloc (r_type, input_section->name)
+	  && (unsigned int)value + (unsigned int)addend >= (unsigned int)globals->ro_module_image_rw_base /* comparison needs to happen in 32-bit space.  */)
+	{
+	  /* Relocation inside RW data, i.e. in module RMA workspace.  */
+	  const bfd_vma osection_addr = input_section->output_section->vma + input_section->output_offset - globals->ro_module_image_rw_base;
+
+	  BFD_ASSERT (h != NULL);
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
+
   switch (r_type)
     {
     case R_ARM_NONE:
@@ -7880,6 +8183,7 @@ elf32_arm_final_link_relocate (reloc_how
          define _GLOBAL_OFFSET_TABLE in a different way, as is
          permitted by the ABI, we might have to change this
          calculation.  */
+      if (globals->s_ro_module_reloccode == NULL)
       value -= sgot->output_section->vma;
       return _bfd_final_link_relocate (howto, input_bfd, input_section,
 				       contents, rel->r_offset, value,
@@ -9437,10 +9741,86 @@ elf32_arm_final_link (bfd *abfd, struct 
 {
   struct elf32_arm_link_hash_table *globals = elf32_arm_hash_table (info);
   asection *sec, *osec;
+  asection *o;
+  struct bfd_link_order *p;
+  asection *ro_module_reloccode_section = NULL;
+  asection *ro_module_relocdata_section = NULL;
 
   if (globals == NULL)
     return FALSE;
 
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
+
   /* Invoke the regular ELF backend linker to do all the work.  */
   if (!bfd_elf_final_link (abfd, info))
     return FALSE;
@@ -9487,6 +9867,57 @@ elf32_arm_final_link (bfd *abfd, struct 
 	return FALSE;
     }
 
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
   return TRUE;
 }
 
@@ -10837,6 +11268,18 @@ elf32_arm_check_relocs (bfd *abfd, struc
 
       eh = (struct elf32_arm_link_hash_entry *) h;
 
+      /* For RISC OS module __RelocCode / __RelocData support.  */
+      if (ro_module_is_datareloc (r_type, sec->name))
+        {
+	  /* This can allocate too much but we can only be sure of the
+	     exact size after linking, e.g. after we know that the relocs
+	     are in RO RMA space or in RW RMA workspace.  */
+          if (h != NULL)
+            htab->ro_module_relocdata_size += 4;
+        }
+      if (ro_module_is_codereloc (r_type, sec->name))
+        htab->ro_module_reloccode_size += 4;
+
       switch (r_type)
         {
 	  case R_ARM_GOT32:
@@ -12035,8 +12478,7 @@ elf32_arm_size_dynamic_sections (bfd * o
 
       if (plt)
 	{
-	  if (   !add_dynamic_entry (DT_PLTGOT, 0)
-	      || !add_dynamic_entry (DT_PLTRELSZ, 0)
+	  if (   !add_dynamic_entry (DT_PLTRELSZ, 0)
 	      || !add_dynamic_entry (DT_PLTREL,
 				     htab->use_rel ? DT_REL : DT_RELA)
 	      || !add_dynamic_entry (DT_JMPREL, 0))
@@ -12061,6 +12503,9 @@ elf32_arm_size_dynamic_sections (bfd * o
 	    }
 	}
 
+      if (!add_dynamic_entry (DT_PLTGOT, 0))
+	return FALSE;
+
       /* If any dynamic relocs apply to a read-only section,
 	 then we need a DT_TEXTREL entry.  */
       if ((info->flags & DF_TEXTREL) == 0)
@@ -12160,6 +12605,9 @@ elf32_arm_finish_dynamic_symbol (bfd * o
 	     in all the symbols for which we are making plt entries.  The
 	     first three entries in .got.plt are reserved; after that
 	     symbols appear in the same order as in .plt.  */
+	  if (info->flag_pic == 2)
+	    plt_index = (got_offset - 20) / 4;
+	  else
 	  plt_index = (got_offset - 12) / 4;
 
 	  /* Calculate the address of the GOT entry.  */
@@ -12252,6 +12700,41 @@ elf32_arm_finish_dynamic_symbol (bfd * o
 				  elf32_arm_plt_thumb_stub[1], ptr - 2);
 		}
 
+	      if (info->flag_pic == 2)
+		{
+		  if (info->shared)
+		    {
+		      put_arm_insn (htab, output_bfd,
+			      elf32_arm_riscos_shared_plt_entry[0]
+			      | ((got_offset & 0x0ff00000) >> 20),
+			      ptr + 0);
+		      put_arm_insn (htab, output_bfd,
+			      elf32_arm_riscos_shared_plt_entry[1]
+			      | ((got_offset & 0x000ff000) >> 12),
+			      ptr + 4);
+		      put_arm_insn (htab, output_bfd,
+			      elf32_arm_riscos_shared_plt_entry[2]
+			      | (got_offset & 0x00000fff),
+			      ptr + 8);
+		    }
+		  else
+		    {
+		      put_arm_insn (htab, output_bfd,
+				  elf32_arm_riscos_exec_plt_entry[0]
+				  | ((got_displacement & 0x0ff00000) >> 20),
+				  ptr + 0);
+		      put_arm_insn (htab, output_bfd,
+				  elf32_arm_riscos_exec_plt_entry[1]
+				  | ((got_displacement & 0x000ff000) >> 12),
+				  ptr + 4);
+		      put_arm_insn (htab, output_bfd,
+				  elf32_arm_riscos_exec_plt_entry[2]
+				  | (got_displacement & 0x00000fff),
+				  ptr + 8);
+		    }
+		}
+	      else
+		{
 	      put_arm_insn (htab, output_bfd,
 			    elf32_arm_plt_entry[0]
 			    | ((got_displacement & 0x0ff00000) >> 20),
@@ -12268,6 +12751,7 @@ elf32_arm_finish_dynamic_symbol (bfd * o
 	      bfd_put_32 (output_bfd, elf32_arm_plt_entry[3], ptr + 12);
 #endif
 	    }
+	    }
 
 	  /* Fill in the entry in the global offset table.  */
 	  bfd_put_32 (output_bfd,
@@ -12600,6 +13084,27 @@ elf32_arm_finish_dynamic_sections (bfd *
 	    {
 	      got_displacement = got_address - (plt_address + 16);
 
+	      if (info->flag_pic == 2)
+	        {
+		  if (info->shared)
+		    {
+		      plt0_entry = elf32_arm_riscos_shared_plt0_entry;
+		      put_arm_insn (htab, output_bfd, plt0_entry[0], splt->contents + 0);
+		      put_arm_insn (htab, output_bfd, plt0_entry[1], splt->contents + 4);
+		      put_arm_insn (htab, output_bfd, plt0_entry[2], splt->contents + 8);
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
+	        {
 	      plt0_entry = elf32_arm_plt0_entry;
 	      put_arm_insn (htab, output_bfd, plt0_entry[0],
 			    splt->contents + 0);
@@ -12619,6 +13124,7 @@ elf32_arm_finish_dynamic_sections (bfd *
 #endif
 	    }
 	}
+	}
 
       /* UnixWare sets the entsize of .plt to 4, although that doesn't
 	 really seem like the right value.  */
@@ -13790,7 +14296,7 @@ elf32_arm_is_function_type (unsigned int
 /* We use this to override swap_symbol_in and swap_symbol_out.  */
 const struct elf_size_info elf32_arm_size_info =
 {
-  sizeof (Elf32_External_Ehdr),
+  sizeof (Elf32_External_Ehdr) + 20 /* RISC OS */,
   sizeof (Elf32_External_Phdr),
   sizeof (Elf32_External_Shdr),
   sizeof (Elf32_External_Rel),
@@ -13883,7 +14389,7 @@ const struct elf_size_info elf32_arm_siz
 #define elf_backend_may_use_rela_p     0
 #define elf_backend_default_use_rela_p 0
 
-#define elf_backend_got_header_size	12
+#define elf_backend_got_header_size	20 /* RISC OS: 12 -> 20 */
 
 #undef  elf_backend_obj_attrs_vendor
 #define elf_backend_obj_attrs_vendor		"aeabi"
