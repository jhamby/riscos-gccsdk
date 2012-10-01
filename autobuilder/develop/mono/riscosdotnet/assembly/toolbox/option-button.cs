//
// toolbox/option-button.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;
using System.Runtime.InteropServices;

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

			/*! \brief An object that encapsulates the arguments for the event that is raised when the
			 * state of an OptionButton changes.  */
			public class StateChangeEventArgs : ToolboxEventArgs
			{
				/*! \brief Constant defining event specific data offset after the header.  */
				public static class EventOffset
				{
					public const int NewState = 16;
				}

				public static class Flags
				{
					public const int Adjust = (1 << 0);
					// Bit 1 reserved
					public const int Select = (1 << 2);
				}

				/*! \brief The new state of the OptionButton.  */
				public State NewState;
				/*! \brief State change was made using the Adjust mouse button.  */
				public bool Adjust;
				/*! \brief State change was made using the Select mouse button.  */
				public bool Select;

				/*! \brief Create the arguments for a StateChange event from the raw event data.  */
				public StateChangeEventArgs (IntPtr unmanagedEventBlock) : base (unmanagedEventBlock)
				{
					uint flags = Header.Flags;

					Adjust = (flags & Flags.Adjust) != 0;
					Select = (flags & Flags.Select) != 0;

					NewState = (OptionButton.State)Marshal.ReadInt32 (RawEventData,
											  EventOffset.NewState);
				}
			}

			/*! \brief The signature of a StateChange event handler.  */
			public delegate void StateChangeEventHandler (object sender, StateChangeEventArgs e);

			/*! \brief The event handlers that will be called when the state
			 * of this OptionButton changes.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, StateChangeEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * OptionButtonObject.StateChange += handler_name;
			 * \endcode  */
			public event StateChangeEventHandler StateChange;

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
				set { Object.MiscOp_SetR3R4 (0, Method.SetEvent, ComponentID, value); }
				get { return Object.MiscOp_SetR3GetR0 (0, Method.GetEvent, ComponentID); }
			}

			/*! \brief Sets/reads the state of the option button, i.e., either on or off.  */
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

			public override void Dispatch (ToolboxEvent ev)
			{
				if (ev.ToolboxArgs.Header.EventCode == EventCode.StateChanged && StateChange != null)
				{
					StateChange (this, new StateChangeEventArgs (ev.ToolboxArgs.RawEventData));
				}
			}
		}
	}
}
