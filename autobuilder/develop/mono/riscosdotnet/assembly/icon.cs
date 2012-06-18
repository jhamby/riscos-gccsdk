//
// icon.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Runtime.InteropServices;
using System.Text;
using riscos;

namespace riscos
{
	public static partial class Wimp
	{
		public class IconBuffer
		{
			public IntPtr Buffer = IntPtr.Zero;
			public int BufferSize = 0;

			public IconBuffer ()
			{
				Set ("");
			}

			public IconBuffer (string text)
			{
				Set (text);
			}

			public IconBuffer (string text,
					   int maxLen)
			{
				Set (text, maxLen);
			}

			~IconBuffer ()
			{
				Clear ();
			}

			public void Clear ()
			{
				if (Buffer != IntPtr.Zero)
				{
					Marshal.FreeHGlobal (Buffer);
					Buffer = IntPtr.Zero;
					BufferSize = 0;
				}
			}

			public void Set (string text)
			{
				Set (text, text.Length + 1);
			}

			public void Set (string text,
				    	 int maxLen)
			{
				if (Buffer == IntPtr.Zero)
				{
					Buffer = Marshal.AllocHGlobal (maxLen);
					BufferSize = maxLen;
				}

				byte[] strbuf = Encoding.UTF8.GetBytes (text);
				int to_copy = strbuf.Length;

				if (to_copy >= BufferSize)
					to_copy = BufferSize;
				Marshal.Copy (strbuf, 0, Buffer, to_copy);

				// Zero terminate
				Marshal.WriteByte ((IntPtr)(Buffer.ToInt32() + to_copy), 0);
			}
		}

		public class IconData
		{
			public IconBuffer Text;
			public IconBuffer Validation;

			public IconData (IconBuffer text, IconBuffer validation)
			{
				Text = text;
				Validation = validation;
			}

			public IconData (IconBuffer text) : this (text, new IconBuffer (""))
			{
			}

			public void SetText (string text, int maxLen)
			{
				Text.Set (text, maxLen);
			}

			public void SetText (string text)
			{
				Text.Set (text);
			}

			public void SetValidation (string text, int maxLen)
			{
				Validation.Set (text, maxLen);
			}

			public void SetValidation (string text)
			{
				Validation.Set (text);
			}
		}

		public class Icon
		{
			public uint Handle;

			IconData Data;
	
			bool Created { get; set; }

			public Icon ()
			{
			}

			~Icon ()
			{
			}

			public void Create (uint windowHandle,
					    OS.Rect boundingBox,
					    uint flags,
					    IconData data,
					    uint priority)
			{
				NativeWimp.IconCreateBlock icon_block =
						new NativeWimp.IconCreateBlock (windowHandle,
										boundingBox,
										flags,
										data);
				Data = data;
				OS.ThrowOnError (NativeMethods.Wimp_CreateIcon (ref icon_block,
										out Handle));
				Created = true;
			}

			public void Create (uint windowHandle,
					    OS.Rect boundingBox,
					    uint flags,
					    IconData data)
			{
				Create (windowHandle, boundingBox, flags, data, 0);
			}

			public void Delete (uint windowHandle)
			{
				OS.ThrowOnError (NativeMethods.Wimp_DeleteIcon (windowHandle, Handle));
			}

			public void SetText (string text)
			{
				SetText (text, text.Length + 1);
			}

			public void SetText (string text, int maxLen)
			{
				Data.Text.Set (text, maxLen);
			}

			public void SetValidation (string text)
			{
				SetValidation (text, text.Length + 1);
			}

			public void SetValidation (string text, int maxLen)
			{
				Data.Validation.Set (text, maxLen);
			}

		}
	}
}
 
