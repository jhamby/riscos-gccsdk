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

				public static class Flags
				{
					public const int StringTooLong = (1 << 0);
				}

				// FIXME: Using the string type is very inefficient as every single change to
				// the writable will allocate a new string leaving the GC to collect the old
				// one. Would be better if a StringBuilder could be used, but there doesn't
				// seem to be a way to marshal to a StringBuilder except as a function
				// parameter in a native method.
				public readonly string NewValue;

				/*! \brief \e true if the text string was too long to fit into the WIMP event
				 * block. */
				public readonly bool StringTooLong;

				/*! \brief Create the arguments for a ValueChange event from the raw event data.  */
				public ValueChangeEventArgs (IntPtr unmanagedEventBlock) : base (unmanagedEventBlock)
				{
					StringTooLong = (Header.Flags & Flags.StringTooLong) != 0;
					NewValue = Marshal.PtrToStringAnsi (new IntPtr (unmanagedEventBlock.ToInt32 () +
											EventOffset.NewValue));
				}
			}

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
			public event EventHandler<ValueChangeEventArgs> ValueChange;

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
				OS.ThrowOnError (NativeMethods.Toolbox_ObjectMiscOp_SetFont (0,
											     Object.ID,
											     Method.SetFont,
											     ComponentID,
											     fontID,
											     width,
											     height));
			}

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

		public static class WritableFieldTemplateOffset
		{
			public const int Text = 36;
			public const int MaxTextLen = 40;
			public const int Allowable = 44;
			public const int MaxAllowableLen = 48;
			public const int Before = 52;
			public const int After = 56;
			public const int TemplateSize = 60;
		}

		/*! \brief The Toolbox flags that help to define the behaviour of a WritableField gadget.  */
		public static class WritableFieldFlags
		{
			public const int InformClient = (1 << 0);
			// Note bit 1 not used.
			public const int JustifyShift = 2;
			public const int JustifyMask = 3;
			public const int Password = (1 << 4);
		}

		/*! \brief Values used to determine how the WritableField gadget is justified.  */
		public enum WritableFieldJustify
		{
			Left,
			Right,
			Centre
		}

		/*! \brief Encapsulates the data required to create a template for a WritableField
		 * gadget.  */
		public class WritableFieldTemplate : GadgetTemplate
		{
			string _text = "";
			/*! \brief The initial text to be shown in the writableField gadget.
			 * \note Default value: Empty string.  */
			public string Text {
				get { return _text; }
				set { _text = value; }
			}

			int _max_text_len = 0;
			/*! \brief The maximum buffer size for the text. Leave as 0 to indicate that
			 * the length of the initial text should be used.  */
			public int MaxTextLen {
				get { return _max_text_len; }
				set { _max_text_len = value; }
			}

			string _allowable = "";
			/*! \brief Get or set the set of allowable characters which can be typed into a
			 * WritableField gadget.
			 * \note Default value: Empty string.  */
			public string Allowable {
				get { return _allowable; }
				set { _allowable = value; }
			}

			int _max_allowable_len = 0;
			/*! \brief The maximum buffer size reserved for the set of allowable characters.
			 * Leave as 0 to indicate that the length of the text itself should be used.  */
			public int MaxAllowableLen {
				get { return _max_allowable_len; }
				set { _max_allowable_len = value; }
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

			/*! \brief Create a Toolbox WritableField gadget template.  */
			public WritableFieldTemplate (string text, int maxTextLen) :
								base (Gadget.ComponentType.WritableField)
			{
				if (string.IsNullOrEmpty (text) && maxTextLen == 0)
					throw new ArgumentException ("Writable gadget text buffer must be greater than 0 in size.");

				_text = text;
				_max_text_len = maxTextLen;
			}

			/*! \brief Create a Toolbox WritableField gadget template.  */
			public WritableFieldTemplate (string text, int maxTextLen,
						      string allowable, int maxAllowableLen) :
								base (Gadget.ComponentType.WritableField)
			{
				_text = text;
				_max_text_len = maxTextLen;
				_allowable = allowable;
				_max_allowable_len = maxAllowableLen;
			}

			/*! \brief Set or get whether a WritableField gadget created from this template
			 * will inform clients of value changes using WritableField_ValueChanged events.
			 * \note Default value: false.  */
			public bool InformClient {
				get { return (_flags & WritableFieldFlags.InformClient) != 0; }
				set {
					_flags = (uint)(value ? _flags |  WritableFieldFlags.InformClient :
								_flags & ~WritableFieldFlags.InformClient);
				}
			}

			/*! \brief Set or get the justification of the text in this WritableField.  */
			public WritableFieldJustify Justify {
				get {
					return (WritableFieldJustify)((_flags >> WritableFieldFlags.JustifyShift) &
									     WritableFieldFlags.JustifyMask);
				}
				set {
					_flags &= ~(uint)(WritableFieldFlags.JustifyMask << WritableFieldFlags.JustifyShift);
					_flags |= (uint)value << WritableFieldFlags.JustifyShift;
				}
			}

			/*! \brief Set or get whether a WritableField gadget created from this template
			 * will use '-' for each character instead of displaying text.
			 * \note Default value: false.  */
			public bool AsPassword {
				get { return (_flags & WritableFieldFlags.Password) != 0; }
				set {
					_flags = (uint)(value ? _flags |  WritableFieldFlags.Password :
								_flags & ~WritableFieldFlags.Password);
				}
			}

			public override int CalculateBufferSize (ref int strStart, ref int msgStart)
			{
				int size = base.CalculateBufferSize (ref strStart, ref msgStart);

				if (!string.IsNullOrEmpty (_text))
					size += Math.Max (_text.Length + 1,
							  _max_text_len);
				if (!string.IsNullOrEmpty (_allowable))
					size += Math.Max (_allowable.Length + 1,
							  _max_allowable_len);
				return size;
			}

			public override void BuildBuffer (IntPtr buffer,
							  int offset,
							  ref int strOffset,
							  ref int msgOffset)
			{
				base.BuildBuffer (buffer, offset, ref strOffset, ref msgOffset);

				msgOffset = ObjectTemplate.WriteString (_text,
									_max_text_len,
									buffer,
									offset + WritableFieldTemplateOffset.Text,
									msgOffset);
				Marshal.WriteInt32 (buffer,
						    offset + WritableFieldTemplateOffset.MaxTextLen,
						    Math.Max (_text.Length + 1, _max_text_len));
				msgOffset = ObjectTemplate.WriteString (_allowable,
									_max_allowable_len,
									buffer,
									offset + WritableFieldTemplateOffset.Allowable,
									msgOffset);
				Marshal.WriteInt32 (buffer,
						    offset + WritableFieldTemplateOffset.MaxAllowableLen,
						    Math.Max (_allowable.Length + 1, _max_allowable_len));
				Marshal.WriteInt32 (buffer,
						    offset + WritableFieldTemplateOffset.Before,
						    _before);
				Marshal.WriteInt32 (buffer,
						    offset + WritableFieldTemplateOffset.After,
						    _after);
			}

			public override int GetTemplateSize ()
			{
				return WritableFieldTemplateOffset.TemplateSize;
			}
		}
	}
}
