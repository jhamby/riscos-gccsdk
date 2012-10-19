//
// toolbox/slider.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;
using System.Runtime.InteropServices;

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

			static class SliderFlags
			{
				// Lower & Upper are the wrong way around in the Acorn User
				// Interface Toolbox manual.
				public const int LowerBound = (1 << 0);
				public const int UpperBound = (1 << 1);
				public const int StepSize = (1 << 2);
			}

			/*! \brief The default Toolbox event that a %Slider can raise.  */
			public static class EventCode
			{
				public const uint ValueChanged = 0x82886;
			}

			/*! \brief An object that encapsulates the arguments for the event that is raised when the
			 * value of a %Slider changes.  */
			public class ValueChangeEventArgs : ToolboxEventArgs
			{
				/*! \brief Constants defining the possible bit flags returned by this event.  */
				public static class Flags
				{
					public const int DragStartClick = (1 << 0);
					public const int DragInProgress = (1 << 1);
					public const int DragEnded = (1 << 2);
				}

				/*! \brief Constant defining event specific data offset after the header.  */
				public static class EventOffset
				{
					public const int NewValue = 16;
				}

				/*! \brief \e true if this event is the start of the drag or just a click.  */
				public readonly bool DragStartClick;
				/*! \brief \e true if the drag of the Slider is still in progress.  */
				public readonly bool DragInProgress;
				/*! \brief \e true if this event is the end of the drag on the Slider.  */
				public readonly bool DragEnded;

				/*! \brief New value of the Slider.  */
				public readonly int NewValue;

				/*! \brief Create the arguments for a ValueChange event from the raw event data.  */
				public ValueChangeEventArgs (IntPtr unmanagedEventBlock) : base (unmanagedEventBlock)
				{
					uint flags = Header.Flags;

					DragStartClick = (flags & Flags.DragStartClick) != 0;
					DragInProgress = (flags & Flags.DragInProgress) != 0;
					DragEnded = (flags & Flags.DragEnded) != 0;

					NewValue = Marshal.ReadInt32 (RawEventData, EventOffset.NewValue);
				}
			}

			/*! \brief The signature of a ValueChange event handler.  */
			public delegate void ValueChangeEventHandler (object sender, ValueChangeEventArgs e);

			/*! \brief The event handlers that will be called when the value of a %Slider changes.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, ValueChangeEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * SliderObject.ValueChange += handler_name;
			 * \endcode  */
			public event ValueChangeEventHandler ValueChange;

			/*! \brief Wrap an existing slider, e.g., from a Resource file created
			 * Window.  */
			public Slider (Window window, uint cmpID) : base (window, cmpID)
			{
			}

			/*! \brief The value of the slider.  */
			public int Value
			{
				set { Object.MiscOp_SetR3R4 (0, Method.SetValue, ComponentID, (uint)value); }
				get { return (int)Object.MiscOp_SetR3GetR0 (0, Method.GetValue, ComponentID); }
			}

			/*! \brief Sets or gets the lower bound of the slider's value.  */
			public int LowerBound
			{
				set {
					Object.MiscOp_SetR3R4R5R6 (SliderFlags.LowerBound,
								   Method.SetBound,
								   ComponentID,
								   value,
								   0,
								   0);
				}
				get {
					int lower;
					int not_required;

					Object.MiscOp_SetR3GetR0R1R2 (SliderFlags.LowerBound,
								      Method.GetBound,
								      ComponentID,
								      out lower,
								      out not_required,
								      out not_required);
					return lower;
				}
			}

			/*! \brief Sets or gets the upper bound of the slider's value.  */
			public int UpperBound
			{
				set {
					Object.MiscOp_SetR3R4R5R6 (SliderFlags.UpperBound,
								   Method.SetBound,
								   ComponentID,
								   0,
								   value,
								   0);
				}
				get {
					int upper;
					int not_required;

					Object.MiscOp_SetR3GetR0R1R2 (SliderFlags.UpperBound,
								      Method.GetBound,
								      ComponentID,
								      out not_required,
								      out upper,
								      out not_required);
					return upper;
				}
			}

			/*! \brief Sets or gets the step size of the slider's value.  */
			public int StepSize
			{
				set {
					Object.MiscOp_SetR3R4R5R6 (SliderFlags.StepSize,
								   Method.SetBound,
								   ComponentID,
								   0,
								   0,
								   value);
				}
				get {
					int step;
					int not_required;

					Object.MiscOp_SetR3GetR0R1R2 (SliderFlags.StepSize,
								      Method.GetBound,
								      ComponentID,
								      out not_required,
								      out not_required,
								      out step);
					return step;
				}
			}

			/*! \brief Set the lower bound, upper bound and step size of the %Slider.
			 *
			 * An alternative to using the individual properties that sets all three
			 * values in one operation.
			 * \param [in] lower The lower bounds of the %Slider's value.
			 * \param [in] upper The upper bounds of the %Slider's value.
			 * \param [in] step The step size of the %Slider.
			 * \return Nothing.  */
			public void SetBounds (int lower, int upper, int step)
			{
				Object.MiscOp_SetR3R4R5R6 (SliderFlags.LowerBound |
							    SliderFlags.UpperBound |
							    SliderFlags.StepSize,
							   Method.SetBound,
							   ComponentID,
							   lower,
							   upper,
							   step);
			}

			/*! \brief Get the lower bound, upper bound and step size of the %Slider.
			 *
			 * An alternative to using the individual properties that gets all three
			 * values in one operation.
			 * \param [out] lower The lower bounds of the %Slider's value.
			 * \param [out] upper The upper bounds of the %Slider's value.
			 * \param [out] step The step size of the %Slider.
			 * \return Nothing.  */
			public void GetBounds (out int lower, out int upper, out int step)
			{
				Object.MiscOp_SetR3GetR0R1R2 (SliderFlags.LowerBound |
							      SliderFlags.UpperBound |
							      SliderFlags.StepSize,
							      Method.SetBound,
							      ComponentID,
							      out lower,
							      out upper,
							      out step);
			}

			/*! \brief Sets the Desktop colour used in the slider.
			 * \param [in] barColour Desktop colour for the bar.
			 * \param [in] bgColour Desktop colour for the background
			 * \return Nothing.  */
			public void SetColour (OS.DesktopColour barColour,
					       OS.DesktopColour bgColour)
			{
				Object.MiscOp_SetR3R4R5 (0,
							 Method.SetColour,
							 ComponentID,
							 (uint)barColour,
							 (uint)bgColour);
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

				Object.MiscOp_SetR3GetR0R1 (0,
							    Method.GetColour,
							    ComponentID,
							    out bar_col,
							    out bg_col);

				barColour = (OS.DesktopColour)bar_col;
				bgColour = (OS.DesktopColour)bg_col;
			}

			protected virtual void OnValueChange (ValueChangeEventArgs e)
			{
				if (ValueChange != null)
					ValueChange (this, e);
			}

			/*! \brief Check if the given event is relevant to the %Slider gadget and call the
			 * associated event handlers.  */
			public override void Dispatch (ToolboxEventArgs e)
			{
				if (e.Header.EventCode == EventCode.ValueChanged)
					OnValueChange (new ValueChangeEventArgs (e.RawEventData));
			}
		}
	}
}
