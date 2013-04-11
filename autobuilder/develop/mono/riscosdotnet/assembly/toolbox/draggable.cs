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
						ToolboxWindow = Object.CreateInstance<Window> (window_id);
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
			public event EventHandler<DragStartEventArgs> DragStart;

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
			public event EventHandler<DragEndEventArgs> DragEnd;

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
				set {
					Object.MiscOp_SetR3R4 (0,
							       Method.SetState,
							       ComponentID, (uint)(value ? 1 : 0));
				}
				get {
					return (Object.MiscOp_SetR3GetR0 (0,
									  Method.GetState,
									  ComponentID) & 1) != 0;
				}
			}

			protected virtual void OnDragStart (DragStartEventArgs e)
			{
				if (DragStart != null)
					DragStart (this, e);
			}

			protected virtual void OnDragEnd (DragEndEventArgs e)
			{
				if (DragEnd != null)
					DragEnd (this, e);
			}

			/*! \brief Check if the given event is relevant to the %Draggable gadget and call the
			 * associated event handlers.  */
			public override void Dispatch (ToolboxEventArgs e)
			{
				switch (e.Header.EventCode)
				{
				case EventCode.DragStarted:
					OnDragStart (new DragStartEventArgs (e.RawEventData));
					break;
				case EventCode.DragEnded:
					OnDragEnd (new DragEndEventArgs (e.RawEventData));
					break;
				}
			}
		}

		public static class DraggableTemplateOffset
		{
			public const int Text = 36;
			public const int MaxTextLen = 40;
			public const int Sprite = 44;
			public const int MaxSpriteLen = 48;
			public const int TemplateSize = 52;
		}

		public enum DraggableType
		{
			DragOnly,
			ClickDragDoubleClick,
			ClickSelectsDoubleClickDrag
		}

		/*! \brief The Toolbox flags that help to define the behaviour of a draggable.  */
		public static class DraggableFlags
		{
			public const int WarnDragStart = (1 << 0);
			public const int ContainsSprite = (1 << 1);
			public const int ContainsText = (1 << 2);
			public const int Type = (1 << 3);
			public const int ToolboxIDs = (1 << 6);
			public const int DropShadow = (1 << 7);
			public const int NoDither = (1 << 8);
		}

		/*! \brief Encapsulate the data required to create a Toolbox Draggable gadget
		 * template.  */
		public class DraggableTemplate : GadgetTemplate
		{
			string _text = "";
			/*! \brief The text to be shown in the gadget.  */
			public string Text {
				get { return _text; }
				set { _text = value; }
			}

			int _max_text_len = 0;
			/*! \brief The maximum buffer size for the text. Leave as 0 to indicate that
			 * the length of the text itself should be used.  */
			public int MaxTextLen {
				get { return _max_text_len; }
				set { _max_text_len = value; }
			}

			string _sprite = "";
			/*! \brief The name of the sprite to be shown in the gadget.  */
			public string Sprite {
				get { return _sprite; }
				set { _sprite = value; }
			}

			int _max_sprite_len = 0;
			/*! \brief The maximum buffer size for the sprite name. Leave as 0 to
			 * indicate that the length of the sprite name itself should be used.  */
			public int MaxSpriteLen {
				get { return _max_sprite_len; }
				set { _max_sprite_len = value; }
			}

			/*! \brief Create a template for a draggable where the text (if any)
			 * and sprite will be set later.  */
			public DraggableTemplate () : base (Gadget.ComponentType.Draggable)
			{
			}

			/*! \brief Create a template for a draggable with the given text.  */
			public DraggableTemplate (string text) : base (Gadget.ComponentType.Draggable)
			{
				_text = text;
				ContainsText = true;
			}

			/*! \brief Create a template for a draggable with the given text.  */
			public DraggableTemplate (string text, string sprite) :
							base (Gadget.ComponentType.Draggable)
			{
				_text = text;
				_sprite = sprite;
				ContainsText = true;
				ContainsSprite = true;
			}

			/*! \brief Create a template for a draggable with the given text.  */
			public DraggableTemplate (string text, int maxTextLen) :
							base (Gadget.ComponentType.Draggable)
			{
				_text = text;
				_max_text_len = maxTextLen;
				ContainsText = true;
			}

			/*! \brief Create a template for a draggable with the given text and sprite.  */
			public DraggableTemplate (string text, int maxTextLen,
						  string sprite, int maxSpriteLen) :
							base (Gadget.ComponentType.Draggable)
			{
				_text = text;
				_max_text_len = maxTextLen;
				_sprite = sprite;
				_max_sprite_len = maxSpriteLen;
				ContainsText = true;
				ContainsSprite = true;
			}

			// Not sure that the getters for these properties are very useful, but they're
			// included for completeness.

			/*! \brief Set or get whether a draggable created from this template will raise
			 * an event when the drag starts.  */
			public bool WarnDragStart {
				get { return (_flags & ~DraggableFlags.WarnDragStart) != 0; }
				set {
					_flags = (uint)(value ? _flags |  DraggableFlags.WarnDragStart :
								_flags & ~DraggableFlags.WarnDragStart);
				}
			}

			/*! \brief Set or get whether a draggable created from this template will contain
			 * text.  */
			public bool ContainsText {
				get { return (_flags & ~DraggableFlags.ContainsText) != 0; }
				set {
					_flags = (uint)(value ? _flags |  DraggableFlags.ContainsText :
								_flags & ~DraggableFlags.ContainsText);
				}
			}

			/*! \brief Set or get whether a draggable created from this template will contain
			 * a sprite.  */
			public bool ContainsSprite {
				get { return (_flags & ~DraggableFlags.ContainsSprite) != 0; }
				set {
					_flags = (uint)(value ? _flags |  DraggableFlags.ContainsSprite :
								_flags & ~DraggableFlags.ContainsSprite);
				}
			}

			/*! \brief Set or get the type of draggable created from this template.  */
			public DraggableType Type {
				get { return (DraggableType)((_flags >> 3) & 7); }
				set {
					_flags &= ~(uint)(7 << 3);
					_flags |= (uint)value << 3;
				}
			}

			/*! \brief Set or get whether a draggable created from this template will
			 * return Toolbox IDs instead of Wimp handles.  */
			public bool ReturnToolboxIDs {
				get { return (_flags & ~DraggableFlags.ToolboxIDs) != 0; }
				set {
					_flags = (uint)(value ? _flags |  DraggableFlags.ToolboxIDs :
								_flags & ~DraggableFlags.ToolboxIDs);
				}
			}

			/*! \brief Set or get whether a draggable created from this template will
			 * display a drop shadow when dragging.  */
			public bool DropShadow {
				get { return (_flags & ~DraggableFlags.DropShadow) != 0; }
				set {
					_flags = (uint)(value ? _flags |  DraggableFlags.DropShadow :
								_flags & ~DraggableFlags.DropShadow);
				}
			}

			/*! \brief Set or get whether a draggable created from this template will
			 * be displayed dithered when dragging.  */
			public bool NoDither {
				get { return (_flags & ~DraggableFlags.NoDither) != 0; }
				set {
					_flags = (uint)(value ? _flags |  DraggableFlags.NoDither :
								_flags & ~DraggableFlags.NoDither);
				}
			}

			public override int CalculateBufferSize (ref int strStart, ref int msgStart)
			{
				int size = base.CalculateBufferSize (ref strStart, ref msgStart);

				if (!string.IsNullOrEmpty (_text))
					size += Math.Max (_text.Length + 1, _max_text_len);
				if (!string.IsNullOrEmpty (_sprite))
				{
					int length = Math.Max (_sprite.Length + 1, _max_sprite_len);
					size += length;
					msgStart += length;
				}

				return size;
			}

			public override void BuildBuffer (IntPtr buffer,
							  int offset,
							  ref int strOffset,
							  ref int msgOffset)
			{
				base.BuildBuffer (buffer, offset, ref strOffset, ref msgOffset);

				msgOffset = ObjectTemplate.WriteString (_text,
									_max_text_len,
									buffer,
									offset + DraggableTemplateOffset.Text,
									msgOffset);
				Marshal.WriteInt32 (buffer,
						    offset + DraggableTemplateOffset.MaxTextLen,
						    Math.Max (_text.Length + 1, _max_text_len));
				strOffset = ObjectTemplate.WriteString (_sprite,
									_max_sprite_len,
									buffer,
									offset + DraggableTemplateOffset.Sprite,
									strOffset);
				Marshal.WriteInt32 (buffer,
						    offset + DraggableTemplateOffset.MaxSpriteLen,
						    Math.Max (_sprite.Length + 1, _max_sprite_len));
			}

			public override int GetTemplateSize ()
			{
				return DraggableTemplateOffset.TemplateSize;
			}
		}
	}
}
