//
// toolbox/window.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Text;
using System.Runtime.InteropServices;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \class Window
		 * \brief Encapsulates a Toolbox window.  */
		public class Window : Object, Wimp.IWimpWindow
		{
			static class Method
			{
				public const int GetWimpHandle = 0;
				public const int AddGadget = 1;
				public const int RemoveGadget = 2;
				public const int SetMenu = 3;
				public const int GetMenu = 4;
				public const int SetPointer = 5;
				public const int GetPointer = 6;
				public const int SetHelpMessage = 7;
				public const int GetHelpMessage = 8;
				public const int AddKeyboardShortcuts = 9;
				public const int RemoveKeyboardShortcuts = 10;
				public const int SetTitle = 11;
				public const int GetTitle = 12;
				public const int SetDefaultFocus = 13;
				public const int GetDefaultFocus = 14;
				public const int SetExtent = 15;
				public const int GetExtent = 16;
				public const int ForceRedraw = 17;
				public const int SetToolBars = 18;
				public const int GetToolBars = 19;
			}

			/*! \brief %Flags used in a %Window %Toolbox template.  */
			public static class Flags
			{
				public const int GenerateAboutToBeShown = (1 << 0);
				public const int AutoOpenWindow = (1 << 1);
				public const int AutoCloseWindow = (1 << 2);
				public const int GenerateHasBeenHidden = (1 << 3);
				public const int ToolbarTemplate = (1 << 4);
			}

			public enum Toolbar
			{
				InternalBottomLeft = 1,
				InternalTopLeft = 2,
				ExternalBottomLeft = 4,
				ExternalTopLeft = 8
			}

			/*! \class EventCode
			 * \brief Default Toolbox events that can be generated by the %Toolbox %Window object.  */
			public static class EventCode
			{
				public const int AboutToBeShown = 0x82880;
				public const int HasBeenHidden = 0x82890;
			}

			private static class TemplateOffset
			{
				public const int Flags = 36;
				public const int HelpMessage = 40;
				public const int MaxHelp = 44;
				public const int PointerShape = 48;
				public const int MaxPointerShape = 52;
				public const int PointerXHot = 56;
				public const int PointerYHot = 60;
				public const int Menu = 64;
				public const int NumKeyboardShortcuts = 68;
				public const int KeyboardShortCuts = 72;
				public const int NumGadgets = 76;
				public const int Gadgets = 80;
				public const int DefaultFocus = 84;
				public const int ShowEvent = 88;
				public const int HideEvent = 92;
				public const int InternalBL = 96;
				public const int InternalTL = 100;
				public const int ExternalBL = 104;
				public const int ExternalTL = 108;
				public const int Window = 112;
				public const int Data = 200;
			}

			/*! \brief Used when setting the \e DefaultFocus property of the Window to
			 * remove the default focus.  */
			public const uint DefaultFocusNone = 0xffffffff;
			/*! \brief Used when setting the \e DefaultFocus property of the Window to
			 * make the Window work area the default focus.  */
			public const uint DefaultFocusWindow = 0xfffffffe;

			/*! \brief The WIMP handle for this toolbox window.  */
			Wimp.WindowHandle WimpWindow;

			private int AboutToBeShownEventCode = 0;
			private int HasBeenHiddenEventCode = 0;

			/*! \brief Create a toolbox window from the named template in the
			 * Resource file.
			 * \param[in] resName The name of the window template to use.  */
			public Window (string resName) : base (resName)
			{
				WimpWindow = new Wimp.WindowHandle (WimpHandle);

				IntPtr template = Toolbox.TemplateLookup (resName);
				RetrieveEventCodes (template);
			}

			/*! \brief Create a toolbox window from the template data given.
			 * \param[in] templateData Pointer to the window template.  */
			public Window (IntPtr templateData)
			{
				Create (templateData);
				WimpWindow = new Wimp.WindowHandle (WimpHandle);
				RetrieveEventCodes (templateData);
			}

			/*! \brief Create a Toolbox Window from an object that already exists.
			 * \param [in] objectID The Toolbox ID of the existing object.  */
			public Window (uint objectID) : base (objectID)
			{
				WimpWindow = new Wimp.WindowHandle (WimpHandle);
				IntPtr template = Toolbox.TemplateLookup (TemplateName);
				RetrieveEventCodes (template);
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
				return PointToWorkArea (point, origin);
			}

			/*! \brief Convert a rectangle from screen to window when the window's origin
			 * is unknown.
			 * \param [in] rect The rectangle to convert in screen coordinates.
			 * \return The converted rectangle in window coordinates.
			 * \note The original rectangle is left unchanged. */
			public OS.Coord PointToWorkArea (OS.Coord point)
			{
				return PointToWorkArea (point);
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
				ForceRedraw (new OS.Rect (minX, minY, maxX, maxY));
			}

			/*! \brief Force a redraw on the area of the window given.
			 * \param [in] area The rectangular area of the window to redraw.
			 * \return Nothing.  */
			public void ForceRedraw (OS.Rect area)
			{
				var rect = new NativeOS.Rect (area);
				OS.ThrowOnError (NativeMethods.Window_ForceRedraw (0,
										   ID,
										   Method.ForceRedraw,
										   ref rect));
			}

			/*! \brief Force the given item of window furniture to be redrawn.  */
			public void ForceRedrawFurniture (Wimp.Furniture item)
			{
				WimpWindow.ForceRedrawFurniture (item);
			}

			/*! \brief Attach toolbars to this Window object. If the object is showing then
			 * the new toolbars will be shown.
			 * \param [in] intBottomLeft Internal Bottom Left toolbar.
			 * \param [in] intTopLeft Internal Top Left toolbar.
			 * \param [in] extBottomLeft External Bottom Left toolbar.
			 * \param [in] extTopLeft External Top Left toolbar.
			 * \return Nothing.
			 * \note Setting a toolbar to \e null means that there is no toolbar of that type. */
			public void SetToolBars (Window intBottomLeft,
						 Window intTopLeft,
						 Window extBottomLeft,
						 Window extTopLeft)
			{
				uint mask = 0;
				uint ibl = 0, itl = 0, ebl = 0, etl = 0;

				if (intBottomLeft != null)
				{
					mask |= (uint)Toolbar.InternalBottomLeft;
					ibl = intBottomLeft.ID;
				}
				if (intTopLeft != null)
				{
					mask |= (uint)Toolbar.InternalTopLeft;
					itl = intTopLeft.ID;
				}
				if (extBottomLeft != null)
				{
					mask |= (uint)Toolbar.ExternalBottomLeft;
					ebl = extBottomLeft.ID;
				}
				if (extTopLeft != null)
				{
					mask |= (uint)Toolbar.ExternalTopLeft;
					etl = extTopLeft.ID;
				}

				OS.ThrowOnError (NativeMethods.Window_SetToolBars (mask,
										   ID,
										   ibl,
										   itl,
										   ebl,
										   etl));
			}

			/*! \brief Return the requested toolbar Window.
			 * \param [in] toolbar The code of the required toolbar.
			 * \return A window object representing the toolbar or null if there is no
			 * attached toolbar.  */
			Window GetToolBar (Toolbar toolbar)
			{
				uint tb_obj_id = 0;
				uint dummy_obj_id; // For the IDs we're not interested in

				switch (toolbar)
				{
				case Toolbar.InternalBottomLeft:
					OS.ThrowOnError (NativeMethods.Window_GetToolBars ((uint)toolbar,
											   ID,
											   out tb_obj_id,
											   out dummy_obj_id,
											   out dummy_obj_id,
											   out dummy_obj_id));
					break;
				case Toolbar.InternalTopLeft:
					OS.ThrowOnError (NativeMethods.Window_GetToolBars ((uint)toolbar,
											   ID,
											   out dummy_obj_id,
											   out tb_obj_id,
											   out dummy_obj_id,
											   out dummy_obj_id));
					break;
				case Toolbar.ExternalBottomLeft:
					OS.ThrowOnError (NativeMethods.Window_GetToolBars ((uint)toolbar,
											   ID,
											   out dummy_obj_id,
											   out dummy_obj_id,
											   out tb_obj_id,
											   out dummy_obj_id));
					break;
				case Toolbar.ExternalTopLeft:
					OS.ThrowOnError (NativeMethods.Window_GetToolBars ((uint)toolbar,
											   ID,
											   out dummy_obj_id,
											   out dummy_obj_id,
											   out dummy_obj_id,
											   out tb_obj_id));
					break;
				}

				return Object.CreateInstance<Window> (tb_obj_id);
			}

			/*! \brief The title of the window.  */
			public string Title
			{
				get { return GetText (Method.GetTitle); }
				set
				{
					if (value == null)
						throw new ArgumentNullException ("value", "Attempted to set Window title to null");
					SetText (0, Method.SetTitle, value);
				}
			}

			/*! \brief The WIMP handle of the Toolbox window.  */
			public uint WimpHandle
			{
				get { return MiscOp_GetR0 (0, Method.GetWimpHandle); }
			}

			/*! \brief The Menu which will be displayed when the Menu button is pressed over
			 * this Window object.
			 * \exception UnknownObjectException Thrown if the toolbox object ID is unknown when
			 * reading the property.
			 * \note Set to null to detach the Menu from the Window.  */
			public Toolbox.Menu Menu
			{
				set { SetMenu (Method.SetMenu, value); }
				get { return GetMenu (Method.GetMenu); }
			}

			/*! \brief The Pointer shape which will be used when the pointer enters this
			 * Window object.
			 * \note Set to null to detach the Pointer for this Window.  */
			public PointerShape Pointer
			{
				set
				{
					OS.ThrowOnError (NativeMethods.Window_SetPointer (0,
											  ID,
											  value.SpriteName,
											  value.xHotSpot,
											  value.yHotSpot));
				}
				get
				{
					int buffer_size, x, y;
					OS.ThrowOnError (NativeMethods.Window_GetPointer (0,
											  ID,
											  null,
											  0,
											  out buffer_size,
											  out x,
											  out y));
					StringBuilder buffer = new StringBuilder (buffer_size);
					OS.ThrowOnError (NativeMethods.Window_GetPointer (0,
											  ID,
											  buffer,
											  buffer_size,
											  out buffer_size,
											  out x,
											  out y));
					return new Window.PointerShape (buffer.ToString(), x, y);
				}
			}

			/*! \brief The help message which will be returned when a Help Request message is
			 * received for this Window object.
			 * \note Set to null to remove the Help Message from this Window.  */
			public string HelpMessage
			{
				set { SetText (0, Method.SetHelpMessage, value); }
				get { return GetText (Method.GetHelpMessage); }
			}

			/*! \brief The default focus component for this window.
			 * \note Set to \e DefaultFocusNone to remove or \e DefaultFocusWindow to make the Window
			 * itself the default focus.<br>
			 * Setting takes effect when next shown.  */
			public uint DefaultFocus
			{
				set { MiscOp_SetR3 (0, Method.SetDefaultFocus, value); }
				get { return MiscOp_GetR0 (0, Method.GetDefaultFocus); }
			}

			/*! \brief Gets or sets the extent of the underlying WIMP Window.  */
			public OS.Rect Extent
			{
				set {
					OS.ThrowOnError (NativeMethods.Window_SetExtent (0,
											 ID,
											 Method.SetExtent,
											 new NativeOS.Rect (value)));
				}
				get {
					NativeOS.Rect extent = new NativeOS.Rect ();
					OS.ThrowOnError (NativeMethods.Window_GetExtent (0,
											 ID,
											 Method.GetExtent,
											 out extent));
					return new OS.Rect (ref extent);
				}
			}

			/*! \brief Gets the visible area of the window.  */
			public OS.Rect VisibleArea
			{
				get {
					return WimpWindow.GetVisibleArea ();
				}
			}

			/*! \brief Return information about the state of the pointer.
			 * \param [out] pointer Object used to return pointer information.
			 * \return The Toolbox Window object currently under the pointer or
			 * null if not over a known toolbox window.  */
			public static Window GetPointerInfo (PointerInfo pointer)
			{
				OS.ThrowOnError (NativeMethods.Window_GetPointerInfo (0,
										      out pointer.Pos.X,
										      out pointer.Pos.Y,
										      out pointer.Buttons,
										      out pointer.ObjectID,
										      out pointer.CmpID));
				if ((pointer.Buttons & PointerInfo.ButtonState.NotToolboxWindow) != 0)
					return null;

				return Object.CreateInstance<Window> (pointer.ObjectID);
			}

			/*! \brief Return the Window object and component ID that contains the
			 * specified icon.
			 * \param [in] windowHandle The handle of the WIMP window.
			 * \param [in] iconHandle The handle of the WIMP icon
			 * \param [out] cmpID The Toolbox ID of the component that contains the icon.
			 * \return The Window object that contains the icon or null if unknown.  */ 
			static public Window WimpToToolbox (uint windowHandle,
							    uint iconHandle,
							    out uint cmpID)
			{
				uint tb_obj_id;

				OS.ThrowOnError (NativeMethods.Window_WimpToToolbox (0,
										     windowHandle,
										     iconHandle,
										     out tb_obj_id,
										     out cmpID));
				return Object.CreateInstance<Window> (tb_obj_id);
			}
/*
			TODO:

			void AddKeyboardShortcuts();
			void RemoveKeyboardShortcuts();
			void ExtractGadgetInfo();
*/
			protected virtual void OnPaint (Wimp.RedrawEventArgs e)
			{
				if (Paint != null)
					Paint (this, e);
			}

			protected virtual void OnRedraw (Wimp.RedrawEventArgs e)
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

			public override void Dispatch (Wimp.EventArgs e)
			{
				switch (e.Type)
				{
				case Wimp.PollCode.RedrawWindow:
					OnRedraw ((Wimp.RedrawEventArgs)e);
					break;
				case Wimp.PollCode.UserMessage:
				case Wimp.PollCode.UserMessageRecorded:
					OnMessage ((Wimp.MessageEventArgs)e);
					break;
				}
			}

			/*! \brief Raising an event invokes the event handler through a delegate.
			 *
			 * The \b OnAboutToBeShown method also allows derived classes to handle the
			 * event without attaching a delegate. This is the preferred technique for
			 * handling the event in a derived class.
			 * \note  When overriding \b OnAboutToBeShown in a derived class, be sure to
			 * call the base class's \b OnAboutToBeShown method so that registered delegates
			 * receive the event.  */
			protected virtual void OnAboutToBeShown (AboutToBeShownEventArgs e)
			{
				if (AboutToBeShown != null)
					AboutToBeShown (this, e);
			}

			/*! \brief Raising an event invokes the event handler through a delegate.
			 *
			 * The \b OnHasBeenHidden method also allows derived classes to handle the
			 * event without attaching a delegate. This is the preferred technique for
			 * handling the event in a derived class.
			 * \note  When overriding \b OnHasBeenHidden in a derived class, be sure to
			 * call the base class's \b OnHasBeenHidden method so that registered delegates
			 * receive the event.  */
			protected virtual void OnHasBeenHidden (ToolboxEventArgs e)
			{
				if (HasBeenHidden != null)
					HasBeenHidden (this, e);
			}

			public override void Dispatch (ToolboxEventArgs e)
			{
				if (e.Header.EventCode == AboutToBeShownEventCode) {
					OnAboutToBeShown (new AboutToBeShownEventArgs (e.RawEventData));
				}
				else if (e.Header.EventCode == HasBeenHiddenEventCode) {
					OnHasBeenHidden (e);
				}
				else
					base.Dispatch (e);
			}

			private void RetrieveEventCodes (IntPtr template)
			{
				int flags = Marshal.ReadInt32 (template, TemplateOffset.Flags);
				if ((flags & Flags.GenerateAboutToBeShown) != 0)
				{
					var show_event = Marshal.ReadInt32 (template,
									    TemplateOffset.ShowEvent);
					AboutToBeShownEventCode = (show_event != 0) ?
								   show_event :
								   EventCode.AboutToBeShown;
				}
				if ((flags & Flags.GenerateHasBeenHidden) != 0)
				{
					var hide_event = Marshal.ReadInt32 (template,
									    TemplateOffset.HideEvent);
					HasBeenHiddenEventCode = (hide_event != 0) ?
								  hide_event :
								  EventCode.HasBeenHidden;
				}
			}

			/*! \brief The event handlers that will be called just before this window is shown.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, AboutToBeShownEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * WindowObject.AboutToBeShown += handler_name;
			 * \endcode  */
			public event EventHandler<AboutToBeShownEventArgs> AboutToBeShown;

			/*! \brief The event handlers that will be called when this window has been hidden.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, ToolboxEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * WindowObject.HasBeenHidden += handler_name;
			 * \endcode  */
			public event EventHandler<ToolboxEventArgs> HasBeenHidden;

			/*! \brief The event handlers that will be called when a Wimp Data Load message is
			 * received for this Wimp window.  */
			public event EventHandler<Wimp.DataLoadMessageEventArgs> MsgDataLoad;

			/*! \brief The event handlers that will be called to redraw this window.
			 *
			 * The necessary WIMP calls will be made for you, these handlers are called
			 * from within a redraw loop. The origin of the window in OS units is passed
			 * in the event's arguments.  */
			public event EventHandler<Wimp.RedrawEventArgs> Paint;

			/*! \class PointerShape
			 * \brief Used to set/read the Pointer property of the Window.  */
			public class PointerShape
			{
				/*! \brief The sprite name of the pointer shape.  */
				public string SpriteName;
				/*! \brief The x coordinate of the active point in the sprite.  */
				public int xHotSpot;
				/*! \brief The y coordinate of the active point in the sprite.  */
				public int yHotSpot;

				public PointerShape (string spriteName, int x, int y)
				{
					SpriteName = spriteName;
					xHotSpot = x;
					yHotSpot = y;
				}
			}

			/*! \class PointerInfo
			 * \brief Used as a buffer to retrieve information about the pointer using
			 * method Window.GetPointerInfo.  */
			public class PointerInfo
			{
				/*! \brief Current position of the mouse pointer in OS units.  */
				public OS.Coord Pos;
				/*! \brief Current state of the mouse buttons. See \e PointerInfo.ButtonState. */
				public uint Buttons;
				/*! \brief Window ID, or WIMP window handle if \e Buttons
				 * bit \e NotToolboxWindow is set.  */
				public uint ObjectID;
				/*! \brief Component ID, or WIMP icon handle if \e Buttons
				 * bit \e NotToolboxWindow is set.  */
				public uint CmpID;

				/*! \class ButtonState
				 * \brief Possible bit states for \e PointerInfo.Buttons  */
				public class ButtonState
				{
					public const uint Adjust = (1 << 0);
					public const uint Menu = (1 << 1);
					public const uint Select = (1 << 2);
					public const uint NotToolboxWindow = (1 << 8);
				}
			}
		}
	}
}
