//
// toolbox/number-range.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \brief Used to display one of a range of possible integer or fixed
		 * point values.  */
		public class NumberRange : Gadget
		{
			static class Method
			{
				public const int SetValue = 832;
				public const int GetValue = 833;
				public const int SetBounds = 834;
				public const int GetBounds = 835;
				public const int GetComponents = 836;
			}

			/*! \brief The default Toolbox event that a number range can raise.  */
			public static class EventCode
			{
				public const uint ValueChanged = 0x8288d;
			}

			/*! \brief Wrap an existing number range, e.g., from a Resource file created
			 * Window.  */
			public NumberRange (Window window, uint cmpID) : base (window, cmpID)
			{
			}

			/*! \brief The value of the number range as an integer.  */
			public int Value
			{
				set { CallMethod_SetR4 (Method.SetValue, (uint)value); }
				get { return (int)CallMethod_GetR0 (Method.GetValue); }
			}

/* TODO:
   GetBounds
   SetBounds
   GetComponents
*/
		}
	}
}
