#ifdef __STDC__
#define PARAMS(paramlist) paramlist
#else
#define PARAMS(paramlist) ()
#endif

extern void __gnat_set_globals 
 PARAMS ((int, int, int, int, int, int, 
          void (*) PARAMS ((void)), int, int));
extern void adafinal PARAMS ((void));
extern void adainit PARAMS ((void));
extern void system__standard_library__adafinal PARAMS ((void));
extern int main PARAMS ((int, char **, char **));
extern void exit PARAMS ((int));
extern void __gnat_break_start PARAMS ((void));
extern void _ada_gnatmake PARAMS ((void));
extern void __gnat_initialize PARAMS ((void));
extern void __gnat_finalize PARAMS ((void));
extern void __gnat_install_handler PARAMS ((void));

extern void ada__exceptions___elabs PARAMS ((void));
extern void system__exception_table___elabb PARAMS ((void));
extern void ada__io_exceptions___elabs PARAMS ((void));
extern void ada__strings___elabs PARAMS ((void));
extern void system__assertions___elabs PARAMS ((void));
extern void system__exceptions___elabs PARAMS ((void));
extern void ada__tags___elabs PARAMS ((void));
extern void ada__tags___elabb PARAMS ((void));
extern void ada__streams___elabs PARAMS ((void));
extern void system__soft_links___elabb PARAMS ((void));
extern void system__secondary_stack___elabb PARAMS ((void));
extern void system__finalization_root___elabs PARAMS ((void));
extern void ada__exceptions___elabb PARAMS ((void));
extern void ada__strings__maps___elabs PARAMS ((void));
extern void ada__strings__maps__constants___elabs PARAMS ((void));
extern void system__finalization_implementation___elabs PARAMS ((void));
extern void system__finalization_implementation___elabb PARAMS ((void));
extern void ada__finalization___elabs PARAMS ((void));
extern void ada__finalization__list_controller___elabs PARAMS ((void));
extern void gnat__directory_operations___elabs PARAMS ((void));
extern void mlib___elabs PARAMS ((void));
extern void sfn_scan___elabs PARAMS ((void));
extern void types___elabs PARAMS ((void));
extern void opt___elabs PARAMS ((void));
extern void csets___elabb PARAMS ((void));
extern void osint___elabs PARAMS ((void));
extern void scans___elabs PARAMS ((void));
extern void tree_io___elabs PARAMS ((void));
extern void elists___elabb PARAMS ((void));
extern void ali___elabs PARAMS ((void));
extern void ali__util___elabs PARAMS ((void));
extern void lib___elabs PARAMS ((void));
extern void make___elabs PARAMS ((void));
extern void namet___elabs PARAMS ((void));
extern void stringt___elabb PARAMS ((void));
extern void osint___elabb PARAMS ((void));
extern void fname___elabb PARAMS ((void));
extern void fmap___elabb PARAMS ((void));
extern void prj___elabs PARAMS ((void));
extern void mlib__prj___elabb PARAMS ((void));
extern void prj__attr___elabs PARAMS ((void));
extern void prj__com___elabs PARAMS ((void));
extern void prj__ext___elabb PARAMS ((void));
extern void prj__tree___elabs PARAMS ((void));
extern void prj__env___elabb PARAMS ((void));
extern void sinput___elabs PARAMS ((void));
extern void uintp___elabs PARAMS ((void));
extern void errout___elabs PARAMS ((void));
extern void prj__proc___elabb PARAMS ((void));
extern void prj__nmsc___elabb PARAMS ((void));
extern void urealp___elabb PARAMS ((void));
extern void prj__strt___elabb PARAMS ((void));
extern void prj__part___elabb PARAMS ((void));
extern void atree___elabs PARAMS ((void));
extern void atree___elabb PARAMS ((void));
extern void nlists___elabb PARAMS ((void));
extern void prj___elabb PARAMS ((void));
extern void switch___elabb PARAMS ((void));
extern void fname__uf___elabb PARAMS ((void));
extern void errout___elabb PARAMS ((void));
extern void make___elabb PARAMS ((void));

extern int  __gnat_handler_installed;

extern int gnat_argc;
extern char **gnat_argv;
extern char **gnat_envp;
extern int gnat_exit_status;


char __gnat_version[] = "GNAT Version: 5.00w (20010924)";
char __gnat_ada_main_program_name[] = "_ada_gnatmake";
void adafinal () {
   system__standard_library__adafinal ();
}

