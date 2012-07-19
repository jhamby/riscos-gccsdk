//
// toolbox/adjuster-arrows.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \class AdjusterArrows
		 * \brief Displayed as an up, down, left or right arrow icon.
		 *
		 * Clicking on the arrow will raise an AdjusterClicked Toolbox event with an
		 * indication of whether the change is up or down.  */
		public class AdjusterArrows : Gadget
		{
			/*! \brief The default Toolbox event that an adjuster arrow can raise.  */
			public static class EventCode
			{
				public const uint AdjusterClicked = 0x8288c;
			}

			/*! \brief Wrap an existing adjuster arrow, e.g., from a Resource file created
			 * Window.  */
			public AdjusterArrows (Window window, uint cmpID) : base (window, cmpID)
			{
			}
		}
	}
}
