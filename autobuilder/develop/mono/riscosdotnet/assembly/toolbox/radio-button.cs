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
			public event EventHandler<StateChangeEventArgs> StateChange;

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

		/*! \brief The Toolbox flags that define the behaviour of an RadioButton gadget.  */
		public static class RadioButtonFlags
		{
			public const int InformClient = (1 << 0);
			// Note bit 1 not used.
			public const int CreatedOn = (1 << 2);
		}

		public static class RadioButtonTemplateOffset
		{
			public const int Group = 36;
			public const int Label = 40;
			public const int MaxLabelLen = 44;
			public const int Event = 48;
			public const int TemplateSize = 52;
		}

		/*! \brief Encapsulates the data required to create a template for a RadioButton
		 * gadget.  */
		public class RadioButtonTemplate : GadgetTemplate
		{
			int _group = 0;
			/*! \brief The Exclusive Selection Group that this RadioButton gadget will belong
			 * to.
			 * \note Default value: 0 (Belongs to no group).
			 * \note Maximum value is 15.  */
			public int Group {
				get { return _group; }
				set {
					if (value <= 0 || value > 15)
						throw new ArgumentOutOfRangeException ("RadioButton ESG must be between 0 and 16", "value");
					_group = value;
				}
			}

			string _label = "";
			/*! \brief The text to be shown next to the radio button.  */
			public string Label {
				get { return _label; }
				set { _label = value; }
			}

			int _max_label_len = 0;
			/*! \brief The maximum buffer size for the text. Leave as 0 to indicate that
			 * the length of the text itself should be used.  */
			public int MaxLabelLen {
				get { return _max_label_len; }
				set { _max_label_len = value; }
			}

			uint _event = 0;
			/*! \brief The event that should be raised when the gadget is clicked.
			 * \note Default value: 0 (Use defined Toolbox event).  */
			public uint Event {
				get { return _event; }
				set { _event = value; }
			}

			/*! \brief Create a template for a radio button where the text (if any)
			 * will be set later.  */
			public RadioButtonTemplate () : base (Gadget.ComponentType.RadioButton)
			{
			}

			//! \brief Create a template for a radio button using the given text.
			public RadioButtonTemplate (string text) : base (Gadget.ComponentType.RadioButton)
			{
				_label = text;
			}

			/*! \brief Create a template for a radio button using the given text and maximum
			 * text buffer size.  */
			public RadioButtonTemplate (string text, int maxLabelLen) :
							base (Gadget.ComponentType.RadioButton)
			{
				_label = text;
				// If this is smaller than the actual text, then it'll be corrected when
				// the template is written to memory.
				_max_label_len = maxLabelLen;
			}

			/*! \brief Set or get whether a RadioButton gadget created from this template
			 * will inform clients of value changes using RadioButton_StateChanged events.
			 * \note Default value: false.  */
			public bool InformClient {
				get { return (_flags & RadioButtonFlags.InformClient) != 0; }
				set {
					_flags = (uint)(value ? _flags |  RadioButtonFlags.InformClient :
								_flags & ~RadioButtonFlags.InformClient);
				}
			}

			/*! \brief Set or get whether the RadioButton gadget will be 'On' when first
			 * created.
			 * \note Default value: false.  */
			public bool OnWhenCreated {
				get { return (_flags & RadioButtonFlags.CreatedOn) != 0; }
				set {
					_flags = (uint)(value ? _flags |  RadioButtonFlags.CreatedOn :
								_flags & ~RadioButtonFlags.CreatedOn);
				}
			}

			public override int CalculateBufferSize (ref int strStart, ref int msgStart)
			{
				int size = base.CalculateBufferSize (ref strStart, ref msgStart);

				if (!string.IsNullOrEmpty (_label))
					size += Math.Max (_label.Length + 1, _max_label_len);

				return size;
			}

			public override void BuildBuffer (IntPtr buffer,
							  int offset,
							  ref int strOffset,
							  ref int msgOffset)
			{
				base.BuildBuffer (buffer, offset, ref strOffset, ref msgOffset);

				Marshal.WriteInt32 (buffer,
						    offset + RadioButtonTemplateOffset.Group,
						    _group);
				msgOffset = ObjectTemplate.WriteString (_label,
									_max_label_len,
									buffer,
									offset + RadioButtonTemplateOffset.Label,
									msgOffset);
				Marshal.WriteInt32 (buffer,
						    offset + RadioButtonTemplateOffset.MaxLabelLen,
						    string.IsNullOrEmpty (_label) ? 0 :
							Math.Max (_label.Length + 1, _max_label_len));
				Marshal.WriteInt32 (buffer,
						    offset + RadioButtonTemplateOffset.Event,
						    (int)_event);
			}

			public override int GetTemplateSize ()
			{
				return RadioButtonTemplateOffset.TemplateSize;
			}
		}
	}
}
