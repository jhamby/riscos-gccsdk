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
		public class Instance
		{
			public IntPtr Handle;

			public Instance (string name)
			{
				Find (name, 12 << 4, 12 << 4, 90, 90);
			}

			public Instance (string name, int xSize, int ySize)
			{
				Find (name, xSize, ySize, 90, 90);
			}

			~Instance ()
			{
				Lose ();
			}

			/*! \brief Get the handle for a font.
			 * \param name Font identifier
			 * \param xSize x point size x 16 (in 1/16ths of a point)
			 * \param ySize y point size x 16 (in 1/16ths of a point)
			 * \param xRes x resolution in dots per inch (0 = use default, -1 = use current)
			 * \param yRes y resolution in dots per inch (0 = use default, -1 = use current)
			 * \return Nothing */
			public void Find (string name, int xSize, int ySize, int xRes, int yRes)
			{
				int xResOut, yResOut;

				OS.ThrowOnError (NativeMethods.Font_FindFont (name, xSize, ySize, xRes, yRes,
									      out Handle, out xResOut, out yResOut));
			}

			public void Lose ()
			{
				if (Handle != IntPtr.Zero) {
					OS.ThrowOnError (NativeMethods.Font_LoseFont (Handle));
					Handle = IntPtr.Zero;
				}
			}

			public void Paint (string s,
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

			public void Paint (string s,
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

			public void Paint (string s,
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

			public void Paint (string s,
					   uint flags,
					   int xPos,
					   int yPos,
					   int length)
			{
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
		public static double ReadCacheAddr (out int cacheSize, out int cacheUsed)
		{
			int version;

			OS.ThrowOnError (NativeMethods.Font_CacheAddr (out version,
								       out cacheSize,
								       out cacheUsed));
			return (double)version / 100;
		}

		// Inputs:
		// 	str 		- string to scan
		// 	Max 		- Maximum offset before termination in millipoints
		// 	splitChar	- Character code of split char
		// 	length		- Length of string to scan
		// Outputs:
		// 	splitOffset	- Offset of split point
		// 	splitCount	- Number of split characters in string
		// Returns:
		//  Index in to string of the split point
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

		public static void SetFont (Font.Instance font)
		{
			OS.ThrowOnError (NativeMethods.Font_SetFont (font.Handle));
		}

		public static OS.Coord ConvertToOS (OS.Coord points)
		{
			int os_x, os_y;

			OS.ThrowOnError (NativeMethods.Font_ConvertToOS (points.X, points.Y,
									 out os_x, out os_y));

			return new OS.Coord (os_x, os_y);
		}

		public static OS.Coord ConvertToOS (int xPoints, int yPoints)
		{
			int os_x, os_y;

			OS.ThrowOnError (NativeMethods.Font_ConvertToOS (xPoints, yPoints,
									 out os_x, out os_y));

			return new OS.Coord (os_x, os_y);
		}

		public static void ConvertToOS (OS.Coord points, OS.Coord os)
		{
			OS.ThrowOnError (NativeMethods.Font_ConvertToOS (points.X, points.Y,
									 out os.X, out os.Y));
		}

		public static OS.Coord ConvertToPoints (OS.Coord os)
		{
			int points_x, points_y;

			OS.ThrowOnError (NativeMethods.Font_ConvertToPoints (os.X, os.Y,
									     out points_x, out points_y));

			return new OS.Coord (points_x, points_y);
		}

		public static OS.Coord ConvertToPoints (int xOS, int yOS)
		{
			int points_x, points_y;

			OS.ThrowOnError (NativeMethods.Font_ConvertToPoints (xOS, yOS,
									     out points_x, out points_y));

			return new OS.Coord (points_x, points_y);
		}

		public static void ConvertToPoints (OS.Coord os, OS.Coord points)
		{
			OS.ThrowOnError (NativeMethods.Font_ConvertToPoints (os.X, os.Y,
									     out points.X, out points.Y));
		}

		public static void DefineCaret (uint colour, int height, uint flags, OS.Coord pos)
		{
			OS.ThrowOnError (NativeMethods.Font_Caret (colour, height, flags, pos.X, pos.Y));
		}

		public static void ReadScaleFactor (OS.Coord factor)
		{
			OS.ThrowOnError (NativeMethods.Font_ReadScaleFactor (out factor.X, out factor.Y));
		}

		public static OS.Coord ReadScaleFactor ()
		{
			int factor_x, factor_y;

			OS.ThrowOnError (NativeMethods.Font_ReadScaleFactor (out factor_x, out factor_y));

			return new OS.Coord (factor_x, factor_y);
		}

		public static void SetScaleFactor (OS.Coord factor)
		{
			OS.ThrowOnError (NativeMethods.Font_SetScaleFactor (factor.X, factor.Y));
		}

		/*! \brief void SetScaleFactor (int factorX, int factorY)
		 * Set the internal to OS conversion factor.
		 * \param factorX x scale factor
		 * \param factorY y scale factor
		 * \return Nothing */
		public static void SetScaleFactor (int factorX, int factorY)
		{
			OS.ThrowOnError (NativeMethods.Font_SetScaleFactor (factorX, factorY));
		}
	}
}
