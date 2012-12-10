//
// NativeMethods/os.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Runtime.InteropServices;

namespace riscos
{
	// Structures that need to be laid out so that they match those used
	// by RISC OS and may need to be embedded in other objects.
	// Embedded objects have to be structs as classes are reference objects.
	// Embedding a reference object means we embed a pointer to it rather
	// than the object itself.
	public static class NativeOS
	{
		// The standard os_error structure
		[StructLayout(LayoutKind.Sequential)]
		public struct Error
		{
			public int ErrNum;

			[MarshalAs(UnmanagedType.ByValTStr, SizeConst=252)]
			public string ErrMess;

			public Error (int errNum, string errMess)
			{
				ErrNum = errNum;
				ErrMess = errMess;
			}
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct Rect
		{
			public int MinX;
			public int MinY;
			public int MaxX;
			public int MaxY;

			public Rect (OS.Rect rect)
			{
				MinX = rect.MinX;
				MinY = rect.MinY;
				MaxX = rect.MaxX;
				MaxY = rect.MaxY;
			}
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct Coord
		{
			public int X;
			public int Y;

			public Coord (OS.Coord coord)
			{
				X = coord.X;
				Y = coord.Y;
			}
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct Matrix
		{
			// Individual ints are easier to marshal than a 3x2 array and this
			// is how the PRMs explain it.
			public int a; // x scale factor, or cos(angle) to rotate
			public int b; // sin(angle) to rotate
			public int c; // -sin(angle) to rotate
			public int d; // y scale factor, or cos(angle) to rotate
			public int e; // x translation
			public int f; // y translation

			// Construct a native transformation matrix from a managed one
			public Matrix (OS.Matrix matrix)
			{
				a = matrix.m[0,0];
				b = matrix.m[0,1];
				c = matrix.m[1,0];
				d = matrix.m[1,1];
				e = matrix.m[2,0];
				f = matrix.m[2,1];
			}
		}
	}

	internal static partial class NativeMethods
	{
		// OS SWIs
		[DllImport("libriscosdotnet.so.1", EntryPoint="xos_plot")]
		internal static extern void OS_Plot (int command, int x, int y);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xoswordreadclock_utc")]
		internal static extern void OSWord_ReadClockUtc ([In,Out,MarshalAs(UnmanagedType.LPArray, SizeConst=5)]
								    byte [] utc);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xos_read_monotonic_time")]
		internal static extern void OS_ReadMonotonicTime (out uint time);

		// RISC OS 3.5+
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosdynamicarea_create")]
		internal static extern IntPtr OS_DynamicAreaCreate (int number,
								    int size,
								    int baseAddress,
								    OS.DynamicAreaFlags flags,
								    int sizeLimit,
								    IntPtr handler,
								    IntPtr workspace,
								    string name,
								    out int number_out,
								    out IntPtr baseAddress_out,
								    out int sizeLimit_out);

		// RISC OS 3.5+
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosdynamicarea_delete")]
		internal static extern IntPtr OS_DynamicAreaDelete (int number);

		// RISC OS 3.5+
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosdynamicarea_read")]
		internal static extern IntPtr OS_DynamicAreaRead (int number,
								  out int size,
								  out IntPtr baseAddress,
								  out OS.DynamicAreaFlags flags,
								  out int sizeLimit,
								  out IntPtr handler,
								  out IntPtr workspace,
								  out IntPtr name);

		// RISC OS 3.5+
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosdynamicarea_enumerate")]
		internal static extern IntPtr OS_DynamicAreaEnumerate (int context, out int context_out);

		// RISC OS 3.5+
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosdynamicarea_renumber")]
		internal static extern IntPtr OS_DynamicAreaRenumber (int old_number, int new_number);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xos_change_dynamic_area")]
		internal static extern IntPtr OS_ChangeDynamicArea (int number,
								    int sizeIncrease,
								    out uint sizeIncrease_out);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xosheap_initialise")]
		internal static extern IntPtr OS_HeapInitialise (IntPtr heap, int size);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xosheap_describe")]
		internal static extern IntPtr OS_HeapDescribe (IntPtr heap,
							       out int largestBlockSize,
							       out int totalFree);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xosheap_alloc")]
		internal static extern IntPtr OS_HeapAlloc (IntPtr heap,
							    int size,
							    out IntPtr block);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xosheap_free")]
		internal static extern IntPtr OS_HeapFree (IntPtr heap, IntPtr block);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xosheap_realloc")]
		internal static extern IntPtr OS_HeapRealloc (IntPtr heap,
							      IntPtr block,
							      int sizeIncrease,
							      out IntPtr block_out);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xosheap_resize")]
		internal static extern IntPtr OS_HeapResize (IntPtr heap,
							     int sizeIncrease);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xosheap_read_size")]
		internal static extern IntPtr OS_HeapReadSize (IntPtr heap,
							       IntPtr block,
							       out int size);
	}
}
