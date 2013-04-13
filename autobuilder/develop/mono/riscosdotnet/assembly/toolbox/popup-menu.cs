//
// toolbox/popup-menu.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;
using System.Runtime.InteropServices;

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

			/*! \brief The default Toolbox event that a %PopupMenu button can raise.  */
			public static class EventCode
			{
				public const uint AboutToBeShown = 0x8288b;
			}

			/*! \brief An object that encapsulates the arguments for the event that is raised when
			 * the Menu of a %PopupMenu gadget is about to be shown.  */
			public class AboutToBeShownEventArgs : ToolboxEventArgs
			{
				/*! \brief Constant defining event specific data offset after the header.  */
				public static class EventOffset
				{
					public const int MenuID = 16;
				}

				/*! \brief The Menu that is about to be shown.  */
				public readonly Toolbox.Menu Menu;

				/*! \brief Create the arguments for an AboutToBeShown event from the raw event data.  */
				public AboutToBeShownEventArgs (IntPtr unmanagedEventBlock) : base (unmanagedEventBlock)
				{
					uint menu_id = (uint)Marshal.ReadInt32 (RawEventData, EventOffset.MenuID);

					Menu = Object.CreateInstance<Menu> (menu_id);
				}
			}

			/*! \brief The event handlers that will be called when the menu for this %PopupMenu
			 * is about to be shown.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, AboutToBeShownEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * PopupMenuObject.AboutToBeShown += handler_name;
			 * \endcode  */
			public event EventHandler<AboutToBeShownEventArgs> AboutToBeShown;

			/*! \brief Wrap an existing popup menu button, e.g., from a Resource file created
			 * Window.  */
			public PopupMenu (Window window, uint cmpID) : base (window, cmpID)
			{
			}

			/*! \brief The Toolbox Menu to show when the button is clicked.  */
			// FIXME: Is it always a menu? Could it be something else?
			public Toolbox.Menu Menu
			{
				set {
					if (value == null)
						throw new ArgumentNullException ("value", "Attempt to set Popup Menu to null value");
					Object.MiscOp_SetR3R4 (0, Method.SetMenu, ComponentID, value.ID);
				}
				get {
					uint menu_id = Object.MiscOp_SetR3GetR0 (0, Method.GetMenu, ComponentID);

					return Object.CreateInstance<Menu> (menu_id);
				}
			}

			protected virtual void OnAboutToBeShown (AboutToBeShownEventArgs e)
			{
				if (AboutToBeShown != null)
					AboutToBeShown (this, e);
			}

			/*! \brief Check if the given event is relevant to the %PopupMenu gadget and call the
			 * associated event handlers.  */
			public override void Dispatch (ToolboxEventArgs e)
			{
				if (e.Header.EventCode == EventCode.AboutToBeShown)
					OnAboutToBeShown (new AboutToBeShownEventArgs (e.RawEventData));
			}
		}

		public static class PopupMenuTemplateOffset
		{
			public const int Menu = 36;
			public const int TemplateSize = 40;
		}

		/*! \brief The Toolbox flags that help to define the behaviour of a Pop-up menu gadget.  */
		public static class PopupMenuFlags
		{
			public const int InformAboutToBeShown = (1 << 0);
		}

		/*! \brief Encapsulates the data required to create a template for a PopupMenu
		 * gadget.  */
		public class PopupMenuTemplate : GadgetTemplate
		{
			string _menu = "";
			/*! \brief Set or get the name of the template which will be used to create
			 * the menu (can also be a dialogue box).  */
			public string Menu {
				get { return _menu; }
				set { _menu = value; }
			}

			/*! \brief Create a Toolbox PopupMenu gadget template.  */
			public PopupMenuTemplate (string menu) : base (Gadget.ComponentType.PopupMenu)
			{
				if (string.IsNullOrEmpty (menu))
					throw new ArgumentException ("Name of menu/dialogue template is not valid", "menu");
				_menu = menu;
			}

			/*! \brief Create a Toolbox PopupMenu gadget template where the menu template
			   * name will be set later.  */
			public PopupMenuTemplate () : base (Gadget.ComponentType.PopupMenu)
			{
			}

			/*! \brief Set or get whether a PopupMenu gadget created from this template
			 * will inform clients just before showing the menu.
			 * \note Default value: false.  */
			public bool InformAboutToBeShown {
				get { return (_flags & PopupMenuFlags.InformAboutToBeShown) != 0; }
				set {
					_flags = (uint)(value ? _flags |  PopupMenuFlags.InformAboutToBeShown :
								_flags & ~PopupMenuFlags.InformAboutToBeShown);
				}
			}

			public override int CalculateBufferSize (ref int strStart, ref int msgStart)
			{
				int size = base.CalculateBufferSize (ref strStart, ref msgStart);
				int length = _menu.Length + 1;

				size += length;
				msgStart += length;

				return size;
			}

			public override void BuildBuffer (IntPtr buffer,
							  int offset,
							  ref int strOffset,
							  ref int msgOffset)
			{
				base.BuildBuffer (buffer, offset, ref strOffset, ref msgOffset);

				strOffset = ObjectTemplate.WriteString (_menu,
									0,
									buffer,
									offset + PopupMenuTemplateOffset.Menu,
									strOffset);
			}

			public override int GetTemplateSize ()
			{
				return PopupMenuTemplateOffset.TemplateSize;
			}
		}
	}
}
