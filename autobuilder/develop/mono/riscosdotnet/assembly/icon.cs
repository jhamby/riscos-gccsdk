//
// icon.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Runtime.InteropServices;
using System.Text;
using riscos;

namespace riscos
{
	public static partial class Wimp
	{
		/*! \brief Encapsulate the data required to represent an icon buffer, ie, either the
		 * text of an icon or its validation string.  */
		public class IconBuffer
		{
			public IntPtr Buffer = IntPtr.Zero;
			public int BufferSize = 0;

			// FixMe: Is this ctor any use?
			public IconBuffer ()
			{
				Set ("");
			}

			/*! \brief Create an icon buffer with a fixed string that is known to be permanent.  */
			public IconBuffer (string text)
			{
				Set (text);
			}

			/*! \brief Create an icon buffer with a string that may need to be changed after the
			 * icon has been created. The buffer can be made bigger than the initial string to
			 * allow a longer string to be set later.  */
			public IconBuffer (string text,
					   int maxLen)
			{
				Set (text, maxLen);
			}

			~IconBuffer ()
			{
				Clear ();
			}

			/*! \brief Free the memory used by an icon buffer. \b Never use when the icon
			 * still exists.  */
			public void Clear ()
			{
				if (Buffer != IntPtr.Zero)
				{
					Marshal.FreeHGlobal (Buffer);
					Buffer = IntPtr.Zero;
					BufferSize = 0;
				}
			}

			/*! \brief Set the icon buffer to the given text. Used where the initial text is
			 * known to be permanent.  */
			public void Set (string text)
			{
				Set (text, text.Length + 1);
			}

			/*! \brief Set the icon buffer to the given text with the given maximum length.
			 * Used where the text may be changed after the icon has been created to something
			 * longer than initially assigned.  */
			public void Set (string text,
				    	 int maxLen)
			{
				if (Buffer == IntPtr.Zero)
				{
					Buffer = Marshal.AllocHGlobal (maxLen);
					BufferSize = maxLen;
				}

				byte[] strbuf = Encoding.UTF8.GetBytes (text);
				int to_copy = strbuf.Length;

				if (to_copy >= BufferSize)
					to_copy = BufferSize - 1;
				Marshal.Copy (strbuf, 0, Buffer, to_copy);

				// Zero terminate
				Marshal.WriteByte ((IntPtr)(Buffer.ToInt32() + to_copy), 0);
			}

			/*! \brief Retrieve the string from the icon buffer.  */
			public string Get ()
			{
				return Marshal.PtrToStringAuto (Buffer);
			}
		}

		/*! \brief Encapsulate the data required to represent icon data, ie, the
		 * text of an icon and its validation string.  */
		public class IconData
		{
			public IconBuffer Text;
			public IconBuffer Validation;

			public IconData (IconBuffer text, IconBuffer validation)
			{
				Text = text;
				Validation = validation;
			}

			public IconData (IconBuffer text) : this (text, new IconBuffer (""))
			{
			}

			public void SetText (string text, int maxLen)
			{
				Text.Set (text, maxLen);
			}

			public void SetText (string text)
			{
				Text.Set (text);
			}

			public void SetValidation (string text, int maxLen)
			{
				Validation.Set (text, maxLen);
			}

			public void SetValidation (string text)
			{
				Validation.Set (text);
			}
		}

		/*! \brief The flags word used in a Wimp icon.
		 * Properties are used to read and write the individual fields.  */
		public struct IconFlagsValue
		{
			IconFlags _flags;

			public IconFlagsValue (IconFlags flags)
			{
				_flags = flags;
			}

			public static implicit operator int (IconFlagsValue value)
			{
				return (int)value._flags;
			}

			public static implicit operator uint (IconFlagsValue value)
			{
				return (uint)value._flags;
			}

			/*! \brief Sets or gets whether the icon contains text.  */
			public bool ContainsText {
				set {
					_flags = value ? _flags |  IconFlags.Text :
							 _flags & ~IconFlags.Text;
				}
				get {
					return (_flags & IconFlags.Text) != 0;
				}
			}

			/*! \brief Sets or gets whether the icon contains a sprite.  */
			public bool ContainsSprite {
				set {
					_flags = value ? _flags |  IconFlags.Sprite :
							 _flags & ~IconFlags.Sprite;
				}
				get {
					return (_flags & IconFlags.Sprite) != 0;
				}
			}

			/*! \brief Sets or gets whether the icon has a border.  */
			public bool HasBorder {
				set {
					_flags = value ? _flags |  IconFlags.Border :
							 _flags & ~IconFlags.Border;
				}
				get {
					return (_flags & IconFlags.Border) != 0;
				}
			}

			/*! \brief Sets or gets whether the icon contents are horizontally centred.  */
			public bool CentredHorizontally {
				set {
					_flags = value ? _flags | IconFlags.HCentred :
							 _flags & ~IconFlags.HCentred;
				}
				get {
					return (_flags & IconFlags.HCentred) != 0;
				}
			}

