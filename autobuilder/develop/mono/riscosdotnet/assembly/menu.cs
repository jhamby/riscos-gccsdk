//
// menu.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Text;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace riscos
{
	public static partial class Wimp
	{
		/*! \class MenuItem */
		public class MenuItem
		{
			IconData Icon;

			/*! \brief Set to a menu object if this menu item should lead to a submenu. */
			public Menu SubMenu { get; set; }
			/*! \brief Set to a window object if this menu item should lead to a dialogue box. */
			public Window SubWindow { get; set; }
			/*! \brief Set to true if this menu item should be ticked. */
			public bool Ticked { get; set; }
			/*! \brief Set to true if this menu item should be followed by a dotted line. */
			public bool DottedLine { get; set; }
			/*! \brief Set to true if this menu item should be writable. */
			public bool Writable { get; set; }

			/*! \brief Create a menu item.
			 * \param [in] text The text of the menu item.  */
			public MenuItem (String text)
			{
				Icon = new IconData (new IconBuffer (text));
				SubMenu = null;
				SubWindow = null;
				Ticked = false;
				DottedLine = false;
				Writable = false;
			}

			/*! \brief Create a menu item.
			 * \param [in] text The text of the menu item.
			 * \param [in] validation The validation string for the menu item. */
			public MenuItem (String text, String validation)
			{
				Icon = new IconData (new IconBuffer (text),
						     new IconBuffer (validation));
				SubMenu = null;
				SubWindow = null;
				Ticked = false;
				DottedLine = false;
				Writable = false;
			}

			/*! \brief Create a menu item.
			 * \param [in] text The text of the menu item.
			 * \param [in] maxTextLen The size of the buffer that should be allocated
			 * to the menu item's text. Used for writable entries where the text may
			 * be shorter than what is allowed to be typed in.
			 * \param [in] validation The validation string for the menu item. */
			public MenuItem (String text, int maxTextLen, String validation)
			{
				Icon = new IconData (new IconBuffer (text, maxTextLen),
						     new IconBuffer (validation));
				SubMenu = null;
				SubWindow = null;
				Ticked = false;
				DottedLine = false;
				Writable = false;
			}

			public int TextBufferSize ()
			{
				return Icon.Text.BufferSize;
			}

			public IntPtr TextBuffer ()
			{
				return Icon.Text.Buffer;
			}

			public IntPtr ValidationBuffer ()
			{
				return Icon.Validation.Buffer;
			}

			/*! \brief Retrieve the text of the menu item.
			 * Useful for a writable menu item where the text can be changed by the User.
			 * \return The text of the menu item in the form of a \e System.String.  */
			public string GetText ()
			{
				return Marshal.PtrToStringAuto (Icon.Text.Buffer);
			}
		}

		/*! \class Menu
		 * \brief Encapsulates a RISC OS WIMP menu. */
		public class Menu
		{
			IconData Title;
			List<MenuItem> Items;

			IntPtr RawMenuData;

			static Menu Root;

			/*! \brief Determine if the supplied menu was the last one to be created.
			 * \param [in] menu The menu to check.
			 * \return true if the supplied menu was the last to be created.
			 *
			 * \note Can be used when decoding menu selections to determine which menu
			 * the selection refers to.  */
			public static bool IsCurrent (Menu menu)
			{
				return (Root != null) && (menu == Root);
			}

			/*! \brief Create a menu object.
			 * \param [in] title The title to be given to the menu. */
			public Menu (String title)
			{
				Title = new IconData (new IconBuffer(title));
				Items = new List<MenuItem>();
			}

			/*! \brief Add an item to the menu.
			 * \param [in] text The text of the menu item.
			 * \return The new menu item.  */
			public MenuItem AddItem (String text)
			{
				MenuItem item = new MenuItem (text);
				Items.Add (item);
				return item;
			}

			/*! \brief Add an item to the menu.
			 * \param [in] text The text of the menu item.
			 * \param [in] validation The validation string for the menu item.
			 * \return The new menu item.  */
			public MenuItem AddItem (String text, String validation)
			{
				MenuItem item = new MenuItem (text, validation);
				Items.Add (item);
				return item;
			}

			/*! \brief Add an item to the menu.
			 * \param [in] text The text of the menu item.
			 * \param [in] maxTextLen The size of the buffer that should be allocated
			 * to the menu item's text. Used for writable entries where the text may
			 * be shorter than what is allowed to be typed in.
			 * \param [in] validation The validation string for the menu item.
			 * \return The new menu item.  */
			public MenuItem AddItem (String text, int maxTextLen, String validation)
			{
				MenuItem item = new MenuItem (text, maxTextLen, validation);
				Items.Add (item);
				return item;
			}

			/*! \brief Free memory previously allocate for a WIMP menu.
			 * \note This function is called recursively for submenus.  */
			void Free ()
			{
				foreach (MenuItem item in Items)
				{
					if (item.SubMenu != null)
						item.SubMenu.Free ();
				}

				Marshal.FreeHGlobal (RawMenuData);
			}

			/*! \brief Allocate memory for a WIMP menu data block and fill in the
			 * data.
			 * \note This function is called recursively for submenus.<br>
			 * The WIMP does not take a copy of this data, so it must remain at a constant
			 * address (the GC cannot be allowed to move it) and it must remain intact
			 * while the menu remains on screen.  */
			IntPtr Generate ()
			{
				int menu_buffer_size = 24 + (Items.Count * 24);

				RawMenuData = Marshal.AllocHGlobal (menu_buffer_size);

				// Write menu title
				Marshal.WriteInt32 (RawMenuData, 0, Title.Text.Buffer.ToInt32());
				Marshal.WriteInt32 (RawMenuData, 4, 1);
				Marshal.WriteInt32 (RawMenuData, 8, Title.Text.BufferSize);

				// Write menu title foreground and frame colour
				Marshal.WriteByte (RawMenuData, 12, 7);
				// Write menu title background colour
				Marshal.WriteByte (RawMenuData, 13, 2);
				// Write menu work area foreground colour
				Marshal.WriteByte (RawMenuData, 14, 7);
				// Write menu work area background colour
				Marshal.WriteByte (RawMenuData, 15, 0);
				// Write width of following menu items
				Marshal.WriteInt32 (RawMenuData, 16, 400);
				// Write height of following menu items
				Marshal.WriteInt32 (RawMenuData, 20, 44);
				// Write vertical gap between items
				Marshal.WriteInt32 (RawMenuData, 24, 0);

				int offset = 28, count = 0;
				foreach (MenuItem item in Items)
				{
					int flags = 0;

					// Indicate that title is indirected.
					if (count == 0)
						flags |= (1 << 8);

					// Set flag to indicate last item
					if (count == Items.Count - 1)
						flags |= (1 << 7);

					if (item.Ticked)
						flags |= (1 << 0);
					if (item.DottedLine)
						flags |= (1 << 1);
					if (item.Writable)
						flags |= (1 << 2);

					Marshal.WriteInt32 (RawMenuData, offset, flags);
					// Write submenu pointer
					if (item.SubMenu != null)
						Marshal.WriteInt32 (RawMenuData, offset + 4, item.SubMenu.Generate ().ToInt32 ());
					else if (item.SubWindow != null)
						Marshal.WriteInt32 (RawMenuData, offset + 4, (int)item.SubWindow.WimpHandle);
					else
						Marshal.WriteInt32 (RawMenuData, offset + 4, -1);
					// Write menu icon flags
					Marshal.WriteInt32 (RawMenuData, offset + 8, 0x70c0121);
					// Write address of text
					Marshal.WriteInt32 (RawMenuData, offset + 12, item.TextBuffer().ToInt32());
					// Write text buffer length
					Marshal.WriteInt32 (RawMenuData, offset + 20, item.TextBufferSize());
					// Write address of validation
					Marshal.WriteInt32 (RawMenuData, offset + 16, item.ValidationBuffer().ToInt32());

					offset += 24;
					count++;
				}

				return RawMenuData;
			}

			/*! \brief Show the menu on the screen.
			 * \param [in] x X coordinate of top left corner of top level menu.
			 * \param [in] y Y coordinate of top left corner of top level menu.
			 * \return Nothing. */
			public void Show (int x, int y)
			{
				if (Root != null)
					Root.Free ();
				Root = this;
				OS.ThrowOnError (NativeMethods.Wimp_CreateMenu (Generate (), x, y));
			}
		}
	}
}
