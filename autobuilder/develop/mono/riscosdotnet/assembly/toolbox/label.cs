//
// toolbox/label.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \class Label
		 * \brief Consists of some explanation text which appears in a dialogue box.
		 *
		 * The client application can choose whether the bounding box of the label
		 * is shown by a visible box or not.  */
		public class Label : Gadget
		{
			/*! \brief Wrap an existing label, e.g., from a Resource file created
			 * Window.  */
			public Label (Window window, uint cmpID) : base (window, cmpID)
			{
			}
		}
	}
}
