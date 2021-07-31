//
// toolbox/button.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;
using System.Runtime.InteropServices;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \brief Similar to a WIMP icon, but always indirected and not all icon flags are settable. */
		public class Button : Gadget
		{
			static class Method
			{
				public const int GetFlags = 960;
				public const int SetFlags = 961;
				public const int SetValue = 962;
				public const int GetValue = 963;
				public const int SetValidation = 964;
				public const int GetValidation = 965;
				public const int SetFont = 966;
			}

			/*! \brief Wrap an existing button, e.g., from a Resource file created
			 * Window.  */
			public Button (Window window, uint cmpID) : base (window, cmpID)
			{
			}

			/*! \brief The text or sprite name of a Button.  */
			public string Value
			{
				set { SetText (Method.SetValue, value); }
				get { return GetText (Method.GetValue); }
			}

			/*! \brief The validation string (e.g. sprite name) of a Button.  */
			public string Validation
			{
				set { SetText (Method.SetValidation, value); }
				get { return GetText (Method.GetValidation); }
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

			/*! \brief Return the WIMP icon flags of this Button.
			 * \return The icon flags.  */
			public uint GetFlags ()
			{
				return Object.MiscOp_SetR3GetR0 (0, Method.GetFlags, ComponentID);
			}

			/*! \brief Sets the WIMP icon flags of the Button.
			 *
			 * The effect of the clear word and the EOR word are analogous to those
			 * of \e Wimp.Icon.SetIconState, except not all combinations are settable.  */
			public void SetFlags (uint clearWord, uint eorWord)
			{
				Object.MiscOp_SetR3R4R5 (0,
							 Method.SetFlags,
							 ComponentID,
							 clearWord,
							 eorWord);
			}
		}

		public static class ButtonTemplateOffset
		{
			public const int ButtonFlags = 36;
			public const int Text = 40;
			public const int MaxTextLen = 44;
			public const int Validation = 48;
			public const int MaxValidation = 52;
			public const int TemplateSize = 56;
		}

		public sealed class ButtonTemplate : GadgetTemplate
		{
			//! \brief The Wimp icon flags that will be used in this button gadget template.
			public Wimp.IconFlagsValue ButtonFlags =
					new Wimp.IconFlagsValue (Wimp.IconFlags.Text |
								 Wimp.IconFlags.Border |
								 Wimp.IconFlags.FilledBackground |
								 Wimp.IconFlags.Indirected |
								 Wimp.IconFlags.VCentred |
								 Wimp.IconFlags.HCentred);

			string _text = "";
			//! \brief The text or sprite name that will be used in this gadget template.
			public string Text {
				get { return _text; }
				set { _text = value; }
			}

			int _max_text_len = 0;
			//! \brief The maximum length of the text.
			public int MaxTextLen {
				get { return _max_text_len; }
				set { _max_text_len = value; }
			}

			string _validation = "";
			/*! \brief The validation string (e.g. sprite name) that will be used in this
			 * gadget template.  */
			public string Validation {
				get { return _validation; }
				set { _validation = value; }
			}

			int _max_validation_len = 0;
			//! \brief The maximum length of the validation string.
			public int MaxValidationLen {
				get { return _max_validation_len; }
				set { _max_validation_len = value; }
			}

			/*! \brief Create a template for a button where the text (if any)
			 * will be set later.  */
			public ButtonTemplate () : base (Gadget.ComponentType.Button)
			{
			}

			/*! \brief Create a template for a button with the given text.  */
			public ButtonTemplate (string text) : base (Gadget.ComponentType.Button)
			{
				_text = text;
			}

			/*! \brief Create a template for a button with the given text.  */
			public ButtonTemplate (string text, string validation) :
							base (Gadget.ComponentType.Button)
			{
				_text = text;
				_validation = validation;
			}

			/*! \brief Create a template for a button with the given text.  */
			public ButtonTemplate (string text, int maxTextLen) : base (Gadget.ComponentType.Button)
			{
				_text = text;
				_max_text_len = maxTextLen;
			}

			/*! \brief Create a template for a button with the given text and validation string.  */
			public ButtonTemplate (string text, int maxTextLen,
					       string validation, int maxValidationLen) :
							base (Gadget.ComponentType.Button)
			{
				_text = text;
				_max_text_len = maxTextLen;
				_validation = validation;
				_max_validation_len = maxValidationLen;
			}

			public override int CalculateBufferSize (ref int strStart, ref int msgStart)
			{
				int size = base.CalculateBufferSize (ref strStart, ref msgStart);

				if (!string.IsNullOrEmpty (_text))
					size += Math.Max (_text.Length + 1, _max_text_len);
				if (!string.IsNullOrEmpty (_validation))
				{
					int length = _validation.Length + 1;
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

				Marshal.WriteInt32 (buffer,
						    offset + ButtonTemplateOffset.ButtonFlags,
						    ButtonFlags);
				msgOffset = ObjectTemplate.WriteString (_text,
									_max_text_len,
									buffer,
									offset + ButtonTemplateOffset.Text,
									msgOffset);
				Marshal.WriteInt32 (buffer,
						    offset + ButtonTemplateOffset.MaxTextLen,
						    Math.Max (_text.Length + 1, _max_text_len));
				strOffset = ObjectTemplate.WriteString (_validation,
									_max_validation_len,
									buffer,
									offset + ButtonTemplateOffset.Validation,
									strOffset);
				Marshal.WriteInt32 (buffer,
						    offset + ButtonTemplateOffset.MaxValidation,
						    Math.Max (_validation.Length + 1, _max_validation_len));
			}

			public override int GetTemplateSize ()
			{
				return ButtonTemplateOffset.TemplateSize;
			}
		}
	}
}