			/*! \brief Sets or gets whether the icon contents are vertically centred.  */
			public bool CentredVertically {
				set {
					_flags = value ? _flags | IconFlags.VCentred :
							 _flags & ~IconFlags.VCentred;
				}
				get {
					return (_flags & IconFlags.VCentred) != 0;
				}
			}

			/*! \brief Sets or gets whether the icon has a filled background.  */
			public bool FilledBackground {
				set {
					_flags = value ? _flags | IconFlags.FilledBackground :
							 _flags & ~IconFlags.FilledBackground;
				}
				get {
					return (_flags & IconFlags.FilledBackground) != 0;
				}
			}

			/*! \brief Sets or gets whether the icon text is anti aliased.  */
			public bool AntiAliased {
				set {
					_flags = value ? _flags | IconFlags.AntiAliased :
							 _flags & ~IconFlags.AntiAliased;
				}
				get {
					return (_flags & IconFlags.AntiAliased) != 0;
				}
			}

			/*! \brief Sets or gets whether the icon requires the task's help to be redrawn.  */
			public bool RedrawnByTask {
				set {
					_flags = value ? _flags | IconFlags.RedrawnByTask :
							 _flags & ~IconFlags.RedrawnByTask;
				}
				get {
					return (_flags & IconFlags.RedrawnByTask) != 0;
				}
			}

			/*! \brief Sets or gets whether the icon data is indirected.  */
			public bool IndirectedData {
				set {
					_flags = value ? _flags | IconFlags.Indirected :
							 _flags & ~IconFlags.Indirected;
				}
				get {
					return (_flags & IconFlags.Indirected) != 0;
				}
			}

			/*! \brief Sets or gets whether the icon is right justified.  */
			public bool RightJustified {
				set {
					_flags = value ? _flags | IconFlags.RightJustified :
							 _flags & ~IconFlags.RightJustified;
				}
				get {
					return (_flags & IconFlags.RightJustified) != 0;
				}
			}

			/*! \brief Sets or gets whether a selection with Adjust should not cancel
			 * others in the same ESG.  */
			public bool DontCancelSameESG {
				set {
					_flags = value ? _flags | IconFlags.DontCancelSameESG :
							 _flags & ~IconFlags.DontCancelSameESG;
				}
				get {
					return (_flags & IconFlags.DontCancelSameESG) != 0;
				}
			}

			/*! \brief Sets or gets whether to display the sprite (if any) at half size.  */
			public bool HalfSizeSprite {
				set {
					_flags = value ? _flags | IconFlags.HalfSizeSprite :
							 _flags & ~IconFlags.HalfSizeSprite;
				}
				get {
					return (_flags & IconFlags.HalfSizeSprite) != 0;
				}
			}

			/*! \brief Sets or gets whether the icon is selected by the user and is inverted.  */
			public bool Selected {
				set {
					_flags = value ? _flags | IconFlags.Selected :
							 _flags & ~IconFlags.Selected;
				}
				get {
					return (_flags & IconFlags.Selected) != 0;
				}
			}

			/*! \brief Sets or gets whether the icon is shaded and cannot be selected.  */
			public bool Shaded {
				set {
					_flags = value ? _flags | IconFlags.Shaded :
							 _flags & ~IconFlags.Shaded;
				}
				get {
					return (_flags & IconFlags.Shaded) != 0;
				}
			}

			/*! \brief Gets whether the icon has been deleted.  */
			public bool Deleted {
				get {
					return (_flags & IconFlags.Deleted) != 0;
				}
			}

			/*! \brief Sets or gets the button type of the icon.  */
			public IconButtonType ButtonType {
				set {
					// Clear all the button type bits first.
					_flags &= (IconFlags)~(0xf << 12);
					// Now insert the required button type.
					_flags |= (IconFlags)(((int)value & 0xf) << 12);
				}
				get {
					return (IconButtonType)(((int)_flags >> 12) & 0xf);
				}
			}

			/*! \brief Sets or gets the Exclusive Selection Group for the icon.  */
			public int ESG {
				set {
					// Clear all the ESG bits first.
					_flags &= (IconFlags)~(0x1f << 16);
					// Now insert the required ESG.
					_flags |= (IconFlags)((value & 0x1f) << 16);
				}
				get {
					return ((int)_flags >> 16) & 0x1f;
				}
			}

			/*! \brief Sets or gets the handle of the font used for this icon.
			 * \note The \e AntiAliased bit is automatically set.
			 * \throw InvalidOperationException Thrown if the icon is not anti-aliased when read.  */
			public uint Font {
				set {
					AntiAliased = true;
					// Clear all the font bits first
					_flags &= (IconFlags)~(0xff << 24);
					// Now insert the required font handle.
					_flags |= (IconFlags)((value & 0xff) << 24);
				}
				get {
					if (!AntiAliased)
						throw new InvalidOperationException ("Icon is not anti-aliased.");

					return ((uint)_flags >> 24) & 0xff;
				}
			}

