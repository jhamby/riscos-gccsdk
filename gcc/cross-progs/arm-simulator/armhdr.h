/* -> $.ARMemu.h.ARMhdr

   ARM emulator HEADER file

   (C) Edwin Dorre 1990
 */

/*****************************************************************************
 Due to system specific features the following macros must be defined:

 WORD: must be a 32 bits unsigned number.
 BYTE: a 8 bits unsigned number.

 ESCAPE_KEY: the ascii value of the key which produces the interrupt signal.

 HUGE: if the system needs a special kind of pointer for a large block
       of memory.

 LITTLE_ENDIAN or BIG_ENDIAN: depending on the byte order in a word.
   One of the symbols must be defined, to determine the byte in word
   order of your machine. If it is a little endian machine like IBM PC or
   a DIGITAL VAX/VMS (least significant part of word on first byte) or big
   endian like the IBM-RS6000 (least significant part of word on last byte).

******************************************************************************/

/* One of the symbols below must be defined! */

#ifndef P_ARCHIMEDES
#ifndef P_RS6000
#ifndef P_VAX
#ifndef P_PC
#ifndef P_LINUX
"ERROR: Please define one of the above symbols!"
#endif
#endif
#endif
#endif
#endif

#ifdef P_ARCHIMEDES
#define ESCAPE_KEY 27
#define LITTLE_ENDIAN
#define HUGE
#define WORD unsigned int
#define BYTE unsigned char
#endif

#ifdef P_RS6000
#define ESCAPE_KEY 3
#define BIG_ENDIAN
#define HUGE
#define WORD unsigned int
#define BYTE unsigned char
#endif

#ifdef P_VAX
#define ESCAPE_KEY 3
#define LITTLE_ENDIAN
#define HUGE
#define WORD unsigned int
#define BYTE unsigned char
#endif

#ifdef P_PC
#define ESCAPE_KEY 3
#define LITTLE_ENDIAN
#define HUGE huge
#define WORD unsigned long
#define BYTE unsigned char
#endif

#ifdef P_LINUX
#define ESCAPE_KEY 3
/* #define LITTLE_ENDIAN */
#define HUGE
#define WORD unsigned long
#define BYTE unsigned char
#endif

#ifdef LITTLE_ENDIAN
#define SWAPWORD(a) a
#else
#define SWAPWORD(a) (((a & 0x000000ff) << 24) | ((a & 0xff000000) >> 24) | \
                       ((a & 0x00ff0000) >>  8) | ((a & 0x0000ff00) <<  8))
#endif

/****************************************************************************/

#define FALSE 0
#define TRUE  1

#define MEMSIZE (8192*1024L)
#define RAM (8000*1024L)
#define ROM (8192*1024L)

#if 0
#define MEMSIZE (256*1024L)
#define RAM     (128*1024L)
#define ROM     (256*1024L)
#endif

#define ever (;;)

#define USR_MODE  0
#define FIQ_MODE  1
#define IRQ_MODE  2
#define SVC_MODE  3
#define ABT_MODE  4

#define N_BIT 0x80000000
#define Z_BIT 0x40000000
#define C_BIT 0x20000000
#define V_BIT 0x10000000
#define I_BIT 0x08000000
#define F_BIT 0x04000000

#define N_SHFT 31
#define Z_SHFT 30
#define C_SHFT 29
#define V_SHFT 28
#define I_SHFT 27
#define F_SHFT 26

#define N_FLAG ((r15 >> N_SHFT) & 1)
#define Z_FLAG ((r15 >> Z_SHFT) & 1)
#define C_FLAG ((r15 >> C_SHFT) & 1)
#define V_FLAG ((r15 >> V_SHFT) & 1)
#define I_FLAG ((r15 >> I_SHFT) & 1)
#define F_FLAG ((r15 >> F_SHFT) & 1)

#define L_PRE_INDEX (ins & 0x1000000)
#define L_UP        (ins & 0x0800000)
#define L_PSR       (ins & 0x0400000)
#define L_BYTE      (ins & 0x0400000)
#define L_WRITEBACK (ins & 0x0200000)
#define L_LOAD      (ins & 0x0100000)

#define VECTOR_RESET                  0x00
#define VECTOR_UNDEFINED_INSTRUCTION  0x04
#define VECTOR_SOFTWARE_INTERRUPT     0x08
#define VECTOR_ABORT_PREFETCH         0x0c
#define VECTOR_ABORT_DATA             0x10
#define VECTOR_ADDRESS_EXCEPTION      0x14
#define VECTOR_IRQ                    0x18
#define VECTOR_FIQ                    0x1c

#define INT_ESCAPE_PRESSED            1

#define r(a) *arm_regs[r15&3][a]
#define addPC(b,a) (((b) & 0xfc000003) | (((b)+(a)) & 0x03fffffc))

extern WORD rg[16], r15, r8fiq, r9fiq, r10fiq, r11fiq, r12fiq, r13fiq,
  r14fiq, r13irq, r14irq, r13svc, r14svc;

extern volatile int IRQint, FIQint;
extern int PCwrite;
extern WORD HUGE *wmem;
extern BYTE HUGE *bmem;

extern WORD *arm_regs[4][16];

extern int num_of_one_bits[256];
extern char *program_command_line;

extern void (*main_decode[]) (WORD);
extern int (*conditie[16]) (void);

extern void debugger (void);
extern void emulator (void);
extern char *disassemble (WORD, WORD);

extern WORD rwmem (WORD);
extern BYTE rbmem (WORD);
extern void wwmem (WORD, WORD);
extern void wbmem (WORD, WORD);

extern void initswi (void);

extern int getkey (void);
extern int inkey (int, int);
extern void writech (BYTE);
extern int xpos (void);
extern int ypos (void);
extern void window (int, int);
extern void gotoxy (int, int);
extern void mode (int);
extern void circlefill (int, int, int, int);
extern void circleoutline (int, int, int, int);
extern void line (int, int, int, int);
extern void trianglefill (int, int, int, int, int, int);
extern void rectanglefill (int, int, int, int, int, int);

/* the end */
