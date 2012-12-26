//
// window.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Collections;
using System.Text;
using riscos;

namespace riscos
{
	public static partial class Wimp
	{
		/*! \class Window
		 * \brief Encapsulates a RISC OS WIMP window. */
		public class Window : IDisposable, IWimpWindow
		{
			static Dictionary<uint, Wimp.Window> AllWindows = new Dictionary<uint, Wimp.Window> ();

			WindowHandle WimpWindow;

			public IconData Title;

			bool disposed = false;

			public Dictionary<int, Icon> Icons = new Dictionary<int, Icon> ();

			public Window (WindowAttributes attr)
			{
				WimpWindow = new WindowHandle (Wimp.CreateWindow (attr));
				AllWindows.Add (WimpWindow.Handle, this);
				Title = attr.Title;
			}

			public Window (uint handle)
			{
				WimpWindow = new WindowHandle (handle);
				AllWindows.Add (WimpWindow.Handle, this);
				Title = null;
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
					Icons.Clear ();
					AllWindows.Remove (WimpWindow.Handle);
					Wimp.DeleteWindow (WimpWindow.Handle);
					Title.Text.Clear ();
					Title.Validation.Clear ();
					Title = null;
					disposed = true;
				}
			}

			/*! \brief Get the Wimp handle for this window.  */
			public uint WimpHandle
			{
				get { return WimpWindow.Handle; }
			}

			/*! \brief Return a summary of this window's state
			 * \param [in,out] block Reference to a native type to receive the window state.
			 * \return Nothing.  */
			public void GetState (ref NativeWimp.WindowStateBlock block)
			{
				WimpWindow.GetState (ref block);
			}

			/*! \brief Returns a summary of the given window's state and window nesting.
			 * \param [in,out] block Reference to a native type to receive the window state.
			 * \param [out] linkage Defines how the window is nested in the parent.
			 * \return The Wimp window handle of the parent.  */
			public uint GetNestedState (ref NativeWimp.WindowStateBlock block,
						    out uint linkage)
			{
				return WimpWindow.GetNestedState (ref block, out linkage);
			}

			/*! \brief Find the origin of the window when its state is already known. */
			public OS.Coord GetOrigin (ref NativeOS.Rect visible,
						   ref NativeOS.Coord scroll)
			{
				return WimpWindow.GetOrigin (ref visible, ref scroll);
			}

			/*! \brief Convert a coordinate from screen to window when the window's origin
			 * is already known.
			 * \param [in] point The coordinate to convert in screen coordinates.
			 * \param [in] origin The origin of the window in screen coordinates.
			 * \return The converted coordinate in window coordinates.
			 * \note The original coordinate is left unchanged. */
			public OS.Coord GetOrigin ()
			{
				return WimpWindow.GetOrigin ();
			}

			/*! \brief Convert a coordinate from screen to window when the window's origin
			 * is already known.
			 * \param [in] point The coordinate to convert in screen coordinates.
			 * \param [in] origin The origin of the window in screen coordinates.
			 * \return The converted coordinate in window coordinates.
			 * \note The original coordinate is left unchanged. */
			public OS.Coord PointToWorkArea (OS.Coord point, OS.Coord origin)
			{
				return WimpWindow.PointToWorkArea (point, origin);
			}

			/*! \brief Convert a rectangle from screen to window when the window's origin
			 * is unknown.
			 * \param [in] rect The rectangle to convert in screen coordinates.
			 * \return The converted rectangle in window coordinates.
			 * \note The original rectangle is left unchanged. */
			public OS.Coord PointToWorkArea (OS.Coord point)
			{
				return WimpWindow.PointToWorkArea (point);
			}

			/*! \brief Convert a coordinate from window to screen when the window's origin
			 * is already known.
			 * \param [in] point The coordinate to convert in window coordinates.
			 * \param [in] origin The origin of the window in screen coordinates.
			 * \return The converted coordinate in screen coordinates.
			 * \note The original coordinate is left unchanged. */
			public OS.Coord PointToScreen (OS.Coord point, OS.Coord origin)
			{
				return WimpWindow.PointToScreen (point, origin);
			}

