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
		public class IconBuffer
		{
			public IntPtr Buffer = IntPtr.Zero;
			public int BufferSize = 0;

			public IconBuffer ()
			{
				Set ("");
			}

			public IconBuffer (string text)
			{
				Set (text);
			}

			public IconBuffer (string text,
					   int maxLen)
			{
				Set (text, maxLen);
			}

			~IconBuffer ()
			{
				Clear ();
			}

			public void Clear ()
			{
				if (Buffer != IntPtr.Zero)
				{
					Marshal.FreeHGlobal (Buffer);
					Buffer = IntPtr.Zero;
					BufferSize = 0;
				}
			}

			public void Set (string text)
			{
				Set (text, text.Length + 1);
			}

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
					to_copy = BufferSize;
				Marshal.Copy (strbuf, 0, Buffer, to_copy);

				// Zero terminate
				Marshal.WriteByte ((IntPtr)(Buffer.ToInt32() + to_copy), 0);
			}
		}

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

		public class IconAttributes
		{
			IconFlags _Flags = IconFlags.Text |
					   IconFlags.Border |
					   IconFlags.FilledBackground |
					   IconFlags.Indirected |
					   IconFlags.VCentred |
					   IconFlags.HCentred;
			public uint Flags
			{
				get { return (uint)_Flags; }
			}

			public OS.Rect BoundingBox = new OS.Rect ();
			public IconData Data;

			public IconAttributes (string text, int textMaxLen,
					       string validation, int validationMaxLen)
			{
				Data = new IconData (new IconBuffer (text, textMaxLen),
						     new IconBuffer (validation, validationMaxLen));
				DefaultColours ();
			}

			public IconAttributes (string text, int textMaxLen, string validation)
			{
				Data = new IconData (new IconBuffer (text, textMaxLen),
						     new IconBuffer (validation));
				DefaultColours ();
			}

			public IconAttributes (string text, int textMaxLen)
			{
				Data = new IconData (new IconBuffer (text, textMaxLen));
				DefaultColours ();
			}

			public IconAttributes (string text)
			{
				Data = new IconData (new IconBuffer (text));
				DefaultColours ();
			}

			public IconAttributes (string text, string validation)
			{
				Data = new IconData (new IconBuffer (text),
						     new IconBuffer (validation));
				DefaultColours ();
			}

			void DefaultColours ()
			{
				FGColour = OS.DesktopColour.Black;
				BGColour = OS.DesktopColour.Grey3;
			}

			/*! \brief Sets or gets whether the icon contains text.  */
			public bool ContainsText {
				set {
					_Flags = value ? _Flags |  IconFlags.Text :
							 _Flags & ~IconFlags.Text;
				}
				get {
					return (_Flags & IconFlags.Text) != 0;
				}
			}

			/*! \brief Sets or gets whether the icon contains a sprite.  */
			public bool ContainsSprite {
				set {
					_Flags = value ? _Flags |  IconFlags.Sprite :
							 _Flags & ~IconFlags.Sprite;
				}
				get {
					return (_Flags & IconFlags.Sprite) != 0;
				}
			}

			/*! \brief Sets or gets whether the icon has a border.  */
			public bool HasBorder {
				set {
					_Flags = value ? _Flags |  IconFlags.Border :
							 _Flags & ~IconFlags.Border;
				}
				get {
					return (_Flags & IconFlags.Border) != 0;
				}
			}

			/*! \brief Sets or gets whether the icon contents are horizontally centred.  */
			public bool CentredHorizontally {
				set {
					_Flags = value ? _Flags | IconFlags.HCentred :
							 _Flags & ~IconFlags.HCentred;
				}
				get {
					return (_Flags & IconFlags.HCentred) != 0;
				}
			}

			/*! \brief Sets or gets whether the icon contents are vertically centred.  */
			public bool CentredVertically {
				set {
					_Flags = value ? _Flags | IconFlags.VCentred :
							 _Flags & ~IconFlags.VCentred;
				}
				get {
					return (_Flags & IconFlags.VCentred) != 0;
				}
			}

			/*! \brief Sets or gets whether the icon has a filled background.  */
			public bool FilledBackground {
				set {
					_Flags = value ? _Flags | IconFlags.FilledBackground :
							 _Flags & ~IconFlags.FilledBackground;
				}
				get {
					return (_Flags & IconFlags.FilledBackground) != 0;
				}
			}

			/*! \brief Sets or gets whether the icon text is anti aliased.  */
			public bool AntiAliased {
				set {
					_Flags = value ? _Flags | IconFlags.AntiAliased :
							 _Flags & ~IconFlags.AntiAliased;
				}
				get {
					return (_Flags & IconFlags.AntiAliased) != 0;
				}
			}

			/*! \brief Sets or gets whether the icon requires the task's help to be redrawn.  */
			public bool RedrawnByTask {
				set {
					_Flags = value ? _Flags | IconFlags.RedrawnByTask :
							 _Flags & ~IconFlags.RedrawnByTask;
				}
				get {
					return (_Flags & IconFlags.RedrawnByTask) != 0;
				}
			}

			/*! \brief Sets or gets whether the icon data is indirected.  */
			public bool IndirectedData {
				set {
					_Flags = value ? _Flags | IconFlags.Indirected :
							 _Flags & ~IconFlags.Indirected;
				}
				get {
					return (_Flags & IconFlags.Indirected) != 0;
				}
			}

			/*! \brief Sets or gets whether the icon is right justified.  */
			public bool RightJustified {
				set {
					_Flags = value ? _Flags | IconFlags.RightJustified :
							 _Flags & ~IconFlags.RightJustified;
				}
				get {
					return (_Flags & IconFlags.RightJustified) != 0;
				}
			}

			/*! \brief Sets or gets whether a selection with Adjust should not cancel
			 * others in the same ESG.  */
			public bool DontCancelSameESG {
				set {
					_Flags = value ? _Flags | IconFlags.DontCancelSameESG :
							 _Flags & ~IconFlags.DontCancelSameESG;
				}
				get {
					return (_Flags & IconFlags.DontCancelSameESG) != 0;
				}
			}

			/*! \brief Sets or gets whether to display the sprite (if any) at half size.  */
			public bool HalfSizeSprite {
				set {
					_Flags = value ? _Flags | IconFlags.HalfSizeSprite :
							 _Flags & ~IconFlags.HalfSizeSprite;
				}
				get {
					return (_Flags & IconFlags.HalfSizeSprite) != 0;
				}
			}

			/*! \brief Sets or gets whether the icon is selected by the user and is inverted.  */
			public bool Selected {
				set {
					_Flags = value ? _Flags | IconFlags.Selected :
							 _Flags & ~IconFlags.Selected;
				}
				get {
					return (_Flags & IconFlags.Selected) != 0;
				}
			}

			/*! \brief Sets or gets whether the icon is shaded and cannot be selected.  */
			public bool Shaded {
				set {
					_Flags = value ? _Flags | IconFlags.Shaded :
							 _Flags & ~IconFlags.Shaded;
				}
				get {
					return (_Flags & IconFlags.Shaded) != 0;
				}
			}

			/*! \brief Gets whether the icon has been deleted.  */
			public bool Deleted {
				get {
					return (_Flags & IconFlags.Deleted) != 0;
				}
			}

			/*! \brief Sets or gets the button type of the icon.  */
			public IconButtonType ButtonType {
				set {
					_Flags |= (IconFlags)(((int)value & 0xf) << 12);
				}
				get {
					return (IconButtonType)(((int)_Flags >> 12) & 0xf);
				}
			}

			/*! \brief Sets or gets the Exclusive Selection Group for the icon.  */
			public int ESG {
				set {
					_Flags |= (IconFlags)((value & 0x1f) << 16);
				}
				get {
					return ((int)_Flags >> 16) & 0x1f;
				}
			}

			/*! \brief Sets or gets the handle of the font used for this icon.
			 * \note The \e AntiAliased bit is automatically set.
			 * \throw InvalidOperationException Thrown if the icon is not anti-aliased when read.  */
			public uint Font {
				set {
					AntiAliased = true;
					_Flags |= (IconFlags)((value & 0xff) << 24);
				}
				get {
					if (!AntiAliased)
						throw new InvalidOperationException ("Icon is not anti-aliased.");

					return ((uint)_Flags >> 24) & 0xff;
				}
			}

			/*! \brief Sets or gets the foreground colour of the icon.
			 * \note The \e AntiAliased bit is automatically cleared.
			 * \throw InvalidOperationException Thrown if the icon is anti-aliased when read.  */
			public OS.DesktopColour FGColour {
				set {
					AntiAliased = false;
					_Flags |= (IconFlags)(((int)value & 0xf) << 24);
				}
				get {
					if (AntiAliased)
						throw new InvalidOperationException ("Icon is anti-aliased; use validation string to set colours.");

					return (OS.DesktopColour)(((int)_Flags >> 24) & 0xf);
				}
			}

			/*! \brief Sets or gets the background colour of the icon.
			 * \note The \e AntiAliased bit is automatically cleared.
			 * \throw InvalidOperationException Thrown if the icon is anti-aliased when read.  */
			public OS.DesktopColour BGColour {
				set {
					AntiAliased = false;
					_Flags |= (IconFlags)(((int)value & 0xf) << 28);
				}
				get {
					if (AntiAliased)
						throw new InvalidOperationException ("Icon is anti-aliased; use validation string to set colours.");

					return (OS.DesktopColour)(((int)_Flags >> 28) & 0xf);
				}
			}
		}

		public class Icon
		{
			public int Handle;

			IconData Data;

			uint WindowHandle;

			public Icon (uint windowHandle, IconAttributes attr)
			{
				WindowHandle = windowHandle;
				Data = attr.Data;
				Handle = Wimp.CreateIcon (windowHandle, attr);
			}

			~Icon ()
			{
			}

			public void Delete ()
			{
				Wimp.DeleteIcon (WindowHandle, Handle);
			}

			public void SetText (string text)
			{
				Data.SetText (text);
			}

			public void SetValidation (string text)
			{
				Data.SetValidation (text);
			}
		}
	}
}
 
