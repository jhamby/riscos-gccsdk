//
// toolbox/writable-field.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \class WritableField
		 * \brief A boxed display area in which a text string is displayed and can be
		 * edited by the user.  */
		public class WritableField : Gadget
		{
			static class Method
			{
				public const int SetValue = 512;
				public const int GetValue = 513;
				public const int SetAllowable = 514;
				public const int SetFont = 516;
			}

			/*! \brief The default Toolbox event that a writable field can raise.  */
			public static class EventCode
			{
				public const uint ValueChanged = 0x82885;
			}

			/*! \brief Wrap an existing writable field, e.g., from a Resource file created
			 * Window.  */
			public WritableField (Window window, uint cmpID) : base (window, cmpID)
			{
			}

			/*! \brief The text string shown in the writable field.  */
			public string Value
			{
				set { SetText (Method.SetValue, value); }
				get { return GetText (Method.GetValue); }
			}

			/*! \brief Defines the set of allowable characters which can be typed into
			 * a writable field. 
			 * 
			 * The set is specified in the same way as a WIMP 'a' validation string
			 * directive (without including the letter 'a').
			 * \note Set to null to allow all characters.  */
			public string Allowable
			{
				set { SetText (Method.SetAllowable, value); }
			}

			/*! \brief Make the writable field use an anti-aliased font. If \e fontID
			 * is null, then the system font is used.  */
			public void SetFont (string fontID, int width, int height)
			{
				OS.ThrowOnError (NativeMethods.Component_SetFont (0,
										  Object.ID,
										  Method.SetFont,
										  ComponentID,
										  fontID,
										  width,
										  height));
			}
		}
	}
}
