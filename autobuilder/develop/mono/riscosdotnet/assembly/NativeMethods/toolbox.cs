//
// NativeMethods/toolbox.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Runtime.InteropServices;
using System.Text;
using riscos;

namespace riscos
{
	public static class NativeToolbox
	{
		[StructLayout(LayoutKind.Sequential)]
		public class IDBlock
		{
			public uint AncestorID { get; private set; }
			public uint AncestorCmp { get; private set; }
			public uint ParentID { get; private set; }
			public uint ParentCmp { get; private set; }
			public uint SelfID { get; private set; }
			public uint SelfCmp { get; private set; }
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct ShowObjectFullSpecBlock
		{
			public NativeOS.Rect Visible;
			public NativeOS.Coord Scroll;
			public uint BehindWindow;

			public uint WindowFlags;
			public uint ParentWindowHandle;
			public uint AlignmentFlags;

			public ShowObjectFullSpecBlock (OS.Rect visible,
							OS.Coord scroll)
			{
				Visible = new NativeOS.Rect (visible);
				Scroll = new NativeOS.Coord (scroll);
				BehindWindow = Wimp.WindowStackPosition.Top;
				WindowFlags = 0;
				ParentWindowHandle = 0;
				AlignmentFlags = 0;
			}
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct ShowObjectTopLeftBlock
		{
			public NativeOS.Coord TopLeft;

			public ShowObjectTopLeftBlock (OS.Coord topLeft)
			{
				TopLeft = new NativeOS.Coord (topLeft);
			}
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct EventHeader
		{
			public uint Size;
			public uint Ref;
			public uint EventCode;
			public uint Flags;
		}
	}

	static partial class NativeMethods
	{
		// Toolbox SWIs
		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_Initialise")]
		internal static extern IntPtr Toolbox_Initialise (
				uint flags,
				uint wimp_version,
				int[] message_list,	// Zero terminated list
				int[] event_list,	// Zero terminated list
				string dir_name,
				out int versionOut,
				out uint taskOut,
				out IntPtr spriteAreaOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_GetIDBlock")]
		internal static extern NativeToolbox.IDBlock Toolbox_GetIDBlock ();

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_GetMessTransFD")]
		[return: MarshalAs(UnmanagedType.LPArray,SizeConst=4)]
		internal static extern uint[] Toolbox_GetMessTransFD ();

		[DllImport("libriscosdotnet.so.1", EntryPoint="xtoolbox_create_object")]
		internal static extern IntPtr Toolbox_CreateObject (Toolbox.CreateObjectFlags flags,
								    string templateName,
								    out uint objectID);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xtoolbox_create_object")]
		internal static extern IntPtr Toolbox_CreateObject (Toolbox.CreateObjectFlags flags,
								    IntPtr templateBlock,
								    out uint objectID);

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
								  ref NativeToolbox.ShowObjectFullSpecBlock type,
								  uint parentID,
								  int parentCmp);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xtoolbox_show_object")]
		internal static extern IntPtr Toolbox_ShowObject (Toolbox.ShowObjectFlags flags,
								  uint objectID,
								  Toolbox.ShowObjectType showType,
								  ref NativeToolbox.ShowObjectTopLeftBlock type,
								  uint parentID,
								  int parentCmp);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xtoolbox_set_client_handle")]
		internal static extern IntPtr Toolbox_SetClientHandle (uint flags,
								       uint objectID,
								       IntPtr handle);

		// Window SWIs
		[DllImport("libriscosdotnet.so.1", EntryPoint="xwindow_get_wimp_handle")]
		internal static extern IntPtr Window_GetWimpHandle (uint flags, uint WindowID, out uint wimpHandle);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwindow_add_gadget")]
		internal static extern IntPtr Window_AddGadget (uint flags, uint WindowID, IntPtr gadgetData);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwindow_remove_gadget")]
		internal static extern IntPtr Window_RemoveGadget (uint flags, uint WindowID, uint cmpID);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwindow_set_menu")]
		internal static extern IntPtr Window_SetMenu (uint flags, uint WindowID, uint menuID);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwindow_get_menu")]
		internal static extern IntPtr Window_GetMenu (uint flags, uint WindowID, out uint menuID);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwindow_set_pointer")]
		internal static extern IntPtr Window_SetPointer (uint flags,
								 uint WindowID,
								 string spriteName,
								 int xHotSpot,
								 int yHotSpot);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwindow_set_title")]
		internal static extern IntPtr Window_SetTitle (uint flags, uint WindowID, string title);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwindow_get_title")]
		internal static extern IntPtr Window_GetTitle (uint flags,
							       uint WindowID,
							       StringBuilder title,
							       int size,
							       out int used);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwindow_get_title")]
		internal static extern IntPtr Window_GetTitle (uint flags,
							       uint WindowID,
							       IntPtr title,
							       int size,
							       out int requiredSize);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwindow_set_default_focus")]
		internal static extern IntPtr Window_SetDefaultFocus (uint flags, uint WindowID, uint cmpID);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwindow_get_default_focus")]
		internal static extern IntPtr Window_GetDefaultFocus (uint flags, uint WindowID, out uint cmpID);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwindow_set_extent")]
		internal static extern IntPtr Window_SetExtent (uint flags, uint WindowID, NativeOS.Rect extent);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwindow_get_extent")]
		internal static extern IntPtr Window_GetExtent (uint flags, uint WindowID, out NativeOS.Rect extent);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwindow_force_redraw")]
		internal static extern IntPtr Window_ForceRedraw (uint flags, uint WindowID, NativeOS.Rect extent);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwindow_get_pointer_info")]
		internal static extern IntPtr Window_GetPointerInfo (uint flags,
								     out int x,
								     out int y,
								     out uint buttons,
								     out uint windowID,
								     out uint cmpID);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwindow_wimp_to_toolbox")]
		internal static extern IntPtr Window_WimpToToolbox (uint flags,
								    uint wimpHandle,
								    uint wimpIcon,
								    out uint objectID,
								    out uint cmpID);
	}
}

