//
// toolbox/colour-menu.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;
using System.Runtime.InteropServices;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \class ColourMenu
		 * \brief Allows the user to specify a colour from a menu of 16
		 * desktop colours.  */
		public class ColourMenu : Object
		{
			static class Method
			{
				public const int SetColour = 0;
				public const int GetColour = 1;
				public const int SetNoneAvailable = 2;
				public const int GetNoneAvailable = 3;
				public const int SetTitle = 4;
				public const int GetTitle = 5;
			}

			/*! \brief The default Toolbox events that this Colour Menu object can
			 * raise.  */
			public class EventCode
			{
				public const uint AboutToBeShown = 0x82980;
				public const uint HasBeenHidden = 0x82981;
				public const uint Selection = 0x82982;
			}

			public enum WimpColour
			{
				Unselected = -1,
				White = 0,
				Grey1,
				Grey2,
				Grey3,
				Grey4,
				Grey5,
				Grey6,
				Black,
				Blue,
				Yellow,
				Green,
				Red,
				Cream,
				DarkGreen,
				Orange,
				Cyan,
				None
			}

			/*! \brief Create a Toolbox %ColourMenu object from the named resource.
			 * \param [in] resName The name of the template in the resource file.  */
			public ColourMenu (string resName) : base (resName)
			{
			}

			/*! \brief Create a Toolbox %ColourMenu from the template data given.
			 * \param[in] templateData Pointer to the menu template.  */
			public ColourMenu (IntPtr templateData)
			{
				Create (templateData);
			}

			/*! \brief Create a %ColourMenu from a Toolbox object that already exists.
			 * \param [in] objectID The Toolbox ID of the existing object.  */
			public ColourMenu (uint ObjectID) : base (ObjectID)
			{
			}

			/*! \brief Gets or sets the Wimp colour which is currently selected for this
			 * %ColourMenu.
			 * The value should be 0-15 for a Wimp colour, or 16 for \b None, or -1 for
			 * nothing selected.  */
			public WimpColour Colour
			{
				set { SetColour (value); }
				get { return GetColour (); }
			}

			/*! \brief Sets or gets whether a \b None option appears in the %ColourMenu.  */
			public bool NoneAvailable
			{
				set { SetNoneAvailable (value); }
				get { return GetNoneAvailable (); }
			}

			/*! \brief Gets or sets the text which is to be used in the title bar
			 * of this %ColourMenu.  */
			public string Title
			{
				set { SetTitle (value); }
				get { return GetTitle(); }
			}

			/*! \brief Selects a colour as being the currently selected one for this %ColourMenu.
			 * \param [in] colour The Wimp colour to be displayed as selected
			 * (0-15, or 16 for \b None, or -1 for 'nothing selected'.
			 * \note The \e Colour property can also be used for this purpose.  */
			public void SetColour (WimpColour colour)
			{
				if (colour < WimpColour.Unselected || colour > WimpColour.None)
					throw new ArgumentOutOfRangeException ("Colour", colour, "Attempt to set ColourMenu to invalid desktop colour number");
				MiscOp_SetR3 (0, Method.SetColour, (uint)colour);
			}

			/*! \brief Return the Wimp colour which is currently selected for this %ColourMenu.
			 * \return The Wimp colour selected (0-15, or 16 for \b None, or -1 for 'nothing selected'.
			 * \note The \e Colour property can also be used for this purpose.  */
			public WimpColour GetColour ()
			{
				return (WimpColour)MiscOp_GetR0 (0, Method.GetColour);
			}

			/*! \brief Sets whether there is a \b None entry for this %ColourMenu.
			 * \param [in] noneAvailable \e true if the %ColourMenu should include a \b None option.
			 * \return Nothing.
			 * \note The \e NoneAvailable property can also be used for this purpose.  */
			public void SetNoneAvailable (bool noneAvailable)
			{
				MiscOp_SetR3 (0, Method.SetNoneAvailable, noneAvailable ? 1U : 0U);
			}

			/*! \brief Returns whether this %ColourMenu has a \b None entry.
			 * \return \e true if the %ColourMenu should include a \b None options.
			 * \note The \e NoneAvailable property can also be used for this purpose.  */
			public bool GetNoneAvailable ()
			{
				return (MiscOp_GetR0 (0, Method.GetNoneAvailable) & 1) != 0;
			}

			/*! \brief Sets the text which is to be used in the title bar of the
			 * %ColourMenu.
			 * \param [in] title The string to be used for the title.
			 * \return Nothing.
			 * \note The \e Title property can also be used for this purpose.  */
			public void SetTitle (string title)
			{
				SetText (0, Method.SetTitle, title);
			}

			/*! \brief Returns the text string used in the title bar of the
			 * %ColourMenu.
			 * \return The title of the %ColourMenu.
			 * \note The \e Title property can also be used for this purpose.  */
			public string GetTitle ()
			{
				return GetText (Method.GetTitle);
			}

			protected virtual void OnAboutToBeShown (ToolboxEvent e)
			{
				if (AboutToBeShown != null)
					AboutToBeShown (this, new AboutToBeShownEventArgs (e.ToolboxArgs.RawEventData));
			}

			protected virtual void OnHasBeenHidden (ToolboxEvent e)
			{
				if (HasBeenHidden != null)
					HasBeenHidden (this, e.ToolboxArgs);
			}

			protected virtual void OnSelection (ToolboxEvent e)
			{
				if (Selection != null)
					Selection (this, new SelectionEventArgs (e.ToolboxArgs.RawEventData));
			}

			/*! \brief Check if the given event is relevant to the Colour Menu and call the
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
				case EventCode.Selection:
					OnSelection (e);
					break;
				}
			}

			new public delegate void AboutToBeShownEventHandler (object sender, AboutToBeShownEventArgs e);
			public delegate void SelectionEventHandler (object sender, SelectionEventArgs e);

			/*! \brief The event handlers that will be called just before this %ColourMenu is shown.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, ColourMenu.AboutToBeShownEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * ColourMenuObject.AboutToBeShown += handler_name;
			 * \endcode  */
			public event AboutToBeShownEventHandler AboutToBeShown;

			/*! \brief The event handlers that will be called when this %ColourMenu has been hidden.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, ToolboxEventHandler e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * ColourMenuObject.HasBeenHidden += handler_name;
			 * \endcode  */
			public event ToolboxEventHandler HasBeenHidden;

			/*! \brief The event handlers that will be called when the user has clicked on one of the
			 * colour entries in the %ColourMenu.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, ToolboxEventHandler e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * ColourMenuObject.Selection += handler_name;
			 * \endcode  */
			public event SelectionEventHandler Selection;

			/*! \class AboutToBeShownEventArgs
			 * \brief An object that encapsulates the arguments for the event that is raised
			 * just before the %ColourMenu object is shown on screen.  */
			new public class AboutToBeShownEventArgs : ToolboxEventArgs
			{
				/*! \brief Constants defining event specific data offsets after the header.  */
				public static class EventOffset
				{
					public const int ShowType = 16;
					public const int Buffer = 20;
				}

				/*! \brief Gives details of where the menu will be displayed.
				 * \note FullSpec is the same as TopLeft in the case of a %ColourMenu. DefaultSpec
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
			 * when a colour selection is made by the user.  */
			public class SelectionEventArgs : ToolboxEventArgs
			{
				/*! \brief Constant defining event specific data offset after the header.  */
				public static class EventOffset
				{
					public const int Colour = 16;
				}

				/*! \brief Wimp colour selected (0-15, or 16 for \b None).  */
				public readonly WimpColour Colour;

				/*! \brief Create the event arguments from the raw event data.  */
				public SelectionEventArgs (IntPtr unmanagedEventData) : base (unmanagedEventData)
				{
					Colour = (WimpColour)Marshal.ReadInt32 (RawEventData, EventOffset.Colour);
				}
			}
		}
	}
}