			/*! \brief Sets or gets the foreground colour of the icon.
			 * \note The \e AntiAliased bit is automatically cleared.
			 * \throw InvalidOperationException Thrown if the icon is anti-aliased when read.  */
			public OS.DesktopColour FGColour {
				set {
					AntiAliased = false;
					// Clear all the foreground colour bits first.
					_flags &= (IconFlags)~(0xf << 24);
					// Now insert the required colour.
					_flags |= (IconFlags)(((int)value & 0xf) << 24);
				}
				get {
					if (AntiAliased)
						throw new InvalidOperationException ("Icon is anti-aliased; use validation string to set colours.");

					return (OS.DesktopColour)(((int)_flags >> 24) & 0xf);
				}
			}

			/*! \brief Sets or gets the background colour of the icon.
			 * \note The \e AntiAliased bit is automatically cleared.
			 * \throw InvalidOperationException Thrown if the icon is anti-aliased when read.  */
			public OS.DesktopColour BGColour {
				set {
					AntiAliased = false;
					// Clear all the background colour bits first.
					_flags &= (IconFlags)~(0xf << 28);
					// Now insert the required colour.
					_flags |= (IconFlags)(((int)value & 0xf) << 28);
				}
				get {
					if (AntiAliased)
						throw new InvalidOperationException ("Icon is anti-aliased; use validation string to set colours.");

					return (OS.DesktopColour)(((int)_flags >> 28) & 0xf);
				}
			}
		}

		public class IconAttributes
		{
			public IconFlagsValue Flags = new IconFlagsValue (IconFlags.Text |
									  IconFlags.Border |
									  IconFlags.FilledBackground |
									  IconFlags.Indirected |
									  IconFlags.VCentred |
									  IconFlags.HCentred);

			public OS.Rect BoundingBox = new OS.Rect ();
			public IconData Data;

			/*! \brief Create IconAttributes where we wish to set the text and validation strings
			 * of the icon and possibly change either one after the icon is created.  */
			public IconAttributes (string text, int textMaxLen,
					       string validation, int validationMaxLen)
			{
				Data = new IconData (new IconBuffer (text, textMaxLen),
						     new IconBuffer (validation, validationMaxLen));
				DefaultColours ();
			}

			/*! \brief Create IconAttributes where we wish to set the text and validation strings
			 * of the icon and possibly change the text after the icon is created. The validation
			 * string is fixed. */
			public IconAttributes (string text, int textMaxLen, string validation)
			{
				Data = new IconData (new IconBuffer (text, textMaxLen),
						     new IconBuffer (validation));
				DefaultColours ();
			}

			/*! \brief Create IconAttributes where we wish to set the text string of the icon
			 * and possibly change the text after the icon is created, but we don't care about
			 * the validation string.  */
			public IconAttributes (string text, int textMaxLen)
			{
				Data = new IconData (new IconBuffer (text, textMaxLen));
				DefaultColours ();
			}

			/*! \brief Create IconAttributes where we wish to set the text string of the icon,
			 * which will be fixed, but we don't care about the validation string.  */
			public IconAttributes (string text)
			{
				Data = new IconData (new IconBuffer (text));
				DefaultColours ();
			}

			/*! \brief Create IconAttributes where we wish to set fixed text and validation
			 * strings for the icon.  */
			public IconAttributes (string text, string validation)
			{
				Data = new IconData (new IconBuffer (text),
						     new IconBuffer (validation));
				DefaultColours ();
			}

			void DefaultColours ()
			{
				Flags.FGColour = OS.DesktopColour.Black;
				Flags.BGColour = OS.DesktopColour.Grey2;
			}

		}

		/*! \brief Encapsulate a Wimp icon.  */
		public class Icon
		{
			/*! \brief The Wimp handle of this icon.  */
			public int Handle;

			/*! \brief The text and validation string buffers.  */
			IconData Data;

			/*! \brief The window that contains this icon.  */
			uint WindowHandle;

			/*! \brief Create an icon in the given window from the given attributes.  */
			public Icon (uint windowHandle, IconAttributes attr)
			{
				WindowHandle = windowHandle;
				Data = attr.Data;
				Handle = Wimp.CreateIcon (windowHandle, attr);
			}

			~Icon ()
			{
			}

			/*! \brief Tell the Wimp to mark this icon as deleted.  */
			public void Delete ()
			{
				Wimp.DeleteIcon (WindowHandle, Handle);
			}

			/*! \brief Set the text of the icon. The buffer that the icon was
			 * created with must be big enough for the new string and terminator.  */
			public void SetText (string text)
			{
				Data.SetText (text);
			}

			/*! \brief Set the validation string of the icon. The buffer that the icon was
			 * created with must be big enough for the new string and terminator.  */
			public void SetValidation (string text)
			{
				Data.SetValidation (text);
			}

			/*! \brief Move the caret to the given index in this icon.  */
			public void SetCaretPosition (int index)
			{
				Wimp.SetCaretPosition (WindowHandle, Handle, 0, 0, -1, index);
			}
		}
	}
}
 
