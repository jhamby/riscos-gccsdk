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
extern void _ada_gnatpsys PARAMS ((void));
extern void __gnat_initialize PARAMS ((void));
extern void __gnat_finalize PARAMS ((void));
extern void __gnat_install_handler PARAMS ((void));

extern void interfaces__c_streams___elabs PARAMS ((void));
extern void ada__exceptions___elabs PARAMS ((void));
extern void system__exception_table___elabb PARAMS ((void));
extern void ada__io_exceptions___elabs PARAMS ((void));
extern void system__assertions___elabs PARAMS ((void));
extern void system__exceptions___elabs PARAMS ((void));
extern void ada__tags___elabs PARAMS ((void));
extern void ada__tags___elabb PARAMS ((void));
extern void ada__streams___elabs PARAMS ((void));
extern void system__soft_links___elabb PARAMS ((void));
extern void system__secondary_stack___elabb PARAMS ((void));
extern void system__finalization_root___elabs PARAMS ((void));
extern void ada__exceptions___elabb PARAMS ((void));
extern void system__finalization_implementation___elabs PARAMS ((void));
extern void system__finalization_implementation___elabb PARAMS ((void));
extern void ada__finalization___elabs PARAMS ((void));
extern void ada__finalization__list_controller___elabs PARAMS ((void));
extern void system__file_control_block___elabs PARAMS ((void));
extern void system__file_io___elabb PARAMS ((void));
extern void ada__text_io___elabs PARAMS ((void));
extern void ada__text_io___elabb PARAMS ((void));

extern int  __gnat_handler_installed;

extern int gnat_argc;
extern char **gnat_argv;
extern char **gnat_envp;
extern int gnat_exit_status;


char __gnat_version[] = "GNAT Version: 5.00w (20010924)";
char __gnat_ada_main_program_name[] = "_ada_gnatpsys";
void adafinal () {
   system__standard_library__adafinal ();
}

