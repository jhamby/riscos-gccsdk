//
// colourtrans.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;

namespace riscos
{
	public static class ColourTrans
	{
		public const uint FullRed = 0x0000ff00U;
		public const uint FullGreen = 0x00ff0000U;
		public const uint FullBlue = 0xff000000U;
		public const uint Black = 0U;
		public const uint White = 0xffffff00U;

		public static void SetGCOL (OS.GCOLAction gcolAction,
					    uint paletteEntry)
		{
			int gcol, log2bpp;

			NativeMethods.ColourTrans_SetGCOL (paletteEntry,
							   0,
							   gcolAction,
							   out gcol,
							   out log2bpp);
		}

		public static void SetFontColours (uint bgHint, uint fill, int offset)
		{
			uint bg_hint_out, fill_out;
			int offset_out;

			NativeMethods.ColourTrans_SetFontColours (IntPtr.Zero,
								  bgHint,
								  fill,
								  offset,
								  out bg_hint_out,
								  out fill_out,
								  out offset_out);
		}

		public static void SetFontColours (IntPtr font, uint bgHint, uint fill, int offset)
		{
			uint bg_hint_out, fill_out;
			int offset_out;

			NativeMethods.ColourTrans_SetFontColours (font,
								  bgHint,
								  fill,
								  offset,
								  out bg_hint_out,
								  out fill_out,
								  out offset_out);
		}
	}
}
