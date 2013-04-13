//
// toolbox/number-range.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;
using System.Runtime.InteropServices;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \brief Used to display one of a range of possible integer or fixed
		 * point values.  */
		public class NumberRange : Gadget
		{
			static class Method
			{
				public const int SetValue = 832;
				public const int GetValue = 833;
				public const int SetBounds = 834;
				public const int GetBounds = 835;
				public const int GetComponents = 836;
			}

			/*! \brief The default Toolbox event that a number range can raise.  */
			public static class EventCode
			{
				public const uint ValueChanged = 0x8288d;
			}

			/*! \brief An object that encapsulates the arguments for the event that is raised when the
			 * value of a NumberRange changes.  */
			public class ValueChangeEventArgs : ToolboxEventArgs
			{
				/*! \brief Constant defining event specific data offset after the header.  */
				public static class EventOffset
				{
					public const int NewValue = 16;
				}

				/*! \brief New value shown in the display area.  */
				public int NewValue;

				/*! \brief Create the arguments for a ValueChange event from the raw event data.  */
				public ValueChangeEventArgs (IntPtr unmanagedEventBlock) : base (unmanagedEventBlock)
				{
					NewValue = Marshal.ReadInt32 (RawEventData, EventOffset.NewValue);
				}
			}

			/*! \brief The event handlers that will be called when the value of this NumberRange
			 * changes.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, ValueChangeEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * NumberRangeObject.ValueChange += handler_name;
			 * \endcode  */
			public event EventHandler<ValueChangeEventArgs> ValueChange;

			/*! \brief Wrap an existing number range, e.g., from a Resource file created
			 * Window.  */
			public NumberRange (Window window, uint cmpID) : base (window, cmpID)
			{
			}

			/*! \brief The value of the number range as an integer.  */
			public int Value
			{
				set { Object.MiscOp_SetR3R4 (0, Method.SetValue, ComponentID, (uint)value); }
				get { return (int)Object.MiscOp_SetR3GetR0 (0, Method.GetValue, ComponentID); }
			}

/* TODO:
   GetBounds
   SetBounds
   GetComponents
*/
			protected virtual void OnValueChange (ValueChangeEventArgs e)
			{
				if (ValueChange != null)
					ValueChange (this, e);
			}

			public override void Dispatch (ToolboxEventArgs e)
			{
				if (e.Header.EventCode == EventCode.ValueChanged)
					OnValueChange (new ValueChangeEventArgs (e.RawEventData));
			}
		}

		/*! \brief The Toolbox flags that define the behaviour of a NumberRange gadget.  */
		public static class NumberRangeFlags
		{
			public const int InformClient = (1 << 0);
			public const int Writable = (1 << 2);
			public const int NoDisplayArea = (1 << 3);
			public const int HasAdjusterArrows = (1 << 4);
			// Bits 5-7
			public const int TypeShift = 5;
			public const int TypeMask = 7;
			// Bits 8-9
			public const int JustifyShift = 8;
			public const int JustifyMask = 3;
			// Bits 12-15
			public const int SliderBarColourShift = 12;
			public const int SliderBarColourMask = 15;
			// Bits 16-19
			public const int SliderBGColourShift = 16;
			public const int SliderBGColourMask = 15;
		}

		public static class NumberRangeTemplateOffset
		{
			public const int LowerBound = 36;
			public const int UpperBound = 40;
			public const int StepSize = 44;
			public const int InitialValue = 48;
			public const int Precision = 52;
			public const int Before = 56;
			public const int After = 60;
			public const int DisplayLength = 64;
			public const int TemplateSize = 68;
		}

		/*! \brief Values used to determine whether a NumberRange gadget includes a slider.  */
		public enum NumberRangeType
		{
			NoSlider,
			SliderRight,
			SliderLeft
		}

		/*! \brief Values used to determine how the numerical field of a NumberRange gadget is
		 * justified.  */
		public enum NumberRangeJustify
		{
			Left,
			Right,
			Centre
		}

		/*! \brief Encapsulates the data required to create a template for a NumberRange
		 * gadget.  */
		public class NumberRangeTemplate : GadgetTemplate
		{
			int _lower_bound = 0;
			/*! \brief The lower bound that a number range gadget created from this
			 * template will have.
			 * \note Default value: 0  */
			public int LowerBound {
				get { return _lower_bound; }
				set { _lower_bound = value; }
			}

			int _upper_bound = 100;
			/*! \brief The upper bound that a number range gadget created from this
			 * template will have.
			 * \note Default value: 100  */
			public int UpperBound {
				get { return _upper_bound; }
				set { _upper_bound = value; }
			}

			int _step_size = 1;
			/*! \brief The step size that a number range gadget created from this
			 * template will have.
			 * \note Default value: 1  */
			public int StepSize {
				get { return _step_size; }
				set { _step_size = value; }
			}

			int _initial_value = 0;
			/*! \brief The initial value that a number range gadget created from this
			 * template will have.
			 * \note Default value: 0  */
			public int InitialValue {
				get { return _initial_value; }
				set { _initial_value = value; }
			}

			int _precision = 0;
			/*! \brief The precision that a number range gadget created from this
			 * template will have.
			 * \note Default value: 0  */
			public int Precision {
				get { return _precision; }
				set { _precision = value; }
			}

			int _before = -1;
			/*! \brief The ID of a gadget before this one that will gain the input focus
			 * when navigating with the keyboard.
			 * \note Default value: -1 (None).  */
			public int Before {
				get { return _before; }
				set { _before = value; }
			}

			int _after = -1;
			/*! \brief The ID of a gadget after this one that will gain the input focus
			 * when navigating with the keyboard.
			 * \note Default value: -1 (None).  */
			public int After {
				get { return _after; }
				set { _after = value; }
			}

			int _display_length = 160;
			/*! \brief The width of the numerical display field in OS Units. Only relevant
			 * when a slider bar is included.
			 * \note Default value: 160  */
			public int DisplayLength {
				get { return _display_length; }
				set { _display_length = value; }
			}

			public NumberRangeTemplate () : base (Gadget.ComponentType.NumberRange)
			{
				SliderBarColour = OS.DesktopColour.Grey4;
				SliderBGColour = OS.DesktopColour.White;
			}

			/*! \brief Set or get whether a NumberRange gadget created from this template
			 * will inform clients of value changes using NumberRange_ValueChanged events.
			 * \note Default value: false.  */
			public bool InformClient {
				get { return (_flags & NumberRangeFlags.InformClient) != 0; }
				set {
					_flags = (uint)(value ? _flags |  NumberRangeFlags.InformClient :
								_flags & ~NumberRangeFlags.InformClient);
				}
			}

			/*! \brief Set or get whether the NumberRange gadget will be writable.
			 * \note Default value: false.  */
			public bool IsWritable {
				get { return (_flags & NumberRangeFlags.Writable) != 0; }
				set {
					_flags = (uint)(value ? _flags |  NumberRangeFlags.Writable :
								_flags & ~NumberRangeFlags.Writable);
				}
			}

			/*! \brief Set or get whether the NumberRange gadget has adjuster arrows.
			 * \note Default value: false.  */
			public bool HasAdjusterArrows {
				get { return (_flags & NumberRangeFlags.HasAdjusterArrows) != 0; }
				set {
					_flags = (uint)(value ? _flags |  NumberRangeFlags.HasAdjusterArrows :
								_flags & ~NumberRangeFlags.HasAdjusterArrows);
				}
			}

			/*! \brief Set or get whether the NumberRange gadget will be writable.
			 * \note Default value: true.  */
			public bool HasDisplayArea {
				get { return (_flags & NumberRangeFlags.NoDisplayArea) == 0; }
				set {
					// Note the logic is reversed here because the flag is set
					// when there is no display area.
					_flags = (uint)(value ? _flags & ~NumberRangeFlags.NoDisplayArea :
								_flags |  NumberRangeFlags.NoDisplayArea);
				}
			}

			/*! \brief Set or get whether the NumberRange gadget will have a slider and if
			 * so whether it will be position to the left or right of the display area.
			 * \note Default value: 0 (NoSlider).  */
			public NumberRangeType Type {
				get {
					return (NumberRangeType)((_flags >> NumberRangeFlags.TypeShift) &
								  NumberRangeFlags.TypeMask);
				}
				set {
					_flags &= ~(uint)(NumberRangeFlags.TypeMask << NumberRangeFlags.TypeShift);
					_flags |= (uint)value << NumberRangeFlags.TypeShift;
				}
			}

			/*! \brief Set or get the justification of the NumberRange gadget.
			 * \note Default value: 0 (Left).  */
			public NumberRangeJustify Justify {
				get {
					return (NumberRangeJustify)((_flags >> NumberRangeFlags.JustifyShift) &
								    NumberRangeFlags.JustifyMask);
				}
				set {
					_flags &= ~(uint)(NumberRangeFlags.JustifyMask <<
							  NumberRangeFlags.JustifyShift);
					_flags |= (uint)value << NumberRangeFlags.JustifyShift;
				}
			}

			/*! \brief Set or get the desktop colour of the slider bar.
			 * \note Default value: 4 (Grey4). */
			public OS.DesktopColour SliderBarColour {
				get {
					return (OS.DesktopColour)((_flags >> NumberRangeFlags.SliderBarColourShift) &
								   NumberRangeFlags.SliderBarColourMask);
				}
				set {
					_flags &= ~(uint)(NumberRangeFlags.SliderBarColourMask <<
							  NumberRangeFlags.SliderBarColourShift);
					_flags |= (uint)value << NumberRangeFlags.SliderBarColourShift;
				}
			}

			/*! \brief Set or get the desktop colour of the slider background.
			 * \note Default value: 0 (White). */
			public OS.DesktopColour SliderBGColour {
				get {
					return (OS.DesktopColour)((_flags >> NumberRangeFlags.SliderBGColourShift) &
								   NumberRangeFlags.SliderBGColourMask);
				}
				set {
					_flags &= ~(uint)(NumberRangeFlags.SliderBGColourMask <<
							  NumberRangeFlags.SliderBGColourShift);
					_flags |= (uint)value << NumberRangeFlags.SliderBGColourShift;
				}
			}

			public override void BuildBuffer (IntPtr buffer,
							  int offset,
							  ref int strOffset,
							  ref int msgOffset)
			{
				base.BuildBuffer (buffer, offset, ref strOffset, ref msgOffset);

				Marshal.WriteInt32 (buffer,
						    offset + NumberRangeTemplateOffset.LowerBound,
						    _lower_bound);
				Marshal.WriteInt32 (buffer,
						    offset + NumberRangeTemplateOffset.UpperBound,
						    _upper_bound);
				Marshal.WriteInt32 (buffer,
						    offset + NumberRangeTemplateOffset.StepSize,
						    _step_size);
				Marshal.WriteInt32 (buffer,
						    offset + NumberRangeTemplateOffset.InitialValue,
						    _initial_value);
				Marshal.WriteInt32 (buffer,
						    offset + NumberRangeTemplateOffset.Precision,
						    _precision);
				Marshal.WriteInt32 (buffer,
						    offset + NumberRangeTemplateOffset.Before,
						    _before);
				Marshal.WriteInt32 (buffer,
						    offset + NumberRangeTemplateOffset.After,
						    _after);
				Marshal.WriteInt32 (buffer,
						    offset + NumberRangeTemplateOffset.DisplayLength,
						    _display_length);
			}

			public override int GetTemplateSize ()
			{
				return NumberRangeTemplateOffset.TemplateSize;
			}
		}
	}
}
