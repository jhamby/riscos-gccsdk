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

		[DllImport("libriscosdotnet.so.1", EntryPoint="xtoolbox_set_client_handle")]
		internal static extern IntPtr Toolbox_SetClientHandle (uint flags,
								       uint objectID,
								       IntPtr handle);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xtoolbox_template_look_up")]
		internal static extern IntPtr Toolbox_TemplateLookUp (uint flags,
								      string resName,
								      out IntPtr templateOut);

		// Toolbox Window SWIs
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

		// Toolbox Iconbar SWIs
		[DllImport("libriscosdotnet.so.1", EntryPoint="xiconbar_get_icon_handle")]
		internal static extern IntPtr Iconbar_GetIconHandle (uint flags,
								     uint iconbarID,
								     out uint iconHandleOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xiconbar_set_menu")]
		internal static extern IntPtr Iconbar_SetMenu (uint flags,
							       uint iconbarID,
							       uint menuID);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xiconbar_get_menu")]
		internal static extern IntPtr Iconbar_GetMenu (uint flags,
							       uint iconbarID,
							       out uint menuID);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xiconbar_set_action")]
		internal static extern IntPtr Iconbar_SetEvent (uint flags,
								uint iconbarID,
								uint selectEventCode,
								uint adjustEventCode);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xiconbar_get_action")]
		internal static extern IntPtr Iconbar_GetEvent (uint flags,
								uint iconbarID,
								out uint selectEventCodeOut,
								out uint adjustEventCodeOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xiconbar_set_show")]
		internal static extern IntPtr Iconbar_SetShow (uint flags,
							       uint iconbarID,
							       uint selectObjectID,
							       uint adjustObjectID);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xiconbar_get_show")]
		internal static extern IntPtr Iconbar_GetShow (uint flags,
							       uint iconbarID,
							       out uint selectObjectIDOut,
							       out uint adjustObjectIDOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xiconbar_set_help_message")]
		internal static extern IntPtr Iconbar_SetHelpMessage (uint flags,
								      uint iconbarID,
								      string message_text);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xiconbar_get_help_message")]
		internal static extern IntPtr Iconbar_GetHelpMessage (uint flags,
								      uint iconbarID,
								      StringBuilder buffer,
								      int bufferSize,
								      out int used);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xiconbar_set_text")]
		internal static extern IntPtr Iconbar_SetText (uint flags,
							       uint iconbarID,
							       string text);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xiconbar_get_text")]
		internal static extern IntPtr Iconbar_GetText (uint flags,
							       uint iconbarID,
							       StringBuilder buffer,
							       int bufferSize,
							       out int used);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xiconbar_set_sprite")]
		internal static extern IntPtr Iconbar_SetSprite (uint flags,
								 uint iconbarID,
								 string spriteName);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xiconbar_get_sprite")]
		internal static extern IntPtr Iconbar_GetSprite (uint flags,
								 uint iconbarID,
								 StringBuilder buffer,
								 int bufferSize,
								 out int used);

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
								 Toolbox.Menu.ClickShowFlags showFlags);

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
	}
}

