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
		/*! \enum PlotType
		 * \brief Flags used to control the behaviour of \e Font.Instance.Paint
		 * and \e Font.Instance.ScanString */
		[Flags]
		public enum PlotType
		{
			/*! If set, use graphics cursor justification coordinates (cannot be used with GivenBlock); else
			 * use PaintCoordBlock to justify (if GivenBlock set) or don't justify
			 *
			 * \note Font.Instance.Paint only */
			Justify = (1 << 0),
			/*! If set, plot rubout box using either graphics cursor rubout coordinates (if GivenBlock clear)
			 * or PaintCoordBlock (if GivenBlock set); else don't plot rubout box
			 * 
			 * \note Font.Instance.Paint only */
			RubOut = (1 << 1),
			/*! If set, coordinates are in OS units; else in millipoints
			 *
			 * \note Font.Instance.Paint only */
			OSUnits = (1 << 4),
			/*! If set, use either PaintCoordBlock or ScanCoordBlock */
			GivenBlock = (1 << 5),
			/*! If set, matrix is given */
			GivenMatrix = (1 << 6),
			/*! If set, only take the given number of characters into consideration */
			GivenLength = (1 << 7),
			/*! If set, a font handle is given */
			GivenFont = (1 << 8),
			/*! If set, perform kerning on the string */
			Kern = (1 << 9),
			/*! If set, writing deirection is right to left, else left to right */
			RightToLeft = (1 << 10),
			/*! If set, perform font blending. */
			BlendFont = (1 << 11),
			/*! If set, string consists of 16bit characters
			 *
			 * \note RISC OS 5+, Font Manager 3.41+ */
			Given16Bit = (1 << 12),
			/*! If set, string consists of 32bit characters
			 *
			 * \note RISC OS 5+, Font Manager 3.41+ */
			Given32Bit = (1 << 13),
			/*! If set, return nearest caret position; else length of string
			 *
			 * \note Font.Instance.ScanString only */
			ReturnCaretPos = (1 << 17),
			/*! If set, return bounding box of string in ScanCoordBlock
			 *
			 * \note Font.Instance.ScanString only */
			ReturnBBox = (1 << 18),
			/*! If set, return matrix applying at end of string
			 *
			 * \note Font.Instance.ScanString only */
			ReturnMatrix = (1 << 19),
			/*! If set, return number of split characters
			 *
			 * \note Font.Instance.ScanString only */
			ReturnSplitCount = (1 << 20)
		}

		/*! \class PaintCoordBlock
		 * \brief Used by \e Font.Instance.Paint for coordinate block and
		 * rubout box. */
		public class PaintCoordBlock
		{
			/*! \brief Additional x,y offset on space. */
			public OS.Coord Space;
			/*! \brief Additional x,y offset between each letter. */
			public OS.Coord Letter;
			/*! \brief Coordinates of rubout box. */
			public OS.Rect RubOut;

			/*! \brief Create a PaintCoordBlock */
			public PaintCoordBlock (OS.Coord space,
						OS.Coord letter,
						OS.Rect rubOut)
			{
				Space = space;
				Letter = letter;
				RubOut = rubOut;
			}
		}

		/*! \class ScanCoordBlock
		 * \brief Used by \e Font.Instance.ScanString for coordinate block and
		 * split character on input and optionally to return a string's bounding box. */
		public class ScanCoordBlock
		{
			/*! \brief Additional x,y offset on space. */
			public OS.Coord Space;
			/*! \brief Additional x,y offset between each letter. */
			public OS.Coord Letter;
			/*! \brief Split character. */
			public int SplitChar;
			/*! \brief Returned bounding box - if bit 18 of flags is set. */
			public OS.Rect BoundingBox;

			/*! \brief Create a ScanCoordBlock with default values. */
			public ScanCoordBlock ()
			{
				Space = new OS.Coord ();
				Letter = new OS.Coord ();
				SplitChar = -1;
				BoundingBox = new OS.Rect ();
			}

			/*! \brief Create a ScanCoordBlock */
			public ScanCoordBlock (OS.Coord space,
					       OS.Coord letter,
					       int splitChar,
					       OS.Rect boundingBox)
			{
				Space = space;
				Letter = letter;
				SplitChar = splitChar;
				BoundingBox = boundingBox;
			}
		}

		// I wanted to call this class Font, but it clashes with the enclosing class.
		// new Font.Instance ("Trinity.Medium") sounds right anyway.
		/*! \class Instance
		 * \brief Encapsulates a RISC OS font handle. */
		public class Instance
		{
			/*! A RISC OS font handle */
			public IntPtr Handle;
			bool owner;

			/*! \brief Create an instance of the named font.
			 * \param[in] name Font identifier
			 *
			 * \note
			 * A default size of 12 points and resolution of 90 dpi is assumed. */
			public Instance (string name)
			{
				Find (name, 12 << 4, 12 << 4, 90, 90);
				owner = true;
			}

			/*! \brief Create an instance of the named font using the size given.
			 * \param[in] name Font identifier
			 * \param[in] xSize x point size x 16 (in 1/16ths of a point)
			 * \param[in] ySize y point size x 16 (in 1/16ths of a point)
			 *
			 * \note
			 * A default resolution of 90 dpi is assumed. */
			public Instance (string name, int xSize, int ySize)
			{
				Find (name, xSize, ySize, 90, 90);
				owner = true;
			}

			/*! \brief Create an instance from an existing font handle.
			 * \param[in] fontHandle Handle of existing font (e.g. the desktop font) */
			public Instance (IntPtr fontHandle)
			{
				Handle = fontHandle;
				owner = false;
			}

			/*! \brief Release the font resource. */
			~Instance ()
			{
				if (owner)
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
			 * \note
			 * Automatically:
			 * \li Sets flag bit 5 to indicate coordinate block is used
			 * \li Sets flag bit 6 to indicate transformation matrix is used.
			 * \li Sets flag bit 8 to indicate font handle is used
			 * \li Clears flag bit 0 because coordinate block is used
			 * \li Clears flag bit 4 - coordinates must be in millipoints when
			 * coordinate block and/or matrix is used
			 */
			public void Paint (string str,
					   PlotType flags,
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

				flags |= PlotType.GivenBlock |
					 PlotType.GivenMatrix |
					 PlotType.GivenFont;
				flags &= ~(PlotType.OSUnits | PlotType.Justify);

				OS.ThrowOnError (NativeMethods.Font_Paint (Handle,
									   str,
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
			 * \note
			 * Automatically:
			 * \li Sets flag bit 6 to indicate transformation matrix is used.
			 * \li Sets flag bit 8 to indicate font handle is used
			 * \li Clears flag bit 5 to indicate coordinate block not in use
			 * \li Clears flag bit 4 - must use millipoints when using matrix
			 */
			public void Paint (string str,
					   PlotType flags,
					   int xPos,
					   int yPos,
					   OS.Matrix matrix,
					   int length)
			{
				NativeOS.Matrix matrixNative =
						new NativeOS.Matrix (matrix);

				flags |= PlotType.GivenMatrix | PlotType.GivenFont;
				flags &= ~(PlotType.GivenBlock | PlotType.OSUnits);

				OS.ThrowOnError (NativeMethods.Font_Paint (Handle,
									   str,
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
			 * \note
			 * Automatically:
			 * \li Sets flag bit 5 to indicate coordinate block in use
			 * \li Sets flag bit 8 to indicate font handle in use
			 * \li Clears flag bit 0 because coordinate block in use
			 * \li Clears flag bit 4 - millipoints must be used if coordinate block is used
			 * \li Clears flag bit 6 to indicate transformation matrix not in use
			 */
			public void Paint (string str,
					   PlotType flags,
					   int xPos,
					   int yPos,
					   Font.PaintCoordBlock coordBlock,
					   int length)
			{
				NativeFont.PaintCoordBlock coordBlockNative =
						new NativeFont.PaintCoordBlock (coordBlock);

				flags |= PlotType.GivenBlock | PlotType.GivenFont;
				flags &= ~(PlotType.GivenMatrix |
					   PlotType.OSUnits |
					   PlotType.Justify);

				OS.ThrowOnError (NativeMethods.Font_Paint (Handle,
									   str,
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
			 * \note
			 * Automatically:
			 * \li Sets flag bit 8 to indicate font handle in use
			 * \li Clears flag bit 5 to indicate coordinate block not in use
			 * \li Clears flag bit 6 to indicate matrix not in use
			 */
			public void Paint (string str,
					   PlotType flags,
					   int xPos,
					   int yPos,
					   int length)
			{
				flags |= PlotType.GivenFont;
				flags &= ~(PlotType.GivenBlock | PlotType.GivenMatrix);

				OS.ThrowOnError (NativeMethods.Font_Paint (Handle,
									   str,
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
			public OS.Rect CharBBox (int charCode, PlotType flags)
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

			/*! \brief Return information on a string.
			 * \param[in] str String to scan
			 * \param[in] flags Plot type
			 * \param[in] offset Offset of mouse click
			 * \param[in,out] coordBlock Coordinate block
			 * \param[in,out] matrix Transformation matrix
			 * \param[in] length Length of string to scan - if \e PlotType.GivenLength (bit 7) of flags set
			 * \param[out] offsetReturn x,y coordinate offset to caret position -
			 * if \e PlotType.ReturnCaretPos (bit 17) of flags set; else to split point,
			 * or end of string if splitting not required
			 * \param[out] splitCount Number of split characters encountered -
			 * if \e PlotType.ReturnSplitCount (bit 20) of flags set
			 * \return Index of point in string of caret position - if \e PlotType.ReturnCaretPos
			 * (bit 17) of flags is set; else of split point, or of end of string if splitting
			 * not required.
			 * 
			 * \note
			 * Automatically:
			 * \li Sets flag bit 5 to indicate coordinate block in use
			 * \li Sets flag bit 6 to indicate transformation matrix in use
			 * \li Sets flag bit 8 to indicate font handle in use
			 *
			 * All coordinates must be in millipoints. */
			public int ScanString (string str,
					       PlotType flags,
					       OS.Coord offset,
					       Font.ScanCoordBlock coordBlock,
					       OS.Matrix matrix,
					       int length,
					       OS.Coord offsetReturn,
					       out int splitCount)
			{
				int result_index;

				NativeFont.ScanCoordBlock coordBlockNative =
						new NativeFont.ScanCoordBlock (coordBlock);
				NativeOS.Matrix matrixNative =
						new NativeOS.Matrix (matrix);

				flags |= PlotType.GivenBlock |
					 PlotType.GivenMatrix |
					 PlotType.GivenFont;

				OS.ThrowOnError (NativeMethods.Font_ScanString (Handle,
										str,
										flags,
										offset.X,
										offset.Y,
										ref coordBlockNative,
										ref matrixNative,
										length,
										out result_index,
										out offsetReturn.X,
										out offsetReturn.Y,
										out splitCount));
				if ((flags & PlotType.ReturnBBox) != 0)
				{
					// Copy bounding box results back to coordBlock
					coordBlock.BoundingBox.MinX = coordBlockNative.BoundingBox.MinX;
					coordBlock.BoundingBox.MinY = coordBlockNative.BoundingBox.MinY;
					coordBlock.BoundingBox.MaxX = coordBlockNative.BoundingBox.MaxX;
					coordBlock.BoundingBox.MaxY = coordBlockNative.BoundingBox.MaxY;
				}

				if ((flags & PlotType.ReturnMatrix) != 0)
				{
					matrix.m[0,0] = matrixNative.a;
					matrix.m[0,1] = matrixNative.b;
					matrix.m[1,0] = matrixNative.c;
					matrix.m[1,1] = matrixNative.d;
					matrix.m[2,0] = matrixNative.e;
					matrix.m[2,1] = matrixNative.f;
				}

				return result_index;
			}

			/*! \brief Return information on a string.
			 * \param[in] str String to scan
			 * \param[in] flags Plot type
			 * \param[in] offset Offset of mouse click
			 * \param[in,out] matrix Transformation matrix
			 * \param[in] length Length of string to scan - if \e PlotType.GivenLength (bit 7) of flags set
			 * \param[out] offsetReturn x,y coordinate offset to caret position -
			 * if \e PlotType.ReturnCaretPos (bit 17) of flags set; else to split point,
			 * or end of string if splitting not required
			 * \param[out] splitCount Number of split characters encountered -
			 * if \e PlotType.ReturnSplitCount (bit 20) of flags set
			 * \return Index of point in string of caret position - if \e PlotType.ReturnCaretPos
			 * (bit 17) of flags is set; else of split point, or of end of string if splitting
			 * not required.
			 * 
			 * \note
			 * Automatically:
			 * \li Sets flag bit 6 to indicate transformation matrix in use
			 * \li Sets flag bit 8 to indicate font handle in use
			 * \li Clears flag bits 5 & 18 to indicate coordinate block not in use.
			 *
			 * All coordinates must be in millipoints. */
			public int ScanString (string str,
					       PlotType flags,
					       OS.Coord offset,
					       OS.Matrix matrix,
					       int length,
					       OS.Coord offsetReturn,
					       out int splitCount)
			{
				NativeOS.Matrix matrixNative =
						new NativeOS.Matrix (matrix);
				int result_index;

				flags |= PlotType.GivenMatrix | PlotType.GivenFont;
				flags &= ~(PlotType.GivenBlock | PlotType.ReturnBBox);

				OS.ThrowOnError (NativeMethods.Font_ScanString (Handle,
										str,
										flags,
										offset.X,
										offset.Y,
										IntPtr.Zero,
										ref matrixNative,
										length,
										out result_index,
										out offsetReturn.X,
										out offsetReturn.Y,
										out splitCount));
				if ((flags & PlotType.ReturnMatrix) != 0)
				{
					matrix.m[0,0] = matrixNative.a;
					matrix.m[0,1] = matrixNative.b;
					matrix.m[1,0] = matrixNative.c;
					matrix.m[1,1] = matrixNative.d;
					matrix.m[2,0] = matrixNative.e;
					matrix.m[2,1] = matrixNative.f;
				}

				return result_index;
			}

			/*! \brief Return information on a string.
			 * \param[in] str String to scan
			 * \param[in] flags Plot type
			 * \param[in] offset Offset of mouse click
			 * \param[in,out] coordBlock Coordinate block
			 * \param[in] length Length of string to scan - if \e PlotType.GivenLength (bit 7) of flags set
			 * \param[out] offsetReturn x,y coordinate offset to caret position -
			 * if \e PlotType.ReturnCaretPos (bit 17) of flags set; else to split point,
			 * or end of string if splitting not required
			 * \param[out] splitCount Number of split characters encountered -
			 * if \e PlotType.ReturnSplitCount (bit 20) of flags set
			 * \return Index of point in string of caret position - if \e PlotType.ReturnCaretPos
			 * (bit 17) of flags is set; else of split point, or of end of string if splitting
			 * not required.
			 * 
			 * \note
			 * Automatically:
			 * \li Sets flag bit 5 to indicate coordinate block in use
			 * \li Sets flag bit 8 to indicate font handle in use
			 * \li Clears flag bits 6 & 19 to indicate transformation matrix not in use.
			 *
			 * All coordinates must be in millipoints. */
			public int ScanString (string str,
					       PlotType flags,
					       OS.Coord offset,
					       Font.ScanCoordBlock coordBlock,
					       int length,
					       OS.Coord offsetReturn,
					       out int splitCount)
			{
				NativeFont.ScanCoordBlock coordBlockNative =
						new NativeFont.ScanCoordBlock (coordBlock);
				int result_index;

				flags |= PlotType.GivenBlock | PlotType.GivenFont;
				flags &= ~(PlotType.GivenMatrix | PlotType.ReturnMatrix);

				OS.ThrowOnError (NativeMethods.Font_ScanString (Handle,
										str,
										flags,
										offset.X,
										offset.Y,
										ref coordBlockNative,
										IntPtr.Zero,
										length,
										out result_index,
										out offsetReturn.X,
										out offsetReturn.Y,
										out splitCount));
				if ((flags & PlotType.ReturnBBox) != 0)
				{
					// Copy bounding box results back to coordBlock
					coordBlock.BoundingBox.MinX = coordBlockNative.BoundingBox.MinX;
					coordBlock.BoundingBox.MinY = coordBlockNative.BoundingBox.MinY;
					coordBlock.BoundingBox.MaxX = coordBlockNative.BoundingBox.MaxX;
					coordBlock.BoundingBox.MaxY = coordBlockNative.BoundingBox.MaxY;
				}

				return result_index;
			}

			/*! \brief Return information on a string.
			 * \param[in] str String to scan
			 * \param[in] flags Plot type
			 * \param[in,out] offset Offset of mouse click
			 * \param[in] length Length of string to scan - if \e PlotType.GivenLength (bit 7) of flags set
			 * \param[out] offsetReturn x,y coordinate offset to caret position -
			 * if \e PlotType.ReturnCaretPos (bit 17) of flags set; else to split point,
			 * or end of string if splitting not required
			 * \param[out] splitCount Number of split characters encountered -
			 * if \e PlotType.ReturnSplitCount (bit 20) of flags set
			 * \return Index of point in string of caret position - if \e PlotType.ReturnCaretPos
			 * (bit 17) of flags is set; else of split point, or of end of string if splitting
			 * not required.
			 * 
			 * \note
			 * Automatically:
			 * \li Sets flag bit 8 to indicate font handle in use.
			 * \li Clears flag bit 5 & 18 to indicate coordinate block not in use.
			 * \li Clears flag bits 6 & 19 to indciate matrix not in use.
			 *
			 * All coordinates must be in millipoints. */
			public int ScanString (string str,
					       PlotType flags,
					       OS.Coord offset,
					       int length,
					       OS.Coord offsetReturn,
					       out int splitCount)
			{
				int result_index;

				flags |= PlotType.GivenFont;
				flags &= ~(PlotType.GivenBlock |
					   PlotType.ReturnBBox |
					   PlotType.GivenMatrix |
					   PlotType.ReturnMatrix);

				OS.ThrowOnError (NativeMethods.Font_ScanString (Handle,
										str,
										flags,
										offset.X,
										offset.Y,
										IntPtr.Zero,
										IntPtr.Zero,
										length,
										out result_index,
										out offsetReturn.X,
										out offsetReturn.Y,
										out splitCount));
				return result_index;
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

		/*! \brief Get current font handle and colours
		 * \param[out] bgCol Current background logical colour
		 * \param[out] fgCol Current foreground logical colour
		 * \param[out] offset Foreground colour offset
		 * \returns A new Font.Instance for current font */
		public static Instance CurrentFont (out uint bgCol, out uint fgCol, int offset)
		{
			IntPtr handle;

			OS.ThrowOnError (NativeMethods.Font_CurrentFont (out handle,
									 out bgCol,
									 out fgCol,
									 out offset));
			return new Instance (handle);
		}

		/*! \brief Check font characteristics after Font.StringWidth
		 * \param[out] bgCol Future background logical colour
		 * \param[out] fgCol Future foreground logical colour
		 * \param[out] offset Foreground colour offset
		 * \returns A new Font.Instance for the font which would be selected */
		public static Instance FutureFont (out uint bgCol, out uint fgCol, int offset)
		{
			IntPtr handle;

			OS.ThrowOnError (NativeMethods.Font_FutureFont (out handle,
									out bgCol,
									out fgCol,
									out offset));
			return new Instance (handle);
		}

	}
}
