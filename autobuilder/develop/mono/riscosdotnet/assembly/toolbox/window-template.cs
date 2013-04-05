//
// toolbox/window-template.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;

namespace riscos
{
	public static partial class Toolbox
	{
		public static class WindowTemplateOffset
		{
			public const int Flags = 36;
			public const int HelpMessage = 40;
			public const int MaxHelp = 44;
			public const int PointerShape = 48;
			public const int MaxPointerShape = 52;
			public const int PointerXHot = 56;
			public const int PointerYHot = 60;
			public const int Menu = 64;
			public const int NumKeyboardShortcuts = 68;
			public const int KeyboardShortcuts = 72;
			public const int NumGadgets = 76;
			public const int Gadgets = 80;
			public const int DefaultFocus = 84;
			public const int ShowEvent = 88;
			public const int HideEvent = 92;
			public const int InternalBL = 96;
			public const int InternalTL = 100;
			public const int ExternalBL = 104;
			public const int ExternalTL = 108;
			public const int Window = 112;
			public const int Data = 200;
		}

		public static class GadgetTemplateOffset
		{
			public const int Flags = 0;
			public const int Type = 4;
			public const int Size = 6;
			public const int MinX = 8;
			public const int MinY = 12;
			public const int MaxX = 16;
			public const int MaxY = 20;
			public const int ComponentID = 24;
			public const int HelpMessage = 28;
			public const int MaxHelp = 32;
			public const int Data = 36;
			public const int TemplateHeaderSize = 36;
		}

		public static class KeyboardShortcutTemplateOffset
		{
			public const int Flags = 0;
			public const int WimpKeyCode = 4;
			public const int KeyEvent = 8;
			public const int KeyShow = 12;
			public const int TemplateSize = 16;
		}

		/*! \brief Encapsulates the data required to add a keyboard short cut
		 * to a window template.  */
		public sealed class KeyboardShortcutTemplate
		{
			uint _flags = 0;
			/*! \brief A flags word:<br>
			 * Bit 0 - when set, show attached object as 'Transient'.  */
			public uint Flags {
				get { return _flags; }
				set { _flags = value; }
			}

			int _wimp_key_code;
			/*! \brief The key code returned by the Wimp in a key pressed event.  */
			public int WimpKeyCode {
				get { return _wimp_key_code; }
				set { _wimp_key_code = value; }
			}

			uint _key_event = 0;
			/*! \brief The Toolbox event to be raised when the Wimp delivers a key
			 * pressed event. 0 means deliver no event.  */
			public uint KeyEvent {
				get { return _key_event; }
				set { _key_event = value; }
			}

			string _key_show = "";
			/*! \brief The name of the template for an object to create and show when
			 * the Wimp delivers a key pressed event. A null or empty string means show
			 * no object.  */
			public string KeyShow {
				get { return _key_show; }
				set { _key_show = value; }
			}

			/*! \brief Create a keyboard shortcut template for the given key code that will
			 * raise the given event.  */
			public KeyboardShortcutTemplate (int wimpKeyCode, uint keyEvent)
			{
				_wimp_key_code = wimpKeyCode;
				_key_event = keyEvent;
			}

			/*! \brief Create a keyboard shortcut template for the given key code that will
			 * create and show an object from the named template.  */
			public KeyboardShortcutTemplate (int wimpKeyCode, string keyShow)
			{
				_wimp_key_code = wimpKeyCode;
				_key_show = keyShow;
			}

			/*! \brief Create a keyboard shortcut template for the given key code that will
			 * raise the given event and create and show an object from the named template.  */
			public KeyboardShortcutTemplate (int wimpKeyCode, string keyShow, uint keyEvent)
			{
				_wimp_key_code = wimpKeyCode;
				_key_show = keyShow;
				_key_event = keyEvent;
			}

			/*! \brief Calculate the number of bytes required to store a keyboard shortcut
			 * template.
			 * \param [in,out] strStart The offset from the start of the template where the
			 * string table starts. Updated to accommodate the keyboard shortcut template.
			 * \param [in,out] msgStart The offset from the start of the template where the
			 * message table starts. Updated to accommodate the keyboard shortcut template.
			 * \return The number of bytes required to store the keyboard shortcut template.  */
			public int CalculateBufferSize (ref int strStart, ref int msgStart)
			{
				int size = KeyboardShortcutTemplateOffset.TemplateSize;

				// Move the string tables up to accommodate the shortcut.
				strStart += size;
				msgStart += size;

				if (!string.IsNullOrEmpty (_key_show))
				{
					int length = _key_show.Length + 1;
					size += length;
					msgStart += length;
				}

				return size;
			}

