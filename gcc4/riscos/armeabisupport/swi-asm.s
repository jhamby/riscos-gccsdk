@ Copyright (c) 2019 GCCSDK Developers
@ Written by Lee Noar

#ifndef USE_INLINE_SWIS

#define XOS_Heap			0x2001D
#define XOS_Module			0x2001E
#define XOS_ChangeDynamicArea		0x2002A
#define XOS_DynamicArea			0x20066
#define XOS_Memory			0x20068
#define XOS_ReportText0			0x74C80
#define XFilter_RegisterPreFilter	0x62640
#define XFilter_RegisterPostFilter	0x62641
#define XFilter_DeRegisterPreFilter	0x62642
#define XFilter_DeRegisterPostFilter	0x62643

	.text

	@ _kernel_oserror *rma_claim(size_t size, void **block);
	.global	rma_claim
rma_claim:
	MOV	ip, lr

	MOV	r3, r0
	MOV	r0, #6
	SWI	XOS_Module
	MOVVC	r0, #0
	MOVVS	r2, #0
	STR	r2, [r1]

	MOV	pc, ip

	@ _kernel_oserror *rma_free(void *block)
	.global	rma_free
rma_free:
	MOV	ip, lr

	MOV	r2, r0
	MOV	r0, #7
	SWI	XOS_Module
	MOVVC	r0, #0

	MOV	pc, ip

	@ _kernel_oserror *dynamic_pmp_claim_release(const int da,
	@					     pmp_phy_page_entry *page_list,
	@		   			     int num_entries)
	.global dynamic_pmp_claim_release
dynamic_pmp_claim_release:
	MOV	ip, lr

	MOV	r3, r2
	MOV	r2, r1
	MOV	r1, r0
	MOV	r0, #21		@ Physical PMP operation
	SWI	XOS_DynamicArea
	MOVVC	r0, #0

	MOV	pc, ip

	@ _kernel_oserror *dynamic_pmp_map_unmap(const int da,
	@					 pmp_phy_page_entry *page_list,
	@		   			 int num_entries)
	.global dynamic_pmp_map_unmap
dynamic_pmp_map_unmap:
	MOV	ip, lr

	MOV	r3, r2
	MOV	r2, r1
	MOV	r1, r0
	MOV	r0, #22		@ Logical PMP operation
	SWI	XOS_DynamicArea
	MOVVC	r0, #0

	MOV	pc, ip

	@ _kernel_oserror *dynamic_pmp_page_info(const int da,
	@					 pmp_page_info_entry *page_list,
	@		   			 int num_entries)
	.global dynamic_pmp_page_info
dynamic_pmp_page_info:
	MOV	ip, lr

	MOV	r3, r2
	MOV	r2, r1
	MOV	r1, r0
	MOV	r0, #25		@ PMP page status
	SWI	XOS_DynamicArea
	MOVVC	r0, #0

	MOV	pc, ip

	@ void report_text(const char *text)
	.global	report_text
report_text:
	MOV	ip, lr

	SWI	XOS_ReportText0

	MOV	pc, ip

	@ _kernel_oserror *heap_init(void *base, size_t size)
	.global	heap_init
heap_init:
	MOV	ip, lr

	MOV	r3, r1
	MOV	r1, r0
	MOV	r0, #0	@ heap initialise
	SWI	XOS_Heap

	MOV	pc, ip

	@ _kernel_oserror *heap_claim (void *base_addr, size_t size, void **block_ret)
	.global heap_claim