void adainit ()
{
   extern char ada__exceptions_E;
   extern char system__exception_table_E;
   extern char ada__io_exceptions_E;
   extern char ada__strings_E;
   extern char system__assertions_E;
   extern char system__exceptions_E;
   extern char system__secondary_stack_E;
   extern char ada__tags_E;
   extern char ada__streams_E;
   extern char system__soft_links_E;
   extern char system__finalization_root_E;
   extern char ada__strings__maps_E;
   extern char ada__strings__maps__constants_E;
   extern char system__finalization_implementation_E;
   extern char ada__finalization_E;
   extern char ada__finalization__list_controller_E;
   extern char gnat__directory_operations_E;
   extern char gnatmake_E;
   extern char makeusg_E;
   extern char mlib_E;
   extern char mlib__fil_E;
   extern char mlib__utl_E;
   extern char sfn_scan_E;
   extern char switch_E;
   extern char types_E;
   extern char binderr_E;
   extern char butil_E;
   extern char casing_E;
   extern char elists_E;
   extern char fmap_E;
   extern char fname_E;
   extern char fname__sf_E;
   extern char fname__uf_E;
   extern char mlib__tgt_E;
   extern char nlists_E;
   extern char opt_E;
   extern char csets_E;
   extern char osint_E;
   extern char scans_E;
   extern char scn_E;
   extern char sdefault_E;
   extern char snames_E;
   extern char stringt_E;
   extern char style_E;
   extern char stylesw_E;
   extern char tree_io_E;
   extern char ali_E;
   extern char ali__util_E;
   extern char lib_E;
   extern char make_E;
   extern char namet_E;
   extern char prj_E;
   extern char mlib__prj_E;
   extern char prj__attr_E;
   extern char prj__com_E;
   extern char prj__env_E;
   extern char prj__ext_E;
   extern char prj__nmsc_E;
   extern char prj__pars_E;
   extern char prj__tree_E;
   extern char prj__dect_E;
   extern char prj__part_E;
   extern char prj__proc_E;
   extern char prj__strt_E;
   extern char prj__util_E;
   extern char sinput_E;
   extern char sinput__l_E;
   extern char sinput__p_E;
   extern char uintp_E;
   extern char errout_E;
   extern char uname_E;
   extern char urealp_E;
   extern char einfo_E;
   extern char sinfo_E;
   extern char atree_E;
   extern char sinfo__cn_E;
   extern char usage_E;
   extern char validsw_E;
   extern char widechar_E;
   extern char stand_E;

   __gnat_set_globals (
      -1,      /* Main_Priority              */
      -1,      /* Time_Slice_Value           */
      'b',     /* WC_Encoding                */
      ' ',     /* Locking_Policy             */
      ' ',     /* Queuing_Policy             */
      ' ',     /* Tasking_Dispatching_Policy */
      0,       /* Finalization routine address, not used anymore */
      0,       /* Unreserve_All_Interrupts */
      0);      /* Exception_Tracebacks */
   if (__gnat_handler_installed == 0)
     {
        __gnat_install_handler ();
     }

   if (ada__exceptions_E == 0) {
      ada__exceptions___elabs ();
   }
   if (system__exception_table_E == 0) {
      system__exception_table___elabb ();
      system__exception_table_E++;
   }
   if (ada__io_exceptions_E == 0) {
      ada__io_exceptions___elabs ();
      ada__io_exceptions_E++;
   }
   if (ada__strings_E == 0) {
      ada__strings___elabs ();
      ada__strings_E++;
   }
   if (system__assertions_E == 0) {
      system__assertions___elabs ();
   }
   system__assertions_E = 1;
   if (system__exceptions_E == 0) {
      system__exceptions___elabs ();
      system__exceptions_E++;
   }
   if (ada__tags_E == 0) {
      ada__tags___elabs ();
   }
   if (ada__tags_E == 0) {
      ada__tags___elabb ();
      ada__tags_E++;
   }
   if (ada__streams_E == 0) {
      ada__streams___elabs ();
      ada__streams_E++;
   }
   if (system__soft_links_E == 0) {
      system__soft_links___elabb ();
      system__soft_links_E++;
   }
   if (system__secondary_stack_E == 0) {
      system__secondary_stack___elabb ();
      system__secondary_stack_E++;
   }
   if (system__finalization_root_E == 0) {
      system__finalization_root___elabs ();
   }
   system__finalization_root_E = 1;
   if (ada__exceptions_E == 0) {
      ada__exceptions___elabb ();
      ada__exceptions_E++;
   }
   if (ada__strings__maps_E == 0) {
      ada__strings__maps___elabs ();
   }
   ada__strings__maps_E = 1;
   if (ada__strings__maps__constants_E == 0) {
      ada__strings__maps__constants___elabs ();
      ada__strings__maps__constants_E++;
   }
   if (system__finalization_implementation_E == 0) {
      system__finalization_implementation___elabs ();
   }
   if (system__finalization_implementation_E == 0) {
      system__finalization_implementation___elabb ();
      system__finalization_implementation_E++;
   }
   if (ada__finalization_E == 0) {
      ada__finalization___elabs ();
   }
   ada__finalization_E = 1;
   if (ada__finalization__list_controller_E == 0) {
      ada__finalization__list_controller___elabs ();
   }
   ada__finalization__list_controller_E = 1;
   if (gnat__directory_operations_E == 0) {
      gnat__directory_operations___elabs ();
   }
   gnat__directory_operations_E = 1;
   if (mlib_E == 0) {
      mlib___elabs ();
   }
   if (sfn_scan_E == 0) {
      sfn_scan___elabs ();
   }
   sfn_scan_E = 1;
   if (types_E == 0) {
      types___elabs ();
   }
   types_E = 1;
   mlib__tgt_E = 1;
   mlib__fil_E = 1;
   if (opt_E == 0) {
      opt___elabs ();
   }
   if (csets_E == 0) {
      csets___elabb ();
      csets_E++;
   }
   if (osint_E == 0) {
      osint___elabs ();
   }
   mlib_E = 1;
   if (scans_E == 0) {
      scans___elabs ();
   }
   scans_E = 1;
   sdefault_E = 1;
   stylesw_E = 1;
   if (tree_io_E == 0) {
      tree_io___elabs ();
   }
   tree_io_E = 1;
   opt_E = 1;
   if (elists_E == 0) {
      elists___elabb ();
      elists_E++;
   }
   if (ali_E == 0) {
      ali___elabs ();
   }
   if (ali__util_E == 0) {
      ali__util___elabs ();
   }
   if (lib_E == 0) {
      lib___elabs ();
   }
   if (make_E == 0) {
      make___elabs ();
   }
   gnatmake_E = 1;
   if (namet_E == 0) {
      namet___elabs ();
   }
   ali__util_E = 1;
   ali_E = 1;
   if (stringt_E == 0) {
      stringt___elabb ();
      stringt_E++;
   }
   snames_E = 1;
   if (osint_E == 0) {
      osint___elabb ();
      osint_E++;
   }
   fname__sf_E = 1;
   if (fname_E == 0) {
      fname___elabb ();
      fname_E++;
   }
   if (fmap_E == 0) {
      fmap___elabb ();
      fmap_E++;
   }
   butil_E = 1;
   binderr_E = 1;
   mlib__utl_E = 1;
   if (prj_E == 0) {
      prj___elabs ();
   }
   if (mlib__prj_E == 0) {
      mlib__prj___elabb ();
      mlib__prj_E++;
   }
   if (prj__attr_E == 0) {
      prj__attr___elabs ();
   }
   prj__attr_E = 1;
   if (prj__com_E == 0) {
      prj__com___elabs ();
   }
   prj__com_E = 1;
   if (prj__ext_E == 0) {
      prj__ext___elabb ();
      prj__ext_E++;
   }
   if (prj__tree_E == 0) {
      prj__tree___elabs ();
   }
   prj__tree_E = 1;
   prj__util_E = 1;
   if (prj__env_E == 0) {
      prj__env___elabb ();
      prj__env_E++;
   }
   if (sinput_E == 0) {
      sinput___elabs ();
   }
   sinput_E = 1;
   sinput__p_E = 1;
   if (uintp_E == 0) {
      uintp___elabs ();
   }
   uintp_E = 1;
   if (errout_E == 0) {
      errout___elabs ();
   }
   if (prj__proc_E == 0) {
      prj__proc___elabb ();
      prj__proc_E++;
   }
   prj__pars_E = 1;
   if (prj__nmsc_E == 0) {
      prj__nmsc___elabb ();
      prj__nmsc_E++;
   }
   if (urealp_E == 0) {
      urealp___elabb ();
      urealp_E++;
   }
   if (prj__strt_E == 0) {
      prj__strt___elabb ();
      prj__strt_E++;
   }
   if (prj__part_E == 0) {
      prj__part___elabb ();
      prj__part_E++;
   }
   prj__dect_E = 1;
   if (atree_E == 0) {
      atree___elabs ();
   }
   if (atree_E == 0) {
      atree___elabb ();
      atree_E++;
   }
   sinfo_E = 1;
   uname_E = 1;
   sinput__l_E = 1;
   if (nlists_E == 0) {
      nlists___elabb ();
      nlists_E++;
   }
   sinfo__cn_E = 1;
   if (prj_E == 0) {
      prj___elabb ();
      prj_E++;
   }
   usage_E = 1;
   makeusg_E = 1;
   validsw_E = 1;
   if (switch_E == 0) {
      switch___elabb ();
      switch_E++;
   }
   widechar_E = 1;
   namet_E = 1;
   scn_E = 1;
   if (fname__uf_E == 0) {
      fname__uf___elabb ();
      fname__uf_E++;
   }
   casing_E = 1;
   stand_E = 1;
   einfo_E = 1;
   if (errout_E == 0) {
      errout___elabb ();
      errout_E++;
   }
   lib_E = 1;
   style_E = 1;
   if (make_E == 0) {
      make___elabb ();
      make_E++;
   }
}
int main (argc, argv, envp)
    int argc;
    char **argv;
    char **envp;
{
   gnat_argc = argc;
   gnat_argv = argv;
   gnat_envp = envp;
 
   __gnat_initialize ();
   adainit ();
   __gnat_break_start ();
 
   _ada_gnatmake ();
 
   system__standard_library__adafinal ();
   __gnat_finalize ();
   exit (gnat_exit_status);
}
unsigned gnatmakeB = 0x9cce8c36;
unsigned gnatmakeS = 0xa19b9bf8;
unsigned system__standard_libraryB = 0xc0e50ac3;
unsigned system__standard_libraryS = 0x6d0b25e7;
unsigned gnatvsnS = 0x575b423a;
unsigned makeB = 0x26680342;
unsigned makeS = 0x99ba40a3;
unsigned adaS = 0xa0108083;
unsigned ada__command_lineB = 0x25dfd6a9;
unsigned ada__command_lineS = 0xd8088025;
unsigned systemS = 0x5f7c428f;
unsigned system__secondary_stackB = 0xdedafdd0;
unsigned system__secondary_stackS = 0x7a466d01;
unsigned system__parametersB = 0x38b39edd;
unsigned system__parametersS = 0xb77e93cc;
unsigned system__soft_linksB = 0xfd0fec16;
unsigned system__soft_linksS = 0xccb8d065;
unsigned system__machine_state_operationsB = 0xafcd5e0c;
unsigned system__machine_state_operationsS = 0x65adb8f1;
unsigned system__exceptionsS = 0xbbfe2e15;
unsigned ada__exceptionsB = 0x98bed957;
unsigned ada__exceptionsS = 0x20d65858;
unsigned gnatS = 0x6380ea48;
unsigned gnat__heap_sort_aB = 0xc5fbb2c4;
unsigned gnat__heap_sort_aS = 0xdc301c49;
unsigned system__exception_tableB = 0x58c090c8;
unsigned system__exception_tableS = 0x7bff8c6d;
unsigned gnat__htableB = 0x2c01a59d;
unsigned gnat__htableS = 0xeb251ef2;
unsigned system__storage_elementsB = 0xa4e49d90;
unsigned system__storage_elementsS = 0x99c2b222;
unsigned system__tracebackB = 0x4d4e8ee2;
unsigned system__tracebackS = 0x69317710;
unsigned system__stack_checkingB = 0x31fdd957;
unsigned system__stack_checkingS = 0x16073c88;
unsigned aliB = 0x85ff58f7;
unsigned aliS = 0x1b1be218;
unsigned butilB = 0x2aca3c89;
unsigned butilS = 0xb94d2b2d;
unsigned hostparmS = 0xdf8d5fd6;
unsigned nametB = 0x19bb11bb;
unsigned nametS = 0x523dbb45;
unsigned debugB = 0x48ebf3d3;
unsigned debugS = 0x800d60b1;
unsigned outputB = 0x67a547c7;
unsigned outputS = 0xa5dbcd3a;
unsigned gnat__os_libB = 0x0c19a86b;
unsigned gnat__os_libS = 0x8c649cc0;
unsigned typesB = 0xe1349812;
unsigned typesS = 0xbff037c9;
unsigned system__assertionsB = 0x26242d0a;
unsigned system__assertionsS = 0xd05925f6;
unsigned tree_ioB = 0x1d0284b7;
unsigned tree_ioS = 0xeaff6db7;
unsigned widecharB = 0xc9f8b042;
unsigned widecharS = 0x0cf59cb1;
unsigned optB = 0x35a14d0f;
unsigned optS = 0x4ed6a981;
unsigned system__wch_conS = 0x3f7eead7;
unsigned system__wch_cnvB = 0xd12cd16f;
unsigned system__wch_cnvS = 0x37b1c87e;
unsigned interfacesS = 0x6e859da6;
unsigned system__wch_jisB = 0x95e9adfa;
unsigned system__wch_jisS = 0xb7a231fb;
unsigned allocS = 0x18f6088c;
unsigned tableB = 0x7873b44c;
unsigned tableS = 0xe8373431;
unsigned fnameB = 0x90a7efac;
unsigned fnameS = 0x78262251;
unsigned osintB = 0x3d7df839;
unsigned osintS = 0x049e9182;
unsigned fmapB = 0x6df0a264;
unsigned fmapS = 0x48dbaef6;
unsigned sdefaultB = 0xd442fc5e;
unsigned sdefaultS = 0x4a51fca6;
unsigned casingB = 0xbe497a11;
unsigned casingS = 0xabc1cf9d;
unsigned csetsB = 0xd14a20be;
unsigned csetsS = 0x8e8b8c48;
unsigned ridentS = 0xfc42eaa2;
unsigned ali__utilB = 0xc0a5f9ab;
unsigned ali__utilS = 0xad497e95;
unsigned binderrB = 0xc139c493;
unsigned binderrS = 0xdfb53419;
unsigned system__crc32B = 0x61928296;
unsigned system__crc32S = 0xc1336b15;
unsigned fname__sfB = 0x4bf34fdd;
unsigned fname__sfS = 0x74b68247;
unsigned fname__ufB = 0x0de659e6;
unsigned fname__ufS = 0x03b43b45;
unsigned krunchB = 0xbb2c7516;
unsigned krunchS = 0xd63ed684;
unsigned sfn_scanB = 0xe5d90a80;
unsigned sfn_scanS = 0x23460864;
unsigned gnat__directory_operationsB = 0x34a5934c;
unsigned gnat__directory_operationsS = 0xaef6d19c;
unsigned ada__charactersS = 0x060f352a;
unsigned ada__characters__handlingB = 0xf6447ab9;
unsigned ada__characters__handlingS = 0x46d230ec;
unsigned ada__characters__latin_1S = 0xa0be30a8;
unsigned ada__stringsS = 0xc4504387;
unsigned ada__strings__mapsB = 0x7f70d6fb;
unsigned ada__strings__mapsS = 0x841ddc3f;
unsigned system__bit_opsB = 0xb73aa381;
unsigned system__bit_opsS = 0x0c033dfd;
unsigned gnat__exceptionsS = 0x350327a4;
unsigned system__unsigned_typesS = 0x5ad175bd;
unsigned ada__strings__maps__constantsS = 0x6b21310c;
unsigned ada__strings__fixedB = 0xa757981e;
unsigned ada__strings__fixedS = 0xa09484f4;
unsigned ada__strings__searchB = 0x6b03b68b;
unsigned ada__strings__searchS = 0xebae4dd3;
unsigned system__string_opsB = 0xcf0ab27e;
unsigned system__string_opsS = 0x424b2b8f;
unsigned ada__finalization__list_controllerB = 0xffad3e68;
unsigned ada__finalization__list_controllerS = 0x29e5afbc;
unsigned ada__finalizationB = 0x78cccf5d;
unsigned ada__finalizationS = 0x0af0a97f;
unsigned system__finalization_rootB = 0x2707dc21;
unsigned system__finalization_rootS = 0xb1c78303;
unsigned ada__streamsS = 0x30f54674;
unsigned ada__tagsB = 0x3c7217eb;
unsigned ada__tagsS = 0x85ab19c8;
unsigned system__finalization_implementationB = 0xf489f239;
unsigned system__finalization_implementationS = 0xce47eeea;
unsigned system__string_ops_concat_3B = 0x9d5e2b8a;
unsigned system__string_ops_concat_3S = 0xa4207beb;
unsigned system__stream_attributesB = 0x951bf591;
unsigned system__stream_attributesS = 0x0a977e44;
unsigned ada__io_exceptionsS = 0x753e9209;
unsigned makeusgB = 0x6dc51c21;
unsigned makeusgS = 0x7bf24c7e;
unsigned usageB = 0x981d27fe;
unsigned usageS = 0xa56030e2;
unsigned mlibB = 0x6bf7bba0;
unsigned mlibS = 0x29c34543;
unsigned mlib__utlB = 0x40ea8763;
unsigned mlib__utlS = 0x8a1b9d37;
unsigned mlib__filB = 0xf80b606a;
unsigned mlib__filS = 0x08a615aa;
unsigned mlib__tgtB = 0xad9d588b;
unsigned mlib__tgtS = 0x545a06b7;
unsigned system__string_ops_concat_4B = 0xcfb745b6;
unsigned system__string_ops_concat_4S = 0x63b86e29;
unsigned mlib__prjB = 0xbfbe8c9d;
unsigned mlib__prjS = 0xdb69a60d;
unsigned system__img_enumB = 0x0d4a8bcc;
unsigned system__img_enumS = 0x7f1f5f41;
unsigned prjB = 0xa4a15a48;
unsigned prjS = 0x30614f44;
unsigned erroutB = 0x3eeb6355;
unsigned erroutS = 0xa6ff255e;
unsigned atreeB = 0xee07953e;
unsigned atreeS = 0x0ceff3af;
unsigned elistsB = 0xc83a6cd8;
unsigned elistsS = 0x1243f236;
unsigned nlistsB = 0x162d17cd;
unsigned nlistsS = 0xb626eba1;
unsigned sinfoB = 0x95fe125a;
unsigned sinfoS = 0x30e9bff4;
unsigned uintpB = 0xdc8c3a14;
unsigned uintpS = 0x9c203f60;
unsigned urealpB = 0xcba6eef6;
unsigned urealpS = 0xebbedd84;
unsigned sinputB = 0x9da0ae67;
unsigned sinputS = 0xc64a01d5;
unsigned einfoB = 0x7e09e612;
unsigned einfoS = 0xa9542998;
unsigned snamesB = 0x838a5113;
unsigned snamesS = 0xd9cee0f5;
unsigned standB = 0x812f9746;
unsigned standS = 0x689239fe;
unsigned libB = 0x8d484c93;
unsigned libS = 0x24d6fbe6;
unsigned stringtB = 0x6b59bc5e;
unsigned stringtS = 0xe79acc9b;
unsigned unameB = 0x0a965a2b;
unsigned unameS = 0xa72c7838;
unsigned scansB = 0x4c691951;
unsigned scansS = 0x561c6df4;
unsigned styleB = 0x80a50211;
unsigned styleS = 0x5f54cab6;
unsigned scnB = 0x636ecdd0;
unsigned scnS = 0x334433be;
unsigned styleswB = 0x9dbd5296;
unsigned styleswS = 0x445f8997;
unsigned prj__attrB = 0x4b999ef1;
unsigned prj__attrS = 0x2c94157d;
unsigned prj__comB = 0x3a130deb;
unsigned prj__comS = 0x1b6f8c89;
unsigned prj__envB = 0xcf5db17a;
unsigned prj__envS = 0xd8bbc450;
unsigned prj__utilB = 0x877b71fa;
unsigned prj__utilS = 0xfb943263;
unsigned sinfo__cnB = 0x21134b18;
unsigned sinfo__cnS = 0x9d24c8ad;
unsigned prj__extB = 0x4990049d;
unsigned prj__extS = 0x7b3f53bd;
unsigned prj__parsB = 0x5be90b88;
unsigned prj__parsS = 0xe7029f49;
unsigned prj__partB = 0x70eca6b4;
unsigned prj__partS = 0xecf1b288;
unsigned prj__dectB = 0x2e0b20ca;
unsigned prj__dectS = 0x8e627d0b;
unsigned prj__strtB = 0xf5aec78f;
unsigned prj__strtS = 0x30bd638b;
unsigned prj__treeB = 0xe4cde4ac;
unsigned prj__treeS = 0xf8e39faa;
unsigned sinput__pB = 0xdf29537e;
unsigned sinput__pS = 0x19883437;
unsigned prj__procB = 0x298935a6;
unsigned prj__procS = 0x6d0cac59;
unsigned gnat__case_utilB = 0x1e439027;
unsigned gnat__case_utilS = 0xf3d435e7;
unsigned prj__nmscB = 0xdf8cbb12;
unsigned prj__nmscS = 0x9291a837;
unsigned system__string_ops_concat_5B = 0xec185bba;
unsigned system__string_ops_concat_5S = 0x084cde6c;
unsigned sinput__lB = 0x4822d799;
unsigned sinput__lS = 0x2a6117e9;
unsigned switchB = 0xc3658e52;
unsigned switchS = 0xf71cfba2;
unsigned validswB = 0x76e052f8;
unsigned validswS = 0x7f25e66a;
unsigned system__memoryB = 0xeac08eae;
unsigned system__memoryS = 0x528e943f;

