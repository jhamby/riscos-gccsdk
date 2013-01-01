//
// task.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Collections;
using System.Runtime.InteropServices;

namespace riscos
{
	public abstract class Task
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
			Wimp.Poll (PollMask, out PollWord);
			Dispatch (GetEvent ());
		}

		// I could overload Poll() above, but PollIdle is probably a better indication of
		// what it does.
		/*! \brief Poll the Wimp and dispatch the event returned to the rest of the system.
		 * Calls Wimp.PollIdle using the time supplied.
		 * \param [in] time The number of centi-seconds to sleep for.
		 * \return Nothing.  */
		public void PollIdle (uint time)
		{
			Wimp.PollIdle (PollMask, time, out PollWord);
			Dispatch (GetEvent ());
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

		/*! \brief Raising an event invokes the event handler through a delegate.
		 *
		 * The \b OnMsgModeChange method also allows derived classes to handle the
		 * event without attaching a delegate. This is the preferred technique for
		 * handling the event in a derived class.
		 * \note  When overriding \b OnMsgModeChange in a derived class, be sure to
		 * call the base class's \b OnMsgModeChange method so that registered delegates
		 * receive the event.  */
		protected virtual void OnMsgModeChange (Wimp.MessageEventArgs e)
		{
			if (MsgModeChange != null)
				MsgModeChange (this, e);
		}

		/*! \brief Raising an event invokes the event handler through a delegate.
		 *
		 * The \b OnUserDragEnd method also allows derived classes to handle the
		 * event without attaching a delegate. This is the preferred technique for
		 * handling the event in a derived class.
		 * \note  When overriding \b OnUserDragEnd in a derived class, be sure to
		 * call the base class's \b OnUserDragEnd method so that registered delegates
		 * receive the event.  */
		protected virtual void OnUserDragEnd (Wimp.UserDragEventArgs e)
		{
			if (UserDragEnd != null)
				UserDragEnd (this, e);
		}

		/*! \brief Dispatch Wimp messages to both types of task.  */
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
			case Wimp.MessageAction.ModeChange:
				OnMsgModeChange (e);
				break;
			}
		}

		/*! \brief Raising an event invokes the event handler through a delegate.
		 *
		 * The \b OnNullEvent method also allows derived classes to handle the
		 * event without attaching a delegate. This is the preferred technique for
		 * handling the event in a derived class.
		 * \note  When overriding \b OnNullEvent in a derived class, be sure to
		 * call the base class's \b OnNullEvent method so that registered delegates
		 * receive the event.  */
		public virtual void OnNullEvent (Wimp.EventArgs e)
		{
			if (NullEvent != null)
				NullEvent (this, e);
		}

		/*! \brief Dispatch any Wimp events that may be of interest to both Wimp and Toolbox tasks.  */
		public virtual void Dispatch (Wimp.EventArgs e)
		{
			switch (e.Type)
			{
			case Wimp.PollCode.Null:
				OnNullEvent (e);
				break;
			case Wimp.PollCode.UserDragBox:
				OnUserDragEnd ((Wimp.UserDragEventArgs)e);
				break;
			case Wimp.PollCode.UserMessage:
			case Wimp.PollCode.UserMessageRecorded:
				OnMessage ((Wimp.MessageEventArgs)e);
				break;
			}
		}

		/*! \brief Retrieve the current Wimp event from the unmanaged buffer.  */
		private Wimp.EventArgs GetEvent ()
		{
			Wimp.PollCode type = NativeMethods.wimp_get_event_type ();
			IntPtr event_ptr = NativeMethods.wimp_get_event ();

			switch (type)
			{
			case Wimp.PollCode.RedrawWindow:
				return new Wimp.RedrawEventArgs (event_ptr);
			case Wimp.PollCode.OpenWindow:
				return new Wimp.OpenEventArgs (event_ptr);
			case Wimp.PollCode.CloseWindow:
				return new Wimp.CloseEventArgs (event_ptr);
			case Wimp.PollCode.PointerLeaveWindow:
				return CreatePointerEventArgs (event_ptr);
			case Wimp.PollCode.PointerEnterWindow:
				return CreatePointerEventArgs (event_ptr);
			case Wimp.PollCode.MouseClick:
				return new Wimp.MouseClickEventArgs (event_ptr);
			case Wimp.PollCode.UserDragBox:
				return new Wimp.UserDragEventArgs (event_ptr);
			case Wimp.PollCode.KeyPressed:
				return new Wimp.KeyPressEventArgs (event_ptr);
			case Wimp.PollCode.ScrollRequest:
				return new Wimp.ScrollRequestEventArgs (event_ptr);
			case Wimp.PollCode.LoseCaret:
				return CreateCaretEventArgs (event_ptr);
			case Wimp.PollCode.GainCaret:
				return CreateCaretEventArgs (event_ptr);
			case Wimp.PollCode.MenuSelection:
				return new Wimp.MenuSelectionEventArgs (event_ptr);
			case Wimp.PollCode.UserMessage:
			case Wimp.PollCode.UserMessageRecorded:
				return GetMessage (event_ptr);
			case Wimp.PollCode.ToolboxEvent:
				return new Toolbox.ToolboxEventArgs (event_ptr);
			case Wimp.PollCode.Null:
			default:
				return new Wimp.EventArgs (event_ptr);
			}
		}

		/*! \brief Retrieve the current Wimp message from the unmanaged buffer.  */
		private Wimp.EventArgs GetMessage (IntPtr unmanagedEventBlock)
		{
			var type = (Wimp.MessageAction)Marshal.
					ReadInt32 (unmanagedEventBlock,
						   Wimp.MessageEventArgs.EventHeaderOffset.MessageType);
			switch (type)
			{
			case Wimp.MessageAction.DataLoad:
				return new Wimp.DataLoadMessageEventArgs (unmanagedEventBlock);
			default:
				return new Wimp.MessageEventArgs (unmanagedEventBlock);
			}
		}

		protected abstract Wimp.CaretEventArgs CreateCaretEventArgs (IntPtr rawEventData);
		protected abstract Wimp.PointerEventArgs CreatePointerEventArgs (IntPtr rawEventData);

		/*! \brief The event handlers that will be called when a Wimp Quit message is received.  */
		public event EventHandler<Wimp.MessageEventArgs> MsgQuit;

		/*! \brief The event handlers that will be called when a Wimp PreQuit message is received.  */
		public event EventHandler<Wimp.MessageEventArgs> MsgPreQuit;

		/*! \brief The event handlers that will be called when a Wimp DataLoad message is received.  */
		public event EventHandler<Wimp.DataLoadMessageEventArgs> MsgDataLoad;

		/*! \brief The event handlers that will be called when a Wimp ModeChange message is received.  */
		public event EventHandler<Wimp.MessageEventArgs> MsgModeChange;

		/*! \brief The event handlers that will be called when a user drag ends due to the release
		 * of the mouse buttons.  */
		public event EventHandler<Wimp.UserDragEventArgs> UserDragEnd;

		/*! \brief The event handlers that will be called when a Null event is received.  */
		public event EventHandler<Wimp.EventArgs> NullEvent;
	}

	public class WimpTask : Task
	{
		public void Initialise (int version, string desc, int[] mess_list)
		{
			uint handle;

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
