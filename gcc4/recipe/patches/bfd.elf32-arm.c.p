--- bfd/elf32-arm.c.orig	2009-03-27 02:04:34.000000000 +0100
+++ bfd/elf32-arm.c	2009-03-28 03:40:28.000000000 +0100
@@ -1470,7 +1470,7 @@ typedef unsigned short int insn16;
 
 /* The name of the dynamic interpreter.  This is put in the .interp
    section.  */
-#define ELF_DYNAMIC_INTERPRETER     "/usr/lib/ld.so.1"
+#define ELF_DYNAMIC_INTERPRETER     "ld-riscos/so/1"
 
 #ifdef FOUR_WORD_PLT
 
@@ -1570,6 +1570,46 @@ static const bfd_vma elf32_arm_symbian_p
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
 /* Used to build a map of a section.  This is required for mixed-endian
    code/data.  */
 
@@ -1763,6 +1803,20 @@ struct elf32_arm_link_hash_table
 
     /* For convenience in allocate_dynrelocs.  */
     bfd * obfd;
+
+    /* For RISC OS module __RelocCode & __RelocData support.  */
+    bfd * bfd_ro_module_owner;
+
+    asection * s_ro_module_reloccode;
+    bfd_vma ro_module_reloccode_offset;
+    bfd_vma ro_module_reloccode_size;
+
+    asection * s_ro_module_relocdata;
+    bfd_vma ro_module_relocdata_offset;
+    bfd_vma ro_module_relocdata_size;
+
+    bfd_vma ro_module_image_ro_base;
+    bfd_vma ro_module_image_rw_base;
   };
 
 /* Create an entry in an ARM ELF linker hash table.  */
