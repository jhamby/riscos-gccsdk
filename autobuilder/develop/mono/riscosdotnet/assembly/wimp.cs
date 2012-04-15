//
// wimp.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Runtime.InteropServices;
using System.Collections;
using riscos;
using riscos.Wimp;

namespace riscos
{
	namespace Wimp
	{
		public enum PollCode
		{
			Null,
			RedrawWindow,
			OpenWindow,
			CloseWindow,
			PointerLeaveWindow,
			PointerEnterWindow,
			MouseClick,
			UserDragBox,
			KeyPressed,
			MenuSelection,
			ScrollRequest,
			LoseCaret,
			GainCaret,
			PollwordNonZero,
			UserMessage=17,
			UserMessageRecorded,
			UserMessageAck,
			ToolboxEvent=512
		}

		public enum PollMasks :uint
		{
			Null = 1 << 0,
			RedrawWindow = 1 << 1,
			PointerLeaveWindow = 1 << 4,
			PointerEnterWindow = 1 << 5,
			MouseClick = 1 << 6,
			KeyPressed = 1 << 8,
			LoseCaret = 1 << 11,
			GainCaret = 1 << 12,
			PollWordNonZero = 1 << 13,
			UserMessage = 1 << 17,
			UserMessageRecorded = 1 << 18,
			UserMessageAck = 1 << 19,
			R3IsPollword = 1 << 22,
			ScanPollword = 1 << 23,
			SaveFP = 1 << 24
		}

		public enum Furniture :int
		{
			Whole,
			Back,
			Close,
			Title,
			Toggle,
			VScroll,
			Size,
			HScroll,
			Iconise
		}

		public static class WindowStackPosition
		{
			public const uint Top = 0xffffffff;
			public const uint Bottom = 0xfffffffe;
			public const uint Iconised = 0xfffffffd;
		}

		public static class IconbarPosition
		{
			public const uint RightSide = 0xffffffff;
			public const uint LeftSide = 0xfffffffe;
			public const uint LeftOfR0 = 0xfffffffd;
			public const uint RightOfR0 = 0xfffffffc;
			public const uint LeftHigh = 0xfffffffb;
			public const uint LeftLow = 0xfffffffa;
			public const uint RightLow = 0xfffffff9;
			public const uint RightHigh = 0xfffffff8;
		}
	
		public class RedrawEventArgs : EventArgs
		{
			public RedrawWindowBlock RedrawWimpBlock;

			// This is filled in by the main event handler after
			//  SWI "WIMP_RedrawWindow" has been called.
			public OS.Coord Origin;

			public RedrawEventArgs (IntPtr unmanaged_event_block)
			{
				RedrawWimpBlock = (RedrawWindowBlock)Marshal.PtrToStructure (
							unmanaged_event_block, typeof(RedrawWindowBlock));
			}
		}

		public class OpenEventArgs : EventArgs
		{
			public WindowStateBlock OpenWimpBlock;

			public OpenEventArgs (IntPtr unmanaged_event_block)
			{
				OpenWimpBlock = (WindowStateBlock)Marshal.PtrToStructure(
							unmanaged_event_block, typeof(WindowStateBlock));
			}
		}

		public class CloseEventArgs : EventArgs
		{
			public WindowHandleBlock CloseWimpBlock;

			public CloseEventArgs (IntPtr unmanaged_event_block)
			{
				CloseWimpBlock = (WindowHandleBlock)Marshal.PtrToStructure (
							unmanaged_event_block, typeof(WindowHandleBlock));
			}
		}

		public class PointerLeaveEventArgs : EventArgs
		{
			public WindowHandleBlock PointerLeaveWimpBlock;

			public PointerLeaveEventArgs (IntPtr unmanaged_event_block)
			{
				PointerLeaveWimpBlock = (WindowHandleBlock)Marshal.PtrToStructure (
							unmanaged_event_block, typeof(WindowHandleBlock));
			}
		}

		public class PointerEnterEventArgs : EventArgs
		{
			public WindowHandleBlock PointerEnterWimpBlock;

			public PointerEnterEventArgs (IntPtr unmanaged_event_block)
			{
				PointerEnterWimpBlock = (WindowHandleBlock)Marshal.PtrToStructure (
							unmanaged_event_block, typeof(WindowHandleBlock));
			}
		}

		public class MouseClickEventArgs : EventArgs
		{
			public MouseClickBlock MouseClickWimpBlock;

			public MouseClickEventArgs (IntPtr unmanaged_event_block)
			{
				MouseClickWimpBlock = (MouseClickBlock)Marshal.PtrToStructure (
							unmanaged_event_block, typeof(MouseClickBlock));
			}
		}

		public class KeyPressedEventArgs : EventArgs
		{
			public KeyPressedBlock KeyPressedWimpBlock;

