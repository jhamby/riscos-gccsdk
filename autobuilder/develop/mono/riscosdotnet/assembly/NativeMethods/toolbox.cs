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
			public readonly uint AncestorID;
			public readonly uint AncestorCmp;
			public readonly uint ParentID;
			public readonly uint ParentCmp;
			public readonly uint SelfID;
			public readonly uint SelfCmp;
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

			public ShowObjectFullSpecBlock (Toolbox.ShowObjectFull spec)
			{
				Visible = new NativeOS.Rect (spec.Visible);
				Scroll = new NativeOS.Coord (spec.Scroll);
				BehindWindow = spec.StackPosition;
				WindowFlags = spec.WindowFlags;
				ParentWindowHandle = spec.ParentWindowHandle;
				AlignmentFlags = spec.AlignmentFlags;
			}
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct ShowObjectTopLeftBlock
		{
			public NativeOS.Coord TopLeft;

			public ShowObjectTopLeftBlock (Toolbox.ShowObjectTopLeft spec)
			{
				TopLeft = new NativeOS.Coord (spec.TopLeft);
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

		[StructLayout(LayoutKind.Sequential)]
		public struct ShowObjectFullSpecEvent
		{
			public EventHeader Header;
			public ShowObjectFullSpecBlock Spec;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct ShowObjectTopLeftEvent
		{
			public EventHeader Header;
			public ShowObjectTopLeftBlock Spec;
		}
	}

	internal static partial class NativeMethods
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
		internal static extern IntPtr Toolbox_GetIDBlock ();

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

		[DllImport("libriscosdotnet.so.1", EntryPoint="xtoolbox_hide_object")]
		internal static extern IntPtr Toolbox_HideObject (uint flags,
								  uint objectID);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xtoolbox_get_object_info")]
		internal static extern IntPtr Toolbox_GetObjectState (uint flags, uint objectID, out uint state);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xtoolbox_set_client_handle")]
		internal static extern IntPtr Toolbox_SetClientHandle (uint flags,
								       uint objectID,
								       IntPtr handle);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xtoolbox_set_client_handle")]
		internal static extern IntPtr Toolbox_SetClientHandle (uint flags,
								       uint objectID,
								       int handle);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xtoolbox_get_client_handle")]
		internal static extern IntPtr Toolbox_GetClientHandle (uint flags,
								       uint objectID,
								       out int handle);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xtoolbox_get_client_handle")]
		internal static extern IntPtr Toolbox_GetClientHandle (uint flags,
								       uint objectID,
								       out IntPtr handle);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xtoolbox_template_look_up")]
		internal static extern IntPtr Toolbox_TemplateLookUp (uint flags,
								      string resName,
								      out IntPtr templateOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xtoolbox_get_object_class")]
		internal static extern IntPtr Toolbox_GetObjectClass (uint flags, uint objectID, out uint class_type);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xtoolbox_get_parent")]
		internal static extern IntPtr Toolbox_GetParent (uint flags,
								 uint objectID,
								 out uint ParentID,
								 out uint ParentCmp);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xtoolbox_get_ancestor")]
		internal static extern IntPtr Toolbox_GetAncestor (uint flags,
								   uint objectID,
								   out uint AncestorID,
								   out uint AncestorCmp);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xtoolbox_get_template_name")]
		internal static extern IntPtr Toolbox_GetTemplateName (uint flags,
								       uint ObjectID,
								       StringBuilder buffer,
								       int size,
								       out int used);

		// Toolbox Window SWIs
		[DllImport("libriscosdotnet.so.1", EntryPoint="xwindow_add_gadget")]
		internal static extern IntPtr Window_AddGadget (uint flags, uint WindowID, IntPtr gadgetData);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwindow_remove_gadget")]
		internal static extern IntPtr Window_RemoveGadget (uint flags, uint WindowID, uint cmpID);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwindow_set_pointer")]
		internal static extern IntPtr Window_SetPointer (uint flags,
								 uint WindowID,
								 string spriteName,
								 int xHotSpot,
								 int yHotSpot);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwindow_get_pointer")]
		internal static extern IntPtr Window_GetPointer (uint flags,
								 uint WindowID,
								 StringBuilder spriteName,
								 int buffer_size,
								 out int used,
								 out int xHotSpot,
								 out int yHotSpot);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_ObjectMiscOp_SetR3")]
		internal static extern IntPtr Window_SetExtent (uint flags,
								uint WindowID,
								int method,
								NativeOS.Rect extent);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_ObjectMiscOp_SetR3")]
		internal static extern IntPtr Window_GetExtent (uint flags,
								uint WindowID,
								int method,
								out NativeOS.Rect extent);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_ObjectMiscOp_SetR3")]
		internal static extern IntPtr Window_ForceRedraw (uint flags,
								  uint WindowID,
								  int method,
								  ref NativeOS.Rect extent);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwindow_set_tool_bars")]
		internal static extern IntPtr Window_SetToolBars (uint flags,
								  uint windowID,
								  uint ibl,
								  uint itl,
								  uint ebl,
								  uint etl);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwindow_get_tool_bars")]
		internal static extern IntPtr Window_GetToolBars (uint flags,
								  uint windowID,
								  out uint ibl,
								  out uint itl,
								  out uint ebl,
								  out uint etl);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwindow_get_pointer_info")]
		internal static extern IntPtr Window_GetPointerInfo (uint flags,
								     out int x,
								     out int y,
								     out uint buttons,
								     out uint windowID,
								     out uint cmpID);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwindow_wimp_to_toolbox")]
		internal static extern IntPtr Window_WimpToToolbox (uint flags,
								    uint wimpWindow,
								    int wimpIcon,
								    out uint objectID,
								    out uint cmpID);

		// Methods to call Toolbox_ObjectMiscOp veneers

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_ObjectMiscOp_SetR3R4")]
		internal static extern IntPtr Gadget_MoveGadget (uint flags,
								 uint windowID,
								 int method,
								 uint componentID,
								 ref NativeOS.Rect bbox);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_ObjectMiscOp_SetR3R4")]
		internal static extern IntPtr Gadget_GetBBox (uint flags,
							      uint windowID,
							      int method,
							      uint componentID,
							      out NativeOS.Rect bbox);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_ObjectMiscOp_SetR3R4R5GetR5")]
		internal static extern IntPtr Gadget_GetIconList (uint flags,
								  uint windowID,
								  int method,
								  uint cmpID,
								  IntPtr buffer,
								  int bufferSize,
								  out int used);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_ObjectMiscOp_SetR3R4R5GetR5")]
		internal static extern IntPtr Toolbox_ObjectMiscOp_SetR3R4R5GetR5 (uint flags,
										   uint windowID,
										   int method,
										   uint r3,
										   StringBuilder r4,
										   int r5,
										   out int r5_out);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_ObjectMiscOp_SetR3R4R5")]
		internal static extern IntPtr Toolbox_ObjectMiscOp_SetR3R4R5 (uint flags,
									      uint windowID,
									      int method,
									      uint cmpID,
									      uint r4,
									      uint r5);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_ObjectMiscOp_SetR3R4R5R6")]
		internal static extern IntPtr Toolbox_ObjectMiscOp_SetFont (uint flags,
									    uint windowID,
									    int method,
									    uint cmpID,
									    string fontID,
									    int width,
									    int height);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_ObjectMiscOp_SetR3GetR0")]
		internal static extern IntPtr Toolbox_ObjectMiscOp_SetR3GetR0 (uint flags,
									       uint objectID,
									       int method,
									       uint r3,
									       out uint r0);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_ObjectMiscOp_SetR3GetR0R1")]
		internal static extern IntPtr Toolbox_ObjectMiscOp_SetR3GetR0R1 (uint flags,
										 uint windowID,
										 int method,
										 uint r3,
										 out uint r0,
										 out uint r1);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_ObjectMiscOp_SetR3GetR0R1R2")]
		internal static extern IntPtr Toolbox_ObjectMiscOp_SetR3GetR0R1R2 (uint flags,
										   uint windowID,
										   int method,
										   uint r3,
										   out int r0,
										   out int r1,
										   out int r2);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_ObjectMiscOp_SetR3")]
		internal static extern IntPtr Toolbox_ObjectMiscOp_SetR3 (uint flags,
									  uint objectID,
									  int method,
									  uint r3);

		// Overload so that we can pass a pointer in R3.
		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_ObjectMiscOp_SetR3")]
		internal static extern IntPtr Toolbox_ObjectMiscOp_SetR3 (uint flags,
									  uint objectID,
									  int method,
									  IntPtr r3);

		// Overload so that we can pass a string in R3.
		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_ObjectMiscOp_SetR3")]
		internal static extern IntPtr Toolbox_ObjectMiscOp_SetR3 (uint flags,
									  uint objectID,
									  int method,
									  string messageText);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_ObjectMiscOp_SetR3R4GetR4")]
		internal static extern IntPtr Toolbox_ObjectMiscOp_SetR3R4GetR4 (uint flags,
										 uint objectID,
										 int method,
										 StringBuilder r3,
										 int r4,
										 out int r4_out);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_ObjectMiscOp_SetR3R4GetR0R4")]
		internal static extern IntPtr Toolbox_ObjectMiscOp_SetR3R4GetR0R4 (uint flags,
										   uint objectID,
										   int method,
										   IntPtr buffer,
										   int bufferSize,
										   out int r0,
										   out int used);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_ObjectMiscOp_GetR0")]
		internal static extern IntPtr Toolbox_ObjectMiscOp_GetR0 (uint flags,
									  uint objectID,
									  int method,
									  out uint r0);


		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_ObjectMiscOp_SetR3R4")]
		internal static extern IntPtr Toolbox_ObjectMiscOp_SetR3R4 (uint flags,
									    uint objectID,
									    int method,
									    uint r3,
									    uint r4);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_ObjectMiscOp_SetR3R4")]
		internal static extern IntPtr Toolbox_ObjectMiscOp_SetR3R4 (uint flags,
									    uint objectID,
									    int method,
									    uint r3,
									    string r4);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_ObjectMiscOp_SetR3R4R5R6")]
		internal static extern IntPtr Toolbox_ObjectMiscOp_SetR3R4R5R6 (uint flags,
										uint objectID,
										int method,
										uint r3,
										int r4,
										int r5,
										int r6);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_ObjectMiscOp_GetR0R1")]
		internal static extern IntPtr Toolbox_ObjectMiscOp_GetR0R1 (uint flags,
									    uint objectID,
									    int method,
									    out uint r0,
									    out uint r1);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Toolbox_ObjectMiscOp_GetR0R1R2")]
		internal static extern IntPtr Toolbox_ObjectMiscOp_GetR0R1R2 (uint flags,
									      uint objectID,
									      int method,
									      out int r0,
									      out int r1,
									      out int r2);
	}
}