heap_claim:
	STR	lr, [sp, #-4]!

	MOV	ip, r2

	MOV	r3, r1
	MOV	r1, r0
	MOV	r0, #2	@ heap claim
	SWI	XOS_Heap
	MOVVC	r0, #0
	MOVVS	r2, #0
	STR	r2, [ip]

	LDR	pc, [sp], #4

	@ _kernel_oserror *heap_release (void *base_addr, void *block)
	.global	heap_release
heap_release:
	MOV	ip, lr

	MOV	r2, r1
	MOV	r1, r0
	MOV	r0, #3	@ heap release
	SWI	XOS_Heap
	MOVVC	r0, #0

	MOV	pc, ip

	@ _kernel_oserror *heap_extend (void *base_addr, size_t by)
	.global	heap_extend
heap_extend:
	MOV	ip, lr

	MOV	r3, r1
	MOV	r1, r0
	MOV	r0, #5	@ heap change size
	SWI	XOS_Heap
	MOVVC	r0, #0

	MOV	pc, ip

	@ _kernel_oserror *heap_extend_block (void *base, void **block, size_t by)
	.global	heap_extend_block
heap_extend_block:
	STR	lr, [sp, #-4]!

	MOV	ip, r1

	MOV	r3, r2
	MOV	r2, r1
	MOV	r1, r0
	MOV	r0, #4 @ heap change block size
	SWI	XOS_Heap
	STRVC	r2, [ip]
	MOVVC	r0, #0

	LDR	pc, [sp], #4

	@ _kernel_oserror *heap_block_size (void *base, void *block, size_t *size)
	.global	heap_block_size
heap_block_size:
	STR	lr, [sp, #-4]!

	MOV	ip, r2

	MOV	r2, r1
	MOV	r1, r0
	MOV	r0, #6	@ heap read block size
	SWI	XOS_Heap
	STRVC	r3, [ip]
	MOVVC	r0, #0

	LDR	pc, [sp], #4

	@ _kernel_oserror *dynamic_area_remove (unsigned da_number)
	.global	dynamic_area_remove
dynamic_area_remove:
	MOVS	r1, r0
	MOVEQ	pc, lr
	MOVMI	pc, lr

	MOV	ip, lr

	MOV	r0, #1	@ XOS_DynamicArea, remove
	SWI	XOS_DynamicArea
	MOVVC	r0, #0

	MOV	pc, ip

	@ _kernel_oserror *dynamic_area_extend (unsigned da_number, size_t by)
	.global	dynamic_area_extend
dynamic_area_extend:
	MOV	ip, lr

	SWI	XOS_ChangeDynamicArea
	MOVVC	r0, #0

	MOV	pc, ip

	@ _kernel_oserror *filter_register_pre_filter (const char *filter_name,
	@					       void (*filter_code)(void),
	@					       void *pw,
	@					       unsigned task_handle)
	.global	filter_register_pre_filter
filter_register_pre_filter:
	MOV	ip, lr

	SWI	XFilter_RegisterPreFilter
	MOVVC	r0, #0

	MOV	pc, ip

	@ _kernel_oserror *filter_register_post_filter (const char *filter_name,
	@		     				void (*filter_code)(void),
	@		     				void *pw,
	@		     				unsigned task_handle)
	.global	filter_register_post_filter
filter_register_post_filter:
	STR	r4, [sp, #-4]!

	MOV	ip, lr
	MOV	r4, #0
	SWI	XFilter_RegisterPostFilter
	MOVVC	r0, #0

	LDR	r4, [sp], #4
	MOV	pc, ip

	@ _kernel_oserror *filter_deregister_pre_filter (const char *filter_name,
	@						 void (*filter_code)(void),
	@						 void *pw,
	@						 unsigned task_handle)
	.global	filter_deregister_pre_filter
filter_deregister_pre_filter:
	MOV	ip, lr

	SWI	XFilter_DeRegisterPreFilter
	MOVVC	r0, #0

	MOV	pc, ip

	@ _kernel_oserror *filter_deregister_post_filter (const char *filter_name,
	@						  void (*filter_code)(void),
	@						  void *pw,
	@						  unsigned task_handle)
	.global	filter_deregister_post_filter
filter_deregister_post_filter:
	STR	r4, [sp, #-4]!

	MOV	ip, lr
	MOV	r4, #0
	SWI	XFilter_DeRegisterPostFilter
	MOVVC	r0, #0

	LDR	r4, [sp], #4
	MOV	pc, ip

	@ _kernel_oserror *get_app_base(unsigned *base_ret)
	.global	get_app_base
get_app_base:
	MOV	ip, lr

	SUB	sp, sp, #12
	MOV	r3, #0
	STR	r3, [sp, #0]
	STR	r3, [sp, #8]
	MOV	r3, #0x8000
	STR	r3, [sp, #4]

	MOV	r3, r0
	MOV	r0, #(1 << 9) | (1 << 13)
	MOV	r1, sp
	MOV	r2, #1
	SWI	XOS_Memory
	MOVVC	r0, #0
	MOVVS	r1, #0
	LDRVC	r1, [sp, #8]
	STR	r1, [r3]

	ADD	sp, sp, #12
	MOV	pc, ip

	.end

#endif
