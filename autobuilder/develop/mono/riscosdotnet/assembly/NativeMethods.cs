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
		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_initialise")]
		internal static extern IntPtr Wimp_Initialise (int version,
							       string name,
							       int[] messages,
							       out int versionOut,
							       out uint taskOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_close_down")]
		internal static extern IntPtr Wimp_CloseDown (uint handle);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Wimp_Poll")]
		internal static extern IntPtr Wimp_Poll (uint mask, IntPtr poll_word);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_create_window")]
		internal static extern IntPtr Wimp_CreateWindow (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref Wimp.WindowBlock block,
				ref uint handle);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_delete_window")]
		internal static extern IntPtr Wimp_DeleteWindow (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref Wimp.WindowHandleBlock block);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_close_window")]
		internal static extern IntPtr Wimp_CloseWindow (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref Wimp.WindowHandleBlock block);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_redraw_window")]
		internal static extern IntPtr Wimp_RedrawWindow (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref Wimp.RedrawWindowBlock block,
				ref int more);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_get_rectangle")]
		internal static extern IntPtr Wimp_GetRectangle (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref Wimp.RedrawWindowBlock block,
				ref int more);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_set_extent")]
		internal static extern IntPtr Wimp_SetExtent (
				uint handle,
				[In, MarshalAs(UnmanagedType.Struct)]
					ref NativeOS.Rect extent);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_open_window")]
		internal static extern IntPtr Wimp_OpenWindow (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref riscos.Wimp.WindowStateBlock block);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_open_window_nested_with_flags")]
		internal static extern IntPtr Wimp_OpenNestedWindow (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref riscos.Wimp.WindowStateBlock block,
				uint parent,
				uint nestedFlags);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_get_window_state")]
		internal static extern IntPtr Wimp_GetWindowState (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref riscos.Wimp.WindowStateBlock block);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_get_window_state_and_nesting")]
		internal static extern IntPtr Wimp_GetNestedWindowState (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref riscos.Wimp.WindowStateBlock block,
				ref IntPtr parent,
				ref IntPtr nestedFlags);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_create_icon")]
		internal static extern IntPtr Wimp_CreateIcon (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref riscos.Wimp.IconCreateBlock block,
				ref uint iconHandle);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_delete_icon")]
		internal static extern IntPtr Wimp_DeleteIcon (
				uint window,
				uint icon);

		// Toolbox SWIs
		[DllImport("libriscosdotnet.so.1", EntryPoint="xoolbox_initialise")]
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
				out int versionOut,
				out uint taskOut,
				out IntPtr spriteAreaOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xtoolbox_create_object_from_template")]
		internal static extern IntPtr Toolbox_CreateObject (Toolbox.CreateObjectFlags flags,
								    string templateName,
								    ref uint objectID);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xtoolbox_create_object")]
		internal static extern IntPtr Toolbox_CreateObject (Toolbox.CreateObjectFlags flags,
								    IntPtr templateBlock,
								    ref uint objectID);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xtoolbox_delete_object")]
		internal static extern IntPtr Toolbox_DeleteObject (uint flags,
								    uint objectID);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xtoolbox_show_object")]
		internal static extern IntPtr Toolbox_ShowObject (Toolbox.ShowObjectFlags flags,
								  uint objectID,
								  Toolbox.ShowObjectType showType,
								  IntPtr type,
								  uint parentID,
								  int parentCmp);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xtoolbox_show_object")]
		internal static extern IntPtr Toolbox_ShowObject (Toolbox.ShowObjectFlags flags,
								  uint objectID,
								  Toolbox.ShowObjectType showType,
								  ref Toolbox.ShowObjectFullSpecBlock type,
								  uint parentID,
								  int parentCmp);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xtoolbox_show_object")]
		internal static extern IntPtr Toolbox_ShowObject (Toolbox.ShowObjectFlags flags,
								  uint objectID,
								  Toolbox.ShowObjectType showType,
								  ref Toolbox.ShowObjectTopLeftBlock type,
								  uint parentID,
								  int parentCmp);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xtoolbox_set_client_handle")]
		internal static extern IntPtr Toolbox_SetClientHandle (uint flags,
								       uint objectID,
								       IntPtr handle);

		// WIMP Support
		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Wimp_GetEventType")]
		internal static extern Wimp.PollCode wimp_get_event_type ();

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Wimp_GetEvent")]
		internal static extern IntPtr wimp_get_event ();

		// Reporter SWIs
		[DllImport("libriscosdotnet.so.1", EntryPoint="xreport_text0")]
		internal static extern void Report_Text0 (string s);

		// OS SWIs
		[DllImport("libriscosdotnet.so.1", EntryPoint="xos_plot")]
		internal static extern void OS_Plot (int command, int x, int y);

		// ColourTrans SWIs
		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_set_gcol")]
		internal static extern void ColourTrans_SetGCOL (uint palette_entry,
								 uint flags,
								 OS.GCOLAction gcol_action,
								 out int gcolOut,
								 out int log2BppOut);
	}
}
