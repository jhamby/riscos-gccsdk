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
		public static void WriteLine (string s)
		{
			NativeMethods.Report_Text0 (s);
		}

		public static void WriteLine (int v)
		{
			WriteLine (v.ToString ());
		}

		public static void WriteLine (IntPtr v)
		{
			WriteLine (v.ToString ("X8"));
		}

		public static void WriteLine (uint v)
		{
			WriteLine (v.ToString ("X8"));
		}

		public static void WriteLine (string format, params object[] args)
		{
			WriteLine (string.Format (format, args));
		}
	}
}
