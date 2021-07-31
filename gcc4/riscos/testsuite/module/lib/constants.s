
	.section .rodata
	.align 2

	.global	Image_RO_Base
Image_RO_Base:
	.word	Image$$RO$$Base

	.global	Image_RO_Limit
Image_RO_Limit:
	.word	Image$$RO$$Limit

	.global	Image_RW_Base
Image_RW_Base:
	.word	Image$$RW$$Base

	.global	Image_RW_Limit
Image_RW_Limit:
	.word	Image$$RW$$Limit

	.global	Image_ZI_Base
Image_ZI_Base:
	.word	Image$$ZI$$Base

	.global	Image_ZI_Limit
Image_ZI_Limit:
	.word	Image$$ZI$$Limit

	.end

