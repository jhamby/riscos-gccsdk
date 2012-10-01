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

			/*! \brief The signature of a ValueChange event handler.  */
			public delegate void ValueChangeEventHandler (object sender, ValueChangeEventArgs e);

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
			public event ValueChangeEventHandler ValueChange;

			/*! \brief The signature of an AboutToBeShown event handler.  */
			public delegate void AboutToBeShownEventHandler (object sender, ToolboxEventArgs e);

			/*! \brief The event handlers that will be called when the %StringSet menu is about
			 * to be shown.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, ValueChangeEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * StringSetObject.AboutToBeShown += handler_name;
			 * \endcode  */
			public event AboutToBeShownEventHandler AboutToBeShown;

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

			/*! \brief Check if the given event is relevant to the StringSet and call the associated
			 * event handlers.  */
			public override void Dispatch (ToolboxEvent ev)
			{
				switch (ev.ToolboxArgs.Header.EventCode)
				{
				case EventCode.ValueChanged:
					if (ValueChange != null)
						ValueChange (this, new ValueChangeEventArgs (ev.ToolboxArgs.RawEventData));
					break;
				case EventCode.AboutToBeShown:
					if (AboutToBeShown != null)
						AboutToBeShown (this, ev.ToolboxArgs);
					break;
				}
			}
		}
	}
}
