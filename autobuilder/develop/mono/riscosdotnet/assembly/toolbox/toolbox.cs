//
// toolbox/toolbox.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Collections;
using System.Text;

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

		/*! \exception UnknownObjectException
		 * \brief Exception thrown by a Toolbox application when an object
		 * with an ID that is unknown to the application is encountered.  */
		public class UnknownObjectException : System.Exception
		{
			public uint ObjectID;

			public UnknownObjectException (uint objectID) : base ()
			{
				ObjectID = objectID;
			}
		}

		/*! \class ClickShow
		 * \brief Used to pass a toolbox object and flags to methods that set/read a click->show
		 * object.  */
		public class ClickShow
		{
			/*! \brief Bit 0 clear - Show object persistently.  */
			public const uint Persistent = 0;
			/*! \brief Bit 0 set - Show object transiently.  */
			public const uint Transient = 1;

			/*! \brief The object to be shown.  */
			public Toolbox.Object Object;

			/*! \brief Flags used to indicate whether a component that can show an object when
			 * clicked should show the object persistently or transiently.  */
			public uint Flags;

			/*! \brief Create a ClickShow object.
			 * \param [in] objID The ID of the Toolbox object that will be shown.
			 * \param [in] flags Set to \e ClickShow.Persistent or \e ClickShow.Transient.
			 * \exception UnknownObjectException The object ID is not wrapped in a
			 * class object and is therefore unknown.  */
			public ClickShow (uint objID, uint flags)
			{
				if (!ToolboxTask.AllObjects.TryGetValue (objID, out Object))
					throw new UnknownObjectException (objID);
				Flags = flags;
			}

			/*! \brief Create a ClickShow object.
			 * \param [in] tbObj The Toolbox object that will be shown.
			 * \param [in] flags Set to \e ClickShow.Persistent or \e ClickShow.Transient.  */
			public ClickShow (Object tbObj, uint flags)
			{
				Object = tbObj;
				Flags = flags;
			}
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

			/*! \brief Create a toolbox object using the named template.
			 * \param [in] resName Name of object in resource file to create.  */
			protected Object (string resName)
			{
				Create (resName);
			}

			protected Object (uint objectID)
			{
				ID = objectID;
				ToolboxTask.AllObjects.Add (objectID, this);
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
					ToolboxTask.AllObjects.Remove (ID);
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

			public void Hide ()
			{
				OS.ThrowOnError (NativeMethods.Toolbox_HideObject (0, ID));
			}

			/*! \brief Return \e true if this object is currently showing on screen.  */
			public bool IsOnScreen ()
			{
				uint state;

				OS.ThrowOnError (NativeMethods.Toolbox_GetObjectState (0, ID, out state));

				return (state & 1) != 0;
			}

			public void OnEvent (ToolboxEvent ev)
			{
				ToolboxEventHandler handler;

				if (ToolboxHandlers.TryGetValue (ev.ToolboxArgs.Header.EventCode, out handler))
					handler (this, ev.ToolboxArgs);
			}

			//
			// Generic protected methods that can be tailored to a derived Toolbox object
			// by passing a Toolbox method number.
			// 

			protected void SetText (int method, string text)
			{
				OS.ThrowOnError (NativeMethods.Object_SetText (0,
									       ID,
									       method,
									       text));
			}

			protected string GetText (int method)
			{
				int buffer_size;
				OS.ThrowOnError (NativeMethods.Object_GetText (0,
									       ID,
									       method,
									       null,
									       0,
									       out buffer_size));
				StringBuilder buffer = new StringBuilder (buffer_size);
				OS.ThrowOnError (NativeMethods.Object_GetText (0,
									       ID,
									       method,
									       buffer,
									       buffer_size,
									       out buffer_size));
				return buffer.ToString ();
			}

			protected uint GetHandle (int method)
			{
				uint handle;

				OS.ThrowOnError (NativeMethods.Object_GetR0 (0, ID, method, out handle));

				return handle;
			}

			protected void SetMenu (int method, Menu menu)
			{
				OS.ThrowOnError (NativeMethods.Object_SetR3 (0,
									     ID,
									     method,
									     (menu == null) ? 0 : menu.ID));
			}

			protected Menu GetMenu (int method)
			{
				uint menu_id;

				OS.ThrowOnError (NativeMethods.Object_GetR0 (0, ID, method, out menu_id));

				Toolbox.Object tb_obj;
				if (!ToolboxTask.AllObjects.TryGetValue (menu_id, out tb_obj))
					throw new UnknownObjectException (menu_id);

				return (Menu)tb_obj;
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

		public static IntPtr TemplateLookup (string resName)
		{
			IntPtr template;

			OS.ThrowOnError (NativeMethods.Toolbox_TemplateLookUp (0, resName, out template));

			return template;
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
					Toolbox.Object tb_obj;
					if (ToolboxTask.AllObjects.TryGetValue (id_block.SelfID, out tb_obj))
					{
						tb_obj.OnEvent (event_full);
					}
				}
				break;
			}
		}
	}
}
