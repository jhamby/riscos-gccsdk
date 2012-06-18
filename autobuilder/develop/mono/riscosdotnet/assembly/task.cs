using System;
using System.Collections;

namespace riscos
{
	public class Task
	{
		public uint Handle { get; set; }
		public bool Quit { get; set; }
		public uint PollWord;

		// The actual version number of the WIMP that Wimp_Initialise returned to use.
		public int wimp_version;

		public virtual void MenuSelection (int [] selection) { }
		public virtual void KeyPress (int charCode) { }
	}

	public class WimpTask : Task
	{
		public static Hashtable AllWindows;

		public uint poll_mask { get; set; }

		public void Initialise (int version, string desc, int[] mess_list)
		{
			uint handle;

			AllWindows = new Hashtable ();

			OS.ThrowOnError (NativeMethods.Wimp_Initialise (version, desc, mess_list, out wimp_version, out handle));

			Handle = handle;
		}

		public void CloseDown ()
		{
			OS.ThrowOnError (NativeMethods.Wimp_CloseDown (Handle));
		}

		public void PollLoop ()
		{
			while (Quit == false)
			{
				OS.ThrowOnError (NativeMethods.Wimp_Poll (poll_mask, out PollWord));

				Wimp.Event event_base = Wimp.Event.GetEvent ();

				switch (event_base.type)
				{
				case Wimp.PollCode.RedrawWindow:
					{
						Wimp.RedrawWindowEvent event_full = (Wimp.RedrawWindowEvent)event_base;
						Wimp.Window event_window = (Wimp.Window)WimpTask.AllWindows [event_full.RedrawArgs.RedrawWimpBlock.WindowHandle];
						event_window.OnRedraw (event_full);
					}
					break;
				case Wimp.PollCode.OpenWindow:
					{
						Wimp.OpenWindowEvent event_full = (Wimp.OpenWindowEvent)event_base;
						Wimp.Window event_window = (Wimp.Window)WimpTask.AllWindows [event_full.OpenWindowArgs.OpenWimpBlock.WindowHandle];
						event_window.OnOpen (event_full);
					}
					break;
				case Wimp.PollCode.CloseWindow:
					{
						Wimp.CloseWindowEvent event_full = (Wimp.CloseWindowEvent)event_base;
						Wimp.Window event_window = (Wimp.Window)WimpTask.AllWindows [event_full.CloseWindowArgs.CloseWimpBlock.WindowHandle];
						event_window.OnClose (event_full);
					}
					break;
				case Wimp.PollCode.PointerLeaveWindow:
					{
						Wimp.PointerLeaveEvent event_full = (Wimp.PointerLeaveEvent)event_base;
						Wimp.Window event_window = (Wimp.Window)WimpTask.AllWindows [event_full.PointerLeaveArgs.PointerLeaveWimpBlock.WindowHandle];
						event_window.OnPointerLeave (event_full);
					}
					break;
				case Wimp.PollCode.PointerEnterWindow:
					{
						Wimp.PointerEnterEvent event_full = (Wimp.PointerEnterEvent)event_base;
						Wimp.Window event_window = (Wimp.Window)WimpTask.AllWindows [event_full.PointerEnterArgs.PointerEnterWimpBlock.WindowHandle];
						event_window.OnPointerEnter (event_full);
					}
					break;
				case Wimp.PollCode.MouseClick:
					{
						Wimp.MouseClickEvent event_full = (Wimp.MouseClickEvent)event_base;
						Wimp.Window event_window = (Wimp.Window)WimpTask.AllWindows [event_full.MouseClickArgs.MouseClickWimpBlock.WindowHandle];
						event_window.OnMouseClick (event_full);
					}
					break;
				case Wimp.PollCode.KeyPressed:
					{
						Wimp.KeyPressedEvent event_full = (Wimp.KeyPressedEvent)event_base;
						Wimp.Window event_window = (Wimp.Window)WimpTask.AllWindows [event_full.KeyPressedArgs.KeyPressedWimpBlock.WindowHandle];
						if (event_window == null)
							KeyPress (event_full.KeyPressedArgs.KeyPressedWimpBlock.CharCode);
						else
							event_window.OnKeyPressed (event_full);
					}
					break;
				case Wimp.PollCode.ScrollRequest:
					{
						Wimp.ScrollRequestEvent event_full = (Wimp.ScrollRequestEvent)event_base;
						Wimp.Window event_window = (Wimp.Window)WimpTask.AllWindows [event_full.ScrollRequestArgs.ScrollRequestWimpBlock.WindowHandle];
						event_window.OnScrollRequest (event_full);
					}
					break;
				case Wimp.PollCode.LoseCaret:
					{
						Wimp.LoseCaretEvent event_full = (Wimp.LoseCaretEvent)event_base;
						Wimp.Window event_window = (Wimp.Window)WimpTask.AllWindows [event_full.LoseCaretArgs.LoseCaretWimpBlock.WindowHandle];
						event_window.OnLoseCaret (event_full);
					}
					break;
				case Wimp.PollCode.GainCaret:
					{
						Wimp.GainCaretEvent event_full = (Wimp.GainCaretEvent)event_base;
						Wimp.Window event_window = (Wimp.Window)WimpTask.AllWindows [event_full.GainCaretArgs.GainCaretWimpBlock.WindowHandle];
						event_window.OnGainCaret (event_full);
					}
					break;
				case Wimp.PollCode.MenuSelection:
					{
						Wimp.MenuSelectionEvent event_full = (Wimp.MenuSelectionEvent)event_base;
						MenuSelection (event_full.MenuSelectionArgs.MenuSelectionWimpBlock.Selection);
					}
					break;
				}
			}
		}

		public override void KeyPress (int charCode)
		{
			Wimp.ProcessKey (charCode);
		}
	}
}
