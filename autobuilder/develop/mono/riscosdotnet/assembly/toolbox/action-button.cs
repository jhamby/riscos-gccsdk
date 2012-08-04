//
// toolbox/action-button.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;
using System.Text;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \class ActionButton
		 * \brief Normally used to invoke an operation which is available from a
		 * dialogue box (e.g. a Cancel button or an OK button).  */
		public class ActionButton : Gadget
		{
			static class Method
			{
				public const int SetText = 128;
				public const int GetText = 129;
				public const int SetEvent = 130;
				public const int GetEvent = 131;
				public const int SetClickShow = 132;
				public const int GetClickShow = 133;
			}

			/*! \brief The default Toolbox event that an action button can raise.  */
			public static class EventCode
			{
				public const uint Selected = 0x82881;
			}

			/*! \brief An object that encapsulates the arguments for the event that is raised when an
			 * action button is selected.  */
			public class SelectedEventArgs : ToolboxEventArgs
			{
				/*! \brief \e true if the event was caused by an Adjust click.  */
				public bool AdjustClicked;
				/*! \brief \e true if the event was caused by a Select click.  */
				public bool SelectClicked;
				/*! \brief \e true if the event was raised for a Default action button.  */
				public bool Default;
				/*! \brief \e true if the event was raised for a Cancel action button.  */
				public bool Cancel;
				/*! \brief \e true if the event was raised for a Local action button.  */
				public bool Local;

				private class Flags
				{
					public const int Adjust = (1 << 0);
					// Bit 1 is reserved.
					public const int Select = (1 << 2);
					public const int Default = (1 << 3);
					public const int Cancel = (1 << 4);
					public const int Local = (1 << 5);
				}

				/*! \brief Create the arguments for a Selected event from the raw event data.  */
				public SelectedEventArgs (IntPtr unmanagedEventBlock) : base (unmanagedEventBlock)
				{
					uint flags = Header.Flags;

					AdjustClicked = (flags & Flags.Adjust) != 0;
					SelectClicked = (flags & Flags.Select) != 0;
					Default = (flags & Flags.Default) != 0;
					Cancel = (flags & Flags.Cancel) != 0;
					Local = (flags & Flags.Local) != 0;
				}
			}

			/*! \brief The signature of a Selected event handler.  */
			public delegate void SelectedHandler(object sender, SelectedEventArgs e);

			/*! \brief The event handlers that will be called when this ActionButton is selected.
			 *
			 * \note The event name is \b Selected rather than than \b Clicked as the event
			 * can be raised as the result of, for example, a keyboard action and not just
			 * a mouse button.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, SelectedEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * ActionButtonObject.Selected += handler_name;
			 * \endcode  */
			public event SelectedHandler Selected;

			/*! \brief Wrap an existing action button, e.g., from a Resource file created
			 * Window.  */
			public ActionButton (Window window, uint cmpID) : base (window, cmpID)
			{
			}

			/*! \brief The text which is displayed in the action button.  */
			public string Text
			{
				set { SetText (Method.SetText, value); }
				get { return GetText (Method.GetText); }
			}

			/*! \brief The event which will be raised when the action button is clicked.  */
			public uint Event
			{
				set { CallMethod_SetR4 (Method.SetEvent, value); }
				get { return CallMethod_GetR0 (Method.GetEvent); }
			}

			/*! \brief The Toolbox object to show and flags to use when the action
			 * button is clicked.  */
			public Toolbox.ClickShow ClickShow
			{
				set { SetClickShow (Method.SetClickShow, value); }
				get { return GetClickShow (Method.GetClickShow); }
			}

			public override void Dispatch (ToolboxEvent ev)
			{
				if (ev.ToolboxArgs.Header.EventCode == Event && Selected != null)
				{
					Selected (this, new SelectedEventArgs (ev.ToolboxArgs.RawEventData));
				}
			}
		}
	}
}
