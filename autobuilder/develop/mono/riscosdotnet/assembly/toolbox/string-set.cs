//
// toolbox/string-set.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;
using System.Runtime.InteropServices;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \class StringSet
		 * \brief Used to display one of an ordered set of text strings.
		 *
		 * The selected string can be set either by text string or by index.  */
		public class StringSet : Gadget
		{
			static class Method
			{
				public const int SetAvailable = 896;
				public const int SetSelected = 898;
				public const int GetSelected = 899;
				public const int SetAllowable = 900;
				public const int GetComponents = 902;
			}

			/*! \brief The default Toolbox events that a %StringSet can raise.  */
			public static class EventCode
			{
				public const uint ValueChanged = 0x8288e;
				public const uint AboutToBeShown = 0x8288f;
			}

			/*! \brief An object that encapsulates the arguments for the event that is raised when the
			 * value of a StringSet changes.  */
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

				/*! \brief The new value of the StringSet that was selected by the user.  */
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

			/*! \brief The event handlers that will be called when the value of this %StringSet
			 * changes.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, ValueChangeEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * StringSetObject.ValueChange += handler_name;
			 * \endcode  */
			public event EventHandler<ValueChangeEventArgs> ValueChange;

			/*! \brief The event handlers that will be called when the %StringSet menu is about
			 * to be shown.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, ToolboxEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * StringSetObject.AboutToBeShown += handler_name;
			 * \endcode  */
			public event EventHandler<ToolboxEventArgs> AboutToBeShown;

			/*! \brief Wrap an existing string set, e.g., from a Resource file created
			 * Window.  */
			public StringSet (Window window, uint cmpID) : base (window, cmpID)
			{
			}

			enum SelectedFlag
			{
				SelectedIsString,
				SelectedIsIndex
			}

			/*! \brief The available set of strings in a %StringSet.
			 *
			 * A popup menu is built from the block of contiguous strings set
			 * by this property. Strings are separated using a comma; a comma
			 * must be escaped using the '\\' character. To get the '\\'
			 * character itself, '\\\' must be used.  */
			// Note: An extra '\' added above for html.
			public string Available
			{
				set { SetText (Method.SetAvailable, value); }
			}

			/*! \brief The selected string as a text string.  */
			public string SelectedString
			{
				set { SetText (Method.SetSelected, value); }
				get { return GetText (Method.GetSelected); }
			}

			/*! \brief The selected string as an index into the available strings.  */
			public int SelectedIndex
			{
				set {
					Object.MiscOp_SetR3R4 ((uint)SelectedFlag.SelectedIsIndex,
							       Method.SetSelected,
							       ComponentID,
							       (uint)value);
				}
				get {
					return (int)Object.MiscOp_SetR3GetR0 ((uint)SelectedFlag.SelectedIsIndex,
									      Method.GetSelected,
									      ComponentID);
				}
			}

			/*! \brief Defines the set of allowable characters which can be typed into
			 * a writable %StringSet.
			 * 
			 * The set is specified in the same way as a WIMP 'a' validation string
			 * directive (without including the letter 'a').  */
			public string Allowable
			{
				set { SetText (Method.SetAllowable, value); }
			}

			/*! \brief Return the objects of the gadgets that make up the %StringSet.
			 * \param [out] alphaNumeric An object that wraps the DisplayField part of the %StringSet.
			 * \param [out] popupMenu An object that wraps the PopupMenu part of the %StringSet.
			 * \return Nothing.
			 * \exception ArgumentException Thrown if the alphaNumeric part is a WritableField rather
			 * than a DisplayField.
			 * \note There is an alternative method to use when the alphaNumeric part is a WritableField.  */
			public void GetComponents (out DisplayField alphaNumeric,
						   out PopupMenu popupMenu)
			{
				uint alphaNumericID;
				uint popupMenuID;

				Object.MiscOp_SetR3GetR0R1 (3, // Return both component IDs
							    Method.GetComponents,
							    ComponentID,
							    out alphaNumericID,
							    out popupMenuID);

				if (GetType (Object.ID, alphaNumericID) != ComponentType.DisplayField)
					throw new ArgumentException ("alphaNumeric", "Component is not a DisplayField");

				alphaNumeric = new DisplayField ((Window)Object, alphaNumericID);
				popupMenu = new PopupMenu ((Window)Object, popupMenuID);
			}

			/*! \brief Return the objects of the gadgets that make up the %StringSet.
			 * \param [out] alphaNumeric An object that wraps the WritableField part of the %StringSet.
			 * \param [out] popupMenu An object that wraps the PopupMenu part of the %StringSet.
			 * \return Nothing.
			 * \exception ArgumentException Thrown if the alphaNumeric part is a WritableField rather
			 * than a DisplayField.
			 * \note There is an alternative method to use when the alphaNumeric part is a DisplayField.  */
			public void GetComponents (out WritableField alphaNumeric,
						   out PopupMenu popupMenu)
			{
				uint alphaNumericID;
				uint popupMenuID;

				Object.MiscOp_SetR3GetR0R1 (3, // Return both component IDs
							    Method.GetComponents,
							    ComponentID,
							    out alphaNumericID,
							    out popupMenuID);

				if (GetType (Object.ID, alphaNumericID) != ComponentType.WritableField)
					throw new ArgumentException ("alphaNumeric", "Component is not a WritableField");

				alphaNumeric = new WritableField ((Window)Object, alphaNumericID);
				popupMenu = new PopupMenu ((Window)Object, popupMenuID);
			}

			protected virtual void OnValueChange (ValueChangeEventArgs e)
			{
				if (ValueChange != null)
					ValueChange (this, e);
			}

			protected virtual void OnAboutToBeShown (ToolboxEventArgs e)
			{
				if (AboutToBeShown != null)
					AboutToBeShown (this, e);
			}

			/*! \brief Check if the given event is relevant to the StringSet and call the associated
			 * event handlers.  */
			public override void Dispatch (ToolboxEventArgs e)
			{
				switch (e.Header.EventCode)
				{
				case EventCode.ValueChanged:
					OnValueChange (new ValueChangeEventArgs (e.RawEventData));
					break;
				case EventCode.AboutToBeShown:
					OnAboutToBeShown (e);
					break;
				}
			}
		}

		public static class StringSetFlags
		{
			public const int InformClient = (1 << 0);
			// Note bit 1 not used.
			// User Interface Toolbox Manual Page 403 - Writable flag is given as
			// bit 1, but is actually bit 2.
			public const int Writable = (1 << 2);
			public const int InformAboutToBeShown = (1 << 3);
			public const int NoDisplayField = (1 << 4);
			public const int JustifyShift = 5;
			public const int JustifyMask = 3;
		}

		public static class StringSetTemplateOffset
		{
			public const int StringSet = 36;
			// User Interface Toolbox Manual Page 410 - Menu title field missing
			// from template layout.
			public const int MenuTitle = 40;
			public const int InitialSelectedString = 44;
			public const int MaxSelectedStringLen = 48;
			public const int Allowable = 52;
			public const int MaxAllowable = 56;
			public const int Before = 60;
			public const int After = 64;
			public const int TemplateSize = 68;
		}

		public enum StringSetJustify
		{
			Left,
			Right,
			Centre
		}

		/*! \brief Encapsulate the data required to create a Toolbox StringSet gadget
		 * template.  */
		public class StringSetTemplate : GadgetTemplate
		{
			string _string_set = "";
			/*! \brief Get or set the comma separated set of strings that are available
			 * for selection.  */
			public string StringSet {
				get { return _string_set; }
				set { _string_set = value; }
			}

			string _menu_title = "Items";
			/*! \brief Get or set the title of the StringSet menu.
			 * \note Default value: "Items".  */
			public string MenuTitle {
				get { return _menu_title; }
				set { _menu_title = value; }
			}

			string _initial_selected_string = "";
			/*! \brief Get or set the string that is initially selected.  */
			public string InitialSelectedString {
				get { return _initial_selected_string; }
				set { _initial_selected_string = value; }
			}

			int _max_selected_string_len = 0;
			/*! \brief The maximum buffer size for the string that is currently selected.
			 * Leave as 0 to indicate that the length of InitialSelectedString should be used.  */
			public int MaxSelectedStringLen {
				get { return _max_selected_string_len; }
				set { _max_selected_string_len = value; }
			}

			string _allowable = "";
			/*! \brief Get or set the set of allowable characters which can be typed into a
			 * writable string set.  */
			public string Allowable {
				get { return _allowable; }
				set { _allowable = value; }
			}


			int _max_allowable = 0;
			/*! \brief The maximum buffer size reserved for the set of allowable characters.
			 * Leave as 0 to indicate that the length of the text itself should be used.  */
			public int MaxAllowable {
				get { return _max_allowable; }
				set { _max_allowable = value; }
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

			/*! \brief Create a Toolbox StringSet gadget template.  */
			public StringSetTemplate (string stringSet,
						  string initial,
						  int maxInitial,
						  string allowable,
						  int maxAllowable) : base (Gadget.ComponentType.StringSet)
			{
				_string_set = stringSet;
				_initial_selected_string = initial;
				_max_selected_string_len = maxInitial;
				_allowable = allowable;
				_max_allowable = maxAllowable;
			}

			/*! \brief Create a Toolbox StringSet gadget template.  */
			public StringSetTemplate (string stringSet,
						  string initial,
						  int maxInitial) : base (Gadget.ComponentType.StringSet)
			{
				_string_set = stringSet;
				_initial_selected_string = initial;
				_max_selected_string_len = maxInitial;
			}

			/*! \brief Set or get whether a StringSet gadget created from this template
			 * will inform clients of value changes using StringSet_ValueChanged events.
			 * \note Default value: false.  */
			public bool InformClient {
				get { return (_flags & StringSetFlags.InformClient) != 0; }
				set {
					_flags = (uint)(value ? _flags |  StringSetFlags.InformClient :
								_flags & ~StringSetFlags.InformClient);
				}
			}

			/*! \brief Set or get whether the StringSet gadget will have a writable field.
			 * \note Default value: false.  */
			public bool IsWritable {
				get { return (_flags & StringSetFlags.Writable) != 0; }
				set {
					_flags = (uint)(value ? _flags |  StringSetFlags.Writable :
								_flags & ~StringSetFlags.Writable);
				}
			}

			/*! \brief Set or get whether a StringSet gadget created from this template
			 * will inform clients just before showing the menu.
			 * \note Default value: false.  */
			public bool InformAboutToBeShown {
				get { return (_flags & StringSetFlags.InformAboutToBeShown) != 0; }
				set {
					_flags = (uint)(value ? _flags |  StringSetFlags.InformAboutToBeShown :
								_flags & ~StringSetFlags.InformAboutToBeShown);
				}
			}

			/*! \brief Set or get whether the StringSet gadget will have a display field or writable.
			 * \note Default value: true.  */
			public bool HasDisplayField {
				get { return (_flags & StringSetFlags.NoDisplayField) == 0; }
				set {
					// Note the logic is reversed here because the flag is set
					// when there is no display field.
					_flags = (uint)(value ? _flags & ~StringSetFlags.NoDisplayField :
								_flags |  StringSetFlags.NoDisplayField);
				}
			}

			/*! \brief Set or get the justification of the text in this StringSet.  */
			public StringSetJustify Justify {
				get {
					return (StringSetJustify)((_flags >> StringSetFlags.JustifyShift) &
									     StringSetFlags.JustifyMask);
				}
				set {
					_flags &= ~(uint)(StringSetFlags.JustifyMask << StringSetFlags.JustifyShift);
					_flags |= (uint)value << StringSetFlags.JustifyShift;
				}
			}

			public override int CalculateBufferSize (ref int strStart, ref int msgStart)
			{
				int size = base.CalculateBufferSize (ref strStart, ref msgStart);

				// The string set buffer has a fixed length; there is no max buffer size
				// variable.
				if (!string.IsNullOrEmpty (_string_set))
					size += _string_set.Length + 1;
				if (!string.IsNullOrEmpty (_menu_title))
					size += _menu_title.Length + 1;
				if (!string.IsNullOrEmpty (_initial_selected_string))
					size += Math.Max (_initial_selected_string.Length + 1,
							  _max_selected_string_len);
				if (!string.IsNullOrEmpty (_allowable))
					size += Math.Max (_allowable.Length + 1,
							  _max_allowable);
				return size;
			}

			public override void BuildBuffer (IntPtr buffer,
							  int offset,
							  ref int strOffset,
							  ref int msgOffset)
			{
				base.BuildBuffer (buffer, offset, ref strOffset, ref msgOffset);

				msgOffset = ObjectTemplate.WriteString (_string_set,
									0,
									buffer,
									offset + StringSetTemplateOffset.StringSet,
									msgOffset);
				msgOffset = ObjectTemplate.WriteString (_menu_title,
									0,
									buffer,
									offset + StringSetTemplateOffset.MenuTitle,
									msgOffset);
				msgOffset = ObjectTemplate.WriteString (_initial_selected_string,
									_max_selected_string_len,
									buffer,
									offset + StringSetTemplateOffset.InitialSelectedString,
									msgOffset);
				Marshal.WriteInt32 (buffer,
						    offset + StringSetTemplateOffset.MaxSelectedStringLen,
						    Math.Max (_initial_selected_string.Length + 1, _max_selected_string_len));
				msgOffset = ObjectTemplate.WriteString (_allowable,
									_max_allowable,
									buffer,
									offset + StringSetTemplateOffset.Allowable,
									msgOffset);
				Marshal.WriteInt32 (buffer,
						    offset + StringSetTemplateOffset.MaxAllowable,
						    Math.Max (_allowable.Length + 1, _max_allowable));
				Marshal.WriteInt32 (buffer,
						    offset + StringSetTemplateOffset.Before,
						    _before);
				Marshal.WriteInt32 (buffer,
						    offset + StringSetTemplateOffset.After,
						    _after);
			}

			public override int GetTemplateSize ()
			{
				return StringSetTemplateOffset.TemplateSize;
			}
		}
	}
}
