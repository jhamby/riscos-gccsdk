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
			Shutdown,
			ModeChange = 0x400C1
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

		/*! \brief The flags used when creating a Wimp window.  */
		public enum WindowFlags
		{
			IsMoveable = (1 << 1),
			NoUserRedraw = (1 << 4),
			IsAPane = (1 << 5),
			CanOpenBeyondScreen = (1 << 6),
			GenerateAutoRepeatScrollRequests = (1 << 8),
			GenerateScrollRequests = (1 << 9),
			GCOLColours = (1 << 10),
			NoWindowsBelow = (1 << 11),
			GenerateHotKeyEvents = (1 << 12),
			KeepOnScreen = (1 << 13),
			IgnoreRightExtent = (1 << 14),
			IgnoreLowerExtent = (1 << 15),

			IsOpen = (1 << 16),
			FullyVisible = (1 << 17),
			IsFullSize = (1 << 18),
			ToggleSizeClicked = (1 << 19),
			HasInputFocus = (1 << 20),
			ForceOnScreen = (1 << 21),

			HasBackIcon = (1 << 24),
			HasCloseIcon = (1 << 25),
			HasTitleBar = (1 << 26),
			HasToggleSizeIcon = (1 << 27),
			HasVerticalScrollBar = (1 << 28),
			HasAdjustSizeIcon = (1 << 29),
			HasHorizontalScrollBar = (1 << 30),
			UseNewControlBits = (1 << 31)
		}

		/*! \brief Extra flags stored at byte offset 39 of the window creation block.  */
		public enum WindowExtraFlags
		{
			/*! \brief Use 24bit colour in title bar validation string. \b RISC \b %OS \b 4.00+  */
			FullColour = (1 << 0),
			/*! \brief Use extended scroll requests. \b RISC \b %OS \b Select \b 2 */
			ExtendedScrollRequests = (1 << 1),
			/*! \brief Window should never be given a 3D border. \b RISC \b %OS \b 4.00+  */
			Never3DBorder = (1 << 2),
			/*! \brief Window should always be given a 3D border. \b RISC \b %OS \b 4.00+  */
			Always3DBorder = (1 << 3),
			/*! \brief Wimp.GetPointerInfo should return shaded icons. \b RISC \b %OS \b 5.00+  */
			ReturnShadedIcons = (1 << 4)
		}

		/*! \brief The flags used for the title bar of a Wimp window when it is created.  */
		public enum WindowTitleFlags
		{
			ContainsText = (1 << 0),
			ContainsSprite = (1 << 1),
			CentredHorizontally = (1 << 3),
			CentredVertically = (1 << 4),
			TextIsAntiAliased = (1 << 6),
			Indirected = (1 << 8),
			RightJustified = (1 << 9)
		}

		/*! \brief The possible button types for a window work area.  */
		public enum WindowButtonType
		{
			/*! \brief Ignore all mouse clicks.  */
			IgnoreAllClicks,
			/*! \brief Notify task continually while pointer is over the work area.  */
			NotifyContinually,
			/*! \brief Click notifies task (auto-repeat).  */
			ClickAutoRepeat,
			/*! \brief Click notifies task (once only).  */
			Click,
			/*! \brief Release over the work area notifies task.  */
			Release,
			/*! \brief Double click notifies task.  */
			DoubleClick,
			/*! \brief As \e Click but can also drag (returns button state * 16).  */
			ClickDrag,
			/*! \brief As \e Release but can also drag (returns button state * 16).  */
			ReleaseDrag,
			/*! \brief As \e Double \e Click but can also drag (returns button state * 16).  */
			DoubleClickDrag,
			/*! \brief Click returns button state * 256.<br>
			 *	   Double Click returns button state * 16.<br>
			 *	   Drag returns button state * 1.  */
			ClickDoubleClickDrag = 10,
			/*! \brief Mouse clicks cause window to gain input focus.  */
			GainInputFocus = 15
		}

		/*! \brief The flags used when creating a icon.  */
		public enum IconFlags
		{
			Text = (1 << 0),
			Sprite = (1 << 1),
			Border = (1 << 2),
			HCentred = (1 << 3),
			VCentred = (1 << 4),
			FilledBackground = (1 << 5),
			AntiAliased = (1 << 6),
			RedrawnByTask = (1 << 7),
			Indirected = (1 << 8),
			RightJustified = (1 << 9),
			DontCancelSameESG = (1 << 10),
			HalfSizeSprite = (1 << 11),
			Selected = (1 << 21),
			Shaded = (1 << 22),
			Deleted = (1 << 23)
		}

		/*! \brief The possible button types of an icon.  */
		public enum IconButtonType
		{
			/*! \brief Ignore all mouse clicks.  */
			IgnoreAllClicks,
			/*! \brief Notify task continually while pointer is over the work area.  */
			NotifyContinually,
			/*! \brief Click notifies task (auto-repeat).  */
			ClickAutoRepeat,
			/*! \brief Click notifies task (once only).  */
			Click,
			/*! \brief Release over the work area notifies task.  */
			Release,
			/*! \brief Double click notifies task.  */
			DoubleClick,
			/*! \brief As \e Click but can also drag (returns button state * 16).  */
			ClickDrag,
			/*! \brief As \e Release but can also drag (returns button state * 16).  */
			ReleaseDrag,
			/*! \brief As \e Double \e Click but can also drag (returns button state * 16).  */
			DoubleClickDrag,
			/*! \brief Pointer over icon selects; moving away from icon deselects; click over
			 * icon notifies task ('menu' icon).  */
			PointerSelects,
			/*! \brief Click returns button state * 256.<br>
			 *	   Double Click returns button state * 16.<br>
			 *	   Drag returns button state * 1.  */
			ClickDoubleClickDrag,
			/*! \brief Click selects icon and returns button state.<br>
			 *	   Drag returns button state * 16.  */
			ClickSelectDrag,
			/*! \brief Click causes the icon to gain caret and its parent window to become the
			 * input focus and can also drag (writable icon).  */
			GainInputFocusDrag = 14,
			/*! \brief Click causes the icon to gain caret and its parent window to become the
			 * input focus (writable icon).  */
			GainInputFocus = 15
		}

		/*! \brief Define the flags that can be used when calling method Wimp.DragBox.  */
		public enum DragFlags
		{
			KeepInline = (1 << 0),
			Clip = (1 << 1)
		}

		/*! \brief Define the drag types.  */
		public enum DragType
		{
			/*! \brief Drag window position.  */
			WindowPosition = 1,
			/*! \brief Drag window size.  */
			WindowSize,
			/*! \brief Drag horizontal scroll bar.  */
			HorizontalScroll,
			/*! \brief Drag vertical scroll bar.  */
			VerticalScroll,
			/*! \brief Drag fixed size 'rotating dash' box.  */
			Fixed,
			/*! \brief Drag rubber 'rotating dash' box.  */
			Rubber,
			/*! \brief Drag point (no Wimp-drawn dragged object).  */
			Point,
			/*! \brief Drag fixed size user-drawn box.  */
			FixedUser,
			/*! \brief Drag rubber user-drawn box.  */
			RubberUser,
			/*! \brief Drag fixed size user-drawn box but don't cancel when buttons are released.  */
			FixedUserNoCancel,
			/*! \brief Drag rubber user-drawn box but don't cancel when buttons are released.  */
			RubberUserNoCancel,
			/*! \brief Drag horizontal and vertical scroll bars.  */
			HorizontalAndVerticalScroll
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

		/*! \brief Provides data for the event raised when a user drag finishes.  */
		public class UserDragEventArgs : Wimp.EventArgs
		{
			public OS.Rect Box;

			public UserDragEventArgs (IntPtr unmanagedEventBlock) : base (unmanagedEventBlock)
			{
				Box = new OS.Rect (Marshal.ReadInt32 (RawEventData, 0),
						   Marshal.ReadInt32 (RawEventData, 4),
						   Marshal.ReadInt32 (RawEventData, 8),
						   Marshal.ReadInt32 (RawEventData, 12));
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
			public NativeWimp.CaretBlock CaretWimpBlock;

			public LoseCaretEventArgs (IntPtr unmanagedEventBlock) : base (unmanagedEventBlock)
			{
				CaretWimpBlock = (NativeWimp.CaretBlock)Marshal.PtrToStructure (
							unmanagedEventBlock, typeof(NativeWimp.CaretBlock));
			}

			public override uint GetWindowHandle ()
			{
				return (uint)Marshal.ReadInt32 (RawEventData, 0);
			}
		}

		/*! \brief Provides data for the event raised when a window gain the caret (input focus).  */
		public class GainCaretEventArgs : Wimp.EventArgs
		{
			public NativeWimp.CaretBlock CaretWimpBlock;

			public GainCaretEventArgs (IntPtr unmanagedEventBlock) : base (unmanagedEventBlock)
			{
				CaretWimpBlock = (NativeWimp.CaretBlock)Marshal.PtrToStructure (
							unmanagedEventBlock, typeof(NativeWimp.CaretBlock));
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
					case PollCode.UserDragBox:
						return new UserDragEventArgs (event_ptr);
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

		/*! \brief Create a Wimp window using the given attributes.
		 * \param [in] attr Reference to a window attributes object.
		 * \return The Wimp handle of the window.  */
		public static uint CreateWindow (WindowAttributes attr)
		{
			uint handle;
			var block = new NativeWimp.WindowBlock (attr);

			OS.ThrowOnError (NativeMethods.Wimp_CreateWindow (ref block, out handle));

			return handle;
		}

		/*! \brief Create a Wimp icon using the given attributes.
		 * \param [in] attr Reference to an icon attributes object.
		 * \return The Wimp handle of the icon.  */
		public static int CreateIcon (uint windowHandle, IconAttributes attr)
		{
			int handle;
			var icon_block = new NativeWimp.IconCreateBlock (windowHandle, attr);

			OS.ThrowOnError (NativeMethods.Wimp_CreateIcon (ref icon_block,
									out handle));
			return handle;
		}

		/*! \brief Delete the given icon from the given window.
		 * \return Nothing.  */
		public static void DeleteIcon (uint windowHandle, int iconHandle)
		{
			OS.ThrowOnError (NativeMethods.Wimp_DeleteIcon (windowHandle, iconHandle));
		}

		/*! \brief Updates the list of active windows (ones that are to be displayed).
		 * \param [in] block Reference to a native window state block.
		 * \return Nothing.  */
		public static void OpenWindow (ref NativeWimp.WindowStateBlock block)
		{
			OS.ThrowOnError (NativeMethods.Wimp_OpenWindow (ref block));
		}

		/*! \brief Updates the list of active windows (ones that are to be displayed).
		 * \param [in] block Reference to a native window state block.
		 * \param [in] parentWindow The Wimp handle of the window to use as the parent.
		 * \param [in] nestedFlags Flags to determine how the window will be nested.
		 * \return Nothing.  */
		public static void OpenWindowNested (ref NativeWimp.WindowStateBlock block,
						     uint parentWindow,
						     uint nestedFlags)
		{
			OS.ThrowOnError (NativeMethods.Wimp_OpenWindowNested (ref block,
									      parentWindow,
									      nestedFlags));
		}

		/*! \brief Removes the specified window from the active list.
		 * \param [in] handle The Wimp handle of the window to close.
		 * \return Nothing.  */
		public static void CloseWindow (uint handle)
		{
			OS.ThrowOnError (NativeMethods.Wimp_CloseWindow (handle));
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

		/*! \brief Starts a redraw of the parts of a window that are not up to date.
		 * \param [in] block A reference to a redraw window block. This is a native type
		 * that can be passed straight from a redraw event.
		 * \return 0 for no more to do, non-zero for update according to returned block.  */
		public static int RedrawWindow (ref NativeWimp.RedrawWindowBlock block)
		{
			int more;

			// Should an exception be thrown here?
			OS.ThrowOnError (NativeMethods.Wimp_RedrawWindow (ref block, out more));

			return more;
		}

		/*! \brief Starts a redraw of the parts of a window that are not up to date.
		 * \param [in] block A reference to a redraw window block.
		 * \return 0 for no more to do, non-zero for update according to returned block.  */
		public static int UpdateWindow (ref NativeWimp.RedrawWindowBlock block)
		{
			int more;

			// Should an exception be thrown here?
			OS.ThrowOnError (NativeMethods.Wimp_UpdateWindow (ref block, out more));

			return more;
		}

		/*! \brief Returns the details of the next rectangle on the work area to be drawn.
		 * \param [in] block A reference to a redraw window block.
		 * \return 0 for no more to do, non-zero for update according to returned block.  */
		public static int GetRectangle (ref NativeWimp.RedrawWindowBlock block)
		{
			int more;

			// Should an exception be thrown here?
			OS.ThrowOnError (NativeMethods.Wimp_GetRectangle (ref block, out more));

			return more;
		}

		/*! \brief Set the work area extent of the specified window.
		 * \param [in] handle The Wimp handle of the window.
		 * \param [in] extent A rectangle giving the size of the new extent.
		 * \return Nothing.  */
		public static void SetExtent (uint handle, ref NativeOS.Rect extent)
		{
			OS.ThrowOnError (NativeMethods.Wimp_SetExtent (handle, ref extent));
		}

		/*! \brief Returns a summary of the given window's state.
		 * \param [in,out] block Reference to a native type to receive the window state.
		 * \return Nothing.
		 * \note The \e WindowHandle field of the block must be set to the handle of the
		 * window before calling this method.  */
		public static void GetWindowState (ref NativeWimp.WindowStateBlock block)
		{
			OS.ThrowOnError (NativeMethods.Wimp_GetWindowState (ref block));
		}

		/*! \brief Returns a summary of the given window's state and window nesting.
		 * \param [in,out] block Reference to a native type to receive the window state.
		 * \param [out] linkage Defines how the window is nested in the parent.
		 * \return The Wimp window handle of the parent.
		 * \note The \e WindowHandle field of the block must be set to the handle of the
		 * window before calling this method.  */
		public static uint GetNestedWindowState (ref NativeWimp.WindowStateBlock block,
							 out uint linkage)
		{
			uint parent;

			OS.ThrowOnError (NativeMethods.Wimp_GetWindowStateAndNesting (ref block,
										      out parent,
										      out linkage));
			return parent;
		}

		/*! \brief Closes the specified window if it is still open, and then removes its
		 * definition.  */
		public static void DeleteWindow (uint handle)
		{
			var block = new NativeWimp.WindowHandleBlock ();

			block.WindowHandle = handle;
			OS.ThrowOnError (NativeMethods.Wimp_DeleteWindow (ref block));
		}

		/*! \brief Returns complete details of the given window's state.
		 * \param [in] handle The Wimp handle of the window.
		 * \param [in,out] block Reference to a native type to receive the window state.
		 * \return Nothing.
		 * \note This method does \b not return details of any icons.  */
		public static void GetWindowInfo (uint handle, ref NativeWimp.WindowInfoBlock block)
		{
			block.WindowHandle = handle;
			OS.ThrowOnError (NativeMethods.Wimp_GetWindowInfoHeaderOnly (ref block));
		}

		/*! \brief Returns the position of the pointer and the state of the mouse buttons.
		 * \param [out] block Reference to a native type to receive the pointer state.
		 * \return Nothing.  */
		public static void GetPointerInfo (out NativeWimp.PointerBlock block)
		{
			OS.ThrowOnError (NativeMethods.Wimp_GetPointerInfo (out block));
		}

		/*! \brief Initiates a dragging operation.
		 * \param [in] block Reference to a native type used to specify the characteristics
		 * of the drag operation.
		 * \return Nothing.
		 * \note Drag types 8-11 are not currently supported.  */
		public static void DragBox (ref NativeWimp.DragStartBlock block)
		{
			if ((int)block.DragType >= 8 && (int)block.DragType <= 11)
				throw new NotSupportedException ("Drag types 8-11 are not supported.");

			OS.ThrowOnError (NativeMethods.Wimp_DragBox (ref block));
		}

		/*! \brief Initiates a dragging operation.
		 * \param [in] block Reference to a native type used to specify the characteristics
		 * of the drag operation.
		 * \param [in] flags Additional control flags.
		 * \return Nothing.
		 * \note Drag types 8-11 are not currently supported.  */
		public static void DragBox (ref NativeWimp.DragStartBlock block, DragFlags flags)
		{
			if ((int)block.DragType >= 8 && (int)block.DragType <= 11)
				throw new NotSupportedException ("Drag types 8-11 are not supported.");

			OS.ThrowOnError (NativeMethods.Wimp_DragBox (ref block, flags));
		}

		/*! \brief Forces an area of a window or the screen to be redrawn later.
		 * \param [in] handle Wimp window handle to force a redraw of.
		 * \param [in] minX Minimum X coordinate of rectangle to redraw.
		 * \param [in] minY Minimum Y coordinate of rectangle to redraw.
		 * \param [in] maxX Maximum X coordinate of rectangle to redraw.
		 * \param [in] maxY Maximum Y coordinate of rectangle to redraw.
		 * \return Nothing.
		 * \note if \e handle is 0xffffffff (-1) the coordinates refer to the screen and
		 * are absolute. Otherwise, the coordinates are relative to the window origin.
		 * A \e handle of 0xfffffffe (-2) refers to the icon bar.  */
		public static void ForceRedraw (uint handle, int minX, int minY, int maxX, int maxY)
		{
			OS.ThrowOnError (NativeMethods.Wimp_ForceRedraw (handle, minX, minY, maxX, maxY));
		}

		/*! \brief Force the title of the given window to be redrawn.  */
		public static void ForceRedrawTitle (uint handle)
		{
			OS.ThrowOnError (NativeMethods.Wimp_ForceRedrawTitle (handle));
		}

		/*! \brief Force the given item of window furniture of the given window to be redrawn.  */
		public static void ForceRedrawFurniture (uint handle, Furniture item)
		{
			OS.ThrowOnError (NativeMethods.Wimp_ForceRedrawFurniture (handle, item));
		}

		/*! \brief Sets up the data for a new caret position and draws it there.
		 * \param [in] window 0xffffffff (-1) to turn off and disown the caret.
		 * \param [in] icon -1 if none.
		 * \param [in] x X offset of caret relative to work area origin.
		 * \param [in] y Y offset of caret relative to work area origin.
		 * \param [in] height Height of caret (if -1, then x, y and height are calculated
		 * from window, icon and index. 
		 * \param [in] index Index into string (if -1, then height and index are calculated
		 * from window, icon, x and y
		 * \return Nothing.  */
		public static void SetCaretPosition (uint window, int icon,
						     int x, int y,
						     int height, int index)
		{
			OS.ThrowOnError (NativeMethods.Wimp_SetCaretPosition (window,
									      icon,
									      x,
									      y,
									      height,
									      index));
		}

		/*! \brief Turn off and disown the caret.  */
		public static void DisownCaret ()
		{
			SetCaretPosition (0xffffffff, -1, 0, 0, 0, 0);
		}

		/*! \brief Return details of the caret's state.  */
		public static void GetCaretPosition (out NativeWimp.CaretBlock block)
		{
			OS.ThrowOnError (NativeMethods.Wimp_GetCaretPosition (out block));
		}

		/*! \brief Used to describe the attributes of a Wimp window before it is created.  */
		public class WindowAttributes
		{
			public WindowFlags Flags = WindowFlags.IsMoveable |
						   WindowFlags.NoUserRedraw |
						   WindowFlags.CanOpenBeyondScreen |
						   WindowFlags.HasBackIcon |
						   WindowFlags.HasCloseIcon |
						   WindowFlags.HasTitleBar |
						   WindowFlags.HasToggleSizeIcon |
						   WindowFlags.HasVerticalScrollBar |
						   WindowFlags.HasAdjustSizeIcon |
						   WindowFlags.HasHorizontalScrollBar |
						   WindowFlags.UseNewControlBits;
			public WindowExtraFlags ExtraFlags = 0;
			public WindowTitleFlags TitleFlags = WindowTitleFlags.ContainsText |
							     WindowTitleFlags.CentredHorizontally |
							     WindowTitleFlags.CentredVertically |
							     WindowTitleFlags.Indirected;

			// Keep the colours as ints rather than OS.DesktopColours because they can
			// be GCOL colours.
			public int TitleFGColour = (int)OS.DesktopColour.Black;
			public int TitleBGColour = (int)OS.DesktopColour.Grey2;
			public int WorkAreaFGColour = (int)OS.DesktopColour.Black;
			public int WorkAreaBGColour = (int)OS.DesktopColour.White;
			public int ScrollBarOuterColour = (int)OS.DesktopColour.Grey3;
			public int ScrollBarInnerColour = (int)OS.DesktopColour.Grey1;
			public int TitleFocusColour = (int)OS.DesktopColour.Cream;

			public OS.Rect WorkArea = new OS.Rect (0, 0, 1000, 1000);
			public WindowButtonType ButtonType = WindowButtonType.IgnoreAllClicks;
			public IntPtr SpriteArea = (IntPtr)1;
			public int MinimumWidth = 0;
			public int MinimumHeight = 0;

			public IconData Title;

			public WindowAttributes (string title, int titleMaxLen, string titleValidation)
			{
				Title = new IconData (new IconBuffer (title, titleMaxLen),
						      new IconBuffer (titleValidation));
			}

			public WindowAttributes (string title, int titleMaxLen)
			{
				Title = new IconData (new IconBuffer (title, titleMaxLen));
			}

			public WindowAttributes (string title)
			{
				Title = new IconData (new IconBuffer (title));
			}

			/*! \brief Sets whether the window is moveable by its title bar.
			 * \note Default value is \e true.  */
			public bool Moveable {
				set {
					Flags = value ? Flags |  WindowFlags.IsMoveable :
							Flags & ~WindowFlags.IsMoveable; 
				}
			}

			/*! \brief Sets whether the window requires the task to redraw it.
			 * \note Default value is \e false.  */
			public bool UserRedrawn {
				set {
					Flags = value ? Flags & ~WindowFlags.NoUserRedraw :
							Flags |  WindowFlags.NoUserRedraw;
				}
			}

			/*! \brief Sets whether the window has a back icon or not.
			 * \note Default value is \e true.  */
			public bool BackIcon {
				set {
					Flags = value ? Flags |  WindowFlags.HasBackIcon :
							Flags & ~WindowFlags.HasBackIcon;
				}
			}

			/*! \brief Sets whether the window has a close icon or not.
			 * \note Default value is \e true.  */
			public bool CloseIcon {
				set {
					Flags = value ? Flags |  WindowFlags.HasCloseIcon :
							Flags & ~WindowFlags.HasCloseIcon;
				}
			}

			/*! \brief Sets whether the window has a toggle size icon or not.
			 * \note Default value is \e true.  */
			public bool ToggleSizeIcon {
				set {
					Flags = value ? Flags |  WindowFlags.HasToggleSizeIcon :
							Flags & ~WindowFlags.HasToggleSizeIcon;
				}
			}

			/*! \brief Sets whether the window has a vertical scroll bar or not.
			 * \note Default value is \e true.  */
			public bool VerticalScrollBar {
				set {
					Flags = value ? Flags |  WindowFlags.HasVerticalScrollBar :
							Flags & ~WindowFlags.HasVerticalScrollBar;
				}
			}

			/*! \brief Sets whether the window has a horizontal scroll bar or not.
			 * \note Default value is \e true.  */
			public bool HorizontalScrollBar {
				set {
					Flags = value ? Flags |  WindowFlags.HasHorizontalScrollBar :
							Flags & ~WindowFlags.HasHorizontalScrollBar;
				}
			}

			/*! \brief Sets whether the window has a adjust size icon or not.
			 * \note Default value is \e true.  */
			public bool AdjustSizeIcon {
				set {
					Flags = value ? Flags |  WindowFlags.HasAdjustSizeIcon :
							Flags & ~WindowFlags.HasAdjustSizeIcon;
				}
			}

			/*! \brief Sets whether the window has a title bar or not.
			 * \note Default value is \e true.  */
			public bool TitleBar {
				set {
					Flags = value ? Flags |  WindowFlags.HasTitleBar :
							Flags & ~WindowFlags.HasTitleBar;
				}
			}

			/*! \brief Sets whether the colours given for the window furniture and work
			 * area are desktop wimp colours (i.e., 0 - 15) or GCOL colours (i.e., 0 - 254).
			 * \note Default value is \e false (i.e., colours are desktop wimp colours.  */
			public bool GCOLColours {
				set {
					Flags = value ? Flags |  WindowFlags.GCOLColours :
							Flags & ~WindowFlags.GCOLColours;
				}
			}

			/*! \brief Sets whether scroll requests (no auto-repeat) should be sent to the
			 * task.
			 * \note Default value is \e false.  */
			public bool GenerateScrollRequests {
				set {
					Flags = value ? Flags |  WindowFlags.GenerateScrollRequests :
							Flags & ~WindowFlags.GenerateScrollRequests;
				}
			}

			/*! \brief Sets whether scroll requests (with auto-repeat) should be sent to the
			 * task.
			 * \note Default value is \e false.  */
			public bool GenerateAutoRepeatScrollRequests {
				set {
					Flags = value ? Flags |  WindowFlags.GenerateAutoRepeatScrollRequests :
							Flags & ~WindowFlags.GenerateAutoRepeatScrollRequests;
				}
			}

			/*! \brief Sets whether 'hot keys' events are generated and passed back through
			 * Wimp.ProcessKey.
			 * \note Default value is \e false.  */
			public bool GenerateHotKeyEvents {
				set {
					Flags = value ? Flags |  WindowFlags.GenerateHotKeyEvents :
							Flags & ~WindowFlags.GenerateHotKeyEvents;
				}
			}

			/*! \brief Sets whether this window will be a pane.
			 * \note Default value is \e false.  */
			public bool Pane {
				set {
					Flags = value ? Flags |  WindowFlags.IsAPane :
							Flags & ~WindowFlags.IsAPane;
				}
			}

			/*! \brief Sets whether the window can be opened or dragged outside the screen
			 * area.
			 * \note Default value is \e true.  */
			public bool AllowBeyondScreen {
				set {
					Flags = value ? Flags |  WindowFlags.CanOpenBeyondScreen :
							Flags & ~WindowFlags.CanOpenBeyondScreen;
				}
			}

			/*! \brief Sets whether the window will be forced to stay on screen.
			 * \note Default value is \e false.  */
			public bool KeepOnScreen {
				set {
					Flags = value ? Flags |  WindowFlags.KeepOnScreen :
							Flags & ~WindowFlags.KeepOnScreen;
				}
			}

			/*! \brief Sets whether to ignore the right-hand extent if the size of the window
			 * is dragged.
			 * \note Default value is \e false.  */
			public bool IgnoreRightExtent {
				set {
					Flags = value ? Flags |  WindowFlags.IgnoreRightExtent :
							Flags & ~WindowFlags.IgnoreRightExtent;
				}
			}

			/*! \brief Sets whether to ignore the lower-hand extent if the size of the window
			 * is dragged.
			 * \note Default value is \e false.  */
			public bool IgnoreLowerExtent {
				set {
					Flags = value ? Flags |  WindowFlags.IgnoreLowerExtent :
							Flags & ~WindowFlags.IgnoreLowerExtent;
				}
			}

			/*! \brief Sets whether the window title bar contains text.
			 * \note Default value is \e true.  */
			public bool TitleContainsText {
				set {
					TitleFlags = value ? TitleFlags |  WindowTitleFlags.ContainsText :
							     TitleFlags & ~WindowTitleFlags.ContainsText;
				}
			}

			/*! \brief Sets whether the window title bar contains a sprite.
			 * \note Default value is \e false.  */
			public bool TitleContainsSprite {
				set {
					TitleFlags = value ? TitleFlags |  WindowTitleFlags.ContainsSprite :
							     TitleFlags & ~WindowTitleFlags.ContainsSprite;
				}
			}

			/*! \brief Sets whether the text in the window title bar is Horizontally centred.
			 * \note Default value is \e true.  */
			public bool TitleCentredHorizontally {
				set {
					TitleFlags = value ? TitleFlags |  WindowTitleFlags.CentredHorizontally :
							     TitleFlags & ~WindowTitleFlags.CentredHorizontally;
				}
			}

			/*! \brief Sets whether the text in the window title bar is vertically centred.
			 * \note Default value is \e true.  */
			public bool TitleCentredVertically {
				set {
					TitleFlags = value ? TitleFlags |  WindowTitleFlags.CentredVertically :
							     TitleFlags & ~WindowTitleFlags.CentredVertically;
				}
			}

			/*! \brief Sets whether the window title is indirected.
			 * \note Default value is \e true.  */
			public bool TitleIndirected {
				set {
					TitleFlags = value ? TitleFlags |  WindowTitleFlags.Indirected :
							     TitleFlags & ~WindowTitleFlags.Indirected;
				}
			}

			/*! \brief Sets whether the text in the window title bar is right justified.
			 * \note Default value is \e false.  */
			public bool TitleRightJustified {
				set {
					TitleFlags = value ? TitleFlags |  WindowTitleFlags.RightJustified :
							     TitleFlags & ~WindowTitleFlags.RightJustified;
				}
			}

			/*! \brief Sets whether 24 bit colour should be enabled via the title bar validation
			 * string.
			 * \note Default value is \e false.
			 * \note RISC %OS 4.00+  */
			public bool FullColour {
				set {
					ExtraFlags = value ? ExtraFlags |  WindowExtraFlags.FullColour :
							     ExtraFlags & ~WindowExtraFlags.FullColour;
				}
			}

			/*! \brief Sets whether extended scroll requests should be sent to the task.
			 * \note Default value is \e false.
			 * \note RISC %OS Select 2.  */
			public bool ExtendedScrollRequests {
				set {
					ExtraFlags = value ? ExtraFlags |  WindowExtraFlags.ExtendedScrollRequests :
							     ExtraFlags & ~WindowExtraFlags.ExtendedScrollRequests;
				}
			}

			/*! \brief Sets whether a 3D border should never be given to the window.
			 * \note Default value is \e false.
			 * \note RISC %OS 4.00+  */
			public bool Never3DBorder {
				set {
					ExtraFlags = value ? ExtraFlags |  WindowExtraFlags.Never3DBorder :
							     ExtraFlags & ~WindowExtraFlags.Never3DBorder;
				}
			}

			/*! \brief Sets whether a 3D border should always be given to the window.
			 * \note Default value is \e false.
			 * \note RISC %OS 4.00+  */
			public bool Always3DBorder {
				set {
					ExtraFlags = value ? ExtraFlags |  WindowExtraFlags.Always3DBorder :
							     ExtraFlags & ~WindowExtraFlags.Always3DBorder;
				}
			}

			/*! \brief Sets whether Wimp.GetPointerInfo should return shaded icons as well.
			 * \note Default value is \e false.
			 * \note RISC %OS 5.  */
			public bool ReturnShadedIcons {
				set {
					ExtraFlags = value ? ExtraFlags |  WindowExtraFlags.ReturnShadedIcons :
							     ExtraFlags & ~WindowExtraFlags.ReturnShadedIcons;
				}
			}
		}
	}
}
