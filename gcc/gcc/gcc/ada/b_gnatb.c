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
extern void _ada_gnatbind PARAMS ((void));
extern void __gnat_initialize PARAMS ((void));
extern void __gnat_finalize PARAMS ((void));
extern void __gnat_install_handler PARAMS ((void));

extern void ada__exceptions___elabs PARAMS ((void));
extern void system__exception_table___elabb PARAMS ((void));
extern void system__assertions___elabs PARAMS ((void));
extern void system__exceptions___elabs PARAMS ((void));
extern void system__soft_links___elabb PARAMS ((void));
extern void system__secondary_stack___elabb PARAMS ((void));
extern void ada__exceptions___elabb PARAMS ((void));
extern void types___elabs PARAMS ((void));
extern void opt___elabs PARAMS ((void));
extern void csets___elabb PARAMS ((void));
extern void osint___elabs PARAMS ((void));
extern void tree_io___elabs PARAMS ((void));
extern void ali___elabs PARAMS ((void));
extern void ali__util___elabs PARAMS ((void));
extern void binde___elabs PARAMS ((void));
extern void namet___elabs PARAMS ((void));
extern void binde___elabb PARAMS ((void));
extern void osint___elabb PARAMS ((void));
extern void fname___elabb PARAMS ((void));
extern void fmap___elabb PARAMS ((void));
extern void switch___elabb PARAMS ((void));

extern int  __gnat_handler_installed;

extern int gnat_argc;
extern char **gnat_argv;
extern char **gnat_envp;
extern int gnat_exit_status;


char __gnat_version[] = "GNAT Version: 5.00w (20010924)";
char __gnat_ada_main_program_name[] = "_ada_gnatbind";
void adafinal () {
   system__standard_library__adafinal ();
}

void adainit ()
{
   extern char ada__exceptions_E;
   extern char system__exception_table_E;
   extern char system__assertions_E;
   extern char system__exceptions_E;
   extern char system__secondary_stack_E;
   extern char system__soft_links_E;
   extern char bcheck_E;
   extern char bindgen_E;
   extern char bindusg_E;
   extern char gnatbind_E;
   extern char switch_E;
   extern char types_E;
   extern char binderr_E;
   extern char butil_E;
   extern char casing_E;
   extern char fmap_E;
   extern char fname_E;
   extern char opt_E;
   extern char csets_E;
   extern char osint_E;
   extern char sdefault_E;
   extern char stylesw_E;
   extern char tree_io_E;
   extern char ali_E;
   extern char ali__util_E;
   extern char binde_E;
   extern char namet_E;
   extern char validsw_E;
   extern char widechar_E;

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
   if (system__assertions_E == 0) {
      system__assertions___elabs ();
   }
   system__assertions_E = 1;
   if (system__exceptions_E == 0) {
      system__exceptions___elabs ();
      system__exceptions_E++;
   }
   if (system__soft_links_E == 0) {
      system__soft_links___elabb ();
      system__soft_links_E++;
   }
   if (system__secondary_stack_E == 0) {
      system__secondary_stack___elabb ();
      system__secondary_stack_E++;
   }
   if (ada__exceptions_E == 0) {
      ada__exceptions___elabb ();
      ada__exceptions_E++;
   }
   if (types_E == 0) {
      types___elabs ();
   }
   types_E = 1;
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
   bindusg_E = 1;
   sdefault_E = 1;
   stylesw_E = 1;
   if (tree_io_E == 0) {
      tree_io___elabs ();
   }
   tree_io_E = 1;
   opt_E = 1;
   if (ali_E == 0) {
      ali___elabs ();
   }
   if (ali__util_E == 0) {
      ali__util___elabs ();
   }
   if (binde_E == 0) {
      binde___elabs ();
   }
   if (namet_E == 0) {
      namet___elabs ();
   }
   if (binde_E == 0) {
      binde___elabb ();
      binde_E++;
   }
   ali__util_E = 1;
   ali_E = 1;
   if (osint_E == 0) {
      osint___elabb ();
      osint_E++;
   }
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
   gnatbind_E = 1;
   bindgen_E = 1;
   bcheck_E = 1;
   validsw_E = 1;
   if (switch_E == 0) {
      switch___elabb ();
      switch_E++;
   }
   widechar_E = 1;
   namet_E = 1;
   casing_E = 1;
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
 
   _ada_gnatbind ();
 
   system__standard_library__adafinal ();
   __gnat_finalize ();
   exit (gnat_exit_status);
}
unsigned gnatbindB = 0xa96f092c;
unsigned gnatbindS = 0xf9afd76c;
unsigned system__standard_libraryB = 0xc0e50ac3;
unsigned system__standard_libraryS = 0x6d0b25e7;
unsigned ada__exceptionsB = 0x98bed957;
unsigned ada__exceptionsS = 0x20d65858;
unsigned adaS = 0xa0108083;
unsigned gnatS = 0x6380ea48;
unsigned gnat__heap_sort_aB = 0xc5fbb2c4;
unsigned gnat__heap_sort_aS = 0xdc301c49;
unsigned systemS = 0x5f7c428f;
unsigned system__exception_tableB = 0x58c090c8;
unsigned system__exception_tableS = 0x7bff8c6d;
unsigned gnat__htableB = 0x2c01a59d;
unsigned gnat__htableS = 0xeb251ef2;
unsigned system__exceptionsS = 0xbbfe2e15;
unsigned system__machine_state_operationsB = 0xafcd5e0c;
unsigned system__machine_state_operationsS = 0x65adb8f1;
unsigned system__storage_elementsB = 0xa4e49d90;
unsigned system__storage_elementsS = 0x99c2b222;
unsigned system__secondary_stackB = 0xdedafdd0;
unsigned system__secondary_stackS = 0x7a466d01;
unsigned system__parametersB = 0x38b39edd;
unsigned system__parametersS = 0xb77e93cc;
unsigned system__soft_linksB = 0xfd0fec16;
unsigned system__soft_linksS = 0xccb8d065;
unsigned system__stack_checkingB = 0x31fdd957;
unsigned system__stack_checkingS = 0x16073c88;
unsigned system__tracebackB = 0x4d4e8ee2;
unsigned system__tracebackS = 0x69317710;
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
unsigned gnatvsnS = 0x575b423a;
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
unsigned bcheckB = 0xcd86f832;
unsigned bcheckS = 0x6a87a1a1;
unsigned system__img_enumB = 0x0d4a8bcc;
unsigned system__img_enumS = 0x7f1f5f41;
unsigned bindeB = 0xeb2080f4;
unsigned bindeS = 0x64c05f93;
unsigned bindgenB = 0x5967207c;
unsigned bindgenS = 0x73bfafcd;
unsigned system__string_opsB = 0xcf0ab27e;
unsigned system__string_opsS = 0x424b2b8f;
unsigned system__string_ops_concat_3B = 0x9d5e2b8a;
unsigned system__string_ops_concat_3S = 0xa4207beb;
unsigned system__string_ops_concat_4B = 0xcfb745b6;
unsigned system__string_ops_concat_4S = 0x63b86e29;
unsigned system__string_ops_concat_5B = 0xec185bba;
unsigned system__string_ops_concat_5S = 0x084cde6c;
unsigned bindusgB = 0xbddb9bec;
unsigned bindusgS = 0xfc90a8b6;
unsigned switchB = 0xc3658e52;
unsigned switchS = 0xf71cfba2;
unsigned styleswB = 0x9dbd5296;
unsigned styleswS = 0x445f8997;
unsigned validswB = 0x76e052f8;
unsigned validswS = 0x7f25e66a;
unsigned system__memoryB = 0xeac08eae;
unsigned system__memoryS = 0x528e943f;

