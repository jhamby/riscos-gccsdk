//
// NativeMethods/toolbox.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Runtime.InteropServices;
using riscos;

namespace riscos
{
	public static class NativeToolbox
	{
		[StructLayout(LayoutKind.Sequential)]
		public struct IDBlock
		{
			public uint	ancestor_id;
			public uint	ancestor_cmp;
			public uint	parent_id;
			public uint	parent_cmp;
			public uint	self_id;
			public uint	self_cmp;
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
	}

	static partial class NativeMethods
	{
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
					ref NativeToolbox.IDBlock idBlock,
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
	}
}

