unsigned long armrom_code[] = {
  0xea001ffe, /* b &00008000 */
  0xea000032, /* b &000000d4 */
  0xea000033, /* b &000000dc */
  0xea000034, /* b &000000e4 */
  0xea000035, /* b &000000ec */
  0xea000036, /* b &000000f4 */
  0xea000037, /* b &000000fc */
  0xea000038, /* b &00000104 */
  0x80000001, /* DCD &80000001 */
  0x65646e55, /* Undefined instruction at & */
  0x656e6966,
  0x6e692064,
  0x75727473,
  0x6f697463,
  0x7461206e,
  0x74002620,
  0x80000002, /* DCD &80000002 */
  0x74666f53, /* Software interrupt at & */
  0x65726177,
  0x746e6920,
  0x75727265,
  0x61207470,
  0x00262074,
  0x80000003, /* DCD &80000003 */
  0x726f6241, /* Abort on prefetch at & */
  0x6e6f2074,
  0x65727020,
  0x63746566,
  0x74612068,
  0x6f002620,
  0x80000004, /* DCD &80000004 */
  0x726f6241, /* Abort on data transfer at & */
  0x6e6f2074,
  0x74616420,
  0x72742061,
  0x66736e61,
  0x61207265,
  0x00262074,
  0x80000005, /* DCD &80000005 */
  0x72646441, /* Address exception at & */
  0x20737365,
  0x65637865,
  0x6f697470,
  0x7461206e,
  0xe2002620,
  0x80000006, /* DCD &80000006 */
  0x20515249, /* IRQ at & */
  0x26207461,
  0xe24f3000,
  0x80000007, /* DCD &80000007 */
  0x20514946, /* FIQ at & */
  0x26207461,
  0xe24f3000, /* sub	r3, pc, #0 */
  0xe24f30bc, /* sub	r3, pc, #188 - label 0xd4 */
  0xea00000b, /* b	&0000010c */
  0xe24f30a4, /* sub	r3, pc, #164 - label 0xdc */
  0xea000009, /* b	&0000010c */
  0xe24f3090, /* sub	r3, pc, #144 - label 0xe4 */
  0xea000007, /* b	&0000010c */
  0xe24f307c, /* sub	r3, pc, #124 - label 0xec */
  0xea000005, /* b	&0000010c */
  0xe24f3064, /* sub	r3, pc, #100 - label 0xf4 */
  0xea000003, /* b	&0000010c */
  0xe24f3050, /* sub	r3, pc, #80 - label 0xfc */
  0xea000001, /* b	&0000010c */
  0xe24f3048, /* sub	r3, pc, #72 - label 0x104 */
  0xeaffffff, /* b	&0000010c */
  0xe1a0400e, /* mov	r4, lr */
  0xe33ff3c3, /* teqp	pc, #201326595 */
  0xf1a00000, /* movnv	r0, r0 */
  0xe28f1030, /* add	r1, pc, #48 */
  0xe1a02003, /* mov	r2, r3 */
  0xe4920004, /* ldr	r0, [r2], #4 */
  0xe4810004, /* str	r0, [r1], #4 */
  0xe4d20001, /* ldrb	r0, [r2], #1 */
  0xe3300000, /* teq	r0, #0 */
  0x0a000001, /* beq	&0000013c */
  0xe4c10001, /* strb	r0, [r1], #1 */
  0xeafffffa, /* b	&00000128 */
  0xe1a00004, /* mov	r0, r4 */
  0xe3a02009, /* mov	r2, #9 */
  0xef0000d4, /* swi	0x000000d4 */
  0xe28f0000, /* add	r0, pc, #0 */
  0xef00002b, /* swi	0x0000002b */
  0x00000000
};







