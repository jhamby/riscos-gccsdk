//
// toolbox/window.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;

namespace riscos
{
	public static partial class Toolbox
	{
		public class Window : Object
		{
			Wimp.WindowHandle Handle;

			public event Wimp.RedrawEventHandler RedrawHandler;

			public Window (string resName)
			{
				Create (resName);
				Handle = new Wimp.WindowHandle (GetWimpHandle ());
			}

			public Window (IntPtr templateData)
			{
				Create (templateData);
				Handle = new Wimp.WindowHandle (GetWimpHandle ());
			}

			public virtual uint GetWimpHandle ()
			{
				uint wimp_handle;

				OS.ThrowOnError (NativeMethods.Window_GetWimpHandle (0, ID, out wimp_handle));

				return wimp_handle;
			}

			public virtual void OnRedraw (Wimp.RedrawWindowEvent ev)
			{
				int more;

				// Start the redraw. Given the window handle, the OS fills in RedrawWimpBlock
				// with details of what needs redrawing.
				NativeMethods.Wimp_RedrawWindow (ref ev.RedrawArgs.RedrawWimpBlock, out more);

				// The origin of the window only needs to be calculated once before entering
				// the redraw loop.
				ev.RedrawArgs.Origin = Handle.GetOrigin (ref ev.RedrawArgs.RedrawWimpBlock.Visible,
									 ref ev.RedrawArgs.RedrawWimpBlock.Scroll);
				while (more != 0)
				{
					if (RedrawHandler != null)
						RedrawHandler (this, ev.RedrawArgs);
					NativeMethods.Wimp_GetRectangle (ref ev.RedrawArgs.RedrawWimpBlock, out more);
				}
			}
		}
	}
}
