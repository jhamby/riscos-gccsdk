//
// toolbox/radio-button.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;
using System.Runtime.InteropServices;

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

			/*! \brief The possible states of a RadioButton.  */
			public enum State
			{
				Off,
				On
			}

			/*! \brief An object that encapsulates the arguments for the event that is raised when the
			 * state of an RadioButton changes.  */
			public class StateChangeEventArgs : ToolboxEventArgs
			{
				/*! \brief Constants defining event specific data offsets after the header.  */
				public static class EventOffset
				{
					public const int NewState = 16;
					public const int PreviousOn = 20;
				}

				/*! \brief Constants defining the possible bit flags returned by this event.  */
				public static class Flags
				{
					public const int Adjust = (1 << 0);
					// Bit 1 reserved
					public const int Select = (1 << 2);
				}

				/*! \brief The new state of the RadioButton.  */
				public State NewState;
				/*! \brief \e true if the state change was made using the Adjust mouse button.  */
				public bool Adjust;
				/*! \brief \e true if the state change was made using the Select mouse button.  */
				public bool Select;
				/*! \brief The radio button within the group which was On before this state change.  */
				public RadioButton Previous = null;

				/*! \brief Create the arguments for a StateChange event from the raw event data.  */
				public StateChangeEventArgs (Gadget gadget, IntPtr unmanagedEventBlock) : base (unmanagedEventBlock)
				{
					uint flags = Header.Flags;

					Adjust = (flags & Flags.Adjust) != 0;
					Select = (flags & Flags.Select) != 0;

					NewState = (RadioButton.State)Marshal.ReadInt32 (RawEventData,
											 EventOffset.NewState);
					uint prev_cmp = (uint)Marshal.ReadInt32 (RawEventData,
										 EventOffset.PreviousOn);
					Gadget prev_gadget;
					if (prev_cmp != 0xffffffff) // Not sure if this is possible
					{
						Previous = gadget.Object.Gadgets.TryGetValue (prev_cmp, out prev_gadget) ?
								(RadioButton)prev_gadget :
								new RadioButton ((Window)(gadget.Object), prev_cmp);
					}
				}
			}

			/*! \brief The signature of a StateChange event handler.  */
			public delegate void StateChangeEventHandler (object sender, StateChangeEventArgs e);

			/*! \brief The event handlers that will be called when the state
			 * of this RadioButton changes.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, StateChangeEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * RadioButtonObject.StateChange += handler_name;
			 * \endcode  */
			public event StateChangeEventHandler StateChange;

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
				set { Object.MiscOp_SetR3R4 (0, Method.SetEvent, ComponentID, value); }
				get { return Object.MiscOp_SetR3GetR0 (0, Method.GetEvent, ComponentID); }
			}

			/*! \brief Sets/reads the state of the radio button, i.e., either on or off.  */
			public State CurrentState
			{
				set {
					Object.MiscOp_SetR3R4 (0,
							       Method.SetState,
							       ComponentID,
							       (uint)((value == State.On) ? 1 : 0));
				}
				get {
					return (Object.MiscOp_SetR3GetR0 (0,
									  Method.GetState,
									  ComponentID) == 0) ? State.Off : State.On;
				}
			}

			protected virtual void OnStateChange (StateChangeEventArgs e)
			{
				if (StateChange != null)
					StateChange (this, e);
			}

			public override void Dispatch (ToolboxEventArgs e)
			{
				if (e.Header.EventCode == Event && StateChange != null)
					OnStateChange (new StateChangeEventArgs (this, e.RawEventData));
			}
		}
	}
}