void adainit ()
{
   extern char interfaces__c_streams_E;
   extern char ada__exceptions_E;
   extern char system__exception_table_E;
   extern char ada__io_exceptions_E;
   extern char system__assertions_E;
   extern char system__exceptions_E;
   extern char system__secondary_stack_E;
   extern char ada__tags_E;
   extern char ada__streams_E;
   extern char system__soft_links_E;
   extern char system__finalization_root_E;
   extern char system__finalization_implementation_E;
   extern char ada__finalization_E;
   extern char ada__finalization__list_controller_E;
   extern char system__file_control_block_E;
   extern char system__file_io_E;
   extern char ada__text_io_E;

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

   if (interfaces__c_streams_E == 0) {
      interfaces__c_streams___elabs ();
   }
   interfaces__c_streams_E = 1;
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
   if (system__file_control_block_E == 0) {
      system__file_control_block___elabs ();
      system__file_control_block_E++;
   }
   if (system__file_io_E == 0) {
      system__file_io___elabb ();
      system__file_io_E++;
   }
   if (ada__text_io_E == 0) {
      ada__text_io___elabs ();
   }
   if (ada__text_io_E == 0) {
      ada__text_io___elabb ();
      ada__text_io_E++;
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
 
   _ada_gnatpsys ();
 
   system__standard_library__adafinal ();
   __gnat_finalize ();
   exit (gnat_exit_status);
}
unsigned gnatpsysB = 0xa95a6995;
unsigned system__standard_libraryB = 0xc0e50ac3;
unsigned system__standard_libraryS = 0x6d0b25e7;
unsigned adaS = 0xa0108083;
unsigned ada__exceptionsB = 0x98bed957;
unsigned ada__exceptionsS = 0x20d65858;
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
unsigned ada__text_ioB = 0x53b374d1;
unsigned ada__text_ioS = 0x3631d409;
unsigned ada__streamsS = 0x30f54674;
unsigned ada__tagsB = 0x3c7217eb;
unsigned ada__tagsS = 0x85ab19c8;
unsigned interfacesS = 0x6e859da6;
unsigned interfaces__c_streamsB = 0xcf1b6eec;
unsigned interfaces__c_streamsS = 0x9dcf6698;
unsigned system__file_ioB = 0x5a728517;
unsigned system__file_ioS = 0x5951418b;
unsigned ada__finalizationB = 0x78cccf5d;
unsigned ada__finalizationS = 0x0af0a97f;
unsigned system__finalization_rootB = 0x2707dc21;
unsigned system__finalization_rootS = 0xb1c78303;
unsigned system__finalization_implementationB = 0xf489f239;
unsigned system__finalization_implementationS = 0xce47eeea;
unsigned system__string_ops_concat_3B = 0x9d5e2b8a;
unsigned system__string_ops_concat_3S = 0xa4207beb;
unsigned system__string_opsB = 0xcf0ab27e;
unsigned system__string_opsS = 0x424b2b8f;
unsigned system__stream_attributesB = 0x951bf591;
unsigned system__stream_attributesS = 0x0a977e44;
unsigned ada__io_exceptionsS = 0x753e9209;
unsigned system__unsigned_typesS = 0x5ad175bd;
unsigned system__file_control_blockS = 0xdbeadbc4;
unsigned ada__finalization__list_controllerB = 0xffad3e68;
unsigned ada__finalization__list_controllerS = 0x29e5afbc;
unsigned gnatvsnS = 0x575b423a;
unsigned system__img_enumB = 0x0d4a8bcc;
unsigned system__img_enumS = 0x7f1f5f41;
unsigned system__img_intB = 0x70f6a786;
unsigned system__img_intS = 0xaec6c6ec;
unsigned system__img_lliB = 0xf02ff885;
unsigned system__img_lliS = 0x06da9b9e;
unsigned system__img_realB = 0x7278d14c;
unsigned system__img_realS = 0x98e8d194;
unsigned system__fat_llfS = 0x780dc031;
unsigned system__assertionsB = 0x26242d0a;
unsigned system__assertionsS = 0xd05925f6;
unsigned system__img_lluB = 0xf2d52f0b;
unsigned system__img_lluS = 0x929bbaab;
unsigned system__img_unsB = 0x675bc9eb;
unsigned system__img_unsS = 0xbfe21627;
unsigned system__powten_tableS = 0xdbc27ad6;
unsigned system__string_ops_concat_5B = 0xec185bba;
unsigned system__string_ops_concat_5S = 0x084cde6c;
unsigned system__string_ops_concat_4B = 0xcfb745b6;
unsigned system__string_ops_concat_4S = 0x63b86e29;
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
system.img_enum (spec)
system.img_int (spec)
system.img_lli (spec)
system.img_real (spec)
system.parameters (spec)
system.parameters (body)
interfaces.c_streams (spec)
interfaces.c_streams (body)
system.powten_table (spec)
system.standard_library (spec)
ada.exceptions (spec)
system.exception_table (spec)
system.exception_table (body)
ada.io_exceptions (spec)
system.assertions (spec)
system.assertions (body)
system.exceptions (spec)
system.storage_elements (spec)
system.storage_elements (body)
system.machine_state_operations (spec)
system.machine_state_operations (body)
system.secondary_stack (spec)
system.img_lli (body)
system.img_int (body)
system.img_enum (body)
ada.tags (spec)
ada.tags (body)
ada.streams (spec)
system.stack_checking (spec)
system.soft_links (spec)
system.soft_links (body)
system.stack_checking (body)
system.secondary_stack (body)
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
system.fat_llf (spec)
system.img_llu (spec)
system.img_llu (body)
system.img_uns (spec)
system.img_uns (body)
system.img_real (body)
system.stream_attributes (spec)
system.stream_attributes (body)
system.finalization_implementation (spec)
system.finalization_implementation (body)
ada.finalization (spec)
ada.finalization (body)
ada.finalization.list_controller (spec)
ada.finalization.list_controller (body)
system.file_control_block (spec)
system.file_io (spec)
system.file_io (body)
ada.text_io (spec)
ada.text_io (body)
gnatvsn (spec)
gnatpsys (body)
   END ELABORATION ORDER */

/* BEGIN Object file/option list
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/ada.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/gnat.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/g-hesora.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/g-htable.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/interfac.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/system.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-parame.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/i-cstrea.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-powtab.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-exctab.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-ioexce.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-assert.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-except.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-stoele.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-mastop.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-imglli.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-imgint.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-imgenu.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-tags.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-stream.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-soflin.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-stache.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-secsta.o
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
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-fatllf.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-imgllu.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-imguns.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-imgrea.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-stratt.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-finimp.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-finali.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-filico.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-ficobl.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/s-fileio.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/a-textio.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/gnatvsn.o
/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/gnatpsys.o
-L/home/nick/gccsdk-3.0/arm-riscos-aof/gcc-3_1_0/gcc/stage1/ada/
-L/usr/local/lib/gcc-lib/i686-pc-linux-gnu/3.1/adalib/
-static
-lgnat
   END Object file/option list */