			public KeyPressedEventArgs (IntPtr unmanaged_event_block)
			{
				KeyPressedWimpBlock = (KeyPressedBlock)Marshal.PtrToStructure (
							unmanaged_event_block, typeof(KeyPressedBlock));
			}
		}

		public class ScrollRequestEventArgs : EventArgs
		{
			public ScrollRequestBlock ScrollRequestWimpBlock;

			public ScrollRequestEventArgs (IntPtr unmanaged_event_block)
			{
				ScrollRequestWimpBlock = (ScrollRequestBlock)Marshal.PtrToStructure (
							unmanaged_event_block, typeof(ScrollRequestBlock));
			}
		}

		public class LoseCaretEventArgs : EventArgs
		{
			public LoseCaretBlock LoseCaretWimpBlock;

			public LoseCaretEventArgs (IntPtr unmanaged_event_block)
			{
				LoseCaretWimpBlock = (LoseCaretBlock)Marshal.PtrToStructure (
							unmanaged_event_block, typeof(LoseCaretBlock));
			}
		}

		public class GainCaretEventArgs : EventArgs
		{
			public GainCaretBlock GainCaretWimpBlock;

			public GainCaretEventArgs (IntPtr unmanaged_event_block)
			{
				GainCaretWimpBlock = (GainCaretBlock)Marshal.PtrToStructure (
							unmanaged_event_block, typeof(GainCaretBlock));
			}
		}

		public delegate void RedrawEventHandler (object sender, RedrawEventArgs args);
		public delegate void OpenEventHandler (object sender, OpenEventArgs args);
		public delegate void CloseEventHandler (object sender, CloseEventArgs args);
		public delegate void PointerLeaveEventHandler (object sender, PointerLeaveEventArgs args);
		public delegate void PointerEnterEventHandler (object sender, PointerEnterEventArgs args);
		public delegate void MouseClickEventHandler (object sender, MouseClickEventArgs args);
		public delegate void KeyPressedEventHandler (object sender, KeyPressedEventArgs args);
		public delegate void ScrollRequestEventHandler (object sender, ScrollRequestEventArgs args);
		public delegate void LoseCaretEventHandler (object sender, LoseCaretEventArgs args);
		public delegate void GainCaretEventHandler (object sender, GainCaretEventArgs args);

		public class Event
		{
			public PollCode type;

			public Event (PollCode type)
			{
				this.type = type;
			}

			public static Event GetEvent ()
			{
				PollCode type = NativeMethods.wimp_get_event_type ();
				IntPtr event_ptr = NativeMethods.wimp_get_event ();

				switch (type)
				{
					case PollCode.RedrawWindow:
						return new RedrawWindowEvent (type, event_ptr);
					case PollCode.OpenWindow:
						return new OpenWindowEvent (type, event_ptr);
					case PollCode.CloseWindow:
						return new CloseWindowEvent (type, event_ptr);
					case PollCode.PointerLeaveWindow:
						return new PointerLeaveEvent (type, event_ptr);
					case PollCode.PointerEnterWindow:
						return new PointerEnterEvent (type, event_ptr);
					case PollCode.MouseClick:
						return new MouseClickEvent (type, event_ptr);
					case PollCode.KeyPressed:
						return new KeyPressedEvent (type, event_ptr);
					case PollCode.ScrollRequest:
						return new ScrollRequestEvent (type, event_ptr);
					case PollCode.LoseCaret:
						return new LoseCaretEvent (type, event_ptr);
					case PollCode.GainCaret:
						return new GainCaretEvent (type, event_ptr);
					case PollCode.Null:
					default:
						return new Event (type);
				}
			}
		}

		// Managed versions of the standard WIMP events. These can be passed
		// by reference and contain an unmanaged version that can be passed
		// directly to SWIs.
		public class OpenWindowEvent : Event
		{
			public OpenEventArgs OpenWindowArgs;

			public OpenWindowEvent (PollCode type, IntPtr unmanaged_event_block) : base (type)
			{
				OpenWindowArgs = new OpenEventArgs (unmanaged_event_block);
			}
		}

		public class CloseWindowEvent : Event
		{
			public CloseEventArgs CloseWindowArgs;

			public CloseWindowEvent (PollCode type, IntPtr unmanaged_event_block) : base (type)
			{
				CloseWindowArgs = new CloseEventArgs (unmanaged_event_block);
			}
		}

		public class RedrawWindowEvent : Event
		{
			public RedrawEventArgs RedrawArgs;

			public RedrawWindowEvent (PollCode type, IntPtr unmanaged_event_block) : base (type)
			{
				RedrawArgs = new RedrawEventArgs (unmanaged_event_block);
			}
		}

		public class PointerLeaveEvent : Event
		{
			public PointerLeaveEventArgs PointerLeaveArgs;

			public PointerLeaveEvent (PollCode type, IntPtr unmanaged_event_block) : base (type)
			{
				PointerLeaveArgs = new PointerLeaveEventArgs (unmanaged_event_block);
			}
		}