			/*! \brief Write the keyboard shortcut template data into the buffer.
			 * \param [in] buffer Pointer to the buffer to write the template to.
			 * \param [in] offset The offset from the start of the buffer where the keyboard
			 * shortcut will be written.
			 * \param [in,out] strOffset The offset from the start of the template where the
			 * next string should be written in the string table. Updated after each string
			 * is written.
			 * \return Nothing.  */
			public void BuildBuffer (IntPtr buffer, int offset, ref int strOffset)
			{
				Marshal.WriteInt32 (buffer,
						    offset + KeyboardShortcutTemplateOffset.Flags,
						    (int)_flags);
				Marshal.WriteInt32 (buffer,
						    offset + KeyboardShortcutTemplateOffset.WimpKeyCode,
						    _wimp_key_code);
				Marshal.WriteInt32 (buffer,
						    offset + KeyboardShortcutTemplateOffset.KeyEvent,
						    (int)_key_event);
				strOffset = ObjectTemplate.WriteString (_key_show,
									0,
									buffer,
									offset + KeyboardShortcutTemplateOffset.KeyShow,
									strOffset);
			}
		}

		/*! \brief Encapsulates the data required to add a gadget to a window template.  */
		public class GadgetTemplate
		{
			uint _flags;
			public uint Flags {
				get { return _flags; }
				set { _flags = value; }
			}

			int Type;

			OS.Rect _bounding_box;
			/*! \brief The location and size of the gadget in the window. Coordinates are in
			 * OS units and are relative to the window origin.  */
			public OS.Rect BoundingBox {
				get { return _bounding_box; }
				set { _bounding_box = value; }
			}

			uint _component_id = 0xffffffff;
			/*! \brief The (user defined) ID that the component will be given. This must be unique
			 * within this window. A value of -1 will force the Toolbox to choose an ID.  */
			public uint ComponentID {
				get { return _component_id; }
				set { _component_id = value; }
			}

			string _help_message;
			/*! \brief The help message to be associated with this gadget.  */
			public string HelpMessage {
				get { return _help_message; }
				set { _help_message = value; }
			}

			int _max_help;
			/*! \brief The maximum buffer size that will be allocated to the help message.
			 * Allows the buffer to be bigger than the initial help message to allow it to be
			 * changed later. If 0, or set to be smaller than the help message, then the buffer
			 * size is set to be the same size as the help message.  */
			public int MaxHelp {
				get { return _max_help; }
				set { _max_help = value; }
			}

			/*! \brief Create a gadget template of the given type (set by a derived object).  */
			public GadgetTemplate (int type)
			{
				Type = type;
			}

			/*! \brief Calculate the number of bytes required to store the part of a gadget
			 * template that is common to all gadgets.
			 * \param [in,out] strStart The offset from the start of the template where the
			 * string table starts. Updated to accommodate the gadget template.
			 * \param [in,out] msgStart The offset from the start of the template where the
			 * message table starts. Updated to accommodate the gadget template.
			 * \return The number of bytes required to store the gadget template header.  */
			public virtual int CalculateBufferSize (ref int strStart, ref int msgStart)
			{
				// Start with the size of the full template body; dervied objects
				// only have to bother about their data.
				int size = GetTemplateSize ();

				// Move the string tables up to accommodate the gadget.
				strStart += size;
				msgStart += size;

				if (!string.IsNullOrEmpty (_help_message))
					size += Math.Max (_help_message.Length + 1, _max_help);

				return size;
			}

