//
// toolbox/display-field.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;
using System.Runtime.InteropServices;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \class DisplayField
		 * Used to display information in a 'read-only' manner.  */
		public class DisplayField : Gadget
		{
			static class Method
			{
				public const int SetValue = 448;
				public const int GetValue = 449;
				public const int SetFont = 450;
			}

			/*! \brief Wrap an existing display field, e.g., from a Resource file created
			 * Window.  */
			public DisplayField (Window window, uint cmpID) : base (window, cmpID)
			{
			}

			/*! \brief The text string shown in the display field.  */
			public string Value
			{
				set { SetText (Method.SetValue, value); }
				get { return GetText (Method.GetValue); }
			}

			/*! \brief Make the display field use an anti-aliased font. If \e fontID
			 * is null, then the system font is used.  */
			public void SetFont (string fontID, int width, int height)
			{
				OS.ThrowOnError (NativeMethods.Toolbox_ObjectMiscOp_SetFont (0,
											     Object.ID,
											     Method.SetFont,
											     ComponentID,
											     fontID,
											     width,
											     height));
			}
		}

		public static class DisplayFieldTemplateOffset
		{
			public const int Text = 36;
			public const int MaxTextLen = 40;
			public const int TemplateSize = 44;
		}

		/*! \brief Encapsulate the data required to create a Toolbox DisplayField gadget
		 * template.  */
		public class DisplayFieldTemplate : GadgetTemplate
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

			/*! \brief Create a template for a display field where the text
			 * will be set later.  */
			public DisplayFieldTemplate () : base (Gadget.ComponentType.DisplayField)
			{
			}

			//! \brief Create a template for a display field using the given text.
			public DisplayFieldTemplate (string text) : base (Gadget.ComponentType.DisplayField)
			{
				_text = text;
			}

			/*! \brief Create a template for a display field using the given text and maximum
			 * text buffer size.  */
			public DisplayFieldTemplate (string text, int maxTextLen) :
							base (Gadget.ComponentType.DisplayField)
			{
				_text = text;
				_max_text_len = maxTextLen;
			}

			public override int CalculateBufferSize (ref int strStart, ref int msgStart)
			{
				int size = base.CalculateBufferSize (ref strStart, ref msgStart);

				if (!string.IsNullOrEmpty (_text))
					size += Math.Max (_text.Length + 1, _max_text_len);

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
									offset + DisplayFieldTemplateOffset.Text,
									msgOffset);
				Marshal.WriteInt32 (buffer,
						    offset + DisplayFieldTemplateOffset.MaxTextLen,
							Math.Max (_text.Length + 1, _max_text_len));
			}

			public override int GetTemplateSize ()
			{
				return DisplayFieldTemplateOffset.TemplateSize;
			}
		}
	}
}
