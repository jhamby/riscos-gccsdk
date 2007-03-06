--- bfd/elf32-arm.c.orig	2007-03-05 22:48:20.000000000 +0100
+++ bfd/elf32-arm.c	2007-03-05 22:46:23.000000000 +0100
@@ -1470,7 +1470,7 @@
 
 /* The name of the dynamic interpreter.  This is put in the .interp
    section.  */
-#define ELF_DYNAMIC_INTERPRETER     "/usr/lib/ld.so.1"
+#define ELF_DYNAMIC_INTERPRETER     "ld-riscos/so/1"
 
 #ifdef FOUR_WORD_PLT
 
@@ -1570,6 +1570,31 @@
     0x00000000,         /* dcd   R_ARM_GLOB_DAT(X) */
   };
 
+#define PLT_RISCOS_HEADER_SIZE 12
+#define PLT_RISCOS_ENTRY_SIZE 20
+
+/* The first entry in a procedure linkage table looks like
+   this.  It is set up so that any shared library function that is
+   called before the relocation has been set up calls the dynamic
+   linker first.  */
+static const bfd_vma elf32_arm_riscos_plt0_entry [PLT_RISCOS_HEADER_SIZE / 4] =
+  {
+    0xe92d509f,		/* stmfd r13!,{r0-r4,r7,r12,r14} */
+    0xef078581,		/* swi	 "XSOM_Resolver" */
+    0xe5b8f008,		/* ldr   pc, [r8, #8]!  */
+  };
+
+/* Subsequent entries in a procedure linkage table look like
+   this.  */
+static const bfd_vma elf32_arm_riscos_plt_entry [PLT_RISCOS_ENTRY_SIZE / 4] =
+  {
+    0xe24f8c00,		/* sub   r8, pc, 0x0000XX00 */
+    0xe2488000,		/* sub   r8, r8, 0x000000XX */
+    0xef078580,		/* swi   "XSOM_Prologue" */
+    0xe2888a00,		/* add   r8, r8, #0x000XX000 */
+    0xe5b8f000,		/* ldr   pc, [r8,#0x00000XXX]! */
+  };
+
 /* Used to build a map of a section.  This is required for mixed-endian
    code/data.  */
 
@@ -1763,6 +1788,20 @@
 
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
@@ -1888,6 +1927,12 @@
 	}
     }
 
+  if (info->flag_pic == 2)
+    {
+      htab->plt_header_size = PLT_RISCOS_HEADER_SIZE;
+      htab->plt_entry_size = PLT_RISCOS_ENTRY_SIZE;
+    }
+
   if (!htab->splt 
       || !htab->srelplt
       || !htab->sdynbss
@@ -2004,9 +2049,198 @@
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
+     RISCOS_MODULE_RELOCCODE_OFFSET_* and -1 sentinal value.  */
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
+  /* Set size which is the size of __RelocData code + its data and -1 sentinal
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
@@ -3005,6 +3239,42 @@
   else
     addend = signed_addend = rel->r_addend;
 
+  if (globals->bfd_ro_module_owner != NULL)
+    {
+      switch (r_type)
+        {
+         case R_ARM_ABS32:
+            {
+              if (strncmp (input_section->name, ".data", sizeof (".data")-1)
+                  && strncmp (input_section->name, ".rodata", sizeof (".rodata")-1))
+                break;
+
+              if (h != NULL) /* <= FIXME: gross hack: we should properly check if it points to RW area */
+                {
+                  /* Relocation inside RW data, i.e. in module RMA workspace.  */
+                  bfd_vma osection_addr = input_section->output_section->vma + input_section->output_offset - globals->ro_module_image_rw_base;
+
+                  BFD_ASSERT (globals->ro_module_relocdata_offset < globals->ro_module_relocdata_size);
+
+                  bfd_put_32 (input_bfd, osection_addr + rel->r_offset, globals->s_ro_module_relocdata->contents + globals->ro_module_relocdata_offset);
+                  globals->ro_module_relocdata_offset += 4;
+                }
+              /* Fall through.  */
+            }
+          case R_ARM_GOT32:
+          case R_ARM_GOTOFF32:
+            {
+              bfd_vma osection_addr = input_section->output_section->vma + input_section->output_offset - globals->ro_module_image_ro_base;
+
+              BFD_ASSERT (globals->ro_module_reloccode_offset < globals->ro_module_reloccode_size);
+
+              bfd_put_32 (input_bfd, osection_addr + rel->r_offset, globals->s_ro_module_reloccode->contents + globals->ro_module_reloccode_offset);
+              globals->ro_module_reloccode_offset += 4;
+              break;
+            }
+        }
+    }
+
   switch (r_type)
     {
     case R_ARM_NONE:
@@ -3706,7 +3976,8 @@
          define _GLOBAL_OFFSET_TABLE in a different way, as is
          permitted by the ABI, we might have to change this
          calculation.  */
-      value -= sgot->output_section->vma;
+      if (globals->s_ro_module_reloccode == NULL)
+        value -= sgot->output_section->vma;
       return _bfd_final_link_relocate (howto, input_bfd, input_section,
 				       contents, rel->r_offset, value,
 				       rel->r_addend);
@@ -4229,6 +4500,11 @@
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
@@ -4255,7 +4531,74 @@
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
+      globals->ro_module_image_ro_base = hash->root.u.def.value
+	+ hash->root.u.def.section->output_section->vma
+	+ hash->root.u.def.section->output_offset;
+
+      hash = elf_link_hash_lookup
+        (&(hash_table)->root, "Image$$RW$$Base", FALSE, FALSE, TRUE);
+
+      if (hash == NULL)
+        (*_bfd_error_handler) (_("Unable to find Image$$RW$$Base"));
+      globals->ro_module_image_rw_base = hash->root.u.def.value
+	+ hash->root.u.def.section->output_section->vma
+	+ hash->root.u.def.section->output_offset;
+
+      BFD_ASSERT (ro_module_reloccode_section != NULL);
+      BFD_ASSERT (ro_module_relocdata_section != NULL);
     }