			/*! \brief Write the template data into the buffer.
			 * \param [in] buffer Pointer to the buffer to write the template to.
			 * \param [in] offset The offset from the start of the buffer where the gadget
			 * will be written.
			 * \param [in,out] strOffset The offset from the start of the template where the
			 * next string should be written in the string table. Updated after each string
			 * is written.
			 * \param [in,out] msgOffset The offset from the start of the template where the
			 * next string should be written in the message table. Updated after each string
			 * is written.
			 * \return Nothing.  */
			public virtual void BuildBuffer (IntPtr buffer,
							 int offset,
							 ref int strOffset,
							 ref int msgOffset)
			{
				Marshal.WriteInt32 (buffer, offset + GadgetTemplateOffset.Flags,
						    (int)_flags);
				Marshal.WriteInt16 (buffer, offset + GadgetTemplateOffset.Type,
						    (short)Type);
				Marshal.WriteInt16 (buffer, offset + GadgetTemplateOffset.Size,
						    (short)GetTemplateSize ());
				Marshal.WriteInt32 (buffer, offset + GadgetTemplateOffset.MinX,
						    _bounding_box.MinX);
				Marshal.WriteInt32 (buffer, offset + GadgetTemplateOffset.MinY,
						    _bounding_box.MinY);
				Marshal.WriteInt32 (buffer, offset + GadgetTemplateOffset.MaxX,
						    _bounding_box.MaxX);
				Marshal.WriteInt32 (buffer, offset + GadgetTemplateOffset.MaxY,
						    _bounding_box.MaxY);
				Marshal.WriteInt32 (buffer, offset + GadgetTemplateOffset.ComponentID,
						    (int)_component_id);
				msgOffset = ObjectTemplate.WriteString (_help_message,
									_max_help,
									buffer,
									offset + GadgetTemplateOffset.HelpMessage,
									msgOffset);
				Marshal.WriteInt32 (buffer, offset + GadgetTemplateOffset.MaxHelp,
						    string.IsNullOrEmpty (_help_message) ? 0 :
							Math.Max (_help_message.Length + 1, _max_help));
			}

			/*! \brief Return the number of bytes that the gadget template header occupies.  */
			public virtual int GetTemplateSize ()
			{
				return GadgetTemplateOffset.TemplateHeaderSize;
			}
		}

		/*! \brief Encapsulates the data required to create a window template.  */
		public sealed class WindowTemplate : ObjectTemplate
		{
			//! \brief The Wimp window attributes to be used in this Toolbox Window template.
			Wimp.WindowAttributes Attributes;

			//! \brief A list of keyboard shortcuts to be added to this Window template.
			List<KeyboardShortcutTemplate> KeyboardShortcuts = new List<KeyboardShortcutTemplate> ();

			//! \brief A list of gadgets to be added to the Window template.
			List<GadgetTemplate> Gadgets = new List<GadgetTemplate> ();

			// Reasonable default view rectangle just to ensure the window is on screen.
			// Either the caller will set their own default view rectangle or they will
			// use a full show spec when opening the window.
			OS.Rect _default_view_rect = new OS.Rect (100, 100, 300, 300);
			public OS.Rect DefaultViewRect {
				get { return _default_view_rect; }
				set { _default_view_rect = value; }
			}

			OS.Coord _default_scroll = new OS.Coord (0, 0);
			public OS.Coord DefaultScroll {
				get { return _default_scroll; }
				set { _default_scroll = value; }
			}

			Window.Flags _flags = 0;
			public Window.Flags Flags {
				get { return _flags; }
				set { _flags = value; }
			}

			string _help_message = "";
			public string HelpMessage {
				get { return _help_message; }
				set { _help_message = value; }
			}

			int _max_help = 0;
			public int MaxHelp {
				get { return _max_help; }
				set { _max_help = value; }
			}

			string _pointer_shape = "";
			public string PointerShape {
				get { return _pointer_shape; }
				set { _pointer_shape = value; }
			}

			int _max_pointer_shape = 0;
			public int MaxPointerShape {
				get { return _max_pointer_shape; }
				set { _max_pointer_shape = value; }
			}

			OS.Coord _pointer_hotspot = new OS.Coord ();
			public OS.Coord PointerHotSpot {
				get { return _pointer_hotspot; }
				set { _pointer_hotspot = value; }
			}

			string _menu = "";
			public string Menu {
				get { return _menu; }
				set { _menu = value; }
			}

			uint _default_focus = Window.DefaultFocusNone;
			public uint DefaultFocus {
				get { return _default_focus; }
				set { _default_focus = value; }
			}

			int _show_event = 0;
			public int ShowEvent {
				get { return _show_event; }
				set { _show_event = value; }
			}