/* BEGIN ELABORATION ORDER
ada (spec)
ada.characters (spec)
ada.characters.handling (spec)
ada.characters.latin_1 (spec)
ada.command_line (spec)
gnat (spec)
gnat.case_util (spec)
gnat.case_util (body)
gnat.exceptions (spec)
gnat.heap_sort_a (spec)
gnat.heap_sort_a (body)
gnat.htable (spec)
gnat.htable (body)
interfaces (spec)
system (spec)
system.bit_ops (spec)
system.crc32 (spec)
system.crc32 (body)
system.img_enum (spec)
system.parameters (spec)
system.parameters (body)
system.standard_library (spec)
ada.exceptions (spec)
system.exception_table (spec)
system.exception_table (body)
ada.io_exceptions (spec)
ada.strings (spec)
system.assertions (spec)
system.assertions (body)
system.exceptions (spec)
system.storage_elements (spec)
system.storage_elements (body)
system.machine_state_operations (spec)
system.machine_state_operations (body)
system.secondary_stack (spec)
system.img_enum (body)
ada.command_line (body)
ada.tags (spec)
ada.tags (body)
ada.streams (spec)
system.stack_checking (spec)
system.soft_links (spec)
system.soft_links (body)
system.stack_checking (body)
system.secondary_stack (body)
gnat.os_lib (spec)
gnat.os_lib (body)
system.finalization_root (spec)
system.finalization_root (body)
system.memory (spec)
system.memory (body)
system.standard_library (body)
system.string_ops (spec)
system.string_ops (body)
system.string_ops_concat_3 (spec)
system.string_ops_concat_3 (body)
system.string_ops_concat_4 (spec)
system.string_ops_concat_4 (body)
system.string_ops_concat_5 (spec)
system.string_ops_concat_5 (body)
system.traceback (spec)
system.traceback (body)
ada.exceptions (body)
system.unsigned_types (spec)
system.bit_ops (body)
ada.strings.maps (spec)
ada.strings.maps (body)
ada.strings.fixed (spec)
ada.strings.maps.constants (spec)
ada.characters.handling (body)
ada.strings.search (spec)
ada.strings.search (body)
ada.strings.fixed (body)
system.stream_attributes (spec)
system.stream_attributes (body)
system.finalization_implementation (spec)
system.finalization_implementation (body)
ada.finalization (spec)
ada.finalization (body)
ada.finalization.list_controller (spec)
ada.finalization.list_controller (body)
gnat.directory_operations (spec)
gnat.directory_operations (body)
system.wch_con (spec)
system.wch_cnv (spec)
system.wch_jis (spec)
system.wch_jis (body)
system.wch_cnv (body)
alloc (spec)
debug (spec)
debug (body)
gnatmake (spec)
gnatvsn (spec)
hostparm (spec)
krunch (spec)
krunch (body)
makeusg (spec)
mlib (spec)
mlib.fil (spec)
mlib.utl (spec)
rident (spec)
sfn_scan (spec)
sfn_scan (body)
switch (spec)
types (spec)
types (body)
binderr (spec)
butil (spec)
casing (spec)
elists (spec)
fmap (spec)
fname (spec)
fname.sf (spec)
fname.uf (spec)
mlib.tgt (spec)
mlib.tgt (body)
mlib.fil (body)
nlists (spec)
opt (spec)
csets (spec)
csets (body)
osint (spec)
output (spec)
output (body)
mlib (body)
scans (spec)
scans (body)
scn (spec)
sdefault (spec)
sdefault (body)
snames (spec)
stringt (spec)
style (spec)
stylesw (spec)
stylesw (body)
tree_io (spec)
tree_io (body)
opt (body)
table (spec)
table (body)
elists (body)
ali (spec)
ali.util (spec)
lib (spec)
make (spec)
gnatmake (body)
namet (spec)
ali.util (body)
ali (body)
stringt (body)
snames (body)
osint (body)
fname.sf (body)
fname (body)
fmap (body)
butil (body)
binderr (body)
mlib.utl (body)
prj (spec)
mlib.prj (spec)
mlib.prj (body)
prj.attr (spec)
prj.attr (body)
prj.com (spec)
prj.com (body)
prj.env (spec)
prj.ext (spec)
prj.ext (body)
prj.nmsc (spec)
prj.pars (spec)
prj.tree (spec)
prj.tree (body)
prj.dect (spec)
prj.part (spec)
prj.proc (spec)
prj.strt (spec)
prj.util (spec)
prj.util (body)
prj.env (body)
sinput (spec)
sinput (body)
sinput.l (spec)
sinput.p (spec)
sinput.p (body)
uintp (spec)
uintp (body)
errout (spec)
prj.proc (body)
prj.pars (body)
prj.nmsc (body)
uname (spec)
urealp (spec)
urealp (body)
einfo (spec)
sinfo (spec)
prj.strt (body)
prj.part (body)
prj.dect (body)
atree (spec)
atree (body)
sinfo (body)
uname (body)
sinput.l (body)
nlists (body)
sinfo.cn (spec)
sinfo.cn (body)
prj (body)
usage (spec)
usage (body)
makeusg (body)
validsw (spec)
validsw (body)
switch (body)
widechar (spec)
widechar (body)
namet (body)
scn (body)
fname.uf (body)
casing (body)
stand (spec)
stand (body)
einfo (body)
errout (body)
lib (body)
style (body)
make (body)
   END ELABORATION ORDER */

