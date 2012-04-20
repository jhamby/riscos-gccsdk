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

			public void Show (NativeToolbox.ShowObjectFullSpecBlock block,
					  uint parentID, int parentCmp)
			{
				OS.ThrowOnError (NativeMethods.Toolbox_ShowObject (0,
										   ObjectID,
										   ShowObjectType.FullSpec,
										   ref block,
										   parentID,
										   parentCmp));				
			}

			public void Show (NativeToolbox.ShowObjectTopLeftBlock block,
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

			NativeToolbox.IDBlock IDBlock;

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
	}
}