			/*! \brief Convert a coordinate from window to screen when the window's origin
			 * is unknown.
			 * \param [in] point The coordinate to convert in window coordinates.
			 * \return The converted coordinate in screen coordinates.
			 * \note The original coordinate is left unchanged. */
			public OS.Coord PointToScreen (OS.Coord point)
			{
				return WimpWindow.PointToScreen (point);
			}

			/*! \brief Convert a rectangle from screen to window when the window's origin
			 * is already known.
			 * \param [in] rect The rectangle to convert in screen coordinates.
			 * \param [in] origin The origin of the window in screen coordinates.
			 * \return The converted rectangle in window coordinates.
			 * \note The original rectangle is left unchanged. */
			public OS.Rect RectangleToWorkArea (OS.Rect rect, OS.Coord origin)
			{
				return WimpWindow.RectangleToWorkArea (rect, origin);
			}

			/*! \brief Convert a rectangle from screen to window when the window's origin
			 * is unknown.
			 * \param [in] rect The rectangle to convert in screen coordinates.
			 * \return The converted rectangle in window coordinates.
			 * \note The original rectangle is left unchanged. */
			public OS.Rect RectangleToWorkArea (OS.Rect rect)
			{
				return WimpWindow.RectangleToWorkArea (rect);
			}

			/*! \brief Convert a rectangle from window to screen when the window's origin
			 * is already known.
			 * \param [in] rect The rectangle to convert in window coordinates.
			 * \param [in] origin The origin of the window in screen coordinates.
			 * \return The converted rectangle in screen coordinates.
			 * \note The original rectangle is left unchanged. */
			public OS.Rect RectangleToScreen (OS.Rect rect, OS.Coord origin)
			{
				return WimpWindow.RectangleToScreen (rect, origin);
			}

			/*! \brief Convert a rectangle from window to screen when the window's origin
			 * is unknown. 
			 * \param [in] rect The rectangle to convert in window coordinates.
			 * \return The converted rectangle in screen coordinates.
			 * \note The original rectangle is left unchanged. */
			public OS.Rect RectangleToScreen (OS.Rect rect)
			{
				return WimpWindow.RectangleToScreen (rect);
			}

			/*! \brief Forces an area of this window to be redrawn later.
			 * \param [in] minX Minimum X coordinate of rectangle to redraw.
			 * \param [in] minY Minimum Y coordinate of rectangle to redraw.
			 * \param [in] maxX Maximum X coordinate of rectangle to redraw.
			 * \param [in] maxY Maximum Y coordinate of rectangle to redraw.
			 * \return Nothing.  */
			public void ForceRedraw (int minX, int minY, int maxX, int maxY)
			{
				WimpWindow.ForceRedraw (minX, minY, maxX, maxY);
			}

			/*! \brief Forces an area of this window to be redrawn later.
			 * \param [in] redrawArea A rectangle defining the area of the window to redraw.
			 * \return Nothing.  */
			public void ForceRedraw (OS.Rect redrawArea)
			{
				WimpWindow.ForceRedraw (redrawArea);
			}

			/*! \brief Force the given item of window furniture to be redrawn.  */
			public void ForceRedrawFurniture (Furniture item)
			{
				WimpWindow.ForceRedrawFurniture (item);
			}

			/*! \brief Set the work area size of the window.  */
			public void SetExtent (OS.Rect extent)
			{
				var native_extent = new NativeOS.Rect (extent);
				Wimp.SetExtent (WimpWindow.Handle, ref native_extent);
			}