		public class PointerEnterEvent : Event
		{
			public PointerEnterEventArgs PointerEnterArgs;

			public PointerEnterEvent (PollCode type, IntPtr unmanaged_event_block) : base (type)
			{
				PointerEnterArgs = new PointerEnterEventArgs (unmanaged_event_block);
			}
		}

		public class MouseClickEvent : Event
		{
			public MouseClickEventArgs MouseClickArgs;

			public MouseClickEvent (PollCode type, IntPtr unmanaged_event_block) : base (type)
			{
				MouseClickArgs = new MouseClickEventArgs (unmanaged_event_block);
			}
		}

		public class KeyPressedEvent : Event
		{
			public KeyPressedEventArgs KeyPressedArgs;

			public KeyPressedEvent (PollCode type, IntPtr unmanaged_event_block) : base (type)
			{
				KeyPressedArgs = new KeyPressedEventArgs (unmanaged_event_block);
			}
		}

		public class ScrollRequestEvent : Event
		{
			public ScrollRequestEventArgs ScrollRequestArgs;

			public ScrollRequestEvent (PollCode type, IntPtr unmanaged_event_block) : base (type)
			{
				ScrollRequestArgs = new ScrollRequestEventArgs (unmanaged_event_block);
			}
		}

		public class LoseCaretEvent : Event
		{
			public LoseCaretEventArgs LoseCaretArgs;

			public LoseCaretEvent (PollCode type, IntPtr unmanaged_event_block) : base (type)
			{
				LoseCaretArgs = new LoseCaretEventArgs (unmanaged_event_block);
			}
		}

		public class GainCaretEvent : Event
		{
			public GainCaretEventArgs GainCaretArgs;

			public GainCaretEvent (PollCode type, IntPtr unmanaged_event_block) : base (type)
			{
				GainCaretArgs = new GainCaretEventArgs (unmanaged_event_block);
			}
		}

		// Unmanaged versions of the standard WIMP events. These can be passed
		// directly to SWIs.
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
			public int MouseX;
			public int MouseY;
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
		public struct KeyPressedBlock
		{
			public uint WindowHandle;
			public int IconHandle;
			public int XOffset;
			public int YOffset;
			public int CaretHeight;
			public int CaretIndex;
			public int CharCode;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct MenuSelectionBlock
		{
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
		public struct LoseCaretBlock
		{
			public uint WindowHandle;
			public int IconHandle;
			public int XOffset;
			public int YOffset;
			public int CaretHeight;
			public int CaretIndex;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct GainCaretBlock
		{
			public uint WindowHandle;
			public int IconHandle;
			public int XOffset;
			public int YOffset;
			public int CaretHeight;
			public int CaretIndex;
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

			public WindowBlock (IconBuffer title)
			{
				Visible.MinX = 0;
				Visible.MinY = 0;
				Visible.MaxX = 0;
				Visible.MaxY = 0;
				Scroll.X = 0;
				Scroll.Y = 0;
				BehindWindow = WindowStackPosition.Top;
				Flags = 0xff000042;
				TitleFg = 7;
				TitleBg = 2;
				WorkAreaFg = 7;
				WorkAreaBg = 0;
				ScrollBarOuterColour = 3;
				ScrollBarInnerColour = 1;
				TitleFocusColour = 12;
				ExtraFlags = 0;
				WorkArea.MinX = 0;
				WorkArea.MinY = 0;
				WorkArea.MaxX = 1000;
				WorkArea.MaxY = 1000;
				TitleFlags = 0x13d;
				WorkAreaButtonFlags = 0;
				SpriteArea = (IntPtr)1;
				MinWidth = 0;
				MinHeight = 0;
				TitleBuffer = title.Buffer;
				TitleValidation = (IntPtr)1;
				TitleBufferLen = title.BufferSize;
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
						OS.Rect boundingBox,
						uint flags,
						IconData data)
			{
				WindowHandle = windowHandle;
				Block.BoundingBox.MinX = boundingBox.MinX;
				Block.BoundingBox.MinY = boundingBox.MinY;
				Block.BoundingBox.MaxX = boundingBox.MaxX;
				Block.BoundingBox.MaxY = boundingBox.MaxY;
				Block.Flags = flags;
				Block.TextBuffer = data.Text.Buffer;
				Block.ValidationString = data.Validation.Buffer;
				Block.TextBufferLength = data.Text.BufferSize;
			}
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct IconDeleteBlock
		{
			public uint WindowHandle;
			public uint IconHandle;
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
			public NativeOS.Rect Pos;
			public uint Buttons;
			public uint WindowHandle;
			public uint IconHandle;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct CaretBlock
		{
			public uint WindowHandle;
			public uint IconHandle;
			public NativeOS.Coord Pos;
			public int height;
			public int index;
		}
	}
}
