//
// reporter.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using riscos;

namespace riscos
{
	public static class Reporter
	{
		public static void Output (string s)
		{
			NativeMethods.Report_Text0 (s);
		}

		public static void Output (int v)
		{
			Output (v.ToString ());
		}

		public static void Output (IntPtr v)
		{
			Output (v.ToString ("X8"));
		}

		public static void Output (uint v)
		{
			Output (v.ToString ("X8"));
		}
	}
}
