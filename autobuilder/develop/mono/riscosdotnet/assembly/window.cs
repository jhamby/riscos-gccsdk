//
// window.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Runtime.InteropServices;
using System.Collections;
using System.Text;
using riscos;

namespace riscos
{
	public static partial class Wimp
	{
		public class WindowHandle
		{
			public uint Handle { get; protected set; }

			public WindowHandle (uint handle)
			{
				this.Handle = handle;
			}

			public void GetState (ref NativeWimp.WindowStateBlock block)
			{
				block.WindowHandle = Handle;
				OS.ThrowOnError (NativeMethods.Wimp_GetWindowState (ref block));
			}

			public void GetNestedState (ref NativeWimp.WindowStateBlock block,
						    out uint parent,
						    out uint nested)
			{
				block.WindowHandle = Handle;
				OS.ThrowOnError (NativeMethods.Wimp_GetWindowStateAndNesting (ref block,
											      out parent,
											      out nested));
			}

			/*! \brief Find the origin of the window when its state is already known. */
			public OS.Coord GetOrigin (ref NativeOS.Rect visible,
						   ref NativeOS.Coord scroll)
			{
				return new OS.Coord (visible.MinX - scroll.X,
						     visible.MaxY - scroll.Y);
			}

			/*! \brief Find the origin of the window when its state is unknown. */
			public OS.Coord GetOrigin ()
			{
				NativeWimp.WindowStateBlock state = new NativeWimp.WindowStateBlock ();
				GetState (ref state);
				return GetOrigin (ref state.Visible, ref state.Scroll);
			}

			/*! \brief Convert a coordinate from screen to window when the window's origin
			 * is already known. */
			public void PointToWorkArea (OS.Coord point, OS.Coord origin)
			{
				point.X -= origin.X;
				point.Y -= origin.Y;
			}

			/*! \brief Convert a coordinate from screen to window when the window's origin
			 * is unknown. */
			public void PointToWorkArea (OS.Coord point)
			{
				OS.Coord origin = GetOrigin ();
				PointToWorkArea (point, origin);
			}

			/*! \brief Convert a coordinate from window to screen when the window's origin
			 * is already known. */
			public void PointToScreen (OS.Coord point, OS.Coord origin)
			{
				point.X += origin.X;
				point.Y += origin.Y;
			}

			/*! \brief Convert a coordinate from window to screen when the window's origin
			 * is unknown. */
			public void PointToScreen (OS.Coord point)
			{
				OS.Coord origin = GetOrigin ();
				PointToScreen (point, origin);
			}

			/*! \brief Convert a rectangle from screen to window when the window's origin
			 * is already known. */
			public void RectangleToWorkArea (OS.Rect rect, OS.Coord origin)
			{
				rect.MinX -= origin.X;
				rect.MinY -= origin.Y;
				rect.MaxX -= origin.X;
				rect.MaxY -= origin.Y;
			}

			/*! \brief Convert a rectangle from screen to window when the window's origin
			 * is unknown. */
			public void RectangleToWorkArea (OS.Rect rect)
			{
				OS.Coord origin = GetOrigin ();
				RectangleToWorkArea (rect, origin);
			}

			/*! \brief Convert a rectangle from window to screen when the window's origin
			 * is already known. */
			public void RectangleToScreen (OS.Rect rect, OS.Coord origin)
			{
				rect.MinX += origin.X;
				rect.MinY += origin.Y;
				rect.MaxX += origin.X;
				rect.MaxY += origin.Y;
			}

			/*! \brief Convert a rectangle from window to screen when the window's origin
			 * is unknown. */
			public void RectangleToScreen (OS.Rect rect)
			{
				OS.Coord origin = GetOrigin ();
				RectangleToScreen (rect, origin);
			}
		}

		/*! \class Window
		 * \brief Encapsulates a RISC OS WIMP window. */
		public class Window
		{
			public WindowHandle Handle;

			public IconBuffer Title;

			bool Created = false;

			// Cache the window extent, the WIMP never changes this unless the
			// user explicitly requests it.
			public OS.Rect Extent;

			public Hashtable MyIcons;

			public event RedrawEventHandler RedrawHandler;
			public event OpenEventHandler OpenHandler;
			public event CloseEventHandler CloseHandler;
			public event PointerLeaveEventHandler PointerLeaveHandler;
			public event PointerEnterEventHandler PointerEnterHandler;
			public event MouseClickEventHandler MouseClickHandler;
			public event KeyPressedEventHandler KeyPressedHandler;
			public event ScrollRequestEventHandler ScrollRequestHandler;
			public event LoseCaretEventHandler LoseCaretHandler;
			public event GainCaretEventHandler GainCaretHandler;

			public Window ()
			{
				MyIcons = new Hashtable ();
			}

			~Window ()
			{
				MyIcons.Clear ();
				WimpTask.AllWindows.Remove (Handle.Handle);
				Title.Clear ();
			}

			public void SetExtent (OS.Rect extent)
			{
				this.Extent = extent;
				if (Created)
				{
					NativeOS.Rect native_extent = new NativeOS.Rect (extent);
					OS.ThrowOnError (NativeMethods.Wimp_SetExtent (Handle.Handle, ref native_extent));
				}
			}

