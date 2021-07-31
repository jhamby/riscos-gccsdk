//
// osargs.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Text;

namespace riscos
{
	public static class OSArgs
	{
		/*! \brief Return the position of the given open file.
		 * \note Calls SWI "OS_Args",0.  */
		public static long ReadFilePtr (IntPtr fileHandle)
		{
			int file_ptr;

			OS.ThrowOnError (NativeMethods.OSArgs_ReadFilePtr (fileHandle, out file_ptr));

			return (long)file_ptr;
		}

		/*! \brief Set the position of the given open file.
		 * \note Calls SWI "OS_Args",1.  */
		public static void SetFilePtr (IntPtr fileHandle, long filePtr)
		{
			OS.ThrowOnError (NativeMethods.OSArgs_SetFilePtr (fileHandle, (int)filePtr));
		}

		/*! \brief Return the extent of the given open file.
		 * \note Calls SWI "OS_Args",2.  */
		public static long ReadFileExt (IntPtr fileHandle)
		{
			int file_ext;

			OS.ThrowOnError (NativeMethods.OSArgs_ReadFileExt (fileHandle, out file_ext));

			return (long)file_ext;
		}

		/*! \brief Set the extent of the given open file.
		 * \note Calls SWI "OS_Args",3.  */
		public static void SetFileExt (IntPtr fileHandle, long fileExt)
		{
			OS.ThrowOnError (NativeMethods.OSArgs_SetFileExt (fileHandle, (int)fileExt));
		}

		/*! \brief Return the allocated size of the given open file.
		 * \note Calls SWI "OS_Args",4.  */
		public static long ReadFileAllocation (IntPtr fileHandle)
		{
			int file_alloc;

			OS.ThrowOnError (NativeMethods.OSArgs_ReadFileAllocation (fileHandle,
										  out file_alloc));

			return (long)file_alloc;
		}

		/*! \brief Return the end-of-file status of the given open file.
		 * \note Calls SWI "OS_Args",5.  */
		public static bool ReadEOF (IntPtr fileHandle)
		{
			int eof;

			OS.ThrowOnError (NativeMethods.OSArgs_ReadFileEOF (fileHandle, out eof));

			return eof != 0;
		}

		/*! \brief Set the allocated size of the given open file.
		 * \return How much space the filing system actually allocated.
		 * \note Calls SWI "OS_Args", 6.  */
		public static long SetFileAllocation (IntPtr fileHandle, long fileAllocation)
		{
			int actual_allocation;

			OS.ThrowOnError (NativeMethods.OSArgs_SetFileAllocation (fileHandle,
										 (int)fileAllocation,
										 out actual_allocation));
			return (long)actual_allocation;
		}

		/*! \brief Converts a file handle to a canonicalised name.
		 * \param [in] fileHandle The handle of the file.
		 * \param [in] buffer A StringBuilder buffer in which to return the result. Can be
		 * \e null to read the size of buffer required.
		 * \param [in] bufferSize The size of the buffer given. Should be 0 to read size required.
		 * \return Either -(length of name) if \b buffer and \b bufferSize are \e null
		 * and \e 0 respectively, or the number of spare bytes in the buffer including the
		 * null terminator.
		 * \note Calls SWI "OS_Args", 7.
		 * \note When reading the required size of buffer, the size returned will be negative.
		 * The buffer size is therefore (1-result).  */
		public static int ReadFilename (IntPtr fileHandle, StringBuilder buffer, int bufferSize)
		{
			int spare;

			OS.ThrowOnError (NativeMethods.OSArgs_ReadFilename (fileHandle,
									    buffer,
									    bufferSize,
									    out spare));

			return spare;
		}

		/*! \brief Reads information on the given file handle.
		 * \note Calls SWI "OS_Args", 254.  */
		public static int ReadInfo (IntPtr fileHandle)
		{
			int stream_status;
			int info_word;

			OS.ThrowOnError (NativeMethods.OSArgs_ReadInfo (fileHandle,
									out stream_status,
									out info_word));

			return stream_status;
		}

		/*! \brief Ensure data has been written to a file, or all files on the temporary filing
		 * system.
		 * \note Calls SWI "OS_Args", 255.  */
		public static void EnsureData (IntPtr fileHandle)
		{
			OS.ThrowOnError (NativeMethods.OSArgs_EnsureData (fileHandle));
		}
	}
}
