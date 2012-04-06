using System;
using System.Runtime.InteropServices;
using System.Collections;
using System.Text;
using riscos;
using riscos.Wimp;

namespace riscos
{
	namespace Wimp
	{
		public class Window
		{
			public uint Handle;

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
				WimpTask.AllWindows.Remove (Handle);
				Title.Clear ();
			}

			public void SetExtent (OS.Rect extent)
			{
				this.Extent = extent;
				if (Created)
				{
					NativeOS.Rect native_extent = new NativeOS.Rect (extent);
					OS.ThrowOnError (NativeMethods.Wimp_SetExtent (Handle, ref native_extent));
				}
			}

			// Set the title of the window. The title buffer is big enough to hold the
			// given string and terminator.
			// Used for a title that will remain constant.
			public void SetTitle (string title)
			{
				SetTitle (title, title.Length + 1);
			}

			// Set the title of the window. The title buffer size is user defined so that
			// it can be bigger than the given string.
			// Used for a title that can be altered.
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
				Title = new IconBuffer (title, titleMaxLen);

				WindowBlock block = new WindowBlock (Title);

				block.WorkAreaMinX = Extent.MinX;
				block.WorkAreaMinY = Extent.MinY;
				block.WorkAreaMaxX = Extent.MaxX;
				block.WorkAreaMaxY = Extent.MaxY;

				OS.ThrowOnError (NativeMethods.Wimp_CreateWindow (ref block, ref Handle));
				WimpTask.AllWindows.Add (Handle, this);
				Created = true;
			}

			// Open a normal window (no nesting)
			public void Open (OS.Rect visible,
					  OS.Coord scroll,
					  uint behind)
			{
				WindowStateBlock block = new WindowStateBlock (Handle,
									       visible,
									       scroll,
									       behind,
									       0); // Flags are ignored on a normal window open
				OS.ThrowOnError (NativeMethods.Wimp_OpenWindow (ref block));
			}

			// Open nested window.
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

				WindowStateBlock block = new WindowStateBlock(Handle,
									      visible,
									      scroll,
									      behindWindow,
									      windowFlags);
				OS.ThrowOnError (NativeMethods.Wimp_OpenNestedWindow (ref block,
										      parentWindow,
										      nestedFlags));
			}

			// Open nested window using the window flags it already has.
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

			public void GetState (ref WindowStateBlock block)
			{
				block.WindowHandle = Handle;
				OS.ThrowOnError (NativeMethods.Wimp_GetWindowState (ref block));
			}

			public void GetNestedState (ref WindowStateBlock block,
						    ref IntPtr parent, // Output - can be NULL
						    ref IntPtr nested) // Output - can be NULL
			{
				block.WindowHandle = Handle;
				OS.ThrowOnError (NativeMethods.Wimp_GetNestedWindowState (ref block,
											  ref parent,
											  ref nested));
			}

			public Icon CreateIcon (OS.Rect boundingBox,
						uint flags,
						IconData data,
						uint priority)
			{
				Icon icon = new Icon ();
				icon.Create (Handle, boundingBox, flags, data, priority);
				MyIcons.Add (icon.Handle, icon);

				return icon;
			}

			// Find the origin of the window when we already have its state.
			public OS.Coord GetOrigin (ref NativeOS.Rect visible,
						   ref NativeOS.Coord scroll)
			{
				return new OS.Coord (visible.MinX - scroll.X,
						     visible.MaxY - scroll.Y);
			}

			// Find the origin of the window when we don't have its state.
			public OS.Coord GetOrigin ()
			{
				WindowStateBlock state = new WindowStateBlock ();
				GetState (ref state);
				return GetOrigin (ref state.Visible, ref state.Scroll);
			}

			// Convert a coordinate from screen to window when we already know
			// the window's origin.
			public void PointToWorkArea (OS.Coord point, OS.Coord origin)
			{
				point.X -= origin.X;
				point.Y -= origin.Y;
			}

			// Convert a coordinate from screen to window when we don't know the
			// window's origin.
			public void PointToWorkArea (OS.Coord point)
			{
				OS.Coord origin = GetOrigin ();
				PointToWorkArea (point, origin);
			}

			// Convert a coordinate from window to screen when we already know
			// the window's origin.
			public void PointToScreen (OS.Coord point, OS.Coord origin)
			{
				point.X += origin.X;
				point.Y += origin.Y;
			}

			// Convert a coordinate from window to screen when we don't know the
			// window's origin.
			public void PointToScreen (OS.Coord point)
			{
				OS.Coord origin = GetOrigin ();
				PointToScreen (point, origin);
			}

			// Convert a rectangle from screen to window when we already know
			// the window's origin.
			public void RectangleToWorkArea (OS.Rect rect, OS.Coord origin)
			{
				rect.MinX -= origin.X;
				rect.MinY -= origin.Y;
				rect.MaxX -= origin.X;
				rect.MaxY -= origin.Y;
			}

			// Convert a rectangle from screen to window when we don't know the
			// window's origin.
			public void RectangleToWorkArea (OS.Rect rect)
			{
				OS.Coord origin = GetOrigin ();
				RectangleToWorkArea (rect, origin);
			}

			// Convert a rectangle from window to screen when we already know
			// the window's origin.
			public void RectangleToScreen (OS.Rect rect, OS.Coord origin)
			{
				rect.MinX += origin.X;
				rect.MinY += origin.Y;
				rect.MaxX += origin.X;
				rect.MaxY += origin.Y;
			}

			// Convert a rectangle from window to screen when we already know
			// the window's origin.
			public void RectangleToScreen (OS.Rect rect)
			{
				OS.Coord origin = GetOrigin ();
				RectangleToScreen (rect, origin);
			}

			// Open window in response to an event
			public virtual void OnOpen (OpenWindowEvent ev)
			{
				OS.ThrowOnError (NativeMethods.Wimp_OpenWindow (ref ev.OpenWindowArgs.OpenWimpBlock));
				if (OpenHandler != null)
					OpenHandler (this, ev.OpenWindowArgs);
			}

			// Close window in response to an event
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
				int more = 0;

				// Start the redraw. Given the window handle, the OS fills in RedrawWimpBlock
				// with details of what needs redrawing.
				NativeMethods.Wimp_RedrawWindow (ref ev.RedrawArgs.RedrawWimpBlock, ref more);

				// The origin of the window only needs to be calculated once before entering
				// the redraw loop.
				ev.RedrawArgs.Origin = GetOrigin (ref ev.RedrawArgs.RedrawWimpBlock.Visible,
								  ref ev.RedrawArgs.RedrawWimpBlock.Scroll);
				while (more != 0)
				{
					if (RedrawHandler != null)
						RedrawHandler (this, ev.RedrawArgs);
					NativeMethods.Wimp_GetRectangle (ref ev.RedrawArgs.RedrawWimpBlock, ref more);
				}
			}
		}
	}
}
