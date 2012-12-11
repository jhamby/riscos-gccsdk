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
		/*! \brief Encapsulate the data and methods shared by both a Toolbox Window
		 * and a plain Wimp window.  */
		public class WindowHandle
		{
			public readonly uint Handle;

			public WindowHandle (uint handle)
			{
				this.Handle = handle;
			}

			/*! \brief Return a summary of this window's state
			 * \param [in,out] block Reference to a native type to receive the window state.
			 * \return Nothing.  */
			public void GetState (ref NativeWimp.WindowStateBlock block)
			{
				block.WindowHandle = Handle;
				Wimp.GetWindowState (ref block);
			}

			/*! \brief Returns a summary of the given window's state and window nesting.
			 * \param [in,out] block Reference to a native type to receive the window state.
			 * \param [out] linkage Defines how the window is nested in the parent.
			 * \return The Wimp window handle of the parent.  */
			public uint GetNestedState (ref NativeWimp.WindowStateBlock block,
						    out uint linkage)
			{
				block.WindowHandle = Handle;
				return Wimp.GetNestedWindowState (ref block, out linkage);
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
		public class Window : IDisposable
		{
			public WindowHandle WimpWindow;

			public IconData Title;

			bool disposed = false;

			// Cache the window extent, the WIMP never changes this unless the
			// user explicitly requests it.
			public OS.Rect Extent;

			public Hashtable MyIcons;

			public Window (WindowAttributes attr)
			{
				MyIcons = new Hashtable ();

				WimpWindow = new WindowHandle (Wimp.CreateWindow (attr));
				WimpTask.AllWindows.Add (WimpWindow.Handle, this);
				Title = attr.Title;
			}

			~Window ()
			{
				Dispose (false);
			}

			public void Dispose ()
			{
				Dispose (true);
				// This object will be cleaned up by the Dispose method.
				// Call GC.SupressFinalize to take this object off the
				// finalization queue and prevent finalization code for
				// this object from executing a second time.
				GC.SuppressFinalize(this);
			}

			protected virtual void Dispose (bool disposing)
			{
				if (!this.disposed)
				{
					MyIcons.Clear ();
					WimpTask.AllWindows.Remove (WimpWindow.Handle);
					Wimp.DeleteWindow (WimpWindow.Handle);
					Title.Text.Clear ();
					Title.Validation.Clear ();
					Title = null;
					disposed = true;
				}
			}

			/*! \brief Set the work area size of the window.  */
			public void SetExtent (OS.Rect extent)
			{
				this.Extent = extent;
				var native_extent = new NativeOS.Rect (extent);
				Wimp.SetExtent (WimpWindow.Handle, ref native_extent);
			}

			/*! \brief Set the title of the window.  */
			public void SetTitle (string title)
			{
				Title.Text.Set (title);

				// TODO: Redraw title bar.
			}

			/*! \brief Open a normal window (no nesting) */
			public void Open (OS.Rect visible,
					  OS.Coord scroll,
					  uint behind)
			{
				var block = new NativeWimp.WindowStateBlock (WimpWindow.Handle,
									     visible,
									     scroll,
									     behind,
									     0); // Flags are ignored on a normal window open
				Wimp.OpenWindow (ref block);
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

				var block = new NativeWimp.WindowStateBlock(WimpWindow.Handle,
									    visible,
									    scroll,
									    behindWindow,
									    windowFlags);
				Wimp.OpenWindowNested (ref block, parentWindow, nestedFlags);
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
				icon.Create (WimpWindow.Handle, boundingBox, flags, data, priority);
				MyIcons.Add (icon.Handle, icon);

				return icon;
			}

			/*! \brief Returns complete details of this window's state.
			 * \note This method does \b not return details of any icons.  */
			public NativeWimp.WindowInfoBlock GetInfo ()
			{
				return Wimp.GetWindowInfo (WimpWindow.Handle);
			}

			/*! \brief Open window in response to an event. */
			protected virtual void OnOpen (OpenEventArgs e)
			{
				if (Opening != null)
					Opening (this, e);
				Wimp.OpenWindow (ref e.OpenWimpBlock);
			}

			/*! \brief Close window in response to an event. */
			protected virtual void OnClose (CloseEventArgs e)
			{
				if (Closing != null)
					Closing (this, e);

				Wimp.CloseWindow (e.CloseWimpBlock.WindowHandle);

				if (Closed != null)
					Closed (this, e);
			}

			protected virtual void OnPointerLeave (PointerLeaveEventArgs e)
			{
				if (PointerLeave != null)
					PointerLeave (this, e);
			}

			protected virtual void OnPointerEnter (PointerEnterEventArgs e)
			{
				if (PointerEnter != null)
					PointerEnter (this, e);
			}

			protected virtual void OnClick (MouseClickEventArgs e)
			{
				if (MouseClick != null)
					MouseClick (this, e);
			}

			protected virtual void OnKeyPress (KeyPressEventArgs e)
			{
				if (KeyPress != null)
					KeyPress (this, e);
			}

			protected virtual void OnLoseCaret (LoseCaretEventArgs e)
			{
				if (LoseCaret != null)
					LoseCaret (this, e);
			}

			protected virtual void OnGainCaret (GainCaretEventArgs e)
			{
				if (GainCaret != null)
					GainCaret (this, e);
			}

			protected virtual void OnScrollRequest (ScrollRequestEventArgs e)
			{
				if (ScrollRequest != null)
					ScrollRequest (this, e);
			}

			protected virtual void OnPaint (RedrawEventArgs e)
			{
				if (Paint != null)
					Paint (this, e);
			}

			protected virtual void OnRedraw (RedrawEventArgs e)
			{
				// Start the redraw. Given the window handle, the OS fills in RedrawWimpBlock
				// with details of what needs redrawing.
				int more = Wimp.RedrawWindow (ref e.RedrawWimpBlock);

				// The origin of the window only needs to be calculated once before entering
				// the redraw loop.
				e.Origin = WimpWindow.GetOrigin (ref e.RedrawWimpBlock.Visible,
								 ref e.RedrawWimpBlock.Scroll);
				while (more != 0)
				{
					OnPaint (e);
					more = Wimp.GetRectangle (ref e.RedrawWimpBlock);
				}
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
				case Wimp.MessageAction.DataLoad:
					OnMsgDataLoad ((Wimp.DataLoadMessageEventArgs) e);
					break;
				}
			}

			public virtual void Dispatch (Wimp.EventArgs e)
			{
				switch (e.Type)
				{
				case PollCode.RedrawWindow:
					OnRedraw ((RedrawEventArgs) e);
					break;
				case PollCode.OpenWindow:
					OnOpen ((OpenEventArgs) e);
					break;
				case PollCode.CloseWindow:
					OnClose ((CloseEventArgs) e);
					break;
				case PollCode.PointerLeaveWindow:
					OnPointerLeave ((PointerLeaveEventArgs) e);
					break;
				case PollCode.PointerEnterWindow:
					OnPointerEnter ((PointerEnterEventArgs) e);
					break;
				case PollCode.MouseClick:
					OnClick ((MouseClickEventArgs) e);
					break;
				case PollCode.KeyPressed:
					OnKeyPress ((KeyPressEventArgs) e);
					break;
				case PollCode.ScrollRequest:
					OnScrollRequest ((ScrollRequestEventArgs) e);
					break;
				case PollCode.LoseCaret:
					OnLoseCaret ((LoseCaretEventArgs) e);
					break;
				case PollCode.GainCaret:
					OnGainCaret ((GainCaretEventArgs) e);
					break;
				case PollCode.UserMessage:
				case PollCode.UserMessageRecorded:
					OnMessage ((Wimp.MessageEventArgs)e);
					break;
				}
			}

			public event RedrawEventHandler Paint;
			public event OpenEventHandler Opening;
			public event CloseEventHandler Closing;
			public event CloseEventHandler Closed;
			public event PointerLeaveEventHandler PointerLeave;
			public event PointerEnterEventHandler PointerEnter;
			public event MouseClickEventHandler MouseClick;
			public event KeyPressEventHandler KeyPress;
			public event ScrollRequestEventHandler ScrollRequest;
			public event LoseCaretEventHandler LoseCaret;
			public event GainCaretEventHandler GainCaret;

			/*! \brief The event handlers that will be called when a Wimp Data Load message is received.
			 * for this Wimp window.  */
			public event DataLoadMessageEventHandler MsgDataLoad;
		}
	}
}