			int _hide_event = 0;
			public int HideEvent {
				get { return _hide_event; }
				set { _hide_event = value; }
			}

			string _internal_bl = "";
			public string InternalBL {
				get { return _internal_bl; }
				set { _internal_bl = value; }
			}

			string _internal_tl = "";
			public string InternalTL {
				get { return _internal_tl; }
				set { _internal_tl = value; }
			}

			string _external_bl = "";
			public string ExternalBL {
				get { return _external_bl; }
				set { _external_bl = value; }
			}

			string _external_tl = "";
			public string ExternalTL {
				get { return _external_tl; }
				set { _external_tl = value; }
			}

			const int TemplateVersion = 102;

			/*! \brief Create a Window template with the given name and Wimp
			 * window attributes.  */
			public WindowTemplate (string name,
					       Wimp.WindowAttributes attr) : base (Class.Window,
										   0,
										   TemplateVersion,
										   name)
			{
				Attributes = attr;
			}

			//! \brief Add the given gadget to this Window template.
			public void AddGadget (GadgetTemplate gadget)
			{
				Gadgets.Add (gadget);
			}

			//! \brief Add the given keyboard shortcut to this Window template.
			public void AddKeyboardShortcut (KeyboardShortcutTemplate shortcut)
			{
				KeyboardShortcuts.Add (shortcut);
			}

			/*! \brief Calculate how much memory is required to hold all of the data required for
			 * a window template.
			 * \param [in,out] strStart The offset from the start of the template where the
			 * string table starts. Updated to accommodate any gadget and/or keyboard
			 * shortcut templates.
			 * \param [in,out] msgStart The offset from the start of the template where the
			 * message table starts. Updated to accommodate any gadget and/or keyboard
			 * shortcut templates.
			 * \return The number of bytes required to store the window template including
			 * gadgets and keyboard shortcuts.  */
			protected override int CalculateBufferSize (out int strStart, out int msgStart)
			{
				int size = WindowTemplateOffset.Data;

				strStart = msgStart = WindowTemplateOffset.Data;

				if (!string.IsNullOrEmpty (_help_message))
					size += Math.Max (_help_message.Length + 1, _max_help);
				if (!string.IsNullOrEmpty (_pointer_shape))
				{
					int length = Math.Max (_pointer_shape.Length + 1, _max_pointer_shape);
					size += length;
					msgStart += length;
				}
				if (!string.IsNullOrEmpty (_menu))
				{
					int length = _menu.Length + 1;
					size += length;
					msgStart += length;
				}
				if (!string.IsNullOrEmpty (_internal_bl))
				{
					int length = _internal_bl.Length + 1;
					size += length;
					msgStart += length;
				}
				if (!string.IsNullOrEmpty (_internal_tl))
				{
					int length = _internal_tl.Length + 1;
					size += length;
					msgStart += length;
				}
				if (!string.IsNullOrEmpty (_external_bl))
				{
					int length = _external_bl.Length + 1;
					size += length;
					msgStart += length;
				}
				if (!string.IsNullOrEmpty (_external_tl))
				{
					int length = _external_tl.Length + 1;
					size += length;
					msgStart += length;
				}

				size += Attributes.Title.Text.BufferSize;
				if (Attributes.Title.Validation.BufferSize > 1)
				{
					size += Attributes.Title.Validation.BufferSize;
					msgStart += Attributes.Title.Validation.BufferSize;
				}

				foreach (KeyboardShortcutTemplate shortcut in KeyboardShortcuts)
					size += shortcut.CalculateBufferSize (ref strStart, ref msgStart);

				foreach (GadgetTemplate gadget in Gadgets)
					size += gadget.CalculateBufferSize (ref strStart, ref msgStart);

				return size;
			}

