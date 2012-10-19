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

		public event Wimp.MenuSelectionEventHandler MenuSelection;

		protected virtual void OnMenuSelection (Wimp.MenuSelectionEventArgs e)
		{
			if (MenuSelection != null)
				MenuSelection (this, e);
		}

		public virtual void KeyPress (int charCode) { }

		public virtual void Dispatch (Wimp.EventArgs e)
		{
			switch (e.Type)
			{
			case Wimp.PollCode.MenuSelection:
				OnMenuSelection ((Wimp.MenuSelectionEventArgs) e);
				break;
			}
		}

		public void Poll ()
		{
			Dispatch (Wimp.Poll (PollMask, out PollWord));
		}

		// I could overload Poll() above, but PollIdle is probably a better indication of
		// what it does.
		public void PollIdle (uint time)
		{
			Dispatch (Wimp.PollIdle (PollMask, time, out PollWord));
		}
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

		public override void Dispatch (Wimp.EventArgs e)
		{
			uint window_handle = e.GetWindowHandle ();

			if (window_handle <= 0)
			{
				// A Wimp event that is not specific to a window, so let Task.Dispatch
				// deal with it.
				base.Dispatch (e);
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
	}
}
