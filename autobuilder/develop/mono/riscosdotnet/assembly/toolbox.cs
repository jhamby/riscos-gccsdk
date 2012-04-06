//
// toolbox.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Runtime.InteropServices;
using System.Collections;

namespace riscos
{
	namespace Toolbox
	{
		enum CreateObjectFlags : uint
		{
			FromMemory = 0x1
		}

		enum ShowObjectType : uint
		{
			Default,
			FullSpec,
			TopLeft,
			Centre,
			AtPointer
		}

		enum ShowObjectFlags
		{
			AsMenu,
			AsSubMenu,
			AsNestedWindow
		}

		public class ToolboxObject
		{
			uint ObjectID;

			public ToolboxObject (uint objectID)
			{
				ObjectID = objectID;
			}

			// Create a toolbox object using the named template.
			public void Create (string objectName)
			{
				OS.ThrowOnError (NativeMethods.Toolbox_CreateObject (0, objectName, ref ObjectID));
			}

			// Create a toolbox object using the template pointed to.
			public void Create (IntPtr template)
			{
				OS.ThrowOnError (NativeMethods.Toolbox_CreateObject (CreateObjectFlags.FromMemory,
										     template, ref ObjectID));
			}

			public void Delete ()
			{
				OS.ThrowOnError (NativeMethods.Toolbox_DeleteObject (0, ObjectID));
			}

			public void Show (uint parentID, int parentCmp)
			{
				OS.ThrowOnError (NativeMethods.Toolbox_ShowObject (0,
										   ObjectID,
										   ShowObjectType.Default,
										   IntPtr.Zero,
										   parentID,
										   parentCmp));
			}

			public void Show ()
			{
				Show (0, -1);
			}

			public void Show (ShowObjectFullSpecBlock block,
					  uint parentID, int parentCmp)
			{
				OS.ThrowOnError (NativeMethods.Toolbox_ShowObject (0,
										   ObjectID,
										   ShowObjectType.FullSpec,
										   ref block,
										   parentID,
										   parentCmp));				
			}

			public void Show (ShowObjectTopLeftBlock block,
					  uint parentID, int parentCmp)
			{
				OS.ThrowOnError (NativeMethods.Toolbox_ShowObject (0,
										   ObjectID,
										   ShowObjectType.TopLeft,
										   ref block,
										   parentID,
										   parentCmp));				
			}
		}

		public class ToolboxTask : Task
		{
			public static Hashtable AllObjects = new Hashtable (); // Toolbox objects

			uint[] MesstransFD = new uint[4];

			Toolbox.IDBlock IDBlock;

			public IntPtr SpriteArea;

			public void Initialise (uint wimp_version,
						uint[] message_list,
						uint[] event_list,
						string dir_name)
			{
				uint handle;
				int version;

				OS.ThrowOnError (NativeMethods.Toolbox_Initialise (0,
										   wimp_version,
										   message_list,
										   event_list,
										   dir_name,
										   ref MesstransFD,
										   ref IDBlock,
										   out version,
										   out handle,
										   out SpriteArea));
				Handle = handle;
			}
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct IDBlock
		{
			public uint	ancestor_id;
			public uint	ancestor_cmp;
			public uint	parent_id;
			public uint	parent_cmp;
			public uint	self_id;
			public uint	self_cmp;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct ShowObjectFullSpecBlock
		{
			public NativeOS.Rect Visible;
			public NativeOS.Coord Scroll;
			public uint BehindWindow;

			public uint WindowFlags;
			public uint ParentWindowHandle;
			public uint AlignmentFlags;

			public ShowObjectFullSpecBlock (OS.Rect visible,
							OS.Coord scroll)
			{
				Visible = new NativeOS.Rect (visible);
				Scroll = new NativeOS.Coord (scroll);
				BehindWindow = Wimp.WindowStackPosition.Top;
				WindowFlags = 0;
				ParentWindowHandle = 0;
				AlignmentFlags = 0;
			}
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct ShowObjectTopLeftBlock
		{
			public NativeOS.Coord TopLeft;

			public ShowObjectTopLeftBlock (OS.Coord topLeft)
			{
				TopLeft = new NativeOS.Coord (topLeft);
			}
		}
	}
}
