//
// toolbox/writable-field.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;
using System.Text;
using System.Runtime.InteropServices;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \class WritableField
		 * \brief A boxed display area in which a text string is displayed and can be
		 * edited by the user.  */
		public class WritableField : Gadget
		{
			static class Method
			{
				public const int SetValue = 512;
				public const int GetValue = 513;
				public const int SetAllowable = 514;
				public const int SetFont = 516;
			}

			/*! \brief The default Toolbox event that a writable field can raise.  */
			public static class EventCode
			{
				public const uint ValueChanged = 0x82885;
			}

			/*! \brief An object that encapsulates the arguments for the event that is raised when the
			 * value of a WritableField changes.  */
			public class ValueChangeEventArgs : ToolboxEventArgs
			{
				/*! \brief Constant defining event specific data offset after the header.  */
				public static class EventOffset
				{
					public const int NewValue = 16;
				}

				// FIXME: Using the string type is very inefficient as every single change to
				// the writable will allocate a new string leaving the GC to collect the old
				// one. Would be better if a StringBuilder could be used, but there doesn't
				// seem to be a way to marshal to a StringBuilder except as a function
				// parameter in a native method.
				public string NewValue;

				public ValueChangeEventArgs (IntPtr unmanagedEventBlock) : base (unmanagedEventBlock)
				{
					NewValue = Marshal.PtrToStringAnsi (new IntPtr (unmanagedEventBlock.ToInt32 () +
											EventOffset.NewValue));
				}
			}

			/*! \brief The signature of a ValueChange event handler.  */
			public delegate void ValueChangeEventHandler (object sender, ValueChangeEventArgs e);

			/*! \brief The event handlers that will be called when the value of this Writable
			 * changes.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, ValueChangeEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * WritableObject.ValueChange += handler_name;
			 * \endcode  */
			public event ValueChangeEventHandler ValueChange;

			/*! \brief Wrap an existing writable field, e.g., from a Resource file created
			 * Window.  */
			public WritableField (Window window, uint cmpID) : base (window, cmpID)
			{
			}

			/*! \brief The text string shown in the writable field.  */
			public string Value
			{
				set { SetText (Method.SetValue, value); }
				get { return GetText (Method.GetValue); }
			}

			/*! \brief Defines the set of allowable characters which can be typed into
			 * a writable field. 
			 * 
			 * The set is specified in the same way as a WIMP 'a' validation string
			 * directive (without including the letter 'a').
			 * \note Set to null to allow all characters.  */
			public string Allowable
			{
				set { SetText (Method.SetAllowable, value); }
			}

			/*! \brief Make the writable field use an anti-aliased font. If \e fontID
			 * is null, then the system font is used.  */
			public void SetFont (string fontID, int width, int height)
			{
				OS.ThrowOnError (NativeMethods.Component_SetFont (0,
										  Object.ID,
										  Method.SetFont,
										  ComponentID,
										  fontID,
										  width,
										  height));
			}

			public override void Dispatch (ToolboxEvent ev)
			{
				if (ev.ToolboxArgs.Header.EventCode == EventCode.ValueChanged && ValueChange != null)
				{
					ValueChange (this, new ValueChangeEventArgs (ev.ToolboxArgs.RawEventData));
				}
			}
		}
	}
}
