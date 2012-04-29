//
// font.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Text;

namespace riscos
{
	public static class Font
	{
		public class PaintCoordBlock
		{
			public OS.Coord Space;
			public OS.Coord Letter;
			public OS.Rect RubOut;

			PaintCoordBlock (OS.Coord space,
					 OS.Coord letter,
					 OS.Rect rubOut)
			{
				Space = space;
				Letter = letter;
				RubOut = rubOut;
			}
		}

		// I wanted to call this class Font, but it clashes with the enclosing class.
		// new Font.Instance ("Trinity.Medium") sounds right anyway.
		/*! \class Instance
		 * \brief Encapsulates a RISC OS font handle. */
		public class Instance
		{
			public IntPtr Handle;

			/*! \brief Create an instance of the named font.
			 * \param[in] name Font identifier
			 *
			 * A default size of 12 points and resolution of 90 dpi is assumed. */
			public Instance (string name)
			{
				Find (name, 12 << 4, 12 << 4, 90, 90);
			}

			/*! \brief Create an instance of the named font using the size given.
			 * \param[in] name Font identifier
			 * \param[in] xSize x point size x 16 (in 1/16ths of a point)
			 * \param[in] ySize y point size x 16 (in 1/16ths of a point)
			 *
			 * A default resolution of 90 dpi is assumed. */
			public Instance (string name, int xSize, int ySize)
			{
				Find (name, xSize, ySize, 90, 90);
			}

			/*! \brief Release the font resource. */
			~Instance ()
			{
				Lose ();
			}

			/*! \brief Get the handle for a font.
			 * \param[in] name Font identifier
			 * \param[in] xSize x point size x 16 (in 1/16ths of a point)
			 * \param[in] ySize y point size x 16 (in 1/16ths of a point)
			 * \param[in] xRes x resolution in dots per inch (0 = use default, -1 = use current)
			 * \param[in] yRes y resolution in dots per inch (0 = use default, -1 = use current)
			 * \return Nothing */
			public void Find (string name, int xSize, int ySize, int xRes, int yRes)
			{
				int xResOut, yResOut;

				OS.ThrowOnError (NativeMethods.Font_FindFont (name, xSize, ySize, xRes, yRes,
									      out Handle, out xResOut, out yResOut));
			}

			/*! \brief Release the font resource. */
			public void Lose ()
			{
				if (Handle != IntPtr.Zero) {
					OS.ThrowOnError (NativeMethods.Font_LoseFont (Handle));
					Handle = IntPtr.Zero;
				}
			}

			/*! \brief Write a string to the screen.
			 * \param[in] str String to write
			 * \param[in] flags plot type
			 * \param[in] xPos Start x coordinate (Must be in millipoints)
			 * \param[in] yPos Start y coordinate (Must be in millipoints)
			 * \param[in] coordBlock Coordinate block
			 * \param[in] matrix Transformation matrix
			 * \param[in] length Length of string to write (if bit 7 of flags set)
			 * \return Nothing
			 * 
			 * Bits 5, 6 and 8 of the flags are automatically set,
			 * bits 0 and 4 are automatically cleared. */
			public void Paint (string str,
					   uint flags,
					   int xPos,
					   int yPos,
					   Font.PaintCoordBlock coordBlock,
					   OS.Matrix matrix,
					   int length)
			{
				NativeFont.PaintCoordBlock coordBlockNative =
						new NativeFont.PaintCoordBlock (coordBlock);
				NativeOS.Matrix matrixNative =
						new NativeOS.Matrix (matrix);

				/* The transformation matrix and coodinate block are being used,
				 * so make sure the relevant bits are set/clear.  */
				flags |= (1U << 5) | ( 1U << 6) | (1U << 8);
				flags &= ~((1U << 4) | (1U << 0));

				OS.ThrowOnError (NativeMethods.Font_Paint (Handle,
									   s,
									   flags,
									   xPos,
									   yPos,
									   ref coordBlockNative,
									   ref matrixNative,
									   length));
			}

