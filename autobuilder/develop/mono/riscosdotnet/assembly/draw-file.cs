//
// draw-file.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;

namespace riscos
{
	public static class Drawfile
	{
		[Flags]
		public enum RenderFlags
		{
			RenderBoundingBoxes = 1,
			SupressObjects = 2,
			GivenFlatness = 4
		}

		/*! \brief Call the Drawfile RISC OS module to render a draw file to screen.
		 * \param [in] flags Additional rendering options.
		 * \param [in] drawfile A byte array holding the draw file.
		 * \param [in] matrix A standard RISC OS matrix, that allows the draw file to
		 * be translated, rotated, scaled, etc.
		 * \param [in] clip A clipping rectangle to limit the parts of the draw file to
		 * be rendered. Usually used in a Wimp redraw loop. Set to \b null to render all
		 * the draw file.
		 * \param [in] flatness A value defining how smooth curves should be rendered.
		 * \return Nothing.
		 * \note There is no x,y coordinate defining the position to render the draw
		 * file. Instead, the matrix is used to translate the file to the correct
		 * position.
		 * \note The origin of the draw file (ie, point 0,0) is at its bottom left
		 * hand corner.
		 * \note The RISC OS call requires the size in bytes of the draw file. This is
		 * assumed to be the length of the byte array that contains it.  */
		public static void Render (RenderFlags flags,
					   byte [] drawfile,
					   OS.Matrix matrix,
					   OS.Rect clip,
					   int flatness)
		{
			OS.ThrowOnError (NativeMethods.Drawfile_Render (flags,
									drawfile,
									drawfile.Length,
									matrix,
									clip,
									flatness));
		}

		/*! \brief Call RISC OS Drawfile module to find the coordinates of the bounding box
		 * that encloses the draw file.
		 * \param [in] flags Currently unused and should be 0.
		 * \param [in] drawfile A byte array holding the draw file.
		 * \param [in] matrix A standard RISC OS matrix, that allows the draw file to
		 * be translated, rotated, scaled, etc before the bounding box is returned.
		 * \return A rectangle giving the bounding box coordinates.
		 * \note The RISC OS call requires the size in bytes of the draw file. This is
		 * assumed to be the length of the byte array that contains it.  */
		public static OS.Rect GetBounds (int flags,
						 byte [] drawfile,
						 OS.Matrix matrix)
		{
			var bounding_box = new OS.Rect ();

			OS.ThrowOnError (NativeMethods.Drawfile_GetBBox (flags,
									 drawfile,
									 drawfile.Length,
									 matrix,
									 bounding_box));
			return bounding_box;
		}
	}
}
