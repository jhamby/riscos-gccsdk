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
		/*! \brief The poll codes returned by the Wimp from Wimp.Poll and Wimp.PollIdle.  */
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

		/*! \brief The message action numbers that can be sent and received.  */
		public enum MessageAction
		{
			Quit,
			DataSave,
			DataSaveAck,
			DataLoad,
			DataLoadAck,
			DataOpen,
			RAMFetch,
			RAMTransmit,
			PreQuit,
			DataSaved = 13,
			Shutdown
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

		public enum ErrorBoxFlags
		{
			OKIcon = (1 << 0),
			CancelIcon = (1 << 1),
			HighlightCancel = (1 << 2),
			NoPrompt = (1 << 3),
			ShortTitle = (1 << 4),
			LeaveOpen = (1 << 5),
			Close = (1 << 6),
			NoBeep = (1 << 7)
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
	
		/*! \brief Provides data for the event raised when a window's contents need updating.  */
		public class RedrawEventArgs : Wimp.EventArgs
		{
			public NativeWimp.RedrawWindowBlock RedrawWimpBlock;

			// This is filled in by the main event handler after
			//  SWI "WIMP_RedrawWindow" has been called.
			public OS.Coord Origin;

			public RedrawEventArgs (IntPtr unmanagedEventBlock) : base (unmanagedEventBlock)
			{
				RedrawWimpBlock = (NativeWimp.RedrawWindowBlock)Marshal.PtrToStructure (
							unmanagedEventBlock, typeof(NativeWimp.RedrawWindowBlock));
			}

			public override uint GetWindowHandle ()
			{
				return (uint)Marshal.ReadInt32 (RawEventData, 0);
			}
		}

		/*! \brief Provides data for the event raised when a window's position requires updating,
		 * or it needs opening for the first time.  */
		public class OpenEventArgs : Wimp.EventArgs
		{
			public NativeWimp.WindowStateBlock OpenWimpBlock;

			public OpenEventArgs (IntPtr unmanagedEventBlock) : base (unmanagedEventBlock)
			{
				OpenWimpBlock = (NativeWimp.WindowStateBlock)Marshal.PtrToStructure(
							unmanagedEventBlock, typeof(NativeWimp.WindowStateBlock));
			}

			public override uint GetWindowHandle ()
			{
				return (uint)Marshal.ReadInt32 (RawEventData, 0);
			}
		}

		/*! \brief Provides data for the event raised when a window is closed by the user.  */
		public class CloseEventArgs : Wimp.EventArgs
		{
			public NativeWimp.WindowHandleBlock CloseWimpBlock;

			public CloseEventArgs (IntPtr unmanagedEventBlock) : base (unmanagedEventBlock)
			{
				CloseWimpBlock = (NativeWimp.WindowHandleBlock)Marshal.PtrToStructure (
							unmanagedEventBlock, typeof(NativeWimp.WindowHandleBlock));
			}

			public override uint GetWindowHandle ()
			{
				return (uint)Marshal.ReadInt32 (RawEventData, 0);
			}
		}

		/*! \brief Provides data for the event raised when the pointer is moved out of the work area
		 * of a window.  */
		public class PointerLeaveEventArgs : Wimp.EventArgs
		{
			public NativeWimp.WindowHandleBlock PointerLeaveWimpBlock;

			public PointerLeaveEventArgs (IntPtr unmanagedEventBlock) : base (unmanagedEventBlock)
			{
				PointerLeaveWimpBlock = (NativeWimp.WindowHandleBlock)Marshal.PtrToStructure (
							unmanagedEventBlock, typeof(NativeWimp.WindowHandleBlock));
			}

			public override uint GetWindowHandle ()
			{
				return (uint)Marshal.ReadInt32 (RawEventData, 0);
			}
		}

		/*! \brief Provides data for the event raised when the pointer is moved into the work area
		 * of a window.  */
		public class PointerEnterEventArgs : Wimp.EventArgs
		{
			public NativeWimp.WindowHandleBlock PointerEnterWimpBlock;

			public PointerEnterEventArgs (IntPtr unmanagedEventBlock) : base (unmanagedEventBlock)
			{
				PointerEnterWimpBlock = (NativeWimp.WindowHandleBlock)Marshal.PtrToStructure (
							unmanagedEventBlock, typeof(NativeWimp.WindowHandleBlock));
			}

			public override uint GetWindowHandle ()
			{
				return (uint)Marshal.ReadInt32 (RawEventData, 0);
			}
		}

		/*! \brief Provides data for the event raised when a mouse button is clicked.  */
		public class MouseClickEventArgs : Wimp.EventArgs
		{
			public NativeWimp.MouseClickBlock MouseClickWimpBlock;

			public MouseClickEventArgs (IntPtr unmanagedEventBlock) : base (unmanagedEventBlock)
			{
				MouseClickWimpBlock = (NativeWimp.MouseClickBlock)Marshal.PtrToStructure (
							unmanagedEventBlock, typeof(NativeWimp.MouseClickBlock));
			}

			public override uint GetWindowHandle ()
			{
				return (uint)Marshal.ReadInt32 (RawEventData, 12);
			}
		}

		/*! \brief Provides data for the event raised when a key is pressed.  */
		public class KeyPressEventArgs : Wimp.EventArgs
		{
			public NativeWimp.KeyPressBlock KeyPressWimpBlock;

			public KeyPressEventArgs (IntPtr unmanagedEventBlock) : base (unmanagedEventBlock)
			{
				KeyPressWimpBlock = (NativeWimp.KeyPressBlock)Marshal.PtrToStructure (
							unmanagedEventBlock, typeof(NativeWimp.KeyPressBlock));
			}

			public override uint GetWindowHandle ()
			{
				return (uint)Marshal.ReadInt32 (RawEventData, 0);
			}
		}

		/*! \brief Provides data for the event raised when a window requires scrolling manually.  */
		public class ScrollRequestEventArgs : Wimp.EventArgs
		{
			public NativeWimp.ScrollRequestBlock ScrollRequestWimpBlock;

			public ScrollRequestEventArgs (IntPtr unmanagedEventBlock) : base (unmanagedEventBlock)
			{
				ScrollRequestWimpBlock = (NativeWimp.ScrollRequestBlock)Marshal.PtrToStructure (
							unmanagedEventBlock, typeof(NativeWimp.ScrollRequestBlock));
			}

			public override uint GetWindowHandle ()
			{
				return (uint)Marshal.ReadInt32 (RawEventData, 0);
			}
		}

		/*! \brief Provides data for the event raised when a window loses the caret (input focus).  */
		public class LoseCaretEventArgs : Wimp.EventArgs
		{
			public NativeWimp.LoseCaretBlock LoseCaretWimpBlock;

			public LoseCaretEventArgs (IntPtr unmanagedEventBlock) : base (unmanagedEventBlock)
			{
				LoseCaretWimpBlock = (NativeWimp.LoseCaretBlock)Marshal.PtrToStructure (
							unmanagedEventBlock, typeof(NativeWimp.LoseCaretBlock));
			}

			public override uint GetWindowHandle ()
			{
				return (uint)Marshal.ReadInt32 (RawEventData, 0);
			}
		}

		/*! \brief Provides data for the event raised when a window gain the caret (input focus).  */
		public class GainCaretEventArgs : Wimp.EventArgs
		{
			public NativeWimp.GainCaretBlock GainCaretWimpBlock;

			public GainCaretEventArgs (IntPtr unmanagedEventBlock) : base (unmanagedEventBlock)
			{
				GainCaretWimpBlock = (NativeWimp.GainCaretBlock)Marshal.PtrToStructure (
							unmanagedEventBlock, typeof(NativeWimp.GainCaretBlock));
			}

			public override uint GetWindowHandle ()
			{
				return (uint)Marshal.ReadInt32 (RawEventData, 0);
			}
		}

		/*! \brief Provides data for the Wimp Menu Selection event.  */
		public class MenuSelectionEventArgs : Wimp.EventArgs
		{
			public NativeWimp.MenuSelectionBlock MenuSelectionWimpBlock;

			public MenuSelectionEventArgs (IntPtr unmanagedEventBlock) : base (unmanagedEventBlock)
			{
				MenuSelectionWimpBlock = (NativeWimp.MenuSelectionBlock)Marshal.PtrToStructure (
							    unmanagedEventBlock, typeof(NativeWimp.MenuSelectionBlock));
			}
		}

		public class MessageEventArgs : Wimp.EventArgs
		{
			public MessageAction MessageType;

			public MessageEventArgs (IntPtr unmanagedEventBlock) : base (unmanagedEventBlock)
			{
				MessageType = (MessageAction)Marshal.ReadInt32 (RawEventData,
										EventHeaderOffset.MessageType);
			}

			public static class EventHeaderOffset
			{
				public const int Size = 0;
				public const int TaskHandle = 4;
				public const int MyRef = 8;
				public const int YourRef = 12;
				public const int MessageType = 16;
			}
		}

		/*! \brief Provides data for the DataLoad Wimp message.  */
		public class DataLoadMessageEventArgs : MessageEventArgs
		{
			public NativeWimp.MessageDataLoad DataLoad;

			public DataLoadMessageEventArgs (IntPtr unmanagedEventBlock) :
							      base (unmanagedEventBlock)
			{
				DataLoad = (NativeWimp.MessageDataLoad)Marshal.PtrToStructure (
								unmanagedEventBlock,
								typeof (NativeWimp.MessageDataLoad));
			}

			public override uint GetWindowHandle ()
			{
				return (uint)Marshal.ReadInt32 (RawEventData, EventOffset.WindowHandle);
			}

			public static class EventOffset
			{
				public const int WindowHandle = 20;
				public const int IconHandle = 24;
				public const int CoordX = 28;
				public const int CoordY = 32;
				public const int EstimatedSize = 36;
				public const int FileType = 40;
				public const int FileName = 44;
			}
		}

		public delegate void RedrawEventHandler (object sender, RedrawEventArgs e);
		public delegate void OpenEventHandler (object sender, OpenEventArgs e);
		public delegate void CloseEventHandler (object sender, CloseEventArgs e);
		public delegate void PointerLeaveEventHandler (object sender, PointerLeaveEventArgs e);
		public delegate void PointerEnterEventHandler (object sender, PointerEnterEventArgs e);
		public delegate void MouseClickEventHandler (object sender, MouseClickEventArgs e);
		public delegate void KeyPressEventHandler (object sender, KeyPressEventArgs e);
		public delegate void ScrollRequestEventHandler (object sender, ScrollRequestEventArgs e);
		public delegate void LoseCaretEventHandler (object sender, LoseCaretEventArgs e);
		public delegate void GainCaretEventHandler (object sender, GainCaretEventArgs e);
		public delegate void MenuSelectionEventHandler (object sender, MenuSelectionEventArgs e);

		/*! \brief The signature of a DataLoad Message event handler.  */
		public delegate void DataLoadMessageEventHandler (object sender, DataLoadMessageEventArgs e);

		/*! \brief The signature of any Wimp Message where only the message header is present.  */
		public delegate void MessageEventHandler (object sender, MessageEventArgs e);

		public class EventArgs : System.EventArgs
		{
			public PollCode Type;
			public IntPtr RawEventData;

			public EventArgs (IntPtr unmanagedEventBlock)
			{
				Type = NativeMethods.wimp_get_event_type ();
				RawEventData = unmanagedEventBlock;
			}

			/*! \brief Return the handle of the window that the current event is for.
			 * \return 0 if the event is not specific to a particular window.  */
			public virtual uint GetWindowHandle ()
			{
				return 0;
			}

			public static Wimp.EventArgs GetEvent ()
			{
				PollCode type = NativeMethods.wimp_get_event_type ();
				IntPtr event_ptr = NativeMethods.wimp_get_event ();

				switch (type)
				{
					case PollCode.RedrawWindow:
						return new RedrawEventArgs (event_ptr);
					case PollCode.OpenWindow:
						return new OpenEventArgs (event_ptr);
					case PollCode.CloseWindow:
						return new CloseEventArgs (event_ptr);
					case PollCode.PointerLeaveWindow:
						return new PointerLeaveEventArgs (event_ptr);
					case PollCode.PointerEnterWindow:
						return new PointerEnterEventArgs (event_ptr);
					case PollCode.MouseClick:
						return new MouseClickEventArgs (event_ptr);
					case PollCode.KeyPressed:
						return new KeyPressEventArgs (event_ptr);
					case PollCode.ScrollRequest:
						return new ScrollRequestEventArgs (event_ptr);
					case PollCode.LoseCaret:
						return new LoseCaretEventArgs (event_ptr);
					case PollCode.GainCaret:
						return new GainCaretEventArgs (event_ptr);
					case PollCode.MenuSelection:
						return new MenuSelectionEventArgs (event_ptr);
					case PollCode.UserMessage:
					case PollCode.UserMessageRecorded:
						return GetMessage (event_ptr);
					case PollCode.ToolboxEvent:
						return new Toolbox.ToolboxEventArgs (event_ptr);
					case PollCode.Null:
					default:
						return new Wimp.EventArgs (event_ptr);
				}
			}

			static Wimp.EventArgs GetMessage (IntPtr unmanagedEventBlock)
			{
				MessageAction type = (MessageAction)Marshal.
							ReadInt32 (unmanagedEventBlock,
								   MessageEventArgs.EventHeaderOffset.MessageType);
				switch (type)
				{
				case MessageAction.DataLoad:
					return new DataLoadMessageEventArgs (unmanagedEventBlock);
				default:
					return new MessageEventArgs (unmanagedEventBlock);
				}
			}
		}

		/*! \brief Sets the anti-aliased font colours from the two Wimp colours specified.
		 * \param [in] bg The Wimp colour to use for the font background (0-15).
		 * \param [in] fg The Wimp colour to use for the font foreground (0-15).
		 * \return Nothing.  */
		public static void SetFontColours (int bg, int fg)
		{
			OS.ThrowOnError (NativeMethods.Wimp_SetFontColours (bg, fg));
		}

		public static void ProcessKey (int key)
		{
			OS.ThrowOnError (NativeMethods.Wimp_ProcessKey (key));
		}

		/*! \brief Poll the Wimp to see whether certain events have occurred.
		 * \param [in] pollMask Bit-mask indicating which events should not be returned.
		 * \param [out] pollWord Pollword associated with Wimp event 13.
		 * \return The current event returned by SWI Wimp_Poll.  */
		public static Wimp.EventArgs Poll (uint pollMask, out uint pollWord)
		{
			OS.ThrowOnError (NativeMethods.Wimp_Poll (pollMask, out pollWord));
			return EventArgs.GetEvent ();
		}

		/*! \brief Poll the Wimp, sleeping for the specified time unless certain events
		 * occur.
		 * \param [in] pollMask Bit-mask indicating which events should not be returned.
		 * \param [in] time The number of centi-seconds to sleep for.
		 * \param [out] pollWord Pollword associated with Wimp event 13.
		 * \return The current event returned by SWI Wimp_PollIdle.  */
		public static Wimp.EventArgs PollIdle (uint pollMask, uint time, out uint pollWord)
		{
			OS.ThrowOnError (NativeMethods.Wimp_PollIdle (pollMask, time, out pollWord));
			return EventArgs.GetEvent ();
		}

		public static int ReportError (ErrorBoxFlags flags, string name, int errno, string message)
		{
			int result;
			NativeOS.Error err = new NativeOS.Error (errno, message);

			// Don't throw an exception if Wimp_ReportError fails. The exception
			// handler may also call Wimp_ReportError.
			NativeMethods.Wimp_ReportError (ref err,
							flags,
							name,
							out result);
			return result;
		}
	}
}
