//
// NativeMethods/misc.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Runtime.InteropServices;

namespace riscos
{
	internal static partial class NativeMethods
	{
		// Reporter SWIs
		[DllImport("libriscosdotnet.so.1", EntryPoint="xreport_text0")]
		internal static extern void Report_Text0 (string s);
	}
}
