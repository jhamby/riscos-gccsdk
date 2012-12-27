//
// NativeMethods/wimp.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Runtime.InteropServices;
using riscos;

namespace riscos
{
	public static class NativeWimp
	{
		[StructLayout(LayoutKind.Sequential)]
		public struct RedrawWindowBlock
		{
			public uint WindowHandle;
			public NativeOS.Rect Visible;
			public NativeOS.Coord Scroll;
			public NativeOS.Rect Clip;

			// Constructor for WIMP_RedrawWindow
			public RedrawWindowBlock (uint handle)
			{
				WindowHandle = handle;
				Visible.MinX =
				Visible.MinY =
				Visible.MaxX =
				Visible.MaxY = 0;
				Scroll.X = Scroll.Y = 0;
				Clip.MinX =
				Clip.MinY =
				Clip.MaxX =
				Clip.MaxY = 0;
			}
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct WindowStateBlock
		{
			public uint WindowHandle;
			public NativeOS.Rect Visible;
			public NativeOS.Coord Scroll;
			public uint BehindWindow;
			public uint Flags; // Used for nested windows

//			public WindowStateBlock (uint handle)
//			{
//				WindowHandle = handle;
//			}

			public WindowStateBlock (uint handle,
						OS.Rect rect,
						OS.Coord scroll,
						uint behind_window,
						uint window_flags)
			{
				WindowHandle = handle;
				Visible.MinX = rect.MinX;
				Visible.MinY = rect.MinY;
				Visible.MaxX = rect.MaxX;
				Visible.MaxY = rect.MaxY;

				Scroll.X = scroll.Y;
				Scroll.Y = scroll.Y;

				BehindWindow = behind_window;
				Flags = window_flags;
			}
		}

