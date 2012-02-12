.set	SWI_X_BIT,			0x20000

.set	SWI_OS_Plot,			0x45

.set	SWI_Wimp_Initialise,		0x400c0 + SWI_X_BIT
.set	SWI_Wimp_CreateWindow,		0x400c1 + SWI_X_BIT
.set	SWI_Wimp_CreateIcon,		0x400c2 + SWI_X_BIT
.set	SWI_Wimp_DeleteWindow,		0x400c3 + SWI_X_BIT
.set	SWI_Wimp_DeleteIcon,		0x400c4 + SWI_X_BIT
.set	SWI_Wimp_OpenWindow,		0x400c5 + SWI_X_BIT
.set	SWI_Wimp_CloseWindow,		0x400c6 + SWI_X_BIT
.set    SWI_Wimp_Poll,        	  	0x400c7 + SWI_X_BIT
.set	SWI_Wimp_RedrawWindow,		0x400c8 + SWI_X_BIT
.set	SWI_Wimp_GetRectangle,		0x400ca + SWI_X_BIT
.set	SWI_Wimp_GetWindowState,	0x400cb + SWI_X_BIT
.set	SWI_Wimp_SetExtent,		0x400d7 + SWI_X_BIT
.set    SWI_Wimp_CloseDown,    	 	0x400dd + SWI_X_BIT

.set	SWI_ColourTrans_SetGCOL,	0x40743 + SWI_X_BIT

.set	SWI_Toolbox_CreateObject,	0x44ec0 + SWI_X_BIT
.set	SWI_Toolbox_DeleteObject,	0x44ec1 + SWI_X_BIT
.set	SWI_Toolbox_ShowObject,		0x44ec3 + SWI_X_BIT
.set	SWI_Toolbox_HideObject,		0x44ec4 + SWI_X_BIT
.set	SWI_Toolbox_GetObjectState,	0x44ec5 + SWI_X_BIT
.set	SWI_Toolbox_ObjectMiscOp,	0x44ec6 + SWI_X_BIT
.set	SWI_Toolbox_SetClientHandle,	0x44ec7 + SWI_X_BIT
.set	SWI_Toolbox_GetClientHandle,	0x44ec8 + SWI_X_BIT
.set	SWI_Toolbox_GetObjectClass,	0x44ec9 + SWI_X_BIT
.set	SWI_Toolbox_GetParent,		0x44eca + SWI_X_BIT
.set	SWI_Toolbox_GetAncestor,	0x44ecb + SWI_X_BIT
.set	SWI_Toolbox_GetTemplateName,	0x44ecc + SWI_X_BIT
.set	SWI_Toolbox_RaiseToolboxEvent,	0x44ecd + SWI_X_BIT
.set	SWI_Toolbox_GetSysInfo,		0x44ece + SWI_X_BIT
.set	SWI_Toolbox_Initialise,		0x44ecf + SWI_X_BIT
.set	SWI_Toolbox_LoadResources,	0x44ed0 + SWI_X_BIT
.set	SWI_Toolbox_TemplateLookup,	0x44efb + SWI_X_BIT

.set	SWI_Report_Text0,		0x54c80
