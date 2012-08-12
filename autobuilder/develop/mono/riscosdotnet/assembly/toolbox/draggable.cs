//
// toolbox/draggable.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;
using System.Runtime.InteropServices;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \class Draggable
		 * Consists of a sprite, text or text & sprite which appears in a dialogue
		 * box, and can be dragged using the mouse.  */
		public class Draggable : Gadget
		{
			static class Method
			{
				public const int SetSprite = 640;
				public const int GetSprite = 641;
				public const int SetText = 642;
				public const int GetText = 643;
				public const int SetState = 644;
				public const int GetState = 645;
			}

			/*! \brief The default Toolbox events that a draggable can raise.  */
			public static class EventCode
			{
				public const uint DragStarted = 0x82887;
				public const uint DragEnded = 0x82888;
			}

			/*! \brief An object that encapsulates the arguments for the event that is raised when
			 * a drag is started on a %Draggable  */
			public class DragStartEventArgs : ToolboxEventArgs
			{
				/*! \brief Constants defining the possible bit flags returned by this event.  */
				public static class Flags
				{
					public const int Adjust = (1 << 0);
					// Bit 1 reserved - always 0
					public const int Select = (1 << 2);
					public const int Shift = (1 << 3);
					public const int Ctrl = (1 << 4);
				}

				/*! \brief \e true if Adjust is held down at the time the event is raised.  */
				public bool Adjust;
				/*! \brief \e true if Select is held down at the time the event is raised.  */
				public bool Select;
				/*! \brief \e true if a Shift key is held down at the time the event is raised.  */
				public bool Shift;
				/*! \brief \e true if a Ctrl key is held down at the time the event is raised.  */
				public bool Ctrl;

				/*! \brief Create the arguments for a DragStart event from the raw event data.  */
				public DragStartEventArgs (IntPtr unmanagedEventBlock) : base (unmanagedEventBlock)
				{
					uint flags = Header.Flags;

					Adjust = (flags & Flags.Adjust) != 0;
					Select = (flags & Flags.Select) != 0;
					Shift = (flags & Flags.Shift) != 0;
					Ctrl = (flags & Flags.Ctrl) != 0;
				}
			}

			/*! \brief An object that encapsulates the arguments for the event that is raised when
			 * a drag ends for a %Draggable  */
			public class DragEndEventArgs : ToolboxEventArgs
			{
				/*! \brief Constants defining the possible bit flags returned by this event.  */
				public static class Flags
				{
					/*! \brief Bit 0 set - returned IDs are Toolbox IDs, otherwise WIMP handles.  */
					public const int ToolboxIDs = (1 << 0);
				}

				/*! \brief Constants defining event specific data offsets after the header.  */
				public static class EventOffset
				{
					public const int Window = 16;
					public const int Icon = 20;
					public const int MouseX = 24;
					public const int MouseY = 28;
				}

				/*! \brief The Window object where the drag ended, or \e null if not a Toolbox
				 * window. */
				public Toolbox.Window ToolboxWindow;
				/*! \brief The component ID where the drag ended.  */
				public uint ToolboxCmpID;
				/*! \brief The WIMP window handle where the drag ended.  */
				public uint WimpWindowHandle;
				/*! \brief The WIMP icon handle where the drag ended.  */
				public int WimpIconHandle;
				/*! \brief The destination X,Y coordinate.  */
				public OS.Coord MousePosition;

				/*! \brief Create the arguments for a DragEnd event from the raw event data.  */
				public DragEndEventArgs (IntPtr unmanagedEventBlock) : base (unmanagedEventBlock)
				{
					uint flags = Header.Flags;
					uint window_id = (uint)Marshal.ReadInt32 (RawEventData, EventOffset.Window);
					int icon_id = Marshal.ReadInt32 (RawEventData, EventOffset.Icon);

					if ((flags & Flags.ToolboxIDs) != 0)
					{
						ToolboxWindow = (Window)Object.LookupOrWrap (window_id);
						ToolboxCmpID = (uint)icon_id;
						WimpWindowHandle = 0;
						WimpIconHandle = -1;
					}
					else
					{
						ToolboxWindow = null;
						ToolboxCmpID = 0xffffffff;
						WimpWindowHandle = window_id;
						WimpIconHandle = icon_id;
					}

					int x = Marshal.ReadInt32 (RawEventData, EventOffset.MouseX);
					int y = Marshal.ReadInt32 (RawEventData, EventOffset.MouseY);
					MousePosition = new OS.Coord (x, y);
				}
			}

			/*! \brief The signature of a DragStart event handler.  */
			public delegate void DragStartEventHandler (object sender, DragStartEventArgs e);

			/*! \brief The event handlers that will be called when a drag is initiated on this
			 * %Draggable.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, DragStartEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * DraggableObject.DragStart += handler_name;
			 * \endcode  */
			public event DragStartEventHandler DragStart;

			/*! \brief The signature of a DragEnd event handler.  */
			public delegate void DragEndEventHandler (object sender, DragEndEventArgs e);

			/*! \brief The event handlers that will be called when a drag from this
			 * %Draggable is ended.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, DragEndEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * DraggableObject.DragEnd += handler_name;
			 * \endcode  */
			public event DragEndEventHandler DragEnd;

			/*! \brief Wrap an existing draggable, e.g., from a Resource file created
			 * Window.  */
			public Draggable (Window window, uint cmpID) : base (window, cmpID)
			{
			}

			/*! \brief The name of the sprite used for this draggable.  */
			public string Sprite
			{
				set { SetText (Method.SetSprite, value); }
				get { return GetText (Method.GetSprite); }
			}

			/*! \brief The text which will be displayed in this draggable.  */
			public string Text
			{
				set { SetText (Method.SetText, value); }
				get { return GetText (Method.GetText); }
			}

			/*! \brief The state of this draggable, i.e. whether it is selected or
			 * deselected.  */
			public bool Selected
			{
				set { CallMethod_SetR4 (Method.SetState, (uint)(value ? 1 : 0)); }
				get { return (CallMethod_GetR0 (Method.GetState) & 1) != 0; }
			}

			/*! \brief Check if the given event is relevant to the %Draggable gadget and call the
			 * associated event handlers.  */
			public override void Dispatch (ToolboxEvent ev)
			{
				switch (ev.ToolboxArgs.Header.EventCode)
				{
				case EventCode.DragStarted:
					if (DragStart != null)
						DragStart (this, new DragStartEventArgs (ev.ToolboxArgs.RawEventData));
					break;
				case EventCode.DragEnded:
					if (DragEnd != null)
						DragEnd (this, new DragEndEventArgs (ev.ToolboxArgs.RawEventData));
					break;
				}
			}
		}
	}
}