			/*! \brief Write a string to the screen.
			 * \param[in] str String to write
			 * \param[in] flags Plot type
			 * \param[in] xPos Start x coordinate (Must be in millipoints)
			 * \param[in] yPos Start y coordinate (Must be in millipoints)
			 * \param[in] matrix Transformation matrix
			 * \param[in] length Length of string to write (if bit 7 of flags set)
			 * \return Nothing
			 * 
			 * Bits 6 and 8 of the flags are automatically set,
			 * bits 4 and 5 are automatically cleared. */
			public void Paint (string str,
					   uint flags,
					   int xPos,
					   int yPos,
					   OS.Matrix matrix,
					   int length)
			{
				NativeOS.Matrix matrixNative =
						new NativeOS.Matrix (matrix);

				/* The transformation matrix is being used,
				 * so make sure the relevant bits are set/clear.  */
				flags |= (1U << 6) | (1U << 8);
				flags &= ~((1U << 5) | (1U << 4));

				OS.ThrowOnError (NativeMethods.Font_Paint (Handle,
									   s,
									   flags,
									   xPos,
									   yPos,
									   IntPtr.Zero,
									   ref matrixNative,
									   length));
			}

			/*! \brief Write a string to the screen.
			 * \param[in] str String to write
			 * \param[in] flags Plot type
			 * \param[in] xPos Start x coordinate (Must be in millipoints)
			 * \param[in] yPos Start y coordinate (Must be in millipoints)
			 * \param[in] coordBlock Coordinate block
			 * \param[in] length Length of string to write (if bit 7 of flags set)
			 * \return Nothing
			 * 
			 * Bits 5 and 8 of the flags are automatically set,
			 * bits 0, 4 and 6 are automatically cleared. */
			public void Paint (string str,
					   uint flags,
					   int xPos,
					   int yPos,
					   Font.PaintCoordBlock coordBlock,
					   int length)
			{
				NativeFont.PaintCoordBlock coordBlockNative =
						new NativeFont.PaintCoordBlock (coordBlock);

				/* The coordinate block is being used, so make
				 * sure the relevant bits are set/clear.  */
				flags |= (1U << 5) | (1U << 8);
				flags &= ~((1U << 6) | (1U << 4) | (1U << 0));

				OS.ThrowOnError (NativeMethods.Font_Paint (Handle,
									   s,
									   flags,
									   xPos,
									   yPos,
									   ref coordBlockNative,
									   IntPtr.Zero,
									   length));
			}

			/*! \brief Write a string to the screen.
			 * \param[in] str String to write
			 * \param[in] flags Plot type
			 * \param[in] xPos Start x coordinate (flags bit 4 set = OS units, clear = millipoints)
			 * \param[in] yPos Start y coordinate (flags bit 4 set = OS units, clear = millipoints)
			 * \param[in] length Length of string to write (if bit 7 of flags set)
			 * \return Nothing
			 * 
			 * Bit 8 of the flags is automatically set,
			 * bits 5 and 6 are automatically cleared. */
			public void Paint (string str,
					   uint flags,
					   int xPos,
					   int yPos,
					   int length)
			{
				flags |= (1U << 8);
				flags &= ~((1U << 5) | (1U << 6));
				OS.ThrowOnError (NativeMethods.Font_Paint (Handle,
									   s,
									   flags,
									   xPos,
									   yPos,
									   IntPtr.Zero,
									   IntPtr.Zero,
									   length));
			}

			/*! \brief Read details about a font.
			 * \param[out] size x,y point size x 16 (in 1/16ths point)
			 * \param[out] res x,y resolution (dots per inch)
			 * \param[out] age Age of font
			 * \param[out] usageCount Usage count of font
			 * \returns Font identifier */
			public string ReadFullDefn (OS.Coord size,
						OS.Coord res,
						out int age,
						out int usageCount)
			{
				int buffer_size;

				OS.ThrowOnError (NativeMethods.Font_ReadIdentifier (Handle,
										    IntPtr.Zero,
										    out buffer_size));

				StringBuilder buffer = new StringBuilder (buffer_size);
				OS.ThrowOnError (NativeMethods.Font_ReadFullDefn (Handle,
										  buffer,
										  out size.X,
										  out size.Y,
										  out res.X,
										  out res.Y,
										  out age,
										  out usageCount));


				return buffer.ToString ();
			}