@@ -1888,6 +1942,15 @@ elf32_arm_create_dynamic_sections (bfd *
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
@@ -2004,9 +2067,202 @@ elf32_arm_link_hash_table_create (bfd *a
   ret->obfd = abfd;
   ret->tls_ldm_got.refcount = 0;
 
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
+  0xE59F102C,	/*	LDR	R1, RelocCode     */
+  0xE24FB00C,	/*	ADR	R11, __RelocCode  */
+  0xE05B1001,	/*	SUBS	R1, R11, R1       */
+  0x159FB024,	/*	LDRNE	R11, ImageROBase  */
+  0x108BB001,	/*	ADDNE	R11, R11,R1       */
+  0x128F2020,	/*	ADRNE	R2, RelocCodeList */
+		/* RelocCodeLoop:                 */
+  0x14920004,	/*	LDRNE	R0, [R2], #4      */
+  0x13700001,	/*	CMPNE	R0, #-1           */
+  0x179B3000,	/*	LDRNE	R3, [R11, R0]     */
+  0x10833001,	/*	ADDNE	R3, R3, R1        */
+  0x178B3000,	/*	STRNE	R3, [R11, R0]     */
+  0x1AFFFFF9,	/*	BNE	RelocCodeLoop     */
+  0xE1A0F00E,	/*	MOV	PC, R14           */
+#define RISCOS_MODULE_RELOCCODE_OFFSET_RELOCCODE (13*4)
+  0x00000000,	/* RelocCode:                     */
+		/*	.word	__RelocCode       */
+#define RISCOS_MODULE_RELOCCODE_OFFSET_IMAGEROBASE (14*4)
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
+  globals->ro_module_reloccode_size += 4*NUM_ELEM (riscos_module_reloccode) + 3*4;
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
+  globals->ro_module_relocdata_size += 4*NUM_ELEM (riscos_module_relocdata) + 1*4;
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
+  for (i = 0; i < NUM_ELEM (riscos_module_reloccode); ++i)
+    bfd_put_32 (globals->bfd_ro_module_owner, riscos_module_reloccode[i], s->contents + i*4);
+
+  globals->ro_module_reloccode_offset = 4*NUM_ELEM (riscos_module_reloccode);
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
+  for (i = 0; i < NUM_ELEM (riscos_module_relocdata); ++i)
+    bfd_put_32 (globals->bfd_ro_module_owner, riscos_module_relocdata[i], s->contents + i*4);
+
+  globals->ro_module_relocdata_offset = 4*NUM_ELEM (riscos_module_relocdata);
+
+  return TRUE;
+}
+
 /* Locate the Thumb encoded calling stub for NAME.  */
 
 static struct elf_link_hash_entry *
@@ -3005,6 +3261,45 @@ elf32_arm_final_link_relocate (reloc_how
   else
     addend = signed_addend = rel->r_addend;
 
+  if (globals->bfd_ro_module_owner != NULL)
+    {
+      /* __RelocData entry:
+         Any ABS32 reloc in a rw data section pointing to RW data (ie module workspace), need to have a __RelocData entry.  */
+#define ro_module_is_datareloc(type, secname) \
+  ((type) == R_ARM_ABS32 \
+   && (!strncmp (secname, ".data", sizeof (".data")-1) && strcmp (secname, ".data.rel.ro")))
+      if (ro_module_is_datareloc (r_type, input_section->name)
+          && (unsigned int)value + (unsigned int)addend >= (unsigned int)globals->ro_module_image_rw_base /* comparison needs to happen in 32-bit space.  */)
+        {
+          /* Relocation inside RW data, i.e. in module RMA workspace.  */
+          const bfd_vma osection_addr = input_section->output_section->vma + input_section->output_offset - globals->ro_module_image_rw_base;
+
+	  BFD_ASSERT (h != NULL); /* Just to make sure our dodgy allocation approach is not underallocating.  */
+          BFD_ASSERT (globals->ro_module_relocdata_offset < globals->ro_module_relocdata_size);
+
+          bfd_put_32 (input_bfd, osection_addr + rel->r_offset, globals->s_ro_module_relocdata->contents + globals->ro_module_relocdata_offset);
+          globals->ro_module_relocdata_offset += 4;
+	}
+
+      /* __RelocCode entry:
+         An ABS32 reloc in any .data* and .rodata* section needs to result in an __RelocCode entry.
+         Any GOT32/GOTOFF32 reloc needs to be mentioned in __RelocCode entry.  */
+#define ro_module_is_codereloc(type, secname) \
+  (((type) == R_ARM_ABS32 && (!strncmp (secname, ".data", sizeof (".data")-1) || !strncmp (secname, ".rodata", sizeof (".rodata")-1))) \
+   || (type) == R_ARM_GOT32 \
+   || (type) == R_ARM_GOTOFF32)
+      if (ro_module_is_codereloc (r_type, input_section->name))
+	{
+	  /* Relocation because of placing Module in RMA instead of application space.  */
+	  const bfd_vma osection_addr = input_section->output_section->vma + input_section->output_offset - globals->ro_module_image_ro_base;
+
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
@@ -3706,7 +4001,8 @@ elf32_arm_final_link_relocate (reloc_how
          define _GLOBAL_OFFSET_TABLE in a different way, as is
          permitted by the ABI, we might have to change this
          calculation.  */
-      value -= sgot->output_section->vma;
+      if (globals->s_ro_module_reloccode == NULL)
+        value -= sgot->output_section->vma;
       return _bfd_final_link_relocate (howto, input_bfd, input_section,
 				       contents, rel->r_offset, value,
 				       rel->r_addend);
@@ -4229,6 +4525,11 @@ elf32_arm_bfd_final_link (bfd *abfd, str
   asection *attr_section = NULL;
   bfd_byte *contents;
   bfd_vma size = 0;
+  asection *ro_module_reloccode_section = NULL;
+  asection *ro_module_relocdata_section = NULL;
+  struct elf32_arm_link_hash_table * globals;
+
+  globals = elf32_arm_hash_table (info);
 
   /* elf32_arm_merge_private_bfd_data will already have merged the
      object attributes.  Remove the input sections from the link, and set
@@ -4255,6 +4556,74 @@ elf32_arm_bfd_final_link (bfd *abfd, str
 	  /* Skip this section later on.  */
 	  o->map_head.link_order = NULL;
 	}
+      else if (strcmp (o->name, ".riscos.module.reloccode") == 0)
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
     }
   /* Invoke the ELF linker to do all the work.  */
   if (!bfd_elf_final_link (abfd, info))
@@ -4269,6 +4638,58 @@ elf32_arm_bfd_final_link (bfd *abfd, str
       bfd_set_section_contents (abfd, attr_section, contents, 0, size);
       free (contents);
     }
+
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
 
@@ -5767,6 +6188,18 @@ elf32_arm_check_relocs (bfd *abfd, struc
 
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
@@ -6815,8 +7248,7 @@ elf32_arm_size_dynamic_sections (bfd * o
 
       if (plt)
 	{
-	  if (   !add_dynamic_entry (DT_PLTGOT, 0)
-	      || !add_dynamic_entry (DT_PLTRELSZ, 0)
+	  if (   !add_dynamic_entry (DT_PLTRELSZ, 0)
 	      || !add_dynamic_entry (DT_PLTREL,
 				     htab->use_rel ? DT_REL : DT_RELA)
 	      || !add_dynamic_entry (DT_JMPREL, 0))
@@ -6841,6 +7273,9 @@ elf32_arm_size_dynamic_sections (bfd * o
 	    }
 	}
 
+      if (!add_dynamic_entry (DT_PLTGOT, 0))
+	return FALSE;
+
       /* If any dynamic relocs apply to a read-only section,
 	 then we need a DT_TEXTREL entry.  */
       if ((info->flags & DF_TEXTREL) == 0)
@@ -6930,7 +7365,10 @@ elf32_arm_finish_dynamic_symbol (bfd * o
 	     in all the symbols for which we are making plt entries.  The
 	     first three entries in .got.plt are reserved; after that
 	     symbols appear in the same order as in .plt.  */
-	  plt_index = (got_offset - 12) / 4;
+          if (info->flag_pic == 2)
+            plt_index = (got_offset - 20) / 4;
+          else
+            plt_index = (got_offset - 12) / 4;
 
 	  /* Calculate the address of the GOT entry.  */
 	  got_address = (sgot->output_section->vma
@@ -7012,22 +7450,58 @@ elf32_arm_finish_dynamic_symbol (bfd * o
 			      splt->contents + h->plt.offset - 2);
 		}
 
-	      bfd_put_32 (output_bfd,
-			  elf32_arm_plt_entry[0]
-			  | ((got_displacement & 0x0ff00000) >> 20),
-			  splt->contents + h->plt.offset + 0);
-	      bfd_put_32 (output_bfd,
-			  elf32_arm_plt_entry[1]
-			  | ((got_displacement & 0x000ff000) >> 12),
-			  splt->contents + h->plt.offset + 4);
-	      bfd_put_32 (output_bfd,
-			  elf32_arm_plt_entry[2]
-			  | (got_displacement & 0x00000fff),
-			  splt->contents + h->plt.offset + 8);
+	      if (info->flag_pic == 2)
+		{
+		  if (info->shared)
+		    {
+		      bfd_put_32 (output_bfd,
+			      elf32_arm_riscos_shared_plt_entry[0]
+			      | ((got_offset & 0x0ff00000) >> 20),
+			      splt->contents + h->plt.offset);
+		      bfd_put_32 (output_bfd,
+			      elf32_arm_riscos_shared_plt_entry[1]
+			      | ((got_offset & 0x000ff000) >> 12),
+			      splt->contents + h->plt.offset + 4);
+		      bfd_put_32 (output_bfd,
+			      elf32_arm_riscos_shared_plt_entry[2]
+			      | (got_offset & 0x00000fff),
+			      splt->contents + h->plt.offset + 8);
+		    }
+		  else
+		    {
+		      bfd_put_32 (output_bfd,
+				  elf32_arm_riscos_exec_plt_entry[0]
+				  | ((got_displacement & 0x0ff00000) >> 20),
+				  splt->contents + h->plt.offset + 0);
+		      bfd_put_32 (output_bfd,
+				  elf32_arm_riscos_exec_plt_entry[1]
+				  | ((got_displacement & 0x000ff000) >> 12),
+				  splt->contents + h->plt.offset + 4);
+		      bfd_put_32 (output_bfd,
+				  elf32_arm_riscos_exec_plt_entry[2]
+				  | (got_displacement & 0x00000fff),
+				  splt->contents + h->plt.offset + 8);
+		    }
+		}
+	      else
+		{
+		  bfd_put_32 (output_bfd,
+			      elf32_arm_plt_entry[0]
+			      | ((got_displacement & 0x0ff00000) >> 20),
+			      splt->contents + h->plt.offset + 0);
+		  bfd_put_32 (output_bfd,
+			      elf32_arm_plt_entry[1]
+			      | ((got_displacement & 0x000ff000) >> 12),
+			      splt->contents + h->plt.offset + 4);
+		  bfd_put_32 (output_bfd,
+			      elf32_arm_plt_entry[2]
+			      | (got_displacement & 0x00000fff),
+			      splt->contents + h->plt.offset + 8);
 #ifdef FOUR_WORD_PLT
-	      bfd_put_32 (output_bfd, elf32_arm_plt_entry[3],
-			  splt->contents + h->plt.offset + 12);
+		  bfd_put_32 (output_bfd, elf32_arm_plt_entry[3],
+			      splt->contents + h->plt.offset + 12);
 #endif
+		}
 	    }
 
 	  /* Fill in the entry in the global offset table.  */
@@ -7352,19 +7826,41 @@ elf32_arm_finish_dynamic_sections (bfd *
 	    {
 	      got_displacement = got_address - (plt_address + 16);
 
-	      plt0_entry = elf32_arm_plt0_entry;
-	      bfd_put_32 (output_bfd, plt0_entry[0], splt->contents + 0);
-	      bfd_put_32 (output_bfd, plt0_entry[1], splt->contents + 4);
-	      bfd_put_32 (output_bfd, plt0_entry[2], splt->contents + 8);
-	      bfd_put_32 (output_bfd, plt0_entry[3], splt->contents + 12);
+	      if (info->flag_pic == 2)
+	        {
+		  if (info->shared)
+		    {
+		      plt0_entry = elf32_arm_riscos_shared_plt0_entry;
+		      bfd_put_32 (output_bfd, plt0_entry[0], splt->contents + 0);
+		      bfd_put_32 (output_bfd, plt0_entry[1], splt->contents + 4);
+		      bfd_put_32 (output_bfd, plt0_entry[2], splt->contents + 8);
+		    }
+		  else
+		    {
+		      plt0_entry = elf32_arm_riscos_exec_plt0_entry;
+		      bfd_put_32 (output_bfd, plt0_entry[0], splt->contents +  0);
+		      bfd_put_32 (output_bfd, plt0_entry[1], splt->contents +  4);
+		      bfd_put_32 (output_bfd, plt0_entry[2], splt->contents +  8);
+		      bfd_put_32 (output_bfd, plt0_entry[3], splt->contents +  12);
+		      bfd_put_32 (output_bfd, got_displacement, splt->contents + 16);
+		    }
+	        }
+	      else
+	        {
+		  plt0_entry = elf32_arm_plt0_entry;
+		  bfd_put_32 (output_bfd, plt0_entry[0], splt->contents + 0);
+		  bfd_put_32 (output_bfd, plt0_entry[1], splt->contents + 4);
+		  bfd_put_32 (output_bfd, plt0_entry[2], splt->contents + 8);
+		  bfd_put_32 (output_bfd, plt0_entry[3], splt->contents + 12);
 
 #ifdef FOUR_WORD_PLT
-	      /* The displacement value goes in the otherwise-unused
-		 last word of the second entry.  */
-	      bfd_put_32 (output_bfd, got_displacement, splt->contents + 28);
+		/* The displacement value goes in the otherwise-unused
+		   last word of the second entry.  */
+		  bfd_put_32 (output_bfd, got_displacement, splt->contents + 28);
 #else
-	      bfd_put_32 (output_bfd, got_displacement, splt->contents + 16);
+		  bfd_put_32 (output_bfd, got_displacement, splt->contents + 16);
 #endif
+	        }
 	    }
 	}
 
@@ -8053,7 +8549,7 @@ elf32_arm_additional_program_headers (bf
 
 /* We use this to override swap_symbol_in and swap_symbol_out.  */
 const struct elf_size_info elf32_arm_size_info = {
-  sizeof (Elf32_External_Ehdr),
+  sizeof (Elf32_External_Ehdr) + 20,
   sizeof (Elf32_External_Phdr),
   sizeof (Elf32_External_Shdr),
   sizeof (Elf32_External_Rel),
@@ -8140,7 +8636,7 @@ const struct elf_size_info elf32_arm_siz
 #define elf_backend_default_use_rela_p 0
 #define elf_backend_rela_normal     0
 
-#define elf_backend_got_header_size	12
+#define elf_backend_got_header_size	20
 
 #include "elf32-target.h"
 
