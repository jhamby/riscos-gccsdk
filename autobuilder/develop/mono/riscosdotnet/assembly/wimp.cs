//
// wimp.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Runtime.InteropServices;
using System.Collections;
using riscos;

namespace riscos
{
	public static partial class Wimp
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
			public NativeWimp.RedrawWindowBlock RedrawWimpBlock;

			// This is filled in by the main event handler after
			//  SWI "WIMP_RedrawWindow" has been called.
			public OS.Coord Origin;

			public RedrawEventArgs (IntPtr unmanaged_event_block)
			{
				RedrawWimpBlock = (NativeWimp.RedrawWindowBlock)Marshal.PtrToStructure (
							unmanaged_event_block, typeof(NativeWimp.RedrawWindowBlock));
			}
		}

		public class OpenEventArgs : EventArgs
		{
			public NativeWimp.WindowStateBlock OpenWimpBlock;

			public OpenEventArgs (IntPtr unmanaged_event_block)
			{
				OpenWimpBlock = (NativeWimp.WindowStateBlock)Marshal.PtrToStructure(
							unmanaged_event_block, typeof(NativeWimp.WindowStateBlock));
			}
		}

		public class CloseEventArgs : EventArgs
		{
			public NativeWimp.WindowHandleBlock CloseWimpBlock;

			public CloseEventArgs (IntPtr unmanaged_event_block)
			{
				CloseWimpBlock = (NativeWimp.WindowHandleBlock)Marshal.PtrToStructure (
							unmanaged_event_block, typeof(NativeWimp.WindowHandleBlock));
			}
		}

		public class PointerLeaveEventArgs : EventArgs
		{
			public NativeWimp.WindowHandleBlock PointerLeaveWimpBlock;

			public PointerLeaveEventArgs (IntPtr unmanaged_event_block)
			{
				PointerLeaveWimpBlock = (NativeWimp.WindowHandleBlock)Marshal.PtrToStructure (
							unmanaged_event_block, typeof(NativeWimp.WindowHandleBlock));
			}
		}

		public class PointerEnterEventArgs : EventArgs
		{
			public NativeWimp.WindowHandleBlock PointerEnterWimpBlock;

			public PointerEnterEventArgs (IntPtr unmanaged_event_block)
			{
				PointerEnterWimpBlock = (NativeWimp.WindowHandleBlock)Marshal.PtrToStructure (
							unmanaged_event_block, typeof(NativeWimp.WindowHandleBlock));
			}
		}

		public class MouseClickEventArgs : EventArgs
		{
			public NativeWimp.MouseClickBlock MouseClickWimpBlock;

			public MouseClickEventArgs (IntPtr unmanaged_event_block)
			{
				MouseClickWimpBlock = (NativeWimp.MouseClickBlock)Marshal.PtrToStructure (
							unmanaged_event_block, typeof(NativeWimp.MouseClickBlock));
			}
		}

		public class KeyPressedEventArgs : EventArgs
		{
			public NativeWimp.KeyPressedBlock KeyPressedWimpBlock;

			public KeyPressedEventArgs (IntPtr unmanaged_event_block)
			{
				KeyPressedWimpBlock = (NativeWimp.KeyPressedBlock)Marshal.PtrToStructure (
							unmanaged_event_block, typeof(NativeWimp.KeyPressedBlock));
			}
		}

		public class ScrollRequestEventArgs : EventArgs
		{
			public NativeWimp.ScrollRequestBlock ScrollRequestWimpBlock;

			public ScrollRequestEventArgs (IntPtr unmanaged_event_block)
			{
				ScrollRequestWimpBlock = (NativeWimp.ScrollRequestBlock)Marshal.PtrToStructure (
							unmanaged_event_block, typeof(NativeWimp.ScrollRequestBlock));
			}
		}

		public class LoseCaretEventArgs : EventArgs
		{
			public NativeWimp.LoseCaretBlock LoseCaretWimpBlock;

			public LoseCaretEventArgs (IntPtr unmanaged_event_block)
			{
				LoseCaretWimpBlock = (NativeWimp.LoseCaretBlock)Marshal.PtrToStructure (
							unmanaged_event_block, typeof(NativeWimp.LoseCaretBlock));
			}
		}

		public class GainCaretEventArgs : EventArgs
		{
			public NativeWimp.GainCaretBlock GainCaretWimpBlock;

			public GainCaretEventArgs (IntPtr unmanaged_event_block)
			{
				GainCaretWimpBlock = (NativeWimp.GainCaretBlock)Marshal.PtrToStructure (
							unmanaged_event_block, typeof(NativeWimp.GainCaretBlock));
			}
		}

		public class MenuSelectionEventArgs : EventArgs
		{
			public NativeWimp.MenuSelectionBlock MenuSelectionWimpBlock;

			public MenuSelectionEventArgs (IntPtr unmanaged_event_block)
			{
				MenuSelectionWimpBlock = (NativeWimp.MenuSelectionBlock)Marshal.PtrToStructure (
							    unmanaged_event_block, typeof(NativeWimp.MenuSelectionBlock));
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
					case PollCode.MenuSelection:
						return new MenuSelectionEvent (type, event_ptr);
					case PollCode.ToolboxEvent:
						return new Toolbox.ToolboxEvent (type, event_ptr);
					case PollCode.Null:
					default:
						return new Event (type);
				}
			}
		}

		public static void ProcessKey (int key)
		{
			OS.ThrowOnError (NativeMethods.Wimp_ProcessKey (key));
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

		public class MenuSelectionEvent : Event
		{
			public MenuSelectionEventArgs MenuSelectionArgs;

			public MenuSelectionEvent (PollCode type, IntPtr unmanaged_event_block) : base (type)
			{
				MenuSelectionArgs = new MenuSelectionEventArgs (unmanaged_event_block);
			}
		}
	}
}
