//
// window-common.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

namespace riscos
{
	public static partial class Wimp
	{
		public interface IWimpWindow
		{
			uint WimpHandle { get; }

			void GetState (ref NativeWimp.WindowStateBlock block);

			uint GetNestedState (ref NativeWimp.WindowStateBlock block,
					     out uint linkage);

			OS.Coord GetOrigin (ref NativeOS.Rect visible,
					    ref NativeOS.Coord scroll);

			OS.Coord GetOrigin ();

			OS.Coord PointToWorkArea (OS.Coord point, OS.Coord origin);

			OS.Coord PointToWorkArea (OS.Coord point);

			OS.Coord PointToScreen (OS.Coord point, OS.Coord origin);

			OS.Coord PointToScreen (OS.Coord point);

			OS.Rect RectangleToWorkArea (OS.Rect rect, OS.Coord origin);

			OS.Rect RectangleToWorkArea (OS.Rect rect);

			OS.Rect RectangleToScreen (OS.Rect rect, OS.Coord origin);

			OS.Rect RectangleToScreen (OS.Rect rect);

			void ForceRedraw (int minX, int minY, int maxX, int maxY);

			void ForceRedraw (OS.Rect redrawArea);

			void ForceRedrawFurniture (Furniture item);

			void SetCaretPosition (OS.Coord Pos, int height, int index);

			OS.Rect VisibleArea { get; }

			OS.Rect Extent { set; get; }
		}

		/*! \brief Encapsulate the data and methods shared by both a Toolbox Window
		 * and a plain Wimp window.  */
		public class WindowHandle
		{
			public readonly uint Handle;

			public WindowHandle (uint handle)
			{
				Handle = handle;
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
			 * is already known.
			 * \param [in] point The coordinate to convert in screen coordinates.
			 * \param [in] origin The origin of the window in screen coordinates.
			 * \return The converted coordinate in window coordinates.
			 * \note The original coordinate is left unchanged. */
			public OS.Coord PointToWorkArea (OS.Coord point, OS.Coord origin)
			{
				return new OS.Coord (point.X - origin.X,
						     point.Y - origin.Y);
			}

			/*! \brief Convert a coordinate from screen to window when the window's origin
			 * is unknown.
			 * \param [in] point The coordinate to convert in screen coordinates.
			 * \return The converted coordinate in window coordinates.
			 * \note The original coordinate is left unchanged. */
			public OS.Coord PointToWorkArea (OS.Coord point)
			{
				return PointToWorkArea (point, GetOrigin ());
			}

			/*! \brief Convert a coordinate from window to screen when the window's origin
			 * is already known.
			 * \param [in] point The coordinate to convert in window coordinates.
			 * \param [in] origin The origin of the window in screen coordinates.
			 * \return The converted coordinate in screen coordinates.
			 * \note The original coordinate is left unchanged. */
			public OS.Coord PointToScreen (OS.Coord point, OS.Coord origin)
			{
				return new OS.Coord (point.X + origin.X,
						     point.Y + origin.Y);
			}

			/*! \brief Convert a coordinate from window to screen when the window's origin
			 * is unknown.
			 * \param [in] point The coordinate to convert in window coordinates.
			 * \return The converted coordinate in screen coordinates.
			 * \note The original coordinate is left unchanged. */
			public OS.Coord PointToScreen (OS.Coord point)
			{
				return PointToScreen (point, GetOrigin ());
			}

			/*! \brief Convert a rectangle from screen to window when the window's origin
			 * is already known.
			 * \param [in] rect The rectangle to convert in screen coordinates.
			 * \param [in] origin The origin of the window in screen coordinates.
			 * \return The converted rectangle in window coordinates.
			 * \note The original rectangle is left unchanged. */
			public OS.Rect RectangleToWorkArea (OS.Rect rect, OS.Coord origin)
			{
				return new OS.Rect (rect.MinX - origin.X,
						    rect.MinY - origin.Y,
						    rect.MaxX - origin.X,
						    rect.MaxY - origin.Y);
			}

			/*! \brief Convert a rectangle from screen to window when the window's origin
			 * is unknown.
			 * \param [in] rect The rectangle to convert in screen coordinates.
			 * \return The converted rectangle in window coordinates.
			 * \note The original rectangle is left unchanged. */
			public OS.Rect RectangleToWorkArea (OS.Rect rect)
			{
				return RectangleToWorkArea (rect, GetOrigin ());
			}

			/*! \brief Convert a rectangle from window to screen when the window's origin
			 * is already known.
			 * \param [in] rect The rectangle to convert in window coordinates.
			 * \param [in] origin The origin of the window in screen coordinates.
			 * \return The converted rectangle in screen coordinates.
			 * \note The original rectangle is left unchanged. */
			public OS.Rect RectangleToScreen (OS.Rect rect, OS.Coord origin)
			{
				return new OS.Rect (rect.MinX + origin.X,
						    rect.MinY + origin.Y,
						    rect.MaxX + origin.X,
						    rect.MaxY + origin.Y);
			}

			/*! \brief Convert a rectangle from window to screen when the window's origin
			 * is unknown. 
			 * \param [in] rect The rectangle to convert in window coordinates.
			 * \return The converted rectangle in screen coordinates.
			 * \note The original rectangle is left unchanged. */
			public OS.Rect RectangleToScreen (OS.Rect rect)
			{
				return RectangleToScreen (rect, GetOrigin ());
			}

			/*! \brief Forces an area of this window to be redrawn later.
			 * \param [in] minX Minimum X coordinate of rectangle to redraw.
			 * \param [in] minY Minimum Y coordinate of rectangle to redraw.
			 * \param [in] maxX Maximum X coordinate of rectangle to redraw.
			 * \param [in] maxY Maximum Y coordinate of rectangle to redraw.
			 * \return Nothing.  */
			public void ForceRedraw (int minX, int minY, int maxX, int maxY)
			{
				Wimp.ForceRedraw (Handle, minX, minY, maxX, maxY);
			}

			/*! \brief Forces an area of this window to be redrawn later.
			 * \param [in] redrawArea A rectangle defining the area of the window to redraw.
			 * \return Nothing.  */
			public void ForceRedraw (OS.Rect redrawArea)
			{
				Wimp.ForceRedraw (Handle, redrawArea.MinX, redrawArea.MinY,
							  redrawArea.MaxX, redrawArea.MaxY);
			}

			/*! \brief Force the given item of window furniture to be redrawn.  */
			public void ForceRedrawFurniture (Furniture item)
			{
				Wimp.ForceRedrawFurniture (Handle, item);
			}

			/*! \brief Move the caret to the given position in this window.  */
			public void SetCaretPosition (OS.Coord Pos, int height, int index)
			{
				Wimp.SetCaretPosition (Handle, -1, Pos.X, Pos.Y, height, index);
			}

			/*! \brief Gets the visible area of this window in screen coordinates.  */
			public OS.Rect GetVisibleArea ()
			{
				var state = new NativeWimp.WindowStateBlock ();

				GetState (ref state);

				return new OS.Rect (ref state.Visible);
			}
		}
	}
}
