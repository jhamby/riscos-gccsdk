//
// toolbox/string-set.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;

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

			/*! \brief The default Toolbox events that a string set can raise.  */
			public static class EventCode
			{
				public const uint ValueChanged = 0x8288e;
				public const uint AboutToBeShown = 0x8288f;
			}

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

			/*! \brief The available set of strings in a string set.
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
				set
				{
					CallMethod_SetR4 ((uint)SelectedFlag.SelectedIsIndex,
							  Method.SetSelected, (uint)value);
				}
				get
				{
					return (int)CallMethod_GetR0 ((uint)SelectedFlag.SelectedIsIndex,
								      Method.GetSelected);
				}
			}

			/*! \brief Defines the set of allowable characters which can be typed into
			 * a writable string set.
			 * 
			 * The set is specified in the same way as a WIMP 'a' validation string
			 * directive (without including the letter 'a').  */
			public string Allowable
			{
				set { SetText (Method.SetAllowable, value); }
			}
/*
TODO:
GetComponents
*/
		}
	}
}