/* BEGIN ELABORATION ORDER
ada (spec)
gnat (spec)
gnat.heap_sort_a (spec)
gnat.heap_sort_a (body)
gnat.htable (spec)
gnat.htable (body)
interfaces (spec)
system (spec)
system.crc32 (spec)
system.crc32 (body)
system.img_enum (spec)
system.parameters (spec)
system.parameters (body)
system.standard_library (spec)
ada.exceptions (spec)
system.exception_table (spec)
system.exception_table (body)
system.assertions (spec)
system.assertions (body)
system.exceptions (spec)
system.storage_elements (spec)
system.storage_elements (body)
system.machine_state_operations (spec)
system.machine_state_operations (body)
system.secondary_stack (spec)
system.img_enum (body)
system.stack_checking (spec)
system.soft_links (spec)
system.soft_links (body)
system.stack_checking (body)
system.secondary_stack (body)
gnat.os_lib (spec)
gnat.os_lib (body)
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
system.wch_con (spec)
system.wch_cnv (spec)
system.wch_jis (spec)
system.wch_jis (body)
system.wch_cnv (body)
alloc (spec)
bcheck (spec)
bindgen (spec)
bindusg (spec)
debug (spec)
debug (body)
gnatbind (spec)
gnatvsn (spec)
hostparm (spec)
rident (spec)
switch (spec)
types (spec)
types (body)
binderr (spec)
butil (spec)
casing (spec)
fmap (spec)
fname (spec)
opt (spec)
csets (spec)
csets (body)
osint (spec)
output (spec)
output (body)
bindusg (body)
sdefault (spec)
sdefault (body)
stylesw (spec)
stylesw (body)
tree_io (spec)
tree_io (body)
opt (body)
table (spec)
table (body)
ali (spec)
ali.util (spec)
binde (spec)
namet (spec)
binde (body)
ali.util (body)
ali (body)
osint (body)
fname (body)
fmap (body)
butil (body)
binderr (body)
gnatbind (body)
bindgen (body)
bcheck (body)
validsw (spec)
validsw (body)
switch (body)
widechar (spec)
widechar (body)
namet (body)
casing (body)
   END ELABORATION ORDER */

/* BEGIN Object file/option list
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/ada.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/gnat.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/g-hesora.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/g-htable.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/interfac.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/system.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-crc32.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-parame.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-exctab.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-assert.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-except.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-stoele.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-mastop.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-imgenu.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-soflin.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-stache.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-secsta.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/g-os_lib.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-memory.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-stalib.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-strops.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-sopco3.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-sopco4.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-sopco5.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-traceb.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-except.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-wchcon.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-wchjis.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-wchcnv.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/alloc.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/debug.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/gnatvsn.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/hostparm.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/rident.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/types.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/csets.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/output.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/bindusg.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/sdefault.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/stylesw.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/tree_io.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/opt.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/table.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/binde.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/ali-util.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/ali.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/osint.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/fname.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/fmap.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/butil.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/binderr.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/gnatbind.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/bindgen.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/bcheck.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/validsw.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/switch.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/widechar.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/namet.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/casing.o
-L/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/
-L/usr/local/lib/gcc-lib/i686-pc-linux-gnu/3.1/adalib/
-static
-lgnat
   END Object file/option list */
