//
// toolbox/labelled-box.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \class LabelledBox
		 * \brief Used for collecting together a set of related items.  */
		public class LabelledBox : Gadget
		{
			/*! \brief Wrap an existing labelled box, e.g., from a Resource file created
			 * Window.  */
			public LabelledBox (Window window, uint cmpID) : base (window, cmpID)
			{
			}
		}
	}
}
