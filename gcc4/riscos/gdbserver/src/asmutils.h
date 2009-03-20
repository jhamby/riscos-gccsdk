#ifndef src_asmutils_h_
#define src_asmutils_h_

void trigger_callbacks(void);

int irqs_off(void);
int irqs_on(void);
void irqs_restore(int state);

void flush_icache(void);
void flush_caches(void);

int cpuconfig(void);

#endif