			/*! \brief Read a font's identifier.
			 * \returns Font identifier */
			public string ReadIdentifier ()
			{
				int buffer_size;

				OS.ThrowOnError (NativeMethods.Font_ReadIdentifier (Handle,
										    IntPtr.Zero,
										    out buffer_size));

				StringBuilder buffer = new StringBuilder (buffer_size);
				OS.ThrowOnError (NativeMethods.Font_ReadIdentifier (Handle,
										    buffer,
										    out buffer_size));
				return buffer.ToString ();
			}

			/*! \brief Get the font bounding box.
			 * \param[out] bbox Bounding box of font in OS units
			 * \returns Nothing.
			 * \deprecated You should use CharBBox in preference. */
			public void ReadInfo (OS.Rect bbox)
			{
				int min_x, min_y, max_x, max_y;

				OS.ThrowOnError (NativeMethods.Font_ReadInfo (Handle,
									      out min_x,
									      out min_y,
									      out max_x,
									      out max_y));
				bbox.MinX = min_x;
				bbox.MinY = min_y;
				bbox.MaxX = max_x;
				bbox.MaxY = max_y;
			}

			/*! \brief Get the font bounding box.
			 * \returns The font bounding box in the form of a new OS.Rect.
			 * \deprecated You should use CharBBox in preference. */
			public OS.Rect ReadInfo ()
			{
				int min_x, min_y, max_x, max_y;

				OS.ThrowOnError (NativeMethods.Font_ReadInfo (Handle,
									      out min_x,
									      out min_y,
									      out max_x,
									      out max_y));
				return new OS.Rect (min_x, min_y, max_x, max_y);
			}

			/*! \brief Get the bounding box of a character.
			 * \param[in] charCode ASCII character code
			 * \param[in] flags bit 4 set => return in OS units, else millipoints
			 * \returns The character bounding box in the form of a new OS.Rect */
			public OS.Rect CharBBox (int charCode, uint flags)
			{
				int min_x, min_y, max_x, max_y;

				OS.ThrowOnError (NativeMethods.Font_CharBBox (Handle,
									      charCode,
									      flags,
									      out min_x,
									      out min_y,
									      out max_x,
									      out max_y));
				return new OS.Rect (min_x, min_y, max_x, max_y);
			}
		}

		// Returns the version as a double
		/*! \brief Get the version number, font cache size and amount used.
		 * \param[out] cacheSize Total size of font cache in bytes
		 * \param[out] cacheUsed Amount of font cache used in bytes
		 * \returns Version number */
		public static double ReadCacheAddr (out int cacheSize, out int cacheUsed)
		{
			int version;

			OS.ThrowOnError (NativeMethods.Font_CacheAddr (out version,
								       out cacheSize,
								       out cacheUsed));
			return (double)version / 100;
		}

		/*! \brief Calculate how wide a string would be in the current font
		 * \param[in] str String to scan
		 * \param[in] max Maximum offset before termination in millipoints
		 * \param[in] splitChar Character code of split char
		 * \param[in] length Length of string to scan
		 * \param[out] splitOffset Offset of split point
		 * \param[out] splitCount Number of split characters in string
		 * \returns Index into string of the split point
		 * \deprecated You should use ScanString in preference */
		public static int StringWidth (string str,
					       OS.Coord max,
					       int splitChar,
					       int length,
					       OS.Coord splitOffset,
					       out int splitCount)
		{
			int index;

			OS.ThrowOnError (NativeMethods.Font_StringWidth (str,
									 max.X,
									 max.Y,
									 splitChar,
									 length,
									 IntPtr.Zero,
									 out splitOffset.X,
									 out splitOffset.Y,
									 out splitCount,
									 out index));
			return index;
		}

		/*! \brief Select the font to be subsequently used.
		 * \param[in] font Font to use.
		 * \return Nothing */
		public static void SetFont (Font.Instance font)
		{
			OS.ThrowOnError (NativeMethods.Font_SetFont (font.Handle));
		}

		/*! \brief Convert internal coordinates to OS coordinates
		 * \param[in] points x,y coordinates in millipoints
		 * \returns New OS.Coord in OS units */
		public static OS.Coord ConvertToOS (OS.Coord points)
		{
			int os_x, os_y;

			OS.ThrowOnError (NativeMethods.Font_ConvertToOS (points.X, points.Y,
									 out os_x, out os_y));

			return new OS.Coord (os_x, os_y);
		}

