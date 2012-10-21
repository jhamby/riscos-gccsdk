using System;
using System.Collections;

namespace riscos
{
	public class Task
	{
		public uint Handle { get; protected set; }
		public bool Quit { get; set; }
		public uint PollWord;

		public uint PollMask { get; set; }

		// The actual version number of the WIMP that Wimp_Initialise returned to us.
		public int WimpVersion;

		public virtual void KeyPress (int charCode) { }

		/*! \brief Poll the Wimp and dispatch the event returned to the rest of the system.
		 * Calls Wimp.Poll.
		 * \return Nothing.  */
		public void Poll ()
		{
			Dispatch (Wimp.Poll (PollMask, out PollWord));
		}

		// I could overload Poll() above, but PollIdle is probably a better indication of
		// what it does.
		/*! \brief Poll the Wimp and dispatch the event returned to the rest of the system.
		 * Calls Wimp.PollIdle using the time supplied.
		 * \param [in] time The number of centi-seconds to sleep for.
		 * \return Nothing.  */
		public void PollIdle (uint time)
		{
			Dispatch (Wimp.PollIdle (PollMask, time, out PollWord));
		}

		/*! \brief Raising an event invokes the event handler through a delegate.
		 *
		 * The \b OnMsgQuit method also allows derived classes to handle the
		 * event without attaching a delegate. This is the preferred technique for
		 * handling the event in a derived class.
		 * \note  When overriding \b OnMsgQuit in a derived class, be sure to
		 * call the base class's \b OnMsgQuit method so that registered delegates
		 * receive the event.  */
		protected virtual void OnMsgQuit (Wimp.MessageEventArgs e)
		{
			if (MsgQuit != null)
				MsgQuit (this, e);
			Quit = true;
		}

		/*! \brief Raising an event invokes the event handler through a delegate.
		 *
		 * The \b OnMsgPreQuit method also allows derived classes to handle the
		 * event without attaching a delegate. This is the preferred technique for
		 * handling the event in a derived class.
		 * \note  When overriding \b OnMsgPreQuit in a derived class, be sure to
		 * call the base class's \b OnMsgPreQuit method so that registered delegates
		 * receive the event.  */
		protected virtual void OnMsgPreQuit (Wimp.MessageEventArgs e)
		{
			if (MsgPreQuit != null)
				MsgPreQuit (this, e);
		}

		/*! \brief Raising an event invokes the event handler through a delegate.
		 *
		 * The \b OnMsgDataLoad method also allows derived classes to handle the
		 * event without attaching a delegate. This is the preferred technique for
		 * handling the event in a derived class.
		 * \note  When overriding \b OnMsgDataLoad in a derived class, be sure to
		 * call the base class's \b OnMsgDataLoad method so that registered delegates
		 * receive the event.  */
		protected virtual void OnMsgDataLoad (Wimp.DataLoadMessageEventArgs e)
		{
			if (MsgDataLoad != null)
				MsgDataLoad (this, e);
		}

		protected virtual void OnMessage (Wimp.MessageEventArgs e)
		{
			switch (e.MessageType)
			{
			case Wimp.MessageAction.Quit:
				OnMsgQuit (e);
				break;
			case Wimp.MessageAction.PreQuit:
				OnMsgPreQuit (e);
				break;
			case Wimp.MessageAction.DataLoad:
				OnMsgDataLoad ((Wimp.DataLoadMessageEventArgs)e);
				break;
			}
		}

		public virtual void Dispatch (Wimp.EventArgs e)
		{
			switch (e.Type)
			{
			case Wimp.PollCode.UserMessage:
			case Wimp.PollCode.UserMessageRecorded:
				OnMessage ((Wimp.MessageEventArgs)e);
				break;
			}
		}

		/*! \brief The event handlers that will be called when a Wimp Quit message is received.  */
		public event Wimp.MessageEventHandler MsgQuit;

		/*! \brief The event handlers that will be called when a Wimp PreQuit message is received.  */
		public event Wimp.MessageEventHandler MsgPreQuit;

		/*! \brief The event handlers that will be called when a Wimp DataLoad message is received.  */
		public event Wimp.DataLoadMessageEventHandler MsgDataLoad;
	}

	public class WimpTask : Task
	{
		public static Hashtable AllWindows;

		public void Initialise (int version, string desc, int[] mess_list)
		{
			uint handle;

			AllWindows = new Hashtable ();

			OS.ThrowOnError (NativeMethods.Wimp_Initialise (version, desc, mess_list, out WimpVersion, out handle));

			Handle = handle;
		}

		public void CloseDown ()
		{
			OS.ThrowOnError (NativeMethods.Wimp_CloseDown (Handle));
		}

		protected virtual void OnMenuSelection (Wimp.MenuSelectionEventArgs e)
		{
			if (MenuSelection != null)
				MenuSelection (this, e);
		}

		public override void Dispatch (Wimp.EventArgs e)
		{
			uint window_handle = e.GetWindowHandle ();

			if (window_handle <= 0)
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
				Wimp.Window window = (Wimp.Window)WimpTask.AllWindows [window_handle];
				window.Dispatch (e);
			}
		}

		public override void KeyPress (int charCode)
		{
			Wimp.ProcessKey (charCode);
		}

		/*! \brief The event handlers that will be called when a menu selection is made.  */
		public event Wimp.MenuSelectionEventHandler MenuSelection;
	}
}
