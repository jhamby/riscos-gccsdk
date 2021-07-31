//
// toolbox/toolbox-task.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Runtime.InteropServices;

namespace riscos
{
	public class ToolboxTask : Task
	{
		IntPtr _SpriteArea;

		//! \brief Gets a pointer to the sprite area used by the toolbox.
		public IntPtr SpriteArea {
			get {
				return _SpriteArea;
			}
		}

		/*! \brief The Toolbox IDs of the current object, its parent and its ancestor.
		 * This is updated before each Toolbox/Wimp event is dispatched.  */
		public static NativeToolbox.IDBlock IDBlock;

		/*! \brief Initialise the Toolbox task.
		 * \param [in] wimpVersion The minimum version of the Wimp that we expect.
		 * \param [in] messageList A 0 terminated list of messages that we're
		 * interested in receiving, or null if we are not interested in any messages
		 * (except Quit), or a list containing only a single 0 element if we want all
		 * messages.
		 * \param [in] eventList A 0 terminated list of Toolbox events that we're
		 * interested in receiving.
		 * \param [in] dirName The name of the directory in which to find the resources.
		 * \return Nothing.
		 * \note The Toolbox makes a copy of the message and event lists, so it is
		 * not necessary to maintain local copies after this call has been made.  */
		public void Initialise (uint wimpVersion,
					int[] messageList,
					int[] eventList,
					string dirName)
		{
			OS.ThrowOnError (NativeMethods.Toolbox_Initialise (0,
									   wimpVersion,
									   messageList,
									   eventList,
									   dirName,
									   out _WimpVersion,
									   out _Handle,
									   out _SpriteArea));
		}

		//! \brief Retrieve the latest Toolbox IDs from the unmanaged memory block.
		public static NativeToolbox.IDBlock GetIDBlock ()
		{
			IntPtr idblock_ptr = NativeMethods.Toolbox_GetIDBlock();

			return (NativeToolbox.IDBlock)Marshal.PtrToStructure (idblock_ptr,
									     typeof (NativeToolbox.IDBlock));
		}

		//! \brief Dispatch Toolbox/Wimp events appropriately.
		public override void Dispatch (Wimp.EventArgs e)
		{
			Toolbox.Object tb_obj;

			IDBlock = GetIDBlock();

			if (Toolbox.Object.TryGetValue (IDBlock.SelfID, out tb_obj))
			{
				if (e.Type == Wimp.PollCode.ToolboxEvent)
				{
					// A Toolbox event for a specific object, so dispatch it to that object.
					tb_obj.Dispatch ((Toolbox.ToolboxEventArgs)e);
				}
				else
				{
					// A Wimp event for a specific object, so dispatch it to that object.
					tb_obj.Dispatch (e);
				}
			}
			else if (e.Type != Wimp.PollCode.ToolboxEvent)
			{
				if (e.Type == Wimp.PollCode.UserMessage ||
				    e.Type == Wimp.PollCode.UserMessageRecorded)
				{
					// Some messages are window specific, but the Toolbox ID Block
					// isn't updated. In this case, we use the Wimp handle to find the
					// Toolbox object.
					uint cmp_id;

					tb_obj = Toolbox.Window.WimpToToolbox (e.GetWindowHandle (),
									       -1,
									       out cmp_id);
					if (tb_obj != null)
						tb_obj.Dispatch (e);
					else
					{
						// The destination of the DataLoad could be the Iconbar, in
						// which case WimpToToolbox will return null, so let
						// Task.Dispatch deal with it.
						base.Dispatch (e);
					}
				}
				else
				{
					// A Wimp event that is not specific to an object, so let
					// Task.Dispatch deal with it.
					base.Dispatch (e);
				}
			}
			else
			{
				var toolbox_event = (Toolbox.ToolboxEventArgs) e;

				switch (toolbox_event.Header.EventCode)
				{
				case Toolbox.EventCode.ObjectAutoCreated:
					OnObjectAutoCreated (new Toolbox.ObjectAutoCreatedEventArgs (e.RawEventData));
					break;
				case Toolbox.EventCode.Error:
					OnError (new Toolbox.ErrorEventArgs (e.RawEventData));
					break;
				}
			}
		}

		//! \brief Create a CaretEventArgs object specific to Toolbox windows and gadgets.
		protected override Wimp.CaretEventArgs CreateCaretEventArgs (IntPtr rawEventData)
		{
			return new Toolbox.Window.CaretEventArgs (rawEventData);
		}

		//! \brief Create a PointerEventArgs object specific to Toolbox windows and gadgets.
		protected override Wimp.PointerEventArgs CreatePointerEventArgs (IntPtr rawEventData)
		{
			return new Toolbox.Window.PointerEventArgs (rawEventData);
		}

		/*! \brief Raising an event invokes the event handler through a delegate.
		 *
		 * The \b OnObjectAutoCreated method also allows derived classes to handle the
		 * event without attaching a delegate. This is the preferred technique for
		 * handling the event in a derived class.
		 * \note  When overriding \b OnObjectAutoCreated in a derived class, be sure to
		 * call the base class's \b OnObjectAutoCreated method so that registered delegates
		 * receive the event.  */
		protected virtual void OnObjectAutoCreated (Toolbox.ObjectAutoCreatedEventArgs e)
		{
			if (ObjectAutoCreated != null)
				ObjectAutoCreated (this, e);
		}

		/*! \brief Raising an event invokes the event handler through a delegate.
		 *
		 * The \b OnError method also allows derived classes to handle the
		 * event without attaching a delegate. This is the preferred technique for
		 * handling the event in a derived class.
		 * \note  When overriding \b OnError in a derived class, be sure to
		 * call the base class's \b OnError method so that registered delegates
		 * receive the event.  */
		protected virtual void OnError (Toolbox.ErrorEventArgs e)
		{
			if (Error != null)
				Error (this, e);
		}

		/*! \brief The event handlers that will be called when a Toolbox object is automatically
		 * created.  */
		public event EventHandler<Toolbox.ObjectAutoCreatedEventArgs> ObjectAutoCreated;

		/*! \brief The event handlers that will be called when the Toolbox indirectly returns
		 * an error to the task.  */
		public event EventHandler<Toolbox.ErrorEventArgs> Error;
	}
}