			/*! \brief Set the title of the window.  */
			public void SetTitle (string title)
			{
				Title.Text.Set (title);
				ForceRedrawTitle ();
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

			public Icon CreateIcon (IconAttributes attr)
			{
				Icon icon = new Icon (WimpWindow.Handle, attr);
				Icons.Add (icon.Handle, icon);

				return icon;
			}

			public Icon GetIcon (int handle)
			{
				if (handle < 0)
					return null;

				Icon icon;

				return Icons.TryGetValue (handle, out icon) ? icon : null;
			}

			/*! \brief Returns complete details of this window's state.
			 * \param [in,out] block Reference to a native type to receive window details.
			 * \note This method does \b not return details of any icons.  */
			public void GetInfo (ref NativeWimp.WindowInfoBlock block)
			{
				Wimp.GetWindowInfo (WimpWindow.Handle, ref block);
			}

			// The Toolbox already redraws the title bar automatically when it is updated,
			// so this is left as a plain Wimp window only method rather than including it
			//  in the WindowHandle class where Toolbox.Window would have access too.
			/*! \brief Force the title of this window to be redrawn.  */
			public void ForceRedrawTitle ()
			{
				Wimp.ForceRedrawTitle (WimpWindow.Handle);
			}

			/*! \brief Sets or gets the work area extent of this window.  */
			public OS.Rect Extent
			{
				set {
					SetExtent (value);
				}
				get {
					var info = new NativeWimp.WindowInfoBlock ();

					GetInfo (ref info);

					return new OS.Rect (ref info.WindowInfo.WorkArea);
				}
			}

			/*! \brief Gets the visible area of the window.  */
			public OS.Rect VisibleArea
			{
				get {
					return WimpWindow.GetVisibleArea ();
				}
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

			/*! \brief The event handlers that will be called when the window contents
			 * require updating.  */
			public event EventHandler<RedrawEventArgs> Paint;

			/*! \brief The event handlers that will be called when the window size
			 * and/or position is changed.  */
			public event EventHandler<OpenEventArgs> Opening;

			/*! \brief The event handlers that will be called when a window is about to
			 * be closed.  */
			public event EventHandler<CloseEventArgs> Closing;

			/*! \brief The event handlers that will be called after a window has been
			 * closed.  */
			public event EventHandler<CloseEventArgs> Closed;

			/*! \brief The event handlers that will be called when the pointer leaves
			 * the window.  */
			public event EventHandler<PointerLeaveEventArgs> PointerLeave;

			/*! \brief The event handlers that will be called when the pointer enters
			 * the window.  */
			public event EventHandler<PointerEnterEventArgs> PointerEnter;

			/*! \brief The event handlers that will be called when a mouse button is
			 * clicked within the window.  */
			public event EventHandler<MouseClickEventArgs> MouseClick;

			/*! \brief The event handlers that will be called when a key is pressed
			 * while the window has the input focus.  */
			public event EventHandler<KeyPressEventArgs> KeyPress;

			/*! \brief The event handlers that will be called when a user scroll request
			 * is received for this window.  */
			public event EventHandler<ScrollRequestEventArgs> ScrollRequest;

			/*! \brief The event handlers that will be called when the caret is moved
			 * away from this window.  */
			public event EventHandler<LoseCaretEventArgs> LoseCaret;

			/*! \brief The event handlers that will be called when the caret is moved
			 * to this window.  */
			public event EventHandler<GainCaretEventArgs> GainCaret;

			/*! \brief The event handlers that will be called when a Wimp Data Load message
			 * is received for this Wimp window.  */
			public event EventHandler<DataLoadMessageEventArgs> MsgDataLoad;

			public static Window GetInstance (uint handle)
			{
				if ((int)handle <= 0)
					return null;

				Window window;

				return AllWindows.TryGetValue (handle, out window) ? window : null;
			}

			/*! \brief Retrieve information about the pointer using method Wimp.GetPointerInfo.  */
			public class PointerInfo
			{
				/*! \brief Current position of the mouse pointer in OS units.  */
				public OS.Coord Pos;
				/*! \brief Current state of the mouse buttons. See \e PointerInfo.ButtonState. */
				public uint Buttons;
				/*! \brief Wimp window.  */
				public Wimp.Window Window;
				/*! \brief Wimp icon.  */
				public Wimp.Icon Icon = null;

				public uint WindowHandle;
				public int IconHandle;

				/*! \class ButtonState
				 * \brief Possible bit states for \e PointerInfo.Buttons  */
				public class ButtonState
				{
					public const uint Adjust = (1 << 0);
					public const uint Menu = (1 << 1);
					public const uint Select = (1 << 2);
				}

				public PointerInfo ()
				{
					Pos = new OS.Coord ();
					Update ();
				}

				public void Update ()
				{
					var block = new NativeWimp.PointerBlock ();

					Wimp.GetPointerInfo (out block);

					Pos.X = block.Pos.X;
					Pos.Y = block.Pos.Y;
					Buttons = block.Buttons;
					WindowHandle = block.WindowHandle;
					IconHandle = block.IconHandle;

					Window = GetInstance (WindowHandle);
					if (Window != null)
					{
						Icon = Window.GetIcon (IconHandle);
					}
				}
			}
		}
	}
}