+
   /* Invoke the ELF linker to do all the work.  */
   if (!bfd_elf_final_link (abfd, info))
     return FALSE;
@@ -4269,6 +4612,58 @@
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
+      BFD_ASSERT (globals->ro_module_relocdata_offset == globals->ro_module_relocdata_size - 4);
+
+      /* Write sentinel.  */
+      bfd_put_32 (globals->bfd_ro_module_owner,
+                  -1,
+                  ro_section->contents + globals->ro_module_relocdata_offset + 0*4);
+
+      /* Write __RelocData section. */
+      if (!bfd_set_section_contents (abfd, ro_module_relocdata_section, ro_section->contents, 0, ro_section->size))
+        return FALSE;
+    }
+
   return TRUE;
 }
 
@@ -5767,6 +6162,23 @@
 
       eh = (struct elf32_arm_link_hash_entry *) h;
 
+      /* For RISC OS module __RelocCode / __RelocData support.  */
+      switch (r_type)
+        {
+          case R_ARM_ABS32:
+            /* Only ABS32 in data & rodata section are taken.  */
+            if (strncmp (sec->name, ".data", sizeof (".data")-1)
+                && strncmp (sec->name, ".rodata", sizeof (".rodata")-1))
+              break;
+            if (h != NULL) /* <= FIXME: gross hack: we should properly check if it points to RW area */
+              htab->ro_module_relocdata_size += 4;
+            /* Fall through */
+          case R_ARM_GOT32:
+          case R_ARM_GOTOFF32:
+            htab->ro_module_reloccode_size += 4;
+            break;
+        }
+
       switch (r_type)
         {
 	  case R_ARM_GOT32:
@@ -6930,7 +7342,10 @@
 	     in all the symbols for which we are making plt entries.  The
 	     first three entries in .got.plt are reserved; after that
 	     symbols appear in the same order as in .plt.  */
-	  plt_index = (got_offset - 12) / 4;
+          if (info->flag_pic == 2)
+            plt_index = (h->plt.offset - PLT_RISCOS_HEADER_SIZE) / PLT_RISCOS_ENTRY_SIZE;
+          else
+            plt_index = (got_offset - 12) / 4;
 
 	  /* Calculate the address of the GOT entry.  */
 	  got_address = (sgot->output_section->vma
@@ -7012,22 +7427,47 @@
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
+		  bfd_put_32 (output_bfd,
+			      elf32_arm_riscos_plt_entry[0]
+			      | ((h->plt.offset & 0x0000ff00) >> 8),
+			      splt->contents + h->plt.offset);
+		  bfd_put_32 (output_bfd,
+			      elf32_arm_riscos_plt_entry[1]
+			      | (h->plt.offset & 0x000000ff),
+			      splt->contents + h->plt.offset + 4);
+		  bfd_put_32 (output_bfd,
+			      elf32_arm_riscos_plt_entry[2],
+			      splt->contents + h->plt.offset + 8);
+		  bfd_put_32 (output_bfd,
+			      elf32_arm_riscos_plt_entry[3]
+			      | ((got_offset & 0x000ff000) >> 12),
+			      splt->contents + h->plt.offset + 12);
+		  bfd_put_32 (output_bfd,
+			      elf32_arm_riscos_plt_entry[4]
+			      | (got_offset & 0x00000fff),
+			      splt->contents + h->plt.offset + 16);
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
@@ -7350,21 +7790,30 @@
 	    }
 	  else
 	    {
-	      got_displacement = got_address - (plt_address + 16);
+	      if (info->flag_pic == 2)
+	        {
+		  bfd_put_32 (output_bfd, elf32_arm_riscos_plt0_entry[0], splt->contents +  0);
+		  bfd_put_32 (output_bfd, elf32_arm_riscos_plt0_entry[1], splt->contents +  4);
+		  bfd_put_32 (output_bfd, elf32_arm_riscos_plt0_entry[2], splt->contents +  8);
+	        }
+	      else
+	        {
+		  got_displacement = got_address - (plt_address + 16);
 
-	      plt0_entry = elf32_arm_plt0_entry;
-	      bfd_put_32 (output_bfd, plt0_entry[0], splt->contents + 0);
-	      bfd_put_32 (output_bfd, plt0_entry[1], splt->contents + 4);
-	      bfd_put_32 (output_bfd, plt0_entry[2], splt->contents + 8);
-	      bfd_put_32 (output_bfd, plt0_entry[3], splt->contents + 12);
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
 
