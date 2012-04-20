//
// font.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;

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
					   OS.Transform transform,
					   int length)
			{
				NativeFont.PaintCoordBlock coordBlockNative =
						new NativeFont.PaintCoordBlock (coordBlock);
				NativeOS.Transform transformNative =
						new NativeOS.Transform (transform);

				/* The transformation matrix and coodinate block are being used,
				 * so make sure the relevant bits are set/clear.  */
				flags |= (1U << 5) | ( 1U << 6) | (1U << 8);
				flags &= ~(1U << 4) | (1U << 0);

				NativeMethods.Font_Paint (Handle,
							  s,
							  flags,
							  xPos,
							  yPos,
							  ref coordBlockNative,
							  ref transformNative,
							  length);
			}

			public void Paint (string s,
					   uint flags,
					   int xPos,
					   int yPos,
					   OS.Transform transform,
					   int length)
			{
				NativeOS.Transform transformNative =
						new NativeOS.Transform (transform);

				/* The transformation matrix is being used,
				 * so make sure the relevant bits are set/clear.  */
				flags |= (1U << 6) | (1U << 8);
				flags &= ~(1U << 5 | 1U << 4);

				NativeMethods.Font_Paint (Handle,
							  s,
							  flags,
							  xPos,
							  yPos,
							  IntPtr.Zero,
							  ref transformNative,
							  length);
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
				flags &= ~(1U << 6 | 1U << 4 | 1U << 0);

				NativeMethods.Font_Paint (Handle,
							  s,
							  flags,
							  xPos,
							  yPos,
							  ref coordBlockNative,
							  IntPtr.Zero,
							  length);
			}

			public void Paint (string s,
					   uint flags,
					   int xPos,
					   int yPos,
					   int length)
			{
				flags &= ~(1U << 5 | 1U << 6);
				NativeMethods.Font_Paint (Handle,
							  s,
							  flags,
							  xPos,
							  yPos,
							  IntPtr.Zero,
							  IntPtr.Zero,
							  length);
			}
		}
	}
}
