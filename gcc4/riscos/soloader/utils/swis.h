#ifndef UTIL_SWIS_H
#define UTIL_SWIS_H

@; SWI definitions for SOM and static loader

.equ	SWI_X_BIT,		0x20000

.equ	XOS_WriteS,		0x1 + SWI_X_BIT
.equ	XOS_Write0,		0x2 + SWI_X_BIT
.equ	XOS_File,		0x8 + SWI_X_BIT
.equ	XOS_Args,		0x9 + SWI_X_BIT
.equ	XOS_BPut,		0xB + SWI_X_BIT
.equ	XOS_GBPB,		0xC + SWI_X_BIT
.equ	XOS_Find,		0xD + SWI_X_BIT
.equ	XOS_GetEnv,		0x10 + SWI_X_BIT
.equ	XOS_Exit,		0x11 + SWI_X_BIT
.equ	XOS_Heap,		0x1D + SWI_X_BIT
.equ	OS_Module,		0x1E
.equ	XOS_Module,		0x1E + SWI_X_BIT
.equ	XOS_ReadUnsigned,	0x21 + SWI_X_BIT
.equ	XOS_ReadVarVal,		0x23 + SWI_X_BIT
.equ	XOS_ChangeDynamicArea,	0x2A + SWI_X_BIT
.equ	OS_GenerateError,	0x2B
.equ	XOS_SWINumberFromString,0x39 + SWI_X_BIT
.equ	XOS_CallEvery,		0x3C + SWI_X_BIT
.equ	XOS_RemoveTickerEvent,	0x3D + SWI_X_BIT
.equ	XOS_ReadMonotonicTime,  0x42 + SWI_X_BIT
.equ	XOS_WriteEnv,		0x48 + SWI_X_BIT
.equ	XOS_AddCallBack,	0x54 + SWI_X_BIT
.equ	XOS_RemoveCallBack,	0x5F + SWI_X_BIT
.equ	XOS_DynamicArea,	0x66 + SWI_X_BIT
.equ	XOS_ConvertHex8,	0xD4 + SWI_X_BIT
.equ	XOS_ConvertCardinal4,	0xD8 + SWI_X_BIT
.equ	XOS_ConvertInteger4,	0xDC + SWI_X_BIT
.equ	OS_WriteI,		0x100
.equ	XDDEUtils_SetCLSize,	0x42581 + SWI_X_BIT
.equ	XDDEUtils_SetCL,	0x42582 + SWI_X_BIT
.equ	XDDEUtils_GetCLSize,	0x42583 + SWI_X_BIT
.equ	XDDEUtils_GetCL,	0x42584 + SWI_X_BIT

.equ	XWimp_Poll,		0x400C7 + SWI_X_BIT

.equ	XTerritory_Collate,	0x4305D + SWI_X_BIT

.equ	XToolbox_CreateObject,	0x44EC0 + SWI_X_BIT
.equ	XToolbox_ShowObject,	0x44EC3 + SWI_X_BIT
.equ	XToolbox_ObjectMiscOp,	0x44EC6 + SWI_X_BIT
.equ	XToolbox_Initialise,	0x44ECF + SWI_X_BIT

.equ	XReport_Text0,		0x54C86 + SWI_X_BIT
.equ	XReport_TextS,		0x54C87 + SWI_X_BIT

.equ	HEAP_REASON_INIT,		0
.equ	HEAP_REASON_DESCRIBE,		1
.equ	HEAP_REASON_CLAIM,		2
.equ	HEAP_REASON_RELEASE,		3
.equ	HEAP_REASON_CHANGE_BLOCK_SIZE,	4
.equ	HEAP_REASON_CHANGE_HEAP_SIZE,	5
.equ	HEAP_REASON_READ_BLOCK_SIZE,	6

.equ	SOM_SWI_CHUNK_BASE,		0x58580
.equ	XSOM_PROLOGUE,			SOM_SWI_CHUNK_BASE + SWI_X_BIT + 0x0
.equ	XSOM_RESOLVER,			SOM_SWI_CHUNK_BASE + SWI_X_BIT + 0x1
.equ	XSOM_ALLOC,			SOM_SWI_CHUNK_BASE + SWI_X_BIT + 0x2
.equ	XSOM_FREE,			SOM_SWI_CHUNK_BASE + SWI_X_BIT + 0x3
.equ	XSOM_ALLOC_LIB,			SOM_SWI_CHUNK_BASE + SWI_X_BIT + 0x4
.equ	XSOM_REGISTER_OBJECT,		SOM_SWI_CHUNK_BASE + SWI_X_BIT + 0x5
.equ	XSOM_QUERY_OBJECT,		SOM_SWI_CHUNK_BASE + SWI_X_BIT + 0x8
.equ	XSOM_HANDLE_FROM_NAME,		SOM_SWI_CHUNK_BASE + SWI_X_BIT + 0xC
.equ	XSOM_ADDR_TO_OFFSET,		SOM_SWI_CHUNK_BASE + SWI_X_BIT + 0xD

.equ	SOM_REGISTER_LOADER,		0
.equ	SOM_REGISTER_CLIENT,		1
.equ	SOM_REGISTER_LIBRARY,		2

.equ	DYNAMIC_AREA_REASON_CREATE,	0
.equ	DYNAMIC_AREA_REASON_REMOVE,	1
.equ	DYNAMIC_AREA_REASON_INFO,	2
.equ	DYNAMIC_AREA_REASON_ENUM,	3
.equ	DYNAMIC_AREA_REASON_RENUMBER,	4
.equ	DYNAMIC_AREA_REASON_FREE_SPACE,	5

#endif
