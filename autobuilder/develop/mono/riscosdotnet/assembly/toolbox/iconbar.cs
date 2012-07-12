//
// toolbox/iconbar.cs - a binding of the RISC OS API to C#.
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
		/*! \class Iconbar
		 * \brief Encapsulates a Toolbox iconbar object.  */
		public class Iconbar : Object
		{
			class Method
			{
				public const int GetIconHandle = 0;
				public const int SetMenu = 1;
				public const int GetMenu = 2;
				public const int SetEvent = 3;
				public const int GetEvent = 4;
				public const int SetShow = 5;
				public const int GetShow = 6;
				public const int SetHelpMessage = 7;
				public const int GetHelpMessage = 8;
				public const int SetText = 9;
				public const int GetText = 10;
				public const int SetSprite = 11;
				public const int GetSprite = 12;
			}

			public class EventCode
			{
				public const int Clicked = 0x82900;
				public const int SelectAboutToBeShown = 0x82901;
				public const int AdjustAboutToBeShown = 0x82902;
			}

			// Track whether the help message has been set to null in the Toolbox.
			// If it has, then attempting to set it again will overwrite address 0.
			private bool null_help_message;

			public Iconbar (string resName) : base (resName)
			{
				IntPtr template = TemplateLookup (resName);
				if (template != IntPtr.Zero)
					null_help_message = (Marshal.ReadInt32 (template, 21 * 4) == 0);
				else
					null_help_message = true;
			}

			/*! \brief The WIMP handle of the icon on the iconbar.  */
			public uint IconHandle
			{
				get { return GetHandle (Method.GetIconHandle); }
			}

			/*! \brief Specifies the menu which will be displayed when the Menu button is
			 * pressed over this Iconbar object.
			 * \note Set to null to detach the current menu.  */
			public Toolbox.Menu Menu
			{
				set { SetMenu (Method.SetMenu, value); }
				get { return GetMenu (Method.GetMenu); }
			}

			/*! \brief Specifies a Toolbox event to be raised when the user clicks Select
			 * on the Iconbar icon.
			 * \note Set to 0 to raise the default toolbox event instead.  */
			public uint SelectEvent
			{
				set { SetSelect (Method.SetEvent, value); }
				get { return GetSelect (Method.GetEvent); }
			}

			/*! \brief Specifies a Toolbox event to be raised when the user clicks Adjust
			 * on the Iconbar icon.
			 * \note Set to 0 to raise the default toolbox event instead.  */
			public uint AdjustEvent
			{
				set { SetAdjust (Method.SetEvent, value); }
				get { return GetAdjust (Method.GetEvent); }
			}

			/*! \brief Specifies an object to be shown when the user clicks Select on
			 * on the Iconbar icon.
			 * \note Set to null to prevent any object from being shown.  */
			public Toolbox.Object SelectShow
			{
				set { SetSelect (Method.SetShow, (value == null) ? 0 : value.ID); }
				get
				{
					uint select_show = GetSelect (Method.GetShow);
					Toolbox.Object tb_obj;
					if (ToolboxTask.AllObjects.TryGetValue (select_show, out tb_obj))
						return tb_obj;
					return null;
				}
			}


			/*! \brief Specifies an object to be shown when the user clicks Adjust on
			 * on the Iconbar icon.
			 * \note Set to null to prevent any object from being shown.  */
			public Toolbox.Object AdjustShow
			{
				set { SetAdjust (Method.SetShow, (value == null) ? 0 : value.ID); }
				get
				{
					uint adjust_show = GetAdjust (Method.GetShow);
					Toolbox.Object tb_obj;
					if (ToolboxTask.AllObjects.TryGetValue (adjust_show, out tb_obj))
						return tb_obj;
					return null;
				}
			}

			/*! \brief The help message which will be returned when a Help Request message
			 * is received for this Iconbar object.
			 * \note Set to null to detach the Help Message for this Iconbar object. Once
			 * set to null, it cannot be re-attached.  */
			public string HelpMessage
			{
				set
				{
					if (!null_help_message)
						SetText (Method.SetHelpMessage, value);
					if (value == null)
						null_help_message = true;
				}
				get
				{
					if (null_help_message)
						return null;

					return GetText (Method.GetHelpMessage);
				}
			}

			/*! \brief The text which is used in a text and sprite Iconbar object.
			 * \exception ArgumentNullException Thrown if Iconbar text set to null.
			 * \note An OS exception is thrown if the text is longer than the maximum size
			 * specified when the Iconbar icon was created.<br>
			 * A null value is ignored.  */
			public string Text
			{
				set
				{
					if (value == null)
						throw new ArgumentNullException ("Attempted to set Iconbar text to null");
					SetText (Method.SetText, value);
				}
				get { return GetText (Method.GetText); }
			}

			/*! \brief The sprite which is used in the Iconbar object.
			 * \exception ArgumentNullException Thrown if Iconbar sprite set to null.  */
			public string Sprite
			{
				set
				{
					if (value == null)
						throw new ArgumentNullException ("Attempted to set Iconbar sprite to null");
				}
				get { return GetText (Method.GetSprite); }
			}

			private void SetSelect (int method, uint select)
			{
				OS.ThrowOnError (NativeMethods.Object_SetR3R4 (0x1,
									       ID,
									       method,
									       select,
									       0));
			}

			private void SetAdjust (int method, uint adjust)
			{
				OS.ThrowOnError (NativeMethods.Object_SetR3R4 (0x2,
									       ID,
									       method,
									       0,
									       adjust));
			}

			private uint GetSelect (int method)
			{
				uint select, adjust;
				OS.ThrowOnError (NativeMethods.Object_GetR0R1 (0x1,
									       ID,
									       method,
									       out select,
									       out adjust));
				return select;
			}

			private uint GetAdjust (int method)
			{
				uint select, adjust;
				OS.ThrowOnError (NativeMethods.Object_GetR0R1 (0x2,
									       ID,
									       method,
									       out select,
									       out adjust));
				return adjust;
			}
		}
	}
}
