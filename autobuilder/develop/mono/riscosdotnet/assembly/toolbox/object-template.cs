//
// toolbox/object-template.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Runtime.InteropServices;
using System.Text;

namespace riscos
{
	public static partial class Toolbox
	{
		public static class ObjectTemplateOffset
		{
			public const int Class = 0;
			public const int Flags = 4;
			public const int Version = 8;
			public const int Name = 12;
			public const int Size = 24;
			public const int BodyOffset = 28;
			public const int BodySize = 32;
			public const int TemplateHeaderSize = 36;
		}

		/*! \brief Object type used as the base object of all Toolbox object template classes.  */
		public class ObjectTemplate : IDisposable
		{
			protected int ObjectClass;
			protected int ObjectFlags;
			protected int Version;
			protected string Name;
			protected int TotalObjectSize;

			protected IntPtr _buffer;
			public IntPtr Buffer {
				get { return _buffer; }
			}

			private bool disposed = false;

			/*! \brief Create a Toolbox object template.
			 * \param [in] objectClass The Toolbox class of the object.
			 * \param [in] flags The flags to be passed to the Toolbox on creation.
			 * \param [in] version The version of this object template.
			 * \param [in] name The name to give the object template.  */
			public ObjectTemplate (int objectClass, int flags, int version, string name)
			{
				if (name.Length >= 12)
					throw new ArgumentException ("Template name must be no longer than 12 characters including terminator", "name");
				if (string.IsNullOrEmpty (name))
					throw new ArgumentException ("Template must be given a name", "name");

				ObjectClass = objectClass;
				ObjectFlags = flags;
				Version = version;
				Name = name;

				_buffer = IntPtr.Zero;
			}

			~ObjectTemplate ()
			{
				Dispose (false);
			}

			public void Dispose ()
			{
				Dispose (true);
				// This object will be cleaned up by the Dispose method.
				// Call GC.SupressFinalize to take this object off the
				// finalization queue and prevent finalization code for
				// this object from executing a second time.
				GC.SuppressFinalize(this);
			}

			protected virtual void Dispose (bool disposing)
			{
				if (!this.disposed)
				{
					if (_buffer != IntPtr.Zero)
					{
						Marshal.FreeHGlobal (_buffer);
						_buffer = IntPtr.Zero;
					}
					disposed = true;
				}
			}

			protected virtual int CalculateBufferSize (out int strStart, out int msgStart)
			{
				strStart = msgStart = 0;

				return ObjectTemplateOffset.TemplateHeaderSize;
			}

			/*! \brief Calculate the size of buffer required for this Toolbox Object template.
			 * allocate it and build the template.  */
			public void CreateBuffer ()
			{
				int str_offset, msg_offset;

				TotalObjectSize = CalculateBufferSize (out str_offset, out msg_offset);

				_buffer = Marshal.AllocHGlobal (TotalObjectSize);

				BuildBuffer (str_offset, msg_offset);
			}

			/*! \brief Write the object template header data into the buffer.
			 * \param [in] strOffset The offset from the start of the template where the
			 * string table begins.
			 * \param [in] msgOffset The offset from the start of the template where the
			 * message table begins.
			 * \return Nothing.  */
			protected virtual void BuildBuffer (int strOffset, int msgOffset)
			{
				Marshal.WriteInt32 (_buffer, ObjectTemplateOffset.Class, ObjectClass);
				Marshal.WriteInt32 (_buffer, ObjectTemplateOffset.Flags, ObjectFlags);
				Marshal.WriteInt32 (_buffer, ObjectTemplateOffset.Version, Version);

				byte[] strbuf = Encoding.UTF8.GetBytes (Name);
				int dst_addr = (int)_buffer + ObjectTemplateOffset.Name;
				int to_copy = strbuf.Length;

				Marshal.Copy (strbuf,
					      0,
					      (IntPtr)dst_addr,
					      to_copy);
				// Zero terminate.
				Marshal.WriteByte ((IntPtr)(dst_addr + to_copy), 0);

				Marshal.WriteInt32 (_buffer, ObjectTemplateOffset.Size,
						    TotalObjectSize);
				Marshal.WriteInt32 (_buffer, ObjectTemplateOffset.BodyOffset,
						    (int)_buffer + ObjectTemplateOffset.BodySize + 4);
				Marshal.WriteInt32 (_buffer, ObjectTemplateOffset.BodySize,
						    GetBodySize ());
			}

			protected virtual int GetBodySize ()
			{
				return 0;
			}

			/*! \brief Write a raw string (as might be stored in an icon buffer) to a
			 * template buffer.
			 * \param [in] s Pointer to the string to write.
			 * \param [in] sMaxLen The maximum size of the string. Can be greater than the
			 * length of the string in which case sMaxLen bytes are reserved.
			 * \param [in] buffer Pointer to the buffer to write the string to.
			 * \param [in] bodyOffset The offset from the start of the template where the
			 * address of the string (in the string table) is written. If sMaxLen indicates
			 * that there is no string then 0 is written to reflect that.
			 * \param [in] tableOffset The offset from the start of the template where the
			 * string is written in the string/message table.
			 * \return The new offset where the next string should be placed after enough
			 * space has been reserved for this one.  */
			public static int WriteString (IntPtr s,
						       int sMaxLen,
						       IntPtr buffer,
						       int bodyOffset,
						       int tableOffset)
			{
				if (sMaxLen <= 1) // Terminator only?
					Marshal.WriteInt32 (buffer, bodyOffset, 0);
				else
				{
					byte[] bytebuf = new byte [sMaxLen];

					// Copy to intermediate byte buffer
					Marshal.Copy (s, bytebuf, 0, sMaxLen);

					IntPtr dst_addr = (IntPtr)((int)buffer + tableOffset);

					Marshal.WriteInt32 (buffer, bodyOffset, (int)dst_addr);

					int to_copy = sMaxLen;
					Marshal.Copy (bytebuf,
						      0,
						      dst_addr,
						      to_copy);
					tableOffset += sMaxLen;
				}

				return tableOffset;
			}

			/*! \brief Write a string to a template buffer.
			 * \param [in] s The string to write.
			 * \param [in] sMaxLen The maximum size of the string. Can be greater than the
			 * length of the string in which case sMaxLen bytes are reserved.
			 * \param [in] buffer Pointer to the buffer to write the string to.
			 * \param [in] bodyOffset The offset from the start of the template where the
			 * address of the string (in the string table) is written. If sMaxLen indicates
			 * that there is no string then 0 is written to reflect that.
			 * \param [in] tableOffset The offset from the start of the template where the
			 * string is written in the string/message table.
			 * \return The new offset where the next string should be placed after enough
			 * space has been reserved for this one.  */
			public static int WriteString (string s,
						       int sMaxLen,
						       IntPtr buffer,
						       int bodyOffset,
						       int tableOffset)
			{
				if (string.IsNullOrEmpty (s))
				{
					Marshal.WriteInt32 (buffer, bodyOffset, 0);
				}
				else
				{
					IntPtr dst_addr = (IntPtr)((int)buffer + tableOffset);

					Marshal.WriteInt32 (buffer, bodyOffset, (int)dst_addr);

					byte[] strbuf = Encoding.UTF8.GetBytes (s);
					int to_copy = strbuf.Length;
					Marshal.Copy (strbuf,
						      0,
						      dst_addr,
						      to_copy);
					// Zero terminate.
					Marshal.WriteByte (dst_addr, to_copy, 0);

					tableOffset = (sMaxLen == 0) ?
						      tableOffset + s.Length + 1 :
						      tableOffset + sMaxLen;
				}

				return tableOffset;
			}
		}
	}
}
