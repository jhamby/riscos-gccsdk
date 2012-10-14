//
// toolbox/font-menu.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;
using System.Runtime.InteropServices;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \class FontMenu
		 * \brief A menu which shows the currently selected font, and allows the user to
		 * set this from a list of font names.  */
		public class FontMenu : Object
		{
			static class Method
			{
				public const int SetFont = 0;
				public const int GetFont = 1;
			}

			/*! \brief The default Toolbox events that this Font Menu object can
			 * raise.  */
			public class EventCode
			{
				public const uint AboutToBeShown = 0x82a40;
				public const uint HasBeenHidden = 0x82a41;
				public const uint FontSelection = 0x82a42;
			}

			/*! \brief Create a Toolbox Font Menu object from the named resource.
			 * \param [in] resName The name of the template in the resource file.  */
			public FontMenu (string resName) : base (resName)
			{
			}

			/*! \brief Create a Toolbox Font Menu from the template data given.
			 * \param[in] templateData Pointer to the menu template.  */
			public FontMenu (IntPtr templateData)
			{
				Create (templateData);
			}

			/*! \brief Create a Font Menu from a Toolbox object that already exists.
			 * \param [in] objectID The Toolbox ID of the existing object.  */
			public FontMenu (uint ObjectID) : base (ObjectID)
			{
			}

			/*! \brief Gets or sets the font id of the font which is currently selected.  */
			public string Font
			{
				set { SetFont (value); }
				get { return GetFont (); }
			}

			/*! \brief Selects a font as being the currently selected one for this Font Menu.
			 *
			 * The special font id 'SystemFont' is used to indicate that the \b System entry
			 * should be ticked.
			 * \param [in] font The id of the font to be shown selected.
			 * \return Nothing.  */
			public void SetFont (string font)
			{
				MiscOp_SetR3 (0, Method.SetFont, font);
			}

			/*! \brief Return the font id for the font which was last specified to be the current
			 * one either programatically or by a user selection.
			 * 
			 * The special font id 'SystemFont' is used to indicate that the \b System entry was
			 * last chosen.
			 * \return The font id of the last chosen font.  */
			public string GetFont ()
			{
				return GetText (Method.GetFont);
			}

			protected virtual void OnAboutToBeShown (ToolboxEvent e)
			{
				if (AboutToBeShown != null)
					AboutToBeShown (this, new MenuAboutToBeShownEventArgs (e.ToolboxArgs.RawEventData));
			}

			protected virtual void OnHasBeenHidden (ToolboxEvent e)
			{
				if (HasBeenHidden != null)
					HasBeenHidden (this, e.ToolboxArgs);
			}

			protected virtual void OnFontSelection (ToolboxEvent e)
			{
				if (FontSelection != null)
					FontSelection (this, new FontSelectionEventArgs (e.ToolboxArgs.RawEventData));
			}

			/*! \brief Check if the given event is relevant to the Font Menu and call the
			 * associated event handlers.  */
			public override void Dispatch (ToolboxEvent e)
			{
				switch (e.ToolboxArgs.Header.EventCode)
				{
				case EventCode.AboutToBeShown:
					OnAboutToBeShown (e);
					break;
				case EventCode.HasBeenHidden:
					OnHasBeenHidden (e);
					break;
				case EventCode.FontSelection:
					OnFontSelection (e);
					break;
				}
			}

			public delegate void FontSelectionEventHandler (object sender, FontSelectionEventArgs e);

			/*! \brief The event handlers that will be called just before this %FontMenu is shown.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, Object.MenuAboutToBeShownEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * FontMenuObject.AboutToBeShown += handler_name;
			 * \endcode  */
			public event MenuAboutToBeShownEventHandler AboutToBeShown;

			/*! \brief The event handlers that will be called when this %FontMenu has been hidden.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, ToolboxEventHandler e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * FontMenuObject.HasBeenHidden += handler_name;
			 * \endcode  */
			public event ToolboxEventHandler HasBeenHidden;

			/*! \brief The event handlers that will be called when the user has clicked on one of the
			 * font entries in the %FontMenu.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, FontMenu.FontSelectionEventHandler e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * FontMenuObject.Selection += handler_name;
			 * \endcode  */
			public event FontSelectionEventHandler FontSelection;

			/*! \brief An object that encapsulates the arguments for the event that is raised
			 * when a font selection is made by the user.  */
			public class FontSelectionEventArgs : ToolboxEventArgs
			{
				/*! \brief Constant defining event specific data offset after the header.  */
				public static class EventOffset
				{
					public const int FontID = 16;
				}

				/*! \brief Font id of selected font.  */
				public readonly string FontID;

				/*! \brief Create the event arguments from the raw event data.  */
				public FontSelectionEventArgs (IntPtr unmanagedEventData) : base (unmanagedEventData)
				{
					FontID = Marshal.PtrToStringAnsi (new IntPtr (RawEventData.ToInt32() + EventOffset.FontID));
				}
			}
		}
	}
}