		/*! \brief Convert internal coordinates to OS coordinates
		 * \param[in] xPoints x coordinate in millipoints
		 * \param[in] yPoints y coordinate in millipoints
		 * \returns New OS.Coord in OS units */
		public static OS.Coord ConvertToOS (int xPoints, int yPoints)
		{
			int os_x, os_y;

			OS.ThrowOnError (NativeMethods.Font_ConvertToOS (xPoints, yPoints,
									 out os_x, out os_y));

			return new OS.Coord (os_x, os_y);
		}

		/*! \brief Convert internal coordinates to OS coordinates
		 * \param[in] points x,y coordinates in millipoints
		 * \param[out] os x,y coordinates in OS units
		 * \returns Nothing */
		public static void ConvertToOS (OS.Coord points, OS.Coord os)
		{
			OS.ThrowOnError (NativeMethods.Font_ConvertToOS (points.X, points.Y,
									 out os.X, out os.Y));
		}

		/*! \brief Convert OS coordinates to internal coordinates
		 * \param[in] os x,y coordinates in OS units
		 * \returns New OS.Coord in millipoints */
		public static OS.Coord ConvertToPoints (OS.Coord os)
		{
			int points_x, points_y;

			OS.ThrowOnError (NativeMethods.Font_ConvertToPoints (os.X, os.Y,
									     out points_x, out points_y));

			return new OS.Coord (points_x, points_y);
		}

		/*! \brief Convert OS coordinates to internal coordinates
		 * \param[in] xOS x coordinate in OS units
		 * \param[in] yOS y coordinate in OS units
		 * \returns New OS.Coord in millipoints */
		public static OS.Coord ConvertToPoints (int xOS, int yOS)
		{
			int points_x, points_y;

			OS.ThrowOnError (NativeMethods.Font_ConvertToPoints (xOS, yOS,
									     out points_x, out points_y));

			return new OS.Coord (points_x, points_y);
		}

		/*! \brief Convert OS coordinates to internal coordinates
		 * \param[in] os x,y coordinates in OS units
		 * \param[out] points x,y coordinates in millipoints
		 * \returns Nothing */
		public static void ConvertToPoints (OS.Coord os, OS.Coord points)
		{
			OS.ThrowOnError (NativeMethods.Font_ConvertToPoints (os.X, os.Y,
									     out points.X, out points.Y));
		}

		/*! \brief Define font cursor for Font Manager
		 * \param[in] colour Colour (exclusively ORd onto screen)
		 * \param[in] height Height in OS units
		 * \param[in] flags bit 4 set => pos in OS units, else millipoints
		 * \param[in] pos x,y coordinates
		 * \returns Nothing */
		public static void DefineCaret (uint colour, int height, uint flags, OS.Coord pos)
		{
			OS.ThrowOnError (NativeMethods.Font_Caret (colour, height, flags, pos.X, pos.Y));
		}

		/*! \brief Read the internal to OS conversion factor.
		 * \param[out] factor x,y scale factors */
		public static void ReadScaleFactor (OS.Coord factor)
		{
			OS.ThrowOnError (NativeMethods.Font_ReadScaleFactor (out factor.X, out factor.Y));
		}

		/*! \brief Read the internal to OS conversion factor.
		 * returns x,y scale factor in the form os a new OS.Coord */
		public static OS.Coord ReadScaleFactor ()
		{
			int factor_x, factor_y;

			OS.ThrowOnError (NativeMethods.Font_ReadScaleFactor (out factor_x, out factor_y));

			return new OS.Coord (factor_x, factor_y);
		}

		/*! \brief Set the internal to OS conversion factor.
		 * \param[in] factor x,y scale factors
		 * \returns Nothing */
		public static void SetScaleFactor (OS.Coord factor)
		{
			OS.ThrowOnError (NativeMethods.Font_SetScaleFactor (factor.X, factor.Y));
		}

		/*! \brief Set the internal to OS conversion factor.
		 * \param[in] factorX x scale factor
		 * \param[in] factorY y scale factor
		 * \returns Nothing */
		public static void SetScaleFactor (int factorX, int factorY)
		{
			OS.ThrowOnError (NativeMethods.Font_SetScaleFactor (factorX, factorY));
		}
	}
}
