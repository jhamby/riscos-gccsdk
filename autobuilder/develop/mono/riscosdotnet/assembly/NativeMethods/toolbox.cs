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

		[DllImport("libriscosdotnet.so.1", EntryPoint="xtoolbox_hide_object")]
		internal static extern IntPtr Toolbox_HideObject (uint flags,
								  uint objectID);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xtoolbox_get_object_info")]
		internal static extern IntPtr Toolbox_GetObjectState (uint flags, uint objectID, out uint state);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xtoolbox_set_client_handle")]
		internal static extern IntPtr Toolbox_SetClientHandle (uint flags,
								       uint objectID,
								       IntPtr handle);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xtoolbox_template_look_up")]
		internal static extern IntPtr Toolbox_TemplateLookUp (uint flags,
								      string resName,
								      out IntPtr templateOut);

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

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Object_SetR3")]
		internal static extern IntPtr Window_SetExtent (uint flags, uint WindowID, NativeOS.Rect extent);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Object_GetR0")]
		internal static extern IntPtr Window_GetExtent (uint flags, uint WindowID, out NativeOS.Rect extent);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Object_SetR3")]
		internal static extern IntPtr Window_ForceRedraw (uint flags, uint WindowID, NativeOS.Rect extent);

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
								    uint wimpHandle,
								    uint wimpIcon,
								    out uint objectID,
								    out uint cmpID);

		// Toolbox Menu SWIs
		[DllImport("libriscosdotnet.so.1", EntryPoint="xmenu_set_tick")]
		internal static extern IntPtr Menu_SetTick (uint flags,
							    uint menuID,
							    uint menuEntry,
							    bool tick);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xmenu_get_tick")]
		internal static extern IntPtr Menu_GetTick (uint flags,
							    uint menuID,
							    uint menuEntry,
							    out bool ticked);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xmenu_set_fade")]
		internal static extern IntPtr Menu_SetFade (uint flags,
							    uint menuID,
							    uint menuEntry,
							    bool fade);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xmenu_get_fade")]
		internal static extern IntPtr Menu_GetFade (uint flags,
							    uint menuID,
							    uint menuEntry,
							    out bool faded);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xmenu_set_entry_text")]
		internal static extern IntPtr Menu_SetEntryText (uint flags,
								 uint menuID,
								 uint menuEntry,
								 string text);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xmenu_get_entry_text")]
		internal static extern IntPtr Menu_GetEntryText (uint flags,
								 uint menuID,
								 uint menuEntry,
								 StringBuilder buffer,
								 int bufferSize,
								 out int used);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xmenu_set_entry_sprite")]
		internal static extern IntPtr Menu_SetEntrySprite (uint flags,
								   uint menuID,
								   uint menuEntry,
								   string spriteName);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xmenu_get_entry_sprite")]
		internal static extern IntPtr Menu_GetEntrySprite (uint flags,
								   uint menuID,
								   uint menuEntry,
								   StringBuilder buffer,
								   int bufferSize,
								   out int used);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xmenu_set_sub_menu_show")]
		internal static extern IntPtr Menu_SetSubMenuShow (uint flags,
								   uint menuID,
								   uint menuEntry,
								   uint submenu);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xmenu_get_sub_menu_show")]
		internal static extern IntPtr Menu_GetSubMenuShow (uint flags,
								   uint menuID,
								   uint menuEntry,
								   out uint submenu);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xmenu_set_sub_menu_action")]
		internal static extern IntPtr Menu_SetSubMenuEvent (uint flags,
								    uint menuID,
								    uint menuEntry,
								    uint eventCode);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xmenu_get_sub_menu_action")]
		internal static extern IntPtr Menu_GetSubMenuEvent (uint flags,
								    uint menuID,
								    uint menuEntry,
								    out uint eventCode);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xmenu_set_click_show")]
		internal static extern IntPtr Menu_SetClickShow (uint flags,
								 uint menuID,
								 uint menuEntry,
								 uint showID,
								 uint showFlags);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xmenu_get_click_show")]
		internal static extern IntPtr Menu_GetClickShow (uint flags,
								 uint menuID,
								 uint menuEntry,
								 out uint showID);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xmenu_set_click_action")]
		internal static extern IntPtr Menu_SetClickEvent (uint flags,
								  uint menuID,
								  uint menuEntry,
								  uint eventCode);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xmenu_get_click_action")]
		internal static extern IntPtr Menu_GetClickEvent (uint flags,
								  uint menuID,
								  uint menuEntry,
								  out uint eventCode);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xmenu_set_help_message")]
		internal static extern IntPtr Menu_SetHelpMessage (uint flags,
								   uint menuID,
								   string messageText);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xmenu_get_help_message")]
		internal static extern IntPtr Menu_GetHelpMessage (uint flags,
								   uint menuID,
								   StringBuilder buffer,
								   int bufferSize,
								   out int used);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xmenu_set_entry_help_message")]
		internal static extern IntPtr Menu_SetEntryHelpMessage (uint flags,
									uint menuID,
									uint menuEntry,
									string messageText);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xmenu_get_entry_help_message")]
		internal static extern IntPtr Menu_GetEntryHelpMessage (uint flags,
									uint menuID,
									uint menuEntry,
									StringBuilder buffer,
									int bufferSize,
									out int used);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xmenu_add_entry")]
		internal static extern IntPtr Menu_AddEntry (uint flags,
							     uint menuID,
							     uint insertPoint,
							     IntPtr newEntry,
							     out uint newEntryID);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xmenu_remove_entry")]
		internal static extern IntPtr Menu_RemoveEntry (uint flags,
								uint menuID,
								uint menuEntry);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xmenu_get_height")]
		internal static extern IntPtr Menu_GetHeight (uint flags,
							      uint menuID,
							      out int height);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xmenu_get_width")]
		internal static extern IntPtr Menu_GetWidth (uint flags,
							     uint menuID,
							     out int width);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xmenu_set_title")]
		internal static extern IntPtr Menu_SetTitle (uint flags,
							     uint menuID,
							     string title);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xmenu_get_title")]
		internal static extern IntPtr Menu_GetTitle (uint flags,
							     uint menuID,
							     StringBuilder title,
							     int bufferSize,
							     out int used);

		// Toolbox Gadget SWIs
		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Component_SetR4")]
		internal static extern IntPtr Gadget_SetFocus (uint flags,
							       uint windowID,
							       uint componentID,
							       uint unused);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Component_SetR4")]
		internal static extern IntPtr Gadget_MoveGadget (uint flags,
								 uint windowID,
								 int method,
								 uint componentID,
								 ref NativeOS.Rect bbox);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Component_SetR4")]
		internal static extern IntPtr Gadget_GetBBox (uint flags,
							      uint windowID,
							      int method,
							      uint componentID,
							      out NativeOS.Rect bbox);

		// Generic methods that can be used to call different Toolbox methods that
		// operate on gadgets/components.
		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Component_SetR4")]
		internal static extern IntPtr Component_SetText (uint flags,
								 uint windowID,
								 int method,
								 uint cmpID,
								 string messageText);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Component_GetText")]
		internal static extern IntPtr Component_GetText (uint flags,
								 uint windowID,
								 int method,
								 uint cmpID,
								 StringBuilder buffer,
								 int bufferSize,
								 out int used);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Component_SetR4")]
		internal static extern IntPtr Component_SetR4 (uint flags,
							       uint windowID,
							       int method,
							       uint cmpID,
							       uint r4);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Component_SetR4R5")]
		internal static extern IntPtr Component_SetR4R5 (uint flags,
								 uint windowID,
								 int method,
								 uint cmpID,
								 uint r4,
								 uint r5);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Component_GetR0")]
		internal static extern IntPtr Component_GetR0 (uint flags,
							       uint windowID,
							       int method,
							       uint cmpID,
							       out uint r0);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Component_GetR0R1")]
		internal static extern IntPtr Component_GetR0R1 (uint flags,
								 uint windowID,
								 int method,
								 uint cmpID,
								 out uint r0,
								 out uint r1);

		// Generic methods that can be used to call different Toolbox methods that
		// operate on Toolbox objects.
		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Object_SetR3")]
		internal static extern IntPtr Object_SetR3 (uint flags,
							    uint objectID,
							    int method,
							    uint r3);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Object_SetR3")]
		internal static extern IntPtr Object_SetText (uint flags,
							      uint objectID,
							      int method,
							      string messageText);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Object_GetText")]
		internal static extern IntPtr Object_GetText (uint flags,
							      uint objectID,
							      int method,
							      StringBuilder buffer,
							      int bufferSize,
							      out int used);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Object_GetR0")]
		internal static extern IntPtr Object_GetR0 (uint flags,
							    uint objectID,
							    int method,
							    out uint r0);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Object_SetR3R4")]
		internal static extern IntPtr Object_SetR3R4 (uint flags,
							      uint windowID,
							      int method,
							      uint r3,
							      uint r4);

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Object_GetR0R1")]
		internal static extern IntPtr Object_GetR0R1 (uint flags,
							      uint windowID,
							      int method,
							      out uint r0,
							      out uint r1);
	}
}

