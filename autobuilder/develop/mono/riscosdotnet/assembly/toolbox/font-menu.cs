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

			new public delegate void AboutToBeShownEventHandler (object sender, AboutToBeShownEventArgs e);
			public delegate void FontSelectionEventHandler (object sender, FontSelectionEventArgs e);

			/*! \brief The event handlers that will be called just before this %FontMenu is shown.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, AboutToBeShownEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * FontMenuObject.AboutToBeShown += handler_name;
			 * \endcode  */
			public event AboutToBeShownEventHandler AboutToBeShown;

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
			 * void handler_name (object sender, ToolboxEventHandler e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * FontMenuObject.Selection += handler_name;
			 * \endcode  */
			public event FontSelectionEventHandler FontSelection;

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

			/*! \brief Check if the given event is relevant to the Font Menu and call the
			 * associated event handlers.  */
			public override void Dispatch (ToolboxEvent ev)
			{
				switch (ev.ToolboxArgs.Header.EventCode)
				{
				case EventCode.AboutToBeShown:
					if (AboutToBeShown != null)
						AboutToBeShown (this, new AboutToBeShownEventArgs (ev.ToolboxArgs.RawEventData));
					break;
				case EventCode.HasBeenHidden:
					if (HasBeenHidden != null)
						HasBeenHidden (this, ev.ToolboxArgs);
					break;
				case EventCode.FontSelection:
					if (FontSelection != null)
						FontSelection (this, new FontSelectionEventArgs (ev.ToolboxArgs.RawEventData));
					break;
				}
			}

			/*! \class AboutToBeShownEventArgs
			 * \brief An object that encapsulates the arguments for the event that is raised
			 * just before the Font Menu object is shown on screen.  */
			new public class AboutToBeShownEventArgs : ToolboxEventArgs
			{
				/*! \brief Constants defining event specific data offsets after the header.  */
				public static class EventOffset
				{
					public const int ShowType = 16;
					public const int Buffer = 20;
				}

				/*! \brief Gives details of where the menu will be displayed.
				 * \note FullSpec is the same as TopLeft in the case of a Font Menu. DefaultSpec
				 * means that the menu will open 64 OS units to the left of the mouse pointer.  */
				public readonly ShowObjectSpec ShowSpec;

				/*! \brief Create the event arguments from the raw event data.  */
				public AboutToBeShownEventArgs (IntPtr unmanagedEventData) : base (unmanagedEventData)
				{
					ShowObjectType show_type = (ShowObjectType)Marshal.ReadInt32 (RawEventData,
												      EventOffset.ShowType);
					switch (show_type)
					{
					case Toolbox.ShowObjectType.FullSpec:
					case Toolbox.ShowObjectType.TopLeft:
						var ev = (NativeToolbox.ShowObjectTopLeftEvent)
								Marshal.PtrToStructure (RawEventData,
											typeof (NativeToolbox.ShowObjectTopLeftEvent));
						ShowSpec = new ShowObjectTopLeft (new OS.Coord (ev.Spec.TopLeft));
						break;
					case Toolbox.ShowObjectType.Default:
						ShowSpec = new ShowObjectSpec (Toolbox.ShowObjectType.Default);
						break;
					default:
						ShowSpec = null;
						break;
					}
				}
			}

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
