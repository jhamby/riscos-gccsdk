//
// toolbox/toolbox.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Collections;

namespace riscos
{
	public static partial class Toolbox
	{
		public enum CreateObjectFlags : uint
		{
			FromMemory = 0x1
		}

		public enum ShowObjectType : uint
		{
			Default,
			FullSpec,
			TopLeft,
			Centre,
			AtPointer
		}

		public enum ShowObjectFlags
		{
			AsMenu,
			AsSubMenu,
			AsNestedWindow
		}

		public delegate void ToolboxEventHandler (object sender, ToolboxEventArgs args);

		public class Object : IDisposable
		{
			public uint ID { get; protected set; }

			public Dictionary<uint, ToolboxEventHandler> ToolboxHandlers = new Dictionary<uint, ToolboxEventHandler>();
//			public event ToolboxEventHandler EventHandler;

			private bool disposed = false;

			protected Object ()
			{
				ID = 0;
			}

//			/*! \brief Create a toolbox object using the named template.
//			 * \param [in] resName Name of object in resource file to create.  */
//			public Object (string resName)
//			{
//				Create (resName);
//			}

			public Object (uint objectID)
			{
				ID = objectID;
			}

			~Object ()
			{
				Dispose (false);
			}

			public void Dispose ()
			{
				Dispose(true);
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
					Delete ();
					disposed = true;
				}
			}

			/*! \brief Create a toolbox object using the named template.
			 * \param [in] objectName Name of object in resource file to create.  */
			protected void Create (string objectName)
			{
				uint id;
				OS.ThrowOnError (NativeMethods.Toolbox_CreateObject (0, objectName, out id));
				ToolboxTask.AllObjects.Add (id, this);
				ID = id;
			}

			/*! \brief Create a toolbox object using the template pointed to.
			 * \param [in] templateData Pointer to template data to create object from.  */
			protected void Create (IntPtr templateData)
			{
				uint id;
				OS.ThrowOnError (NativeMethods.Toolbox_CreateObject (CreateObjectFlags.FromMemory,
										     templateData, out id));
				ToolboxTask.AllObjects.Add (id, this);
				ID = id;
			}

			/*! \brief Delete the object.  */
			private void Delete ()
			{
				ToolboxTask.AllObjects.Remove (ID);
				OS.ThrowOnError (NativeMethods.Toolbox_DeleteObject (0, ID));
			}

			/*! \brief Display the object on the screen at its default location.  */
			public void Show (uint parentID, int parentCmp)
			{
				OS.ThrowOnError (NativeMethods.Toolbox_ShowObject (0,
										   ID,
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
										   ID,
										   ShowObjectType.FullSpec,
										   ref block,
										   parentID,
										   parentCmp));
			}

			public void Show (NativeToolbox.ShowObjectTopLeftBlock block,
					  uint parentID, int parentCmp)
			{
				OS.ThrowOnError (NativeMethods.Toolbox_ShowObject (0,
										   ID,
										   ShowObjectType.TopLeft,
										   ref block,
										   parentID,
										   parentCmp));
			}

			public void OnEvent (ToolboxEvent ev)
			{
				ToolboxEventHandler handler;

				if (ToolboxHandlers.TryGetValue (ev.ToolboxArgs.Header.EventCode, out handler))
					handler (this, ev.ToolboxArgs);
			}
		}

		public class ToolboxEventArgs : EventArgs
		{
			public NativeToolbox.EventHeader Header;

			public ToolboxEventArgs (IntPtr unmanaged_event_block)
			{
				Header = (NativeToolbox.EventHeader)Marshal.PtrToStructure(
						unmanaged_event_block, typeof(NativeToolbox.EventHeader));
			}
		}

		public class ToolboxEvent : Wimp.Event
		{
			public ToolboxEventArgs ToolboxArgs;

			public ToolboxEvent (Wimp.PollCode type, IntPtr unmanaged_event_block) : base (type)
			{
				ToolboxArgs = new ToolboxEventArgs (unmanaged_event_block);
			}
		}

	}

	public class ToolboxTask : Task
	{
		public static Dictionary<uint, Toolbox.Object> AllObjects = new Dictionary<uint, Toolbox.Object>();

		public IntPtr SpriteArea;

		public void Initialise (uint wimp_version,
					int[] message_list,
					int[] event_list,
					string dir_name)
		{
			uint handle;

			OS.ThrowOnError (NativeMethods.Toolbox_Initialise (0,
									   wimp_version,
									   message_list,
									   event_list,
									   dir_name,
									   out WimpVersion,
									   out handle,
									   out SpriteArea));
			Handle = handle;
		}

		public override void Dispatch (Wimp.Event event_base)
		{
			NativeToolbox.IDBlock id_block = NativeMethods.Toolbox_GetIDBlock();

			switch (event_base.type)
			{
			case Wimp.PollCode.RedrawWindow:
				{
					Wimp.RedrawWindowEvent event_full = (Wimp.RedrawWindowEvent)event_base;
					Toolbox.Window tb_obj = (Toolbox.Window)ToolboxTask.AllObjects[id_block.SelfID];
					tb_obj.OnRedraw (event_full);
				}
				break;
			case Wimp.PollCode.ToolboxEvent:
				{
					Toolbox.ToolboxEvent event_full = (Toolbox.ToolboxEvent)event_base;
					Toolbox.Window tb_obj = (Toolbox.Window)ToolboxTask.AllObjects[id_block.SelfID];
					tb_obj.OnEvent (event_full);
				}
				break;
			}
		}
	}
}
