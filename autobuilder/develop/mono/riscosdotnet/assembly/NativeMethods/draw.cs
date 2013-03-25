//
// NativeMethods/draw.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Runtime.InteropServices;

namespace riscos
{
	internal static partial class NativeMethods
	{
		[DllImport("libriscosdotnet.so.1", EntryPoint="xdrawfile_render")]
		internal static extern IntPtr Drawfile_Render (Drawfile.RenderFlags flags,
							       [In] byte [] drawfile,
							       int size,
							       [In] OS.Matrix matrix,
							       ref NativeOS.Rect clip,
							       int flatness);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xdrawfile_render")]
		internal static extern IntPtr Drawfile_Render (Drawfile.RenderFlags flags,
							       [In] byte [] drawfile,
							       int size,
							       [In] OS.Matrix matrix,
							       IntPtr clip,
							       int flatness);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xdrawfile_bbox")]
		internal static extern IntPtr Drawfile_GetBBox (int flags,
								[In] byte [] drawfile,
								int size,
								[In] OS.Matrix matrix,
								[Out] OS.Rect bbox_out);
	}
}
