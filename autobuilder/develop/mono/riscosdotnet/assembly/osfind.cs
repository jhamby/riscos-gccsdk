//
// osfind.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;

namespace riscos
{
	public static class OSFind
	{
		//! \brief The various flags that can be used with OSFind.
		public enum Flags
		{
			UseFilePath,
			UsePathString,
			UsePathVariable,
			UseNoPath,
			ErrorIfDir,
			ErrorIfAbsent = 8
		}

		/*! \brief Open an existing file with read only access.
		* \param [in] flags The flags used to modify where the file is opened and how errors
		* are handled.
		* \param [in] filename Name of the file to open.
		* \param [in] path Name of path, path string, path variable or \e null (depending on flags).
		* \return Handle of file.
		* \note Calls SWI "OS_Find".  */
		public static IntPtr OpenInput (Flags flags, string filename, string path)
		{
			IntPtr handle;

			OS.ThrowOnError (NativeMethods.OSFind_Openin (flags, filename, path, out handle));

			return handle;
		}

		/*! \brief Create a new file with read/write access.
		* \param [in] flags The flags used to modify where the file is created and how errors
		* are handled.
		* \param [in] filename Name of the file to create.
		* \param [in] path Name of path, path string, path variable or \e null (depending on flags).
		* \return Handle of file.
		* \note Calls SWI "OS_Find".  */
		public static IntPtr OpenOutput (Flags flags, string filename, string path)
		{
			IntPtr handle;

			OS.ThrowOnError (NativeMethods.OSFind_Openout (flags, filename, path, out handle));

			return handle;
		}

		/*! \brief Open an existing file with read/write access.
		* \param [in] flags The flags used to modify where the file is opened and how errors
		* are handled.
		* \param [in] filename Name of the file to open.
		* \param [in] path Name of path, path string, path variable or \e null (depending on flags).
		* \return Handle of file.
		* \note Calls SWI "OS_Find".  */
		public static IntPtr OpenUpdate (Flags flags, string filename, string path)
		{
			IntPtr handle;

			OS.ThrowOnError (NativeMethods.OSFind_Openup (flags, filename, path, out handle));

			return handle;
		}

		//! \brief Close the given file handle.
		public static void Close (IntPtr fileHandle)
		{
			if (fileHandle == IntPtr.Zero)
				return;

			// I don't think an exception should be thrown on error here. If this
			// is being called as part of a clean up from a file exception elsewhere,
			// then we don't want to have to deal with another exception at the same time.
			NativeMethods.OSFind_Close (fileHandle);
		}
	}
}