			/*! \brief Set the title of the window.
			 * The title buffer is big enough to hold the given string and terminator.
			 * Used for a title that will remain constant. */
			public void SetTitle (string title)
			{
				SetTitle (title, title.Length + 1);
			}

			/*! \brief Set the title of the window.
			 * The title buffer size is user defined so that it can be bigger than the given string.
			 * Used for a title that can be altered. */
			public void SetTitle (string title, int maxLen)
			{
				// Allow the size of the title buffer to be changed if the window has
				// not been created yet.
				if (Created == false)
					Title.Clear ();

				Title.Set (title, maxLen);

				// TODO: Redraw title bar.
			}

			public void Create (string title)
			{
				Create (title, title.Length + 1);
			}

			public void Create (string title, int titleMaxLen)
			{
				uint handle;

				Title = new IconBuffer (title, titleMaxLen);

				NativeWimp.WindowBlock block = new NativeWimp.WindowBlock (Title);

				block.WorkArea = new NativeOS.Rect (Extent);

				OS.ThrowOnError (NativeMethods.Wimp_CreateWindow (ref block, out handle));
				Handle = new WindowHandle (handle);
				WimpTask.AllWindows.Add (handle, this);
				Created = true;
			}

			/*! \brief Open a normal window (no nesting) */
			public void Open (OS.Rect visible,
					  OS.Coord scroll,
					  uint behind)
			{
				NativeWimp.WindowStateBlock block = 
					new NativeWimp.WindowStateBlock (Handle.Handle,
									 visible,
									 scroll,
									 behind,
									 0); // Flags are ignored on a normal window open
				OS.ThrowOnError (NativeMethods.Wimp_OpenWindow (ref block));
			}

			/*! \brief Open nested window. */
			public void OpenNested (OS.Rect visible,
						OS.Coord scroll,
						uint behindWindow,
						uint parentWindow,
						uint nestedFlags,
						uint windowFlags)
			{
				// Make sure bit 0 of nested flags is set correctly depending on whether
				// window flags are supplied.
				if (windowFlags == 0xffffffff)
				{
					nestedFlags &= (~1U);
					windowFlags = 0;
				}
				else
					nestedFlags |= 1;

				NativeWimp.WindowStateBlock block =
					new NativeWimp.WindowStateBlock(Handle.Handle,
									visible,
									scroll,
									behindWindow,
									windowFlags);
				OS.ThrowOnError (NativeMethods.Wimp_OpenWindowNested (ref block,
										      parentWindow,
										      nestedFlags));
			}

			/*! \brief Open nested window using the window flags it already has. */
			public void OpenNested (OS.Rect visible,
						OS.Coord scroll,
						uint behindWindow,
						uint parentWindow,
						uint nestedFlags)
			{
				OpenNested (visible,
					    scroll,
					    behindWindow,
					    parentWindow,
					    nestedFlags,
					    0xffffffff); // Use the window's existing flags.
			}

			public Icon CreateIcon (OS.Rect boundingBox,
						uint flags,
						IconData data,
						uint priority)
			{
				Icon icon = new Icon ();
				icon.Create (Handle.Handle, boundingBox, flags, data, priority);
				MyIcons.Add (icon.Handle, icon);

				return icon;
			}

			/*! \brief Open window in response to an event. */
			public virtual void OnOpen (OpenWindowEvent ev)
			{
				OS.ThrowOnError (NativeMethods.Wimp_OpenWindow (ref ev.OpenWindowArgs.OpenWimpBlock));
				if (OpenHandler != null)
					OpenHandler (this, ev.OpenWindowArgs);
			}

			/*! \brief Close window in response to an event. */
			public virtual void OnClose (CloseWindowEvent ev)
			{
				OS.ThrowOnError (NativeMethods.Wimp_CloseWindow (ref ev.CloseWindowArgs.CloseWimpBlock));
				if (CloseHandler != null)
					CloseHandler (this, ev.CloseWindowArgs);
			}

			public virtual void OnPointerLeave (PointerLeaveEvent ev)
			{
				if (PointerLeaveHandler != null)
					PointerLeaveHandler (this, ev.PointerLeaveArgs);
			}

			public virtual void OnPointerEnter (PointerEnterEvent ev)
			{
				if (PointerEnterHandler != null)
					PointerEnterHandler (this, ev.PointerEnterArgs);
			}

			public virtual void OnMouseClick (MouseClickEvent ev)
			{
				if (MouseClickHandler != null)
					MouseClickHandler (this, ev.MouseClickArgs);
			}

			public virtual void OnKeyPressed (KeyPressedEvent ev)
			{
				if (KeyPressedHandler != null)
					KeyPressedHandler (this, ev.KeyPressedArgs);
			}

			public virtual void OnLoseCaret (LoseCaretEvent ev)
			{
				if (LoseCaretHandler != null)
					LoseCaretHandler (this, ev.LoseCaretArgs);
			}

			public virtual void OnGainCaret (GainCaretEvent ev)
			{
				if (GainCaretHandler != null)
					GainCaretHandler (this, ev.GainCaretArgs);
			}

			public virtual void OnScrollRequest (ScrollRequestEvent ev)
			{
				if (ScrollRequestHandler != null)
					ScrollRequestHandler (this, ev.ScrollRequestArgs);
			}

			public virtual void OnRedraw (RedrawWindowEvent ev)
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
