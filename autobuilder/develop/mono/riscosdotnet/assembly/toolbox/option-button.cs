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
			public event EventHandler<StateChangeEventArgs> StateChange;

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

			protected virtual void OnStateChange (StateChangeEventArgs e)
			{
				if (StateChange != null)
					StateChange (this, e);
			}

			public override void Dispatch (ToolboxEventArgs e)
			{
				if (e.Header.EventCode == EventCode.StateChanged)
					OnStateChange (new StateChangeEventArgs (e.RawEventData));
			}
		}

		/*! \brief The Toolbox flags that define the behaviour of an OptionButton gadget.  */
		public static class OptionButtonFlags
		{
			public const int InformClient = (1 << 0);
			// Note bit 1 not used.
			public const int CreatedOn = (1 << 2);
		}

		public static class OptionButtonTemplateOffset
		{
			// User Interface Toolbox Manual Page 382 - option button template shows
			// 'flags' as the first field, however, there is no such field. All flags
			// refer to the field in the gadget header.
			public const int Label = 36;
			public const int MaxLabelLen = 40;
			public const int Event = 44;
			public const int TemplateSize = 48;
		}

		/*! \brief Encapsulates the data required to create a template for an OptionButton
		 * gadget.  */
		public class OptionButtonTemplate : GadgetTemplate
		{
			string _label = "";
			/*! \brief The text to be shown next to the option button.  */
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
			/*! \brief The event that should be raised when the gadget is clicked.  */
			public uint Event {
				get { return _event; }
				set { _event = value; }
			}

			/*! \brief Create a template for an option button where the text (if any)
			 * will be set later.  */
			public OptionButtonTemplate () : base (Gadget.ComponentType.OptionButton)
			{
			}


			//! \brief Create a template for an option button using the given text.
			public OptionButtonTemplate (string text) : base (Gadget.ComponentType.OptionButton)
			{
				_label = text;
			}

			/*! \brief Create a template for an option button using the given text and maximum
			 * text buffer size.  */
			public OptionButtonTemplate (string text, int maxLabelLen) :
							base (Gadget.ComponentType.OptionButton)
			{
				_label = text;
				// If this is smaller than the actual text, then it'll be corrected when
				// the template is written to memory.
				_max_label_len = maxLabelLen;
			}

			/*! \brief Set or get whether an OptionButton gadget created from this template
			 * will inform clients of value changes using OptionButton_StateChanged events.
			 * \note Default value: false.  */
			public bool InformClient {
				get { return (_flags & OptionButtonFlags.InformClient) != 0; }
				set {
					_flags = (uint)(value ? _flags |  OptionButtonFlags.InformClient :
								_flags & ~OptionButtonFlags.InformClient);
				}
			}

			/*! \brief Set or get whether the OptionButton gadget will be 'On' when first
			 * created.
			 * \note Default value: false.  */
			public bool OnWhenCreated {
				get { return (_flags & OptionButtonFlags.CreatedOn) != 0; }
				set {
					_flags = (uint)(value ? _flags |  OptionButtonFlags.CreatedOn :
								_flags & ~OptionButtonFlags.CreatedOn);
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

				msgOffset = ObjectTemplate.WriteString (_label,
									_max_label_len,
									buffer,
									offset + OptionButtonTemplateOffset.Label,
									msgOffset);
				Marshal.WriteInt32 (buffer,
						    offset + OptionButtonTemplateOffset.MaxLabelLen,
						    string.IsNullOrEmpty (_label) ? 0 :
							Math.Max (_label.Length + 1, _max_label_len));
				Marshal.WriteInt32 (buffer,
						    offset + OptionButtonTemplateOffset.Event,
						    (int)_event);
			}

			public override int GetTemplateSize ()
			{
				return OptionButtonTemplateOffset.TemplateSize;
			}

		}
	}
}