/* BEGIN Object file/option list
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/ada.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-charac.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-chlat1.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/gnat.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/g-casuti.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/g-except.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/g-hesora.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/g-htable.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/interfac.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/system.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-crc32.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-parame.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-exctab.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-ioexce.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-string.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-assert.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-except.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-stoele.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-mastop.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-imgenu.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-comlin.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-tags.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-stream.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-soflin.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-stache.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-secsta.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/g-os_lib.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-finroo.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-memory.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-stalib.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-strops.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-sopco3.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-sopco4.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-sopco5.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-traceb.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-except.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-unstyp.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-bitops.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-strmap.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-stmaco.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-chahan.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-strsea.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-strfix.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-stratt.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-finimp.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-finali.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-filico.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/g-dirope.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-wchcon.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-wchjis.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-wchcnv.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/alloc.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/debug.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/gnatvsn.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/hostparm.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/krunch.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/rident.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/sfn_scan.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/types.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/mlib-tgt.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/mlib-fil.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/csets.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/output.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/mlib.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/scans.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/sdefault.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/stylesw.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/tree_io.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/opt.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/table.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/elists.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/gnatmake.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/ali-util.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/ali.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/stringt.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/snames.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/osint.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/fname-sf.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/fname.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/fmap.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/butil.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/binderr.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/mlib-utl.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/mlib-prj.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/prj-attr.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/prj-com.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/prj-ext.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/prj-tree.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/prj-util.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/prj-env.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/sinput.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/sinput-p.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/uintp.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/prj-proc.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/prj-pars.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/prj-nmsc.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/urealp.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/prj-strt.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/prj-part.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/prj-dect.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/atree.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/sinfo.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/uname.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/sinput-l.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/nlists.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/sinfo-cn.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/prj.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/usage.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/makeusg.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/validsw.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/switch.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/widechar.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/namet.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/scn.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/fname-uf.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/casing.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/stand.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/einfo.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/errout.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/lib.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/style.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/make.o
-L/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/
-L/usr/local/lib/gcc-lib/i686-pc-linux-gnu/3.1/adalib/
-static
-lgnat
   END Object file/option list */
