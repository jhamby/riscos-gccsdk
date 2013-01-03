//
// toolbox/button.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;

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
	}
}
