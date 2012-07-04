//
// toolbox/window.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Text;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \class Window
		 * \brief Encapsulates a Toolbox window.  */
		public class Window : Object
		{
			/*! \brief The WIMP handle for this toolbox window.  */
			Wimp.WindowHandle Handle;

			/*! \brief The title of the window.  */
			public string Title
			{
				get
				{
					// Find out how big the buffer needs to be.
					int buffer_size;
					OS.ThrowOnError (NativeMethods.Window_GetTitle (0,
											ID,
											null,
											0,
											out buffer_size));

					// A StringBuilder can be initialised to the right size ready
					// to receive the title text.
					StringBuilder buffer = new StringBuilder (buffer_size);
					OS.ThrowOnError (NativeMethods.Window_GetTitle (0,
											ID,
											buffer,
											buffer_size,
											out buffer_size));

					return buffer.ToString ();
				}
				set
				{
					if (value != null)
						OS.ThrowOnError (NativeMethods.Window_SetTitle (0, ID, value));
				}
			}

			public event Wimp.RedrawEventHandler RedrawHandler;

			/*! \brief Create a toolbox window from the named template in the
			 * Resource file.
			 * \param[in] resName The name of the window template to use.  */
			public Window (string resName)
			{
				Create (resName);
				Handle = new Wimp.WindowHandle (GetWimpHandle ());
			}

			/*! \brief Create a toolbox window from the template data given.
			 * \param[in] templateData Pointer to the window template.  */
			public Window (IntPtr templateData)
			{
				Create (templateData);
				Handle = new Wimp.WindowHandle (GetWimpHandle ());
			}

			/*! \brief Get the WIMP handle of the window from the toolbox.
			 * \return The WIMP window handle.  */
			public uint GetWimpHandle ()
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
