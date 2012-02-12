//
// NativeMethods.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Runtime.InteropServices;

namespace riscos
{
	internal static class NativeMethods
	{
		// Wimp SWIs
		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Wimp_Initialise")]
		internal static extern IntPtr Wimp_Initialise (int version, string desc, int[] mess_list, ref UIntPtr handle);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Wimp_CloseDown")]
		internal static extern IntPtr Wimp_CloseDown (UIntPtr handle);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Wimp_Poll")]
		internal static extern IntPtr Wimp_Poll (uint mask, IntPtr poll_word, IntPtr sender);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Wimp_CreateWindow")]
		internal static extern IntPtr Wimp_CreateWindow (
				[In, MarshalAs(UnmanagedType.Struct)]
				ref Wimp.WindowBlock block,
				ref uint handle);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Wimp_DeleteWindow")]
		internal static extern IntPtr Wimp_DeleteWindow (
				[In, MarshalAs(UnmanagedType.Struct)]
				ref Wimp.WindowHandleBlock block);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Wimp_CloseWindow")]
		internal static extern IntPtr Wimp_CloseWindow (
				[In, MarshalAs(UnmanagedType.Struct)]
				ref Wimp.WindowHandleBlock block);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Wimp_RedrawWindow")]
		internal static extern IntPtr Wimp_RedrawWindow (
				[In, MarshalAs(UnmanagedType.Struct)]
				ref Wimp.RedrawWindowBlock block,
				ref int more);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Wimp_GetRectangle")]
		internal static extern IntPtr Wimp_GetRectangle (
				[In, MarshalAs(UnmanagedType.Struct)]
				ref Wimp.RedrawWindowBlock block,
				ref int more);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Wimp_SetExtent")]
		internal static extern IntPtr Wimp_SetExtent (
				uint handle,
				[In, MarshalAs(UnmanagedType.Struct)]
				ref NativeOS.Rect extent);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Wimp_OpenWindow")]
		internal static extern IntPtr Wimp_OpenWindow (
				[In, MarshalAs(UnmanagedType.Struct)]
				ref riscos.Wimp.WindowStateBlock block);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Wimp_OpenNestedWindow")]
		internal static extern IntPtr Wimp_OpenNestedWindow (
				[In, MarshalAs(UnmanagedType.Struct)]
				ref riscos.Wimp.WindowStateBlock block,
				uint parent,
				uint nestedFlags);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Wimp_GetWindowState")]
		internal static extern IntPtr Wimp_GetWindowState (
				[In, MarshalAs(UnmanagedType.Struct)]
				ref riscos.Wimp.WindowStateBlock block);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Wimp_GetNestedWindowState")]
		internal static extern IntPtr Wimp_GetNestedWindowState (
				[In, MarshalAs(UnmanagedType.Struct)]
				ref riscos.Wimp.WindowStateBlock block,
				ref UIntPtr parent,
				ref UIntPtr nestedFlags);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Wimp_CreateIcon")]
		internal static extern IntPtr Wimp_CreateIcon (
				uint priority,
				[In, MarshalAs(UnmanagedType.Struct)]
				ref riscos.Wimp.IconCreateBlock block,
				ref uint iconHandle);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Wimp_DeleteIcon")]
		internal static extern IntPtr Wimp_DeleteIcon (
				[In, MarshalAs(UnmanagedType.Struct)]
				ref riscos.Wimp.IconDeleteBlock block);

		// Toolbox SWIs
		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_Initialise")]
		internal static extern IntPtr Toolbox_Initialise (
				uint flags,
				uint wimp_version,
				uint[] message_list,	// Zero terminated list
				uint[] event_list,	// Zero terminated list
				string dir_name,
				[In, MarshalAs(UnmanagedType.LPArray, SizeConst=4)]
				ref uint[] message_file_dec,
				[In, MarshalAs(UnmanagedType.Struct)]
				ref riscos.Toolbox.IDBlock idBlock,
				ref UIntPtr taskHandle,
				ref UIntPtr spriteArea);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_CreateObject")]
		internal static extern IntPtr Toolbox_CreateObject (Toolbox.CreateObjectFlags flags,
								    string templateName,
								    ref uint objectID);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_CreateObject")]
		internal static extern IntPtr Toolbox_CreateObject (Toolbox.CreateObjectFlags flags,
								    UIntPtr templateBlock,
								    ref uint objectID);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_DeleteObject")]
		internal static extern IntPtr Toolbox_DeleteObject (uint flags,
								    uint objectID);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_ShowObject")]
		internal static extern IntPtr Toolbox_ShowObject (Toolbox.ShowObjectFlags flags,
								  uint objectID,
								  Toolbox.ShowObjectType showType,
								  UIntPtr type,
								  uint parentID,
								  int parentCmp);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_ShowObject")]
		internal static extern IntPtr Toolbox_ShowObject (Toolbox.ShowObjectFlags flags,
								  uint objectID,
								  Toolbox.ShowObjectType showType,
								  ref Toolbox.ShowObjectFullSpecBlock type,
								  uint parentID,
								  int parentCmp);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_ShowObject")]
		internal static extern IntPtr Toolbox_ShowObject (Toolbox.ShowObjectFlags flags,
								  uint objectID,
								  Toolbox.ShowObjectType showType,
								  ref Toolbox.ShowObjectTopLeftBlock type,
								  uint parentID,
								  int parentCmp);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_SetClientHandle")]
		internal static extern IntPtr Toolbox_SetClientHandle (uint flags,
								       uint objectID,
								       UIntPtr handle);

		// WIMP Support
		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Wimp_GetEventType")]
		internal static extern Wimp.PollCode wimp_get_event_type ();

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Wimp_GetEvent")]
		internal static extern IntPtr wimp_get_event ();

		// Reporter SWIs
		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Report_Text0")]
		internal static extern void Report_Text0 (string s);

		// OS SWIs
		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_OS_Plot")]
		internal static extern void OS_Plot (int command, int x, int y);

		// ColourTrans SWIs
		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_ColourTrans_SetGCOL")]
		internal static extern void ColourTrans_SetGCOL (OS.GCOLAction gcol_action,
								 uint palette_entry);
	}
}
