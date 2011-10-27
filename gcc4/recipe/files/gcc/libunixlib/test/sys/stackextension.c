#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ul_memory
{
  /* Serialise access to this structure (opaque type).  */
  void *mutex;

  /* This variable sets the value of what is considered to be the
     maximum value of addressable memory within application space.
     This value can increase during program execution if there is a
     request for more memory.

     The value of 'appspace_himem' will always be less-than or equal-to
     the value of 'appspace_limit'.

     Note that when dynamic areas are in use, the only function that will
     try to increase 'appspace_himem' and 'appspace_limit' is the stack
     extension code.  */
  unsigned int appspace_himem;

  /* Current lowest position of the stack.  It lowers in memory address
     until it reaches 'stack_limit'.  Once reached, attempts to
     increase stack size will happen by increasing the wimpslot.  */
  unsigned int stack;

  /* This points to the start of application memory, usually 0x8000.
     Defined as Image$$RO$$Base for AOF.
     Defined as __executable_start for ELF.  */
  const unsigned int robase;

  /* This points to the end of the executable, including any read/write
     data that has been initialised before program start.

     It is called 'rwlomem' because it refers to the lowest memory address
     that is usable for run-time allocation of memory.

     Defined as Image$$RW$$Limit for AOF.
     Defined as __end__ for ELF.  */
  const unsigned int rwlomem;

  /* This points to the start of read/write data.
     Defined as Image$$RW$$Base for AOF.
     Defined as __data_start for ELF.  */
  const unsigned int rwbase;

  /* As data is requested from the system by 'brk' to satisfy allocation
     and deallocation requests and we're not using a dynamic area to store
     our heap, the value of 'rwbreak' will change.

     Initially it starts off equal to 'rwlomem', meaning that no data
     has been allocated at run-time.

     When a request is made for more space, then 'rwbreak' will increase.  */
  unsigned int rwbreak;

  /* This variable points to the lowest extent to which the stack can
     grow.  Its value increases in line with 'rwbreak' as more memory
     is taken from the system.  */
  unsigned int stack_limit;

  /* Holds the base memory address of the dynamic area.  It is used to
     ensure that any requests by 'brk' or 'sbrk' to change the data segment
     size, cannot reduce it past this value.

     This variable is used only when dynamic areas are in use.  */
  unsigned int dalomem;

  /* This variable is used only when dynamic areas are in use.  It records
     the used space in a dynamic area, which will increase or decrease
     as required according to the user's calls to 'brk'.  */
  unsigned int dabreak;

  /* This variable is used only when dynamic areas are in use.  It records
     the upper limit of the dynamic area and will increase/decrease as
     we claim or release memory.  */
  unsigned int dalimit;

  /* This variable is used by the non-contiguous stack chunk code.
     It records the upper limit of application space (or wimpslot)
     and will increase as more is required in multiples of 32K.  In such
     circumstances, this means that 'appspace_limit' will be larger than
     'appspace_himem'.  */
  unsigned int appspace_limit;

  /* Value of 'appspace_himem' last time the stack was increased.  */
  unsigned int old_himem;
};

extern struct ul_memory __ul_memory;

static unsigned int sigstack;

int recurse(int x)
{
  char y[5 * 1024];
  register unsigned int sl __asm__("sl");

  /* Ensure that stack extension doesn't trample the signal handler stack */
  if (sigstack <= sl && sl <= sigstack + 4096) {
    printf("ERROR: sl found in sigstack (%08x <= %08x <= %08x)\n", 
           sigstack, sl, sigstack + 4096);
    exit(1);
  }

  if (x == 0)
    return x;

  /* Pointless, but ensures we don't optimise y out */
  memset(y, x, sizeof(y));

  /* Cunningly not tail-recursive, to ensure stack grows */
  return x + recurse(x - 1);
}

int main(void)
{
  /* Signal handler stack is a 4k chunk located 8 bytes below himem */
  sigstack = __ul_memory.old_himem - 8 - 4096;

  recurse(500);

  printf("SUCCESS\n");

  return 0;
}
