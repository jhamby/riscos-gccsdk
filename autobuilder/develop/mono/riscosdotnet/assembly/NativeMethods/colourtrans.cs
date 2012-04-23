//
// NativeMethods/colourtrans.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Runtime.InteropServices;

namespace riscos
{
	static partial class NativeMethods
	{
		// ColourTrans SWIs
		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_set_gcol")]
		internal static extern void ColourTrans_SetGCOL (uint palette_entry,
								 uint flags,
								 OS.GCOLAction gcol_action,
								 out int gcolOut,
								 out int log2BppOut);


		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_set_font_colours")]
		internal static extern void ColourTrans_SetFontColours (IntPtr font,
									uint bgHint,
									uint fill,
									int offset,
									out uint bgHintOut,
									out uint fillOut,
									out int offsetOut);
	}
}
 