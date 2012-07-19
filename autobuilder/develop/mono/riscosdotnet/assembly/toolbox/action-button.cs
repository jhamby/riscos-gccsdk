//
// toolbox/action-button.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;
using System.Text;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \class ActionButton
		 * \brief Normally used to invoke an operation which is available from a
		 * dialogue box (e.g. a Cancel button or an OK button).  */
		public class ActionButton : Gadget
		{
			static class Method
			{
				public const int SetText = 128;
				public const int GetText = 129;
				public const int SetEvent = 130;
				public const int GetEvent = 131;
				public const int SetClickShow = 132;
				public const int GetClickShow = 133;
			}

			/*! \brief The default Toolbox event that an action button can raise.  */
			public static class EventCode
			{
				public const uint Selected = 0x82881;
			}

			/*! \brief Wrap an existing action button, e.g., from a Resource file created
			 * Window.  */
			public ActionButton (Window window, uint cmpID) : base (window, cmpID)
			{
			}

			/*! \brief The text which is displayed in the action button.  */
			public string Text
			{
				set { SetText (Method.SetText, value); }
				get { return GetText (Method.GetText); }
			}

			/*! \brief The event which will be raised when the action button is clicked.  */
			public uint Event
			{
				set { CallMethod_SetR4 (Method.SetEvent, value); }
				get { return CallMethod_GetR0 (Method.GetEvent); }
			}

			/*! \brief The Toolbox object to show and flags to use when the action
			 * button is clicked.  */
			public Toolbox.ClickShow ClickShow
			{
				set { SetClickShow (Method.SetClickShow, value); }
				get { return GetClickShow (Method.GetClickShow); }
			}
		}
	}
}
