/* RISC OS module initialisation for GCC 2.95.
   Written by Nick Burrett, 1999.

   Use and modify this code at your free will.  */

static int *module_base[10] __attribute__ ((__section__ ("!!Module$$Header")));

