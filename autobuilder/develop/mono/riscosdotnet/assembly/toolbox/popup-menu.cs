//
// toolbox/popup-menu.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \class PopupMenu
		 * \brief Displayed as a 'menu-arrow' icon.
		 *
		 * Its associated Menu object will be displayed when a mouse button
		 * is clicked over the icon.  */
		public class PopupMenu : Gadget
		{
			static class Method
			{
				public const int SetMenu = 704;
				public const int GetMenu = 705;
			}

			/*! \brief The default Toolbox event that a popup menu button can raise.  */
			public static class EventCode
			{
				public const uint AboutToBeShown = 0x8288b;
			}

			/*! \brief Wrap an existing popup menu button, e.g., from a Resource file created
			 * Window.  */
			public PopupMenu (Window window, uint cmpID) : base (window, cmpID)
			{
			}

			/*! \brief The Toolbox Menu to show when the button is clicked.  */
			public Toolbox.Menu Menu
			{
				set
				{
					if (value == null)
						throw new ArgumentNullException ("Attempt to set Popup Menu to null value");
					CallMethod_SetR4 (Method.SetMenu, value.ID);
				}
				get
				{
					uint menu_id = CallMethod_GetR0 (Method.GetMenu);

					Toolbox.Object tb_obj;
					if (!ToolboxTask.AllObjects.TryGetValue (menu_id, out tb_obj))
						throw new UnknownObjectException (menu_id);

					return (Menu)tb_obj;
				}
			}
		}
	}
}
