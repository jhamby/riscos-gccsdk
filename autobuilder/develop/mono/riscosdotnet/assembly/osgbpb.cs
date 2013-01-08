//
// osgbpb.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;

namespace riscos
{
	public static class OSGBPB
	{
		public static int Write (IntPtr fileHandle, IntPtr data, int size, long filePtr)
		{
			int unwritten;

			OS.ThrowOnError (NativeMethods.OSGBPB_WriteAt (fileHandle,
								       data,
								       size,
								       (int)filePtr,
								       out unwritten));
			return unwritten;
		}

		public static int Write (IntPtr fileHandle, IntPtr data, int size)
		{
			int unwritten;

			OS.ThrowOnError (NativeMethods.OSGBPB_Write (fileHandle,
								     data,
								     size,
								     out unwritten));
			return unwritten;
		}

		public static int Read (IntPtr fileHandle, IntPtr data, int size, long filePtr)
		{
			int unread;

			OS.ThrowOnError (NativeMethods.OSGBPB_ReadAt (fileHandle,
								      data,
								      size,
								      (int)filePtr,
								      out unread));
			return unread;
		}

		public static int Read (IntPtr fileHandle, IntPtr data, int size)
		{
			int unread;

			OS.ThrowOnError (NativeMethods.OSGBPB_Read (fileHandle,
								    data,
								    size,
								    out unread));
			return unread;
		}

	}
}
