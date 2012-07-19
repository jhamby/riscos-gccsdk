//
// toolbox/option-button.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \class OptionButton
		 * \brief Used to indicate whether a particular option has been chosen or not.  */
		public class OptionButton : Gadget
		{
			static class Method
			{
				public const int SetLabel = 192;
				public const int GetLabel = 193;
				public const int SetEvent = 194;
				public const int GetEvent = 195;
				public const int SetState = 196;
				public const int GetState = 197;
			}

			/*! \brief The default Toolbox event that an option button can raise.  */
			public static class EventCode
			{
				public const uint StateChanged = 0x82882;
			}

			public enum State
			{
				Off,
				On
			}

			/*! \brief Wrap an existing option button, e.g., from a Resource file created
			 * Window.  */
			public OptionButton (Window window, uint cmpID) : base (window, cmpID)
			{
			}

			/*! \brief The label which will be used for this option button.  */
			public string Label
			{
				set { SetText (Method.SetLabel, value); }
				get { return GetText (Method.GetLabel); }
			}

			/*! \brief The event which will be raised when the state of the option
			 * button changes.  */
			public uint Event
			{
				set { CallMethod_SetR4 (Method.SetEvent, value); }
				get { return CallMethod_GetR0 (Method.GetEvent); }
			}

			/*! \brief Sets/reads the state of the option button, i.e., either on or off.  */
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
