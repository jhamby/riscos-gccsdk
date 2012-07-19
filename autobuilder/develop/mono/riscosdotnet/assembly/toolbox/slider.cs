//
// toolbox/slider.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \class Slider
		 * \brief Used to display a 'bar', which may be draggable by the user,
		 * displayed in a 'well'.  */
		public class Slider : Gadget
		{
			static class Method
			{
				public const int SetValue = 576;
				public const int GetValue = 577;
				public const int SetBound = 578;
				public const int GetBound = 579;
				public const int SetColour = 580;
				public const int GetColour = 581;
			}

			/*! \brief The default Toolbox event that a slider can raise.  */
			public static class EventCode
			{
				public const uint ValueChanged = 0x82886;
			}

			/*! \brief Wrap an existing slider, e.g., from a Resource file created
			 * Window.  */
			public Slider (Window window, uint cmpID) : base (window, cmpID)
			{
			}

			/*! \brief The value of the slider.  */
			public int Value
			{
				set { CallMethod_SetR4 (Method.SetValue, (uint)value); }
				get { return (int)CallMethod_GetR0 (Method.GetValue); }
			}

			/*! \brief Sets the Desktop colour used in the slider.
			 * \param [in] barColour Desktop colour for the bar.
			 * \param [in] bgColour Desktop colour for the background
			 * \return Nothing.  */
			public void SetColour (OS.DesktopColour barColour,
					       OS.DesktopColour bgColour)
			{
				CallMethod_SetR4R5 (Method.SetColour, (uint)barColour, (uint)bgColour);
			}

			/*! \brief Returns the Desktop colour used in the slider.
			 * \param [out] barColour Desktop colour for the bar.
			 * \param [out] bgColour Desktop colour for the background
			 * \return Nothing.  */
			public void GetColour (out OS.DesktopColour barColour,
					       out OS.DesktopColour bgColour)
			{
				uint bar_col;
				uint bg_col;

				CallMethod_GetR0R1 (Method.GetColour, out bar_col, out bg_col);

				barColour = (OS.DesktopColour)bar_col;
				bgColour = (OS.DesktopColour)bg_col;
			}
		}
	}
}
