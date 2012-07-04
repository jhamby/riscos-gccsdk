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
				get
				{
					uint icon_handle;
					OS.ThrowOnError (NativeMethods.Iconbar_GetIconHandle (0, ID, out icon_handle));
					return icon_handle;
				}
			}

			/*! \brief Specifies the menu which will be displayed when the Menu button is
			 * pressed over this Iconbar object.
			 * \note Set to null to detach the current menu.  */
			public Toolbox.Menu Menu
			{
				set
				{
					OS.ThrowOnError (NativeMethods.Iconbar_SetMenu (0,
											ID,
											(value == null) ? 0 : value.ID));
				}
				get
				{
					uint menu_id;
					OS.ThrowOnError (NativeMethods.Iconbar_GetMenu (0, ID, out menu_id));

					Toolbox.Object tb_obj;
					if (ToolboxTask.AllObjects.TryGetValue (menu_id, out tb_obj))
						return (Toolbox.Menu)tb_obj;
					return null;
				}
			}

			/*! \brief Specifies a Toolbox event to be raised when the user clicks Select
			 * on the Iconbar icon.
			 * \note Set to 0 to raise the default toolbox event instead.  */
			public uint SelectEvent
			{
				set
				{
					OS.ThrowOnError (NativeMethods.Iconbar_SetEvent (0x1, ID, value, 0));
				}
				get
				{
					uint select_event, adjust_event;
					OS.ThrowOnError (NativeMethods.Iconbar_GetEvent (0x1, ID,
											 out select_event,
											 out adjust_event));
					return select_event;
				}
			}

			/*! \brief Specifies a Toolbox event to be raised when the user clicks Adjust
			 * on the Iconbar icon.
			 * \note Set to 0 to raise the default toolbox event instead.  */
			public uint AdjustEvent
			{
				set
				{
					OS.ThrowOnError (NativeMethods.Iconbar_SetEvent (0x2, ID, 0, value));
				}
				get
				{
					uint select_event, adjust_event;
					OS.ThrowOnError (NativeMethods.Iconbar_GetEvent (0x2, ID,
											 out select_event,
											 out adjust_event));
					return adjust_event;
				}
			}

			/*! \brief Specifies an object to be shown when the user clicks Select on
			 * on the Iconbar icon.
			 * \note Set to null to prevent any object from being shown.  */
			public Toolbox.Object SelectShow
			{
				set
				{
					OS.ThrowOnError (NativeMethods.Iconbar_SetShow (0x1,
											ID,
											(value == null) ? 0 : value.ID,
											0));
				}
				get
				{
					uint select_show, adjust_show;
					Toolbox.Object tb_obj;
					OS.ThrowOnError (NativeMethods.Iconbar_GetShow (0x1,
											ID,
											out select_show,
											out adjust_show));
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
				set
				{
					OS.ThrowOnError (NativeMethods.Iconbar_SetShow (0x2,
											ID,
											0,
											(value == null) ? 0 : value.ID));
				}
				get
				{
					uint select_show, adjust_show;
					Toolbox.Object tb_obj;
					OS.ThrowOnError (NativeMethods.Iconbar_GetShow (0x2,
											ID,
											out select_show,
											out adjust_show));
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
						OS.ThrowOnError (NativeMethods.Iconbar_SetHelpMessage (0, ID, value));
					if (value == null)
						null_help_message = true;
				}
				get
				{
					if (null_help_message)
						return null;

					int buffer_size;
					OS.ThrowOnError (NativeMethods.Iconbar_GetHelpMessage (0, ID, null, 0,
											       out buffer_size));
					StringBuilder buffer = new StringBuilder (buffer_size);
					OS.ThrowOnError (NativeMethods.Iconbar_GetHelpMessage (0, ID, buffer,
											       buffer_size,
											       out buffer_size));
					return buffer.ToString ();
				}
			}

			/*! \brief The text which is used in a text and sprite Iconbar object.
			 * \note An OS exception is thrown if the text is longer than the maximum size
			 * specified when the Iconbar icon was created.<br>
			 * A null value is ignored.  */
			public string Text
			{
				set
				{
					if (value != null)
						OS.ThrowOnError (NativeMethods.Iconbar_SetText (0, ID, value));
				}
				get
				{
					int buffer_size;
					OS.ThrowOnError (NativeMethods.Iconbar_GetText (0,
											ID,
											null,
											0,
											out buffer_size));
					StringBuilder buffer = new StringBuilder (buffer_size);
					OS.ThrowOnError (NativeMethods.Iconbar_GetText (0,
											ID,
											buffer,
											buffer_size,
											out buffer_size));
					return buffer.ToString ();
				}
			}

			/*! \brief The sprite which is used in the Iconbar object.  */
			public string Sprite
			{
				set
				{
					if (value != null)
						OS.ThrowOnError (NativeMethods.Iconbar_SetSprite (0, ID, value));
				}
				get
				{
					int buffer_size;
					OS.ThrowOnError (NativeMethods.Iconbar_GetSprite (0,
											  ID,
											  null,
											  0,
											  out buffer_size));
					StringBuilder buffer = new StringBuilder (buffer_size);
					OS.ThrowOnError (NativeMethods.Iconbar_GetSprite (0,
											  ID,
											  buffer,
											  buffer_size,
											  out buffer_size));
					return buffer.ToString ();
				}
			}
		}
	}
}
