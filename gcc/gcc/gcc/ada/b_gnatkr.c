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
extern void _ada_gnatkr PARAMS ((void));
extern void __gnat_initialize PARAMS ((void));
extern void __gnat_finalize PARAMS ((void));
extern void __gnat_install_handler PARAMS ((void));

extern void ada__exceptions___elabs PARAMS ((void));
extern void system__exception_table___elabb PARAMS ((void));
extern void ada__strings___elabs PARAMS ((void));
extern void system__exceptions___elabs PARAMS ((void));
extern void system__soft_links___elabb PARAMS ((void));
extern void system__secondary_stack___elabb PARAMS ((void));
extern void ada__exceptions___elabb PARAMS ((void));
extern void ada__strings__maps___elabs PARAMS ((void));
extern void ada__strings__maps__constants___elabs PARAMS ((void));

extern int  __gnat_handler_installed;

extern int gnat_argc;
extern char **gnat_argv;
extern char **gnat_envp;
extern int gnat_exit_status;


char __gnat_version[] = "GNAT Version: 5.00w (20010924)";
char __gnat_ada_main_program_name[] = "_ada_gnatkr";
void adafinal () {
   system__standard_library__adafinal ();
}

void adainit ()
{
   extern char ada__exceptions_E;
   extern char system__exception_table_E;
   extern char ada__strings_E;
   extern char system__exceptions_E;
   extern char system__secondary_stack_E;
   extern char system__soft_links_E;
   extern char ada__strings__maps_E;
   extern char ada__strings__maps__constants_E;
   extern char gnatkr_E;

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
   if (ada__strings_E == 0) {
      ada__strings___elabs ();
      ada__strings_E++;
   }
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
   if (ada__strings__maps_E == 0) {
      ada__strings__maps___elabs ();
   }
   ada__strings__maps_E = 1;
   if (ada__strings__maps__constants_E == 0) {
      ada__strings__maps__constants___elabs ();
      ada__strings__maps__constants_E++;
   }
   gnatkr_E = 1;
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
 
   _ada_gnatkr ();
 
   system__standard_library__adafinal ();
   __gnat_finalize ();
   exit (gnat_exit_status);
}
unsigned gnatkrB = 0x7d32fb7a;
unsigned gnatkrS = 0xd92ab7d5;
unsigned system__standard_libraryB = 0xc0e50ac3;
unsigned system__standard_libraryS = 0x6d0b25e7;
unsigned adaS = 0xa0108083;
unsigned ada__charactersS = 0x060f352a;
unsigned ada__characters__handlingB = 0xf6447ab9;
unsigned ada__characters__handlingS = 0x46d230ec;
unsigned ada__characters__latin_1S = 0xa0be30a8;
unsigned ada__stringsS = 0xc4504387;
unsigned systemS = 0x5f7c428f;
unsigned system__exception_tableB = 0x58c090c8;
unsigned system__exception_tableS = 0x7bff8c6d;
unsigned gnatS = 0x6380ea48;
unsigned gnat__htableB = 0x2c01a59d;
unsigned gnat__htableS = 0xeb251ef2;
unsigned ada__strings__mapsB = 0x7f70d6fb;
unsigned ada__strings__mapsS = 0x841ddc3f;
unsigned ada__exceptionsB = 0x98bed957;
unsigned ada__exceptionsS = 0x20d65858;
unsigned gnat__heap_sort_aB = 0xc5fbb2c4;
unsigned gnat__heap_sort_aS = 0xdc301c49;
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
unsigned system__bit_opsB = 0xb73aa381;
unsigned system__bit_opsS = 0x0c033dfd;
unsigned gnat__exceptionsS = 0x350327a4;
unsigned system__unsigned_typesS = 0x5ad175bd;
unsigned ada__strings__maps__constantsS = 0x6b21310c;
unsigned ada__command_lineB = 0x25dfd6a9;
unsigned ada__command_lineS = 0xd8088025;
unsigned gnatvsnS = 0x575b423a;
unsigned krunchB = 0xbb2c7516;
unsigned krunchS = 0xd63ed684;
unsigned hostparmS = 0xdf8d5fd6;
unsigned system__ioB = 0x779b0745;
unsigned system__ioS = 0x6e59e467;
unsigned system__memoryB = 0xeac08eae;
unsigned system__memoryS = 0x528e943f;

/* BEGIN ELABORATION ORDER
ada (spec)
ada.characters (spec)
ada.characters.handling (spec)
ada.characters.latin_1 (spec)
ada.command_line (spec)
gnat (spec)
gnat.exceptions (spec)
gnat.heap_sort_a (spec)
gnat.heap_sort_a (body)
gnat.htable (spec)
gnat.htable (body)
system (spec)
system.bit_ops (spec)
system.io (spec)
system.io (body)
system.parameters (spec)
system.parameters (body)
system.standard_library (spec)
ada.exceptions (spec)
system.exception_table (spec)
system.exception_table (body)
ada.strings (spec)
system.exceptions (spec)
system.storage_elements (spec)
system.storage_elements (body)
system.machine_state_operations (spec)
system.machine_state_operations (body)
system.secondary_stack (spec)
ada.command_line (body)
system.stack_checking (spec)
system.soft_links (spec)
system.soft_links (body)
system.stack_checking (body)
system.secondary_stack (body)
system.memory (spec)
system.memory (body)
system.standard_library (body)
system.traceback (spec)
system.traceback (body)
ada.exceptions (body)
system.unsigned_types (spec)
system.bit_ops (body)
ada.strings.maps (spec)
ada.strings.maps (body)
ada.strings.maps.constants (spec)
ada.characters.handling (body)
gnatkr (spec)
gnatvsn (spec)
hostparm (spec)
krunch (spec)
krunch (body)
gnatkr (body)
   END ELABORATION ORDER */

/* BEGIN Object file/option list
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/ada.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-charac.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-chlat1.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/gnat.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/g-except.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/g-hesora.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/g-htable.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/system.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-io.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-parame.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-exctab.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-string.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-except.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-stoele.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-mastop.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-comlin.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-soflin.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-stache.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-secsta.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-memory.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-stalib.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-traceb.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-except.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-unstyp.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-bitops.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-strmap.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-stmaco.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-chahan.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/gnatvsn.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/hostparm.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/krunch.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/gnatkr.o
-L/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/
-L/usr/local/lib/gcc-lib/i686-pc-linux-gnu/3.1/adalib/
-static
-lgnat
   END Object file/option list */
