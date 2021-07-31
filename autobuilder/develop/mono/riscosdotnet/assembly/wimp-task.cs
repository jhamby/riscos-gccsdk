//
// wimp-task.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;

namespace riscos
{
	public class WimpTask : Task
	{
		/*! \brief Initialise the Wimp task.
		 * \param [in] version The minimum version of the Wimp that we expect.
		 * \param [in] desc The name of the task as displayed in the task manager.
		 * \param [in] messageList A 0 terminated list of messages that we're
		 * interested in receiving, or null if we are not interested in any messages
		 * (except Quit), or a list containing only a single 0 element if we want all
		 * messages.
		 * \return Nothing.
		 * \note The Wimp makes a copy of the message list, so it is not necessary to
		 * maintain a local copy after this call has been made.  */
		public void Initialise (int version, string desc, int[] messageList)
		{
			OS.ThrowOnError (NativeMethods.Wimp_Initialise (version,
									desc,
									messageList,
									out _WimpVersion,
									out _Handle));
		}

		//! \brief End the current task.
		public void CloseDown ()
		{
			OS.ThrowOnError (NativeMethods.Wimp_CloseDown (_Handle));
		}

		/*! \brief Raising an event invokes the event handler through a delegate.
		 *
		 * The \b OnMenuSelection method also allows derived classes to handle the
		 * event without attaching a delegate. This is the preferred technique for
		 * handling the event in a derived class.
		 * \note  When overriding \b OnMenuSelection in a derived class, be sure to
		 * call the base class's \b OnMenuSelection method so that registered delegates
		 * receive the event.  */
		protected virtual void OnMenuSelection (Wimp.MenuSelectionEventArgs e)
		{
			if (MenuSelection != null)
				MenuSelection (this, e);
		}

		/*! \brief Dispatch events that are specific to a plain Wimp task and are of no
		 * use to a Toolbox task.  */
		public override void Dispatch (Wimp.EventArgs e)
		{
			uint window_handle = e.GetWindowHandle ();

			if ((int)window_handle <= 0)
			{
				switch (e.Type)
				{
				case Wimp.PollCode.MenuSelection:
					OnMenuSelection ((Wimp.MenuSelectionEventArgs) e);
					break;
				default:
					// A Wimp event that is not specific to a window, so let Task.Dispatch
					// deal with it.
					base.Dispatch (e);
					break;
				}
			}
			else
			{
				Wimp.Window window = Wimp.Window.GetInstance (window_handle);
				window.Dispatch (e);
			}
		}

		public override void KeyPress (int charCode)
		{
			Wimp.ProcessKey (charCode);
		}

		//! \brief Create a GainCaretEventArgs object specific to plain Wimp window/icons.
		protected override Wimp.CaretEventArgs CreateCaretEventArgs (IntPtr rawEventData)
		{
			return new Wimp.Window.CaretEventArgs (rawEventData);
		}

		//! \brief Create a PointerEventArgs object specific to plain Wimp window/icons.
		protected override Wimp.PointerEventArgs CreatePointerEventArgs (IntPtr rawEventData)
		{
			return new Wimp.Window.PointerEventArgs (rawEventData);
		}

		/*! \brief The event handlers that will be called when a menu selection is made.  */
		public event EventHandler<Wimp.MenuSelectionEventArgs> MenuSelection;
	}
}
