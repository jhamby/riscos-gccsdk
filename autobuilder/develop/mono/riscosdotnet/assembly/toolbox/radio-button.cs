//
// toolbox/radio-button.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \class RadioButton
		 * \brief Used for making a single choice from a set of options.  */
		public class RadioButton : Gadget
		{
			static class Method
			{
				public const int SetLabel = 384;
				public const int GetLabel = 385;
				public const int SetEvent = 386;
				public const int GetEvent = 387;
				public const int SetState = 388;
				public const int GetState = 389;
			}

			/*! \brief The default Toolbox event that a radio button can raise.  */
			public static class EventCode
			{
				public const uint StateChanged = 0x82883;
			}

			public enum State
			{
				Off,
				On
			}

			/*! \brief Wrap an existing radio button, e.g., from a Resource file created
			 * Window.  */
			public RadioButton (Window window, uint cmpID) : base (window, cmpID)
			{
			}

			/*! \brief The label which will be used for this radio button.  */
			public string Label
			{
				set { SetText (Method.SetLabel, value); }
				get { return GetText (Method.GetLabel); }
			}

			/*! \brief The event which will be raised when the state of the radio
			 * button changes.  */
			public uint Event
			{
				set { CallMethod_SetR4 (Method.SetEvent, value); }
				get { return CallMethod_GetR0 (Method.GetEvent); }
			}

			/*! \brief Sets/reads the state of the radio button, i.e., either on or off.  */
			public State CurrentState
			{
				set
				{
					CallMethod_SetR4 (Method.SetState, (uint)((value == State.On) ? 1 : 0));
				}
				get
				{
					return (CallMethod_GetR0 (Method.GetState) == 0) ? State.Off : State.On;
				}
			}
		}
	}
}
