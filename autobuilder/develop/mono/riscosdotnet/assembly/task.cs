using System;
using System.Collections;
using riscos.Wimp;

namespace riscos
{
	public class Task
	{
		public UIntPtr Handle { get; set; }
		public bool Quit { get; set; }
	}

	public class WimpTask : Task
	{
		public static Hashtable AllWindows;

		public uint poll_mask { get; set; }

		public void Initialise (int version, string desc, int[] mess_list)
		{
			UIntPtr handle = UIntPtr.Zero;

			AllWindows = new Hashtable ();

			OS.ThrowOnError (NativeMethods.Wimp_Initialise (version, desc, mess_list, ref handle));

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
				OS.ThrowOnError (NativeMethods.Wimp_Poll (poll_mask, IntPtr.Zero, IntPtr.Zero));

				Event event_base = Event.GetEvent ();

				switch (event_base.type)
				{
				case PollCode.RedrawWindow:
					{
						RedrawWindowEvent event_full = (RedrawWindowEvent)event_base;
						Window event_window = (Window)WimpTask.AllWindows [event_full.RedrawArgs.RedrawWimpBlock.WindowHandle];
						event_window.OnRedraw (event_full);
					}
					break;
				case PollCode.OpenWindow:
					{
						OpenWindowEvent event_full = (OpenWindowEvent)event_base;
						Window event_window = (Window)WimpTask.AllWindows [event_full.OpenWindowArgs.OpenWimpBlock.WindowHandle];
						event_window.OnOpen (event_full);
					}
					break;
				case PollCode.CloseWindow:
					{
						CloseWindowEvent event_full = (CloseWindowEvent)event_base;
						Window event_window = (Window)WimpTask.AllWindows [event_full.CloseWindowArgs.CloseWimpBlock.WindowHandle];
						event_window.OnClose (event_full);
					}
					break;
				case PollCode.PointerLeaveWindow:
					{
						PointerLeaveEvent event_full = (PointerLeaveEvent)event_base;
						Window event_window = (Window)WimpTask.AllWindows [event_full.PointerLeaveArgs.PointerLeaveWimpBlock.WindowHandle];
						event_window.OnPointerLeave (event_full);
					}
					break;
				case PollCode.PointerEnterWindow:
					{
						PointerEnterEvent event_full = (PointerEnterEvent)event_base;
						Window event_window = (Window)WimpTask.AllWindows [event_full.PointerEnterArgs.PointerEnterWimpBlock.WindowHandle];
						event_window.OnPointerEnter (event_full);
					}
					break;
				case PollCode.MouseClick:
					{
						MouseClickEvent event_full = (MouseClickEvent)event_base;
						Window event_window = (Window)WimpTask.AllWindows [event_full.MouseClickArgs.MouseClickWimpBlock.WindowHandle];
						event_window.OnMouseClick (event_full);
					}
					break;
				case PollCode.KeyPressed:
					{
						KeyPressedEvent event_full = (KeyPressedEvent)event_base;
						Window event_window = (Window)WimpTask.AllWindows [event_full.KeyPressedArgs.KeyPressedWimpBlock.WindowHandle];
						event_window.OnKeyPressed (event_full);
					}
					break;
				case PollCode.ScrollRequest:
					{
						ScrollRequestEvent event_full = (ScrollRequestEvent)event_base;
						Window event_window = (Window)WimpTask.AllWindows [event_full.ScrollRequestArgs.ScrollRequestWimpBlock.WindowHandle];
						event_window.OnScrollRequest (event_full);
					}
					break;
				case PollCode.LoseCaret:
					{
						LoseCaretEvent event_full = (LoseCaretEvent)event_base;
						Window event_window = (Window)WimpTask.AllWindows [event_full.LoseCaretArgs.LoseCaretWimpBlock.WindowHandle];
						event_window.OnLoseCaret (event_full);
					}
					break;
				case PollCode.GainCaret:
					{
						GainCaretEvent event_full = (GainCaretEvent)event_base;
						Window event_window = (Window)WimpTask.AllWindows [event_full.GainCaretArgs.GainCaretWimpBlock.WindowHandle];
						event_window.OnGainCaret (event_full);
					}
					break;
				}
			}
		}
	}
}