		// A common structure that contains just a window handle.
		// Used by several SWIs and WIMP events.
		[StructLayout(LayoutKind.Sequential)]
		public struct WindowHandleBlock
		{
			public uint WindowHandle;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct MouseClickBlock
		{
			public NativeOS.Coord Pos;
			public uint Buttons;
			public uint WindowHandle;
			public int IconHandle;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct DragBoxBlock
		{
			public int MinX;
			public int MinY;
			public int MaxX;
			public int MaxY;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct KeyPressBlock
		{
			public CaretBlock Caret;
			public int CharCode;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct MenuSelectionBlock
		{
			// A size of 10 is an arbitary choice, in theory it could be the size of the
			// WIMP block, however, in practice the depth of menu selections is likely to
			// be small, so the less data we marshal the better the performance.
			[MarshalAs(UnmanagedType.ByValArray, SizeConst=10)]
			public int[] Selection;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct ScrollRequestBlock
		{
			public uint WindowHandle;
			public int MinX;
			public int MinY;
			public int MaxX;
			public int Maxy;
			public int ScrollXOffset;
			public int ScrollYOffset;
			public uint BehindWindow;
			public int ScrollXDir;
			public int ScrollYDir;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct PollWordNonZeroBlock
		{
			public IntPtr PollWordAddr;
			public int PollWord;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct WindowBlock
		{
			public NativeOS.Rect Visible;
			public NativeOS.Coord Scroll;
			public uint BehindWindow;
			public uint Flags;
			public byte TitleFg;
			public byte TitleBg;
			public byte WorkAreaFg;
			public byte WorkAreaBg;
			public byte ScrollBarOuterColour;
			public byte ScrollBarInnerColour;
			public byte TitleFocusColour;
			public byte ExtraFlags;
			public NativeOS.Rect WorkArea;
			public uint TitleFlags;
			public uint WorkAreaButtonFlags;
			public IntPtr SpriteArea;
			public short MinWidth;
			public short MinHeight;
			public IntPtr TitleBuffer;
			public IntPtr TitleValidation;
			public int TitleBufferLen;
			public int IconCount;

			public WindowBlock (Wimp.WindowAttributes attr)
			{
				// The first seven entries are not used during window creation.
				Visible.MinX = 0;
				Visible.MinY = 0;
				Visible.MaxX = 0;
				Visible.MaxY = 0;
				Scroll.X = 0;
				Scroll.Y = 0;
				BehindWindow = Wimp.WindowStackPosition.Top;

				// We only support indirected titles.
				attr.TitleIndirected = true;

				Flags = (uint)attr.Flags;
				TitleFg = (byte)attr.TitleFGColour;
				TitleBg = (byte)attr.TitleBGColour;
				WorkAreaFg = (byte)attr.WorkAreaFGColour;
				WorkAreaBg = (byte)attr.WorkAreaBGColour;
				ScrollBarOuterColour = (byte)attr.ScrollBarOuterColour;
				ScrollBarInnerColour = (byte)attr.ScrollBarInnerColour;
				TitleFocusColour = (byte)attr.TitleFocusColour;
				ExtraFlags = (byte)attr.ExtraFlags;
				WorkArea.MinX = attr.WorkArea.MinX;
				WorkArea.MinY = attr.WorkArea.MinY;
				WorkArea.MaxX = attr.WorkArea.MaxX;
				WorkArea.MaxY = attr.WorkArea.MaxY;
				TitleFlags = (uint)attr.TitleFlags;
				WorkAreaButtonFlags = (uint)attr.ButtonType << 12;
				SpriteArea = attr.SpriteArea;
				MinWidth = (short)attr.MinimumWidth;
				MinHeight = (short)attr.MinimumHeight;

				if ((attr.TitleFlags & Wimp.WindowTitleFlags.ContainsText) != 0)
				{
					// Indirected, text only.
					// Indirected, text plus sprite.
					TitleBuffer = attr.Title.Text.Buffer;
					TitleValidation = attr.Title.Validation.Buffer;
					TitleBufferLen = attr.Title.Text.BufferSize;
				}
				else if ((attr.TitleFlags & Wimp.WindowTitleFlags.ContainsSprite) != 0)
				{
					// Indirected, sprite only.
					TitleBuffer = attr.Title.Text.Buffer;
					TitleValidation = attr.SpriteArea;
					TitleBufferLen = attr.Title.Text.BufferSize;
				}
				else
				{
					// Keep the compiler happy.
					TitleBuffer = TitleValidation = IntPtr.Zero;
					TitleBufferLen = 0;
				}

				IconCount = 0;
			}
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct IconBlock
		{
			public NativeOS.Rect BoundingBox;
			public uint Flags;
			public IntPtr TextBuffer;
			public IntPtr ValidationString;
			public int TextBufferLength;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct IconCreateBlock
		{
			public uint WindowHandle;
			public IconBlock Block;
		
			public IconCreateBlock (uint windowHandle,
						Wimp.IconAttributes attr)
			{
				WindowHandle = windowHandle;
				Block.BoundingBox = new NativeOS.Rect (attr.BoundingBox);
				Block.Flags = attr.Flags;
				Block.TextBuffer = attr.Data.Text.Buffer;
				Block.ValidationString = attr.Data.Validation.Buffer;
				Block.TextBufferLength = attr.Data.Text.BufferSize;
			}
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct IconDeleteBlock
		{
			public uint WindowHandle;
			public int IconHandle;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct WindowInfoBlock
		{
			public uint WindowHandle;
			public WindowBlock WindowInfo;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct PointerBlock
		{
			public NativeOS.Coord Pos;
			public uint Buttons;
			public uint WindowHandle;
			public int IconHandle;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct CaretBlock
		{
			public uint WindowHandle;
			public int IconHandle;
			public NativeOS.Coord Offset;
			public int Height;
			public int Index;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct DragStartBlock
		{
			public uint WindowHandle;
			public Wimp.DragType DragType;
			public NativeOS.Rect InitialBox;
			public NativeOS.Rect ParentBox;
			// Drag types 8-11 not supported.

			public DragStartBlock (uint handle,
					       Wimp.DragType type,
					       OS.Rect initial,
					       OS.Rect parent)
			{
				WindowHandle = handle;
				DragType = type;
				InitialBox = new NativeOS.Rect (initial);
				ParentBox = new NativeOS.Rect (parent);
			}
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct MessageHeader
		{
			public int Size;
			public uint TaskHandle;
			public int MyRef;
			public int YourRef;
			public int MessageType;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct MessageDataLoad
		{
			public MessageHeader Header;
			public uint WindowHandle;
			public uint IconHandle;
			public NativeOS.Coord Position;
			public int EstimatedSize;
			public int FileType;
			[MarshalAs(UnmanagedType.ByValTStr, SizeConst=212)]
			public string FileName;
		}
	}

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
					ref NativeWimp.WindowBlock block,
				out uint handle);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_delete_window")]
		internal static extern IntPtr Wimp_DeleteWindow (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref NativeWimp.WindowHandleBlock block);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_close_window")]
		internal static extern IntPtr Wimp_CloseWindow (uint windowHandle);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_redraw_window")]
		internal static extern IntPtr Wimp_RedrawWindow (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref NativeWimp.RedrawWindowBlock block,
				out int more);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_update_window")]
		internal static extern IntPtr Wimp_UpdateWindow (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref NativeWimp.RedrawWindowBlock block,
				out int more);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_get_rectangle")]
		internal static extern IntPtr Wimp_GetRectangle (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref NativeWimp.RedrawWindowBlock block,
				out int more);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_set_extent")]
		internal static extern IntPtr Wimp_SetExtent (
				uint handle,
				[In, MarshalAs(UnmanagedType.Struct)]
					ref NativeOS.Rect extent);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_open_window")]
		internal static extern IntPtr Wimp_OpenWindow (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref NativeWimp.WindowStateBlock block);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_open_window_nested")]
		internal static extern IntPtr Wimp_OpenWindowNested (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref NativeWimp.WindowStateBlock block,
				uint parent,
				uint linkage);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_open_window_nested_with_flags")]
		internal static extern IntPtr Wimp_OpenWindowNestedWithFlags (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref NativeWimp.WindowStateBlock block,
				uint parent,
				uint nestedFlags);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_get_window_state")]
		internal static extern IntPtr Wimp_GetWindowState (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref NativeWimp.WindowStateBlock block);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_get_window_state_and_nesting")]
		internal static extern IntPtr Wimp_GetWindowStateAndNesting (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref NativeWimp.WindowStateBlock block,
				out uint parent,
				out uint nestedFlags);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_create_icon")]
		internal static extern IntPtr Wimp_CreateIcon (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref NativeWimp.IconCreateBlock block,
				out int iconHandle);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_create_icon_relative")]
		internal static extern IntPtr Wimp_CreateIconRelative (
				int neighbour,
				[In, MarshalAs(UnmanagedType.Struct)]
					ref NativeWimp.IconCreateBlock block,
				out int iconHandle);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_create_icon_prioritised")]
		internal static extern IntPtr Wimp_CreateIconPrioritised (
				int priority,
				[In, MarshalAs(UnmanagedType.Struct)]
					ref NativeWimp.IconCreateBlock block,
				out int iconHandle);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_delete_icon")]
		internal static extern IntPtr Wimp_DeleteIcon (uint windowHandle,
							       int iconHandle);

		/* Includes icons - difficult to use due to variable size of returned data?  */
		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_get_window_info")]
		internal static extern IntPtr Wimp_GetWindowInfo (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref NativeWimp.WindowInfoBlock block);

		/* Excludes icons.  */
		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_get_window_info_header_only")]
		internal static extern IntPtr Wimp_GetWindowInfoHeaderOnly (
				[In, MarshalAs(UnmanagedType.Struct)]
					ref NativeWimp.WindowInfoBlock block);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_set_icon_state")]
		internal static extern IntPtr Wimp_SetIconState (uint windowHandle,
								 uint iconHandle,
								 uint eorBits,
								 uint clearBits);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_get_pointer_info")]
		internal static extern IntPtr Wimp_GetPointerInfo ([Out, MarshalAs(UnmanagedType.Struct)]
									out NativeWimp.PointerBlock block);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_drag_box")]
		internal static extern IntPtr Wimp_DragBox ([In, MarshalAs(UnmanagedType.Struct)]
								 ref NativeWimp.DragStartBlock block);

		// RISC OS 4+
		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_drag_box_with_flags")]
		internal static extern IntPtr Wimp_DragBox ([In, MarshalAs(UnmanagedType.Struct)]
								 ref NativeWimp.DragStartBlock block,
							    Wimp.DragFlags flags);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_force_redraw")]
		internal static extern IntPtr Wimp_ForceRedraw (uint windowHandle,
								int minX,
								int minY,
								int maxX,
								int maxY);

		// Nested Wimp/RISC OS 3.8+
		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_force_redraw_furniture")]
		internal static extern IntPtr Wimp_ForceRedrawFurniture (uint windowHandle,
									 Wimp.Furniture item);

		// Nested Wimp/RISC OS 3.8+
		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_force_redraw_title")]
		internal static extern IntPtr Wimp_ForceRedrawTitle (uint windowHandle);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_set_caret_position")]
		internal static extern IntPtr Wimp_SetCaretPosition (uint windowHandle,
								     int iconHandle,
								     int x,
								     int y,
								     int height,
								     int index);


		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_get_caret_position")]
		internal static extern IntPtr Wimp_GetCaretPosition (
				[Out, MarshalAs(UnmanagedType.Struct)]
					out NativeWimp.CaretBlock block);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_create_menu")]
		internal static extern IntPtr Wimp_CreateMenu (IntPtr menu,
							       int x,
							       int y);

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

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_report_error")]
		internal static extern IntPtr Wimp_ReportError (ref NativeOS.Error error,
								Wimp.ErrorBoxFlags flags,
								string name,
								out int click);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_plot_icon")]
		internal static extern IntPtr Wimp_PlotIcon ([In, MarshalAs(UnmanagedType.Struct)]
								  ref NativeWimp.IconBlock block);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_resize_icon")]
		internal static extern IntPtr Wimp_ResizeIcon (uint windowHandle,
							       uint iconHandle,
							       int x0,
							       int y0,
							       int x1,
							       int y1);

		// Note that the OSLib veneer has fg and bg the wrong way around
		[DllImport("libriscosdotnet.so.1", EntryPoint="xwimp_set_font_colours")]
		internal static extern IntPtr Wimp_SetFontColours (int bg, int fg);

		// WIMP Support
		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Wimp_GetEventType")]
		internal static extern Wimp.PollCode wimp_get_event_type ();

		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Wimp_GetEvent")]
		internal static extern IntPtr wimp_get_event ();
	}
}
