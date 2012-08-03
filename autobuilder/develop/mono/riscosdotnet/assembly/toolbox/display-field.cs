//
// toolbox/display-field.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;

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
