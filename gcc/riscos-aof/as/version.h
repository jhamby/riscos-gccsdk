#define VERSION \
  "AS v1.30, " __DATE__ "\n"
#ifdef CROSS_COMPILE
#define COPYRIGHT \
  "Based on AS for Acorn RISC OS\n" \
  "Original author: Niklas Röjemo\n" \
  "Maintainers: Nick Burrett, Darren Salt\n" \
  "Freeware, © 1992-2000 Niklas Röjemo, Nick Burrett, Darren Salt\n" \
  "Unix port: Andy Duplain\n"
#define MESSAGE "Creator: Niklas UNIX ARM ASM vsn 1.30 [" __DATE__ "]\n"
#else
#define COPYRIGHT \
  "Original author: Niklas Röjemo\n" \
  "Maintainers: Nick Burrett, Darren Salt\n" \
  "Freeware, © 1992-1999 Niklas Röjemo, Nick Burrett, Darren Salt\n"
#define MESSAGE "Creator: Niklas RISC OS ARM ASM vsn 1.30 [" __DATE__ "]\n"
#endif