			/*! \brief Write the window template data into the buffer.
			 * \param [in] strOffset The offset from the start of the template where the
			 * string table begins.
			 * \param [in] msgOffset The offset from the start of the template where the
			 * message table begins.
			 * \return Nothing.  */
			protected override void BuildBuffer (int strOffset, int msgOffset)
			{
				base.BuildBuffer (strOffset, msgOffset);

				Marshal.WriteInt32 (_buffer, WindowTemplateOffset.Flags, (int)_flags);

				msgOffset = WriteString (_help_message,
							 _max_help,
							 _buffer,
							 WindowTemplateOffset.HelpMessage,
							 msgOffset);
				// If a help message is given then write either the length of the message string
				// or the maximum buffer size, which ever is bigger. If not given, write 0.
				Marshal.WriteInt32 (_buffer,
						    WindowTemplateOffset.MaxHelp,
						    string.IsNullOrEmpty (_help_message) ? 0 :
							Math.Max (_help_message.Length + 1, _max_help));

				strOffset = WriteString (_pointer_shape,
							 _max_pointer_shape,
							 _buffer,
							 WindowTemplateOffset.PointerShape,
							 strOffset);

				// If pointer shape given then write either the length of the pointer shape
				// sprite name or the maximum buffer size, which ever is bigger. If not given,
				// write 0.
				Marshal.WriteInt32 (_buffer,
						    WindowTemplateOffset.MaxPointerShape,
						    string.IsNullOrEmpty (_pointer_shape) ? 0 :
							Math.Max (_pointer_shape.Length + 1, _max_pointer_shape));

				Marshal.WriteInt32 (_buffer, WindowTemplateOffset.PointerXHot, _pointer_hotspot.X);
				Marshal.WriteInt32 (_buffer, WindowTemplateOffset.PointerYHot, _pointer_hotspot.Y);
				strOffset = WriteString (_menu,
							 0,
							 _buffer,
							 WindowTemplateOffset.Menu,
							 strOffset);

				Marshal.WriteInt32 (_buffer, WindowTemplateOffset.NumKeyboardShortcuts,
						    KeyboardShortcuts.Count);
				// ToDo: Add keyboard shortcuts.
				Marshal.WriteInt32 (_buffer, WindowTemplateOffset.KeyboardShortcuts,
						    KeyboardShortcuts.Count == 0 ? 0 :
							(int)_buffer + WindowTemplateOffset.Window + 88);
				Marshal.WriteInt32 (_buffer, WindowTemplateOffset.NumGadgets,
						    Gadgets.Count);
				Marshal.WriteInt32 (_buffer, WindowTemplateOffset.Gadgets,
						    Gadgets.Count == 0 ? 0 :
							(int)_buffer + WindowTemplateOffset.Window + 88 +
								KeyboardShortcuts.Count * KeyboardShortcutTemplateOffset.TemplateSize);
				Marshal.WriteInt32 (_buffer, WindowTemplateOffset.DefaultFocus,
						    (int)_default_focus);
				Marshal.WriteInt32 (_buffer, WindowTemplateOffset.ShowEvent, _show_event);
				Marshal.WriteInt32 (_buffer, WindowTemplateOffset.HideEvent, _hide_event);

				strOffset = WriteString (_internal_bl,
							 0,
							 _buffer,
							 WindowTemplateOffset.InternalBL,
							 strOffset);
				strOffset = WriteString (_internal_tl,
							 0,
							 _buffer,
							 WindowTemplateOffset.InternalTL,
							 strOffset);
				strOffset = WriteString (_external_bl,
							 0,
							 _buffer,
							 WindowTemplateOffset.ExternalBL,
							 strOffset);
				strOffset = WriteString (_external_tl,
							 0,
							 _buffer,
							 WindowTemplateOffset.ExternalTL,
							 strOffset);

				// Window Template
				Marshal.WriteInt32 (_buffer, WindowTemplateOffset.Window + 0,
						    _default_view_rect.MinX);
				Marshal.WriteInt32 (_buffer, WindowTemplateOffset.Window + 4,
						    _default_view_rect.MinY);
				Marshal.WriteInt32 (_buffer, WindowTemplateOffset.Window + 8,
						    _default_view_rect.MaxX);
				Marshal.WriteInt32 (_buffer, WindowTemplateOffset.Window + 12,
						    _default_view_rect.MaxY);
				Marshal.WriteInt32 (_buffer, WindowTemplateOffset.Window + 16,
						    _default_scroll.X);
				Marshal.WriteInt32 (_buffer, WindowTemplateOffset.Window + 20,
						    _default_scroll.Y);
				// Use unchecked syntax because we're converting 0xffffffff to
				// an int which causes an overflow error.
				Marshal.WriteInt32 (_buffer, WindowTemplateOffset.Window + 24,
						    unchecked((int)Wimp.WindowStackPosition.Top));
				Marshal.WriteInt32 (_buffer, WindowTemplateOffset.Window + 28,
						    (int)Attributes.Flags);
				Marshal.WriteByte (_buffer, WindowTemplateOffset.Window + 32,
						   (byte)Attributes.TitleFGColour);
				Marshal.WriteByte (_buffer, WindowTemplateOffset.Window + 33,
						   (byte)Attributes.TitleBGColour);
				Marshal.WriteByte (_buffer, WindowTemplateOffset.Window + 34,
						   (byte)Attributes.WorkAreaFGColour);
				Marshal.WriteByte (_buffer, WindowTemplateOffset.Window + 35,
						   (byte)Attributes.WorkAreaBGColour);
				Marshal.WriteByte (_buffer, WindowTemplateOffset.Window + 36,
						   (byte)Attributes.ScrollBarOuterColour);
				Marshal.WriteByte (_buffer, WindowTemplateOffset.Window + 37,
						   (byte)Attributes.ScrollBarInnerColour);
				Marshal.WriteByte (_buffer, WindowTemplateOffset.Window + 38,
						   (byte)Attributes.TitleFocusColour);
				Marshal.WriteByte (_buffer, WindowTemplateOffset.Window + 39,
						   (byte)Attributes.ExtraFlags);
				Marshal.WriteInt32 (_buffer, WindowTemplateOffset.Window + 40,
						    Attributes.WorkArea.MinX);
				Marshal.WriteInt32 (_buffer, WindowTemplateOffset.Window + 44,
						    Attributes.WorkArea.MinY);
				Marshal.WriteInt32 (_buffer, WindowTemplateOffset.Window + 48,
						    Attributes.WorkArea.MaxX);
				Marshal.WriteInt32 (_buffer, WindowTemplateOffset.Window + 52,
						    Attributes.WorkArea.MaxY);
				Marshal.WriteInt32 (_buffer, WindowTemplateOffset.Window + 56,
						    (int)Attributes.TitleFlags);
				Marshal.WriteInt32 (_buffer, WindowTemplateOffset.Window + 60,
						    (int)Attributes.ButtonType << 12);
				Marshal.WriteInt32 (_buffer, WindowTemplateOffset.Window + 64,
						    Attributes.SpriteArea.ToInt32 ());
				Marshal.WriteInt16 (_buffer, WindowTemplateOffset.Window + 68,
						    (short)Attributes.MinimumWidth);
				Marshal.WriteInt16 (_buffer, WindowTemplateOffset.Window + 70,
						    (short)Attributes.MinimumHeight);

				msgOffset = WriteString (Attributes.Title.Text.Buffer,
							 Attributes.Title.Text.BufferSize,
							 _buffer,
							 WindowTemplateOffset.Window + 72,
							 msgOffset);
				strOffset = WriteString (Attributes.Title.Validation.Buffer,
							 Attributes.Title.Validation.BufferSize,
							 _buffer,
							 WindowTemplateOffset.Window + 76,
							 strOffset);
				Marshal.WriteInt32 (_buffer, WindowTemplateOffset.Window + 80,
						    Attributes.Title.Text.BufferSize);
				// Number of icons - must be 0.
				Marshal.WriteInt32 (_buffer, WindowTemplateOffset.Window + 84, 0);

				int offset = WindowTemplateOffset.Window + 88;
				foreach (KeyboardShortcutTemplate shortcut in KeyboardShortcuts)
				{
					shortcut.BuildBuffer (_buffer, offset, ref strOffset);
					offset += KeyboardShortcutTemplateOffset.TemplateSize;
				}

				foreach (GadgetTemplate gadget in Gadgets)
				{
					gadget.BuildBuffer (_buffer, offset, ref strOffset, ref msgOffset);
					offset += gadget.GetTemplateSize ();
				}
			}

			/*! \brief Return the number of bytes that the template data occupies not including
			 * the string tables.  */
			protected override int GetBodySize ()
			{
				int size = WindowTemplateOffset.Data - WindowTemplateOffset.Flags;

				size += KeyboardShortcuts.Count * KeyboardShortcutTemplateOffset.TemplateSize;

				foreach (GadgetTemplate gadget in Gadgets)
					size += gadget.GetTemplateSize ();

				return size;
			}
		}
	}
}
