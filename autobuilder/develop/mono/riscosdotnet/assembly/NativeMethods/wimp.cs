//
// NativeMethods/wimp.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Runtime.InteropServices;

namespace riscos
{
	internal static partial class NativeMethods
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
		internal static extern IntPtr Wimp_Poll (uint mask, out uint poll_word);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Wimp_PollIdle")]
		internal static extern IntPtr Wimp_PollIdle (uint mask, uint time, out uint poll_word);

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
				out int more);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_update_window")]
		internal static extern IntPtr Wimp_UpdateWindow (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref Wimp.RedrawWindowBlock block,
				out int more);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_get_rectangle")]
		internal static extern IntPtr Wimp_GetRectangle (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref Wimp.RedrawWindowBlock block,
				out int more);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_set_extent")]
		internal static extern IntPtr Wimp_SetExtent (
				uint handle,
				[In, MarshalAs(UnmanagedType.Struct)]
					ref NativeOS.Rect extent);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_open_window")]
		internal static extern IntPtr Wimp_OpenWindow (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref riscos.Wimp.WindowStateBlock block);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_open_window_nested")]
		internal static extern IntPtr Wimp_OpenWindowNested (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref riscos.Wimp.WindowStateBlock block,
				uint parent,
				uint linkage);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_open_window_nested_with_flags")]
		internal static extern IntPtr Wimp_OpenWindowNestedWithFlags (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref riscos.Wimp.WindowStateBlock block,
				uint parent,
				uint nestedFlags);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_get_window_state")]
		internal static extern IntPtr Wimp_GetWindowState (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref riscos.Wimp.WindowStateBlock block);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_get_window_state_and_nesting")]
		internal static extern IntPtr Wimp_GetWindowStateAndNesting (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref riscos.Wimp.WindowStateBlock block,
				out uint parent,
				out uint nestedFlags);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_create_icon")]
		internal static extern IntPtr Wimp_CreateIcon (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref riscos.Wimp.IconCreateBlock block,
				out uint iconHandle);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_create_icon_relative")]
		internal static extern IntPtr Wimp_CreateIconRelative (
				int neighbour,
				[In, MarshalAs(UnmanagedType.Struct)]
					ref riscos.Wimp.IconCreateBlock block,
				out uint iconHandle);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_create_icon_prioritised")]
		internal static extern IntPtr Wimp_CreateIconPrioritised (
				int priority,
				[In, MarshalAs(UnmanagedType.Struct)]
					ref riscos.Wimp.IconCreateBlock block,
				out uint iconHandle);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_delete_icon")]
		internal static extern IntPtr Wimp_DeleteIcon (uint windowHandle,
							       uint iconHandle);

		/* Includes icons - difficult to use due to variable size of returned data?  */
		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_get_window_info")]
		internal static extern IntPtr Wimp_GetWindowInfo (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref riscos.Wimp.WindowInfoBlock block);

		/* Excludes icons.  */
		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_get_window_info_header_only")]
		internal static extern IntPtr Wimp_GetWindowInfoHeaderOnly (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref riscos.Wimp.WindowInfoBlock block);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_set_icon_state")]
		internal static extern IntPtr Wimp_SetIconState (uint windowHandle,
								 uint iconHandle,
								 uint eorBits,
								 uint clearBits);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_get_pointer_info")]
		internal static extern IntPtr Wimp_GetPointerInfo ([In, MarshalAs(UnmanagedType.Struct)]
									ref riscos.Wimp.PointerBlock block);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_drag_box")]
		internal static extern IntPtr Wimp_DragBox ([In, MarshalAs(UnmanagedType.Struct)]
								 ref riscos.NativeOS.Rect dragRect);

		// RISC OS 4+
		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_drag_box_with_flags")]
		internal static extern IntPtr Wimp_DragBox ([In, MarshalAs(UnmanagedType.Struct)]
								 ref riscos.NativeOS.Rect dragRect,
							    uint flags);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_force_redraw")]
		internal static extern IntPtr Wimp_ForceRedraw (uint windowHandle,
								int minX,
								int minY,
								int maxX,
								int maxY);

		// Nested Wimp/RISC OS 3.8+
		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_force_redraw_furniture")]
		internal static extern IntPtr Wimp_ForceRedrawFurniture (uint windowHandle,
									 riscos.Wimp.Furniture item);

		// Nested Wimp/RISC OS 3.8+
		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_force_redraw_title")]
		internal static extern IntPtr Wimp_ForceRedrawTitle (uint windowHandle);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_set_caret_position")]
		internal static extern IntPtr Wimp_SetCaretPosition (uint windowHandle,
								     uint iconHandle,
								     int x,
								     int y,
								     int height,
								     int index);


		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_get_caret_position")]
		internal static extern IntPtr Wimp_GetCaretPosition (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref riscos.Wimp.CaretBlock block);

/*		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_create_menu")]
		internal static extern IntPtr Wimp_CreateMenu (
				IntPtr menu,
				int x,
				int y);*/

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_open_template")]
		internal static extern IntPtr Wimp_OpenTemplate (string file_name);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_close_template")]
		internal static extern IntPtr Wimp_CloseTemplate ();

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_process_key")]
		internal static extern IntPtr Wimp_ProcessKey (int key);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_report_error")]
		internal static extern IntPtr Wimp_ReportError (IntPtr error,
								uint flags,
								string name,
								out int click);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_plot_icon")]
		internal static extern IntPtr Wimp_PlotIcon ([In, MarshalAs(UnmanagedType.Struct)]
								  ref riscos.Wimp.IconBlock block);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_resize_icon")]
		internal static extern IntPtr Wimp_ResizeIcon (uint windowHandle,
							       uint iconHandle,
							       int x0,
							       int y0,
							       int x1,
							       int y1);

		// WIMP Support
		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Wimp_GetEventType")]
		internal static extern Wimp.PollCode wimp_get_event_type ();

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Wimp_GetEvent")]
		internal static extern IntPtr wimp_get_event ();
	}
}
