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

		public static class Class
		{
			public const int Window = 0x82880;
			public const int Menu = 0x828c0;
			public const int Iconbar = 0x82900;
			public const int ColourMenu = 0x82980;
			public const int ColourDbox = 0x829c0;
			public const int FontDbox = 0x82a00;
			public const int FontMenu = 0x82a40;
			public const int FileInfo = 0x82ac0;
			public const int PrintDbox = 0x82b00;
			public const int ProgInfo = 0x82b40;
			public const int SaveAs = 0x82bc0;
			public const int Scale = 0x82c00;
		}

		public static class EventHeaderOffset
		{
			public const int Size = 0;
			public const int Ref = 4;
			public const int EventCode = 8;
			public const int Flags = 12;
		}

		public static class TemplateOffset
		{
			public const int Class = 0;
			public const int Flags = 4;
			public const int Version = 8;
			public const int Name = 12;
			public const int Size = 24;
			public const int BodyOffset = 28;
			public const int BodySize = 32;
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
			 * \exception UnknownObjectException Thrown if the class of the given object is not known.  */
			public ClickShow (uint objID, uint flags)
			{
				Object = Object.LookupOrWrap (objID);
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

		/*! \brief Base object for the details of how an object is shown on screen.  */ 
		public class ShowObjectSpec
		{
			public ShowObjectType Type;

			public ShowObjectSpec (ShowObjectType type)
			{
				Type = type;
			}
		}

		/*! \brief %Object used to specify/receive the full details of how an object is shown on screen.  */
		public class ShowObjectFull : ShowObjectSpec
		{
			/*! \brief The visible area of the object to be shown.  */
			public OS.Rect Visible;
			/*! \brief The scroll offsets of the object to be shown.  */
			public OS.Coord Scroll;
			/*! \brief The position in the window stack that the object will be shown.  */
			public uint StackPosition;

			// The following are valid only for nested windows.
			/*! \brief The window flags to used when nesting the window.  */
			public uint WindowFlags;
			/*! \brief The handle of the window in which to nest the one being shown.  */
			public uint ParentWindowHandle;
			/*! \brief Flags indicating how the nested window should be aligned with the parent.  */
			public uint AlignmentFlags;

			/*! \brief Create a full show object specification from the details given.
			 *
			 * Allows access to full window nesting parameters.  */
			public ShowObjectFull (OS.Rect visible,
					       OS.Coord scroll,
					       uint stackPosition,
					       uint windowFlags,
					       uint parentWindowHandle,
					       uint alignmentFlags) : base (ShowObjectType.FullSpec)
			{
				Visible = visible;
				Scroll = scroll;
				StackPosition = stackPosition;
				WindowFlags = windowFlags;
				ParentWindowHandle = parentWindowHandle;
				AlignmentFlags = alignmentFlags;
			}

			/*! \brief Create a full show object specification from the details given.
			 *
			 * Assumes that window nesting is not required, and so assumes default values for
			 * nesting parameters.  */
			public ShowObjectFull (OS.Rect visible,
					       OS.Coord scroll) : base (ShowObjectType.FullSpec)
			{
				Visible = visible;
				Scroll = scroll;
				StackPosition = Wimp.WindowStackPosition.Top;
				WindowFlags = 0;
				ParentWindowHandle = 0;
				AlignmentFlags = 0;
			}
		}

		/*! \brief A show object specification used to specify the top left hand corner of
		 * an object to be shown on screen.  */
		public class ShowObjectTopLeft : ShowObjectSpec
		{
			/*! \brief The top left hand corner of the object.  */
			public OS.Coord TopLeft;

			/*! \brief Create a show object specification from the details given.
			 *
			 * The top left hand corner of the object will be placed at the coordinate given.  */
			public ShowObjectTopLeft (OS.Coord topLeft) : base (ShowObjectType.TopLeft)
			{
				TopLeft = topLeft;
			}
		}

		public delegate void ToolboxEventHandler (object sender, ToolboxEventArgs args);

		/*! \brief The base object of all Toolbox objects.  */
		public class Object : IDisposable
		{
			public uint ID { get; protected set; }

			/*! \brief A list of all event handlers for this Toolbox object.  */
			public Dictionary<uint, ToolboxEventHandler> ToolboxHandlers = new Dictionary<uint, ToolboxEventHandler>();
//			public event ToolboxEventHandler EventHandler;

			/*! \brief A list of gadgets that we have an interest in within this Toolbox object.
			 *
			 * This doesn't necessarily contain all gadgets, just the ones that have been wrapped
			 * because they need to be manipulated or have events associated with them.  */
			public Dictionary<uint, Gadget> Gadgets = new Dictionary<uint, Gadget>();

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

			public void Show (ShowObjectFull spec, uint parentID, int parentCmp)
			{
				NativeToolbox.ShowObjectFullSpecBlock block =
						new NativeToolbox.ShowObjectFullSpecBlock (spec);

				OS.ThrowOnError (NativeMethods.Toolbox_ShowObject (0,
										   ID,
										   ShowObjectType.FullSpec,
										   ref block,
										   parentID,
										   parentCmp));
			}

			public void Show (ShowObjectTopLeft spec, uint parentID, int parentCmp)
			{
				NativeToolbox.ShowObjectTopLeftBlock block =
						new NativeToolbox.ShowObjectTopLeftBlock (spec);

				OS.ThrowOnError (NativeMethods.Toolbox_ShowObject (0,
										   ID,
										   ShowObjectType.TopLeft,
										   ref block,
										   parentID,
										   parentCmp));
			}

			/*! \brief Display the object in the centre of the screen.  */
			public void ShowCentred ()
			{
				OS.ThrowOnError (NativeMethods.Toolbox_ShowObject (0,
										   ID,
										   ShowObjectType.Centre,
										   IntPtr.Zero,
										   0,
										   -1));
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

			/*! \brief Query the Toolbox to find the class type of this Toolbox object.
			 * \return The class type of the object.  */
			public uint GetClass ()
			{
				return GetClass (ID);
			}

			/*! \brief Get the name of the template that this Toolbox object was created from.  */
			public string TemplateName
			{
				get
				{
					int buffer_size;

					OS.ThrowOnError (NativeMethods.Toolbox_GetTemplateName (0,
												ID,
												null,
												0,
												out buffer_size));
					StringBuilder buffer = new StringBuilder (buffer_size);
					OS.ThrowOnError (NativeMethods.Toolbox_GetTemplateName (0,
												ID,
												buffer,
												buffer_size,
												out buffer_size));
					return buffer.ToString();
				}
			}

			public virtual void Dispatch (ToolboxEvent ev)
			{
				ToolboxEventHandler handler;

				if (ToolboxHandlers.TryGetValue (ev.ToolboxArgs.Header.EventCode, out handler))
					handler (this, ev.ToolboxArgs);
			}

			/*! \brief Query the Toolbox to find the class type of the Toolbox object
			 * with the given ID.
			 * \param [in] ObjectID The Toolbox ID of the object.
			 * \return The class type of the object.  */
			public static uint GetClass (uint ObjectID)
			{
				uint class_type;

				OS.ThrowOnError (NativeMethods.Toolbox_GetObjectClass (0, ObjectID, out class_type));

				return class_type;
			}

			/*! \brief Check if the given Toolbox ID is already known to us and return the
			 * wrapper for it if so. Otherwise, create a new wrapper based on its
			 * Toolbox class type.
			 * \param [in] ObjectID The Toolbox ID of the object.
			 * \return The Object wrapper.
			 * \exception UnknownObjectException Thrown when an unrecognised object class is
			 * found.  */
			public static Object LookupOrWrap (uint ObjectID)
			{
				Object obj;

				if (ToolboxTask.AllObjects.TryGetValue (ObjectID, out obj))
					return obj;

				switch (GetClass (ObjectID))
				{
				case Class.Window:
					return new Window (ObjectID);
				case Class.Menu:
					return new Menu (ObjectID);
				case Class.SaveAs:
					return new SaveAsDialogue (ObjectID);
				case Class.ProgInfo:
					return new ProgInfoDialogue (ObjectID);
				case Class.FileInfo:
					return new FileInfoDialogue (ObjectID);
				case Class.ColourDbox:
					return new ColourDialogue (ObjectID);
				case Class.ColourMenu:
					return new ColourMenu (ObjectID);
				case Class.FontDbox:
					return new FontDialogue (ObjectID);
				case Class.FontMenu:
					return new FontMenu (ObjectID);
				default:
					throw new UnknownObjectException (ObjectID);
				}
			}

			//
			// SWI Toolbox_ObjectMiscOp methods with various register input/output
			// and type combinations.
			//

			public void SetText (uint flags, int method, string text)
			{
				MiscOp_SetR3 (flags, method, text);
			}

			public int GetBuffer (uint flags, int method, IntPtr buffer, int buffer_size, out int r0_out)
			{
				int used_out;

				OS.ThrowOnError (NativeMethods.Toolbox_ObjectMiscOp_SetR3R4GetR0R4 (flags,
												    ID,
												    method,
												    buffer,
												    buffer_size,
												    out r0_out,
												    out used_out));
				return used_out;
			}

			public int GetBuffer (uint flags, int method, StringBuilder buffer, int buffer_size)
			{
				int used_out;

				OS.ThrowOnError (NativeMethods.Toolbox_ObjectMiscOp_SetR3R4GetR4 (flags,
												  ID,
												  method,
												  buffer,
												  buffer_size,
												  out used_out));
				return used_out;
			}

			public string GetText (int method)
			{
				int buffer_size = GetBuffer (0, method, null, 0);
				StringBuilder buffer = new StringBuilder (buffer_size);
				GetBuffer (0, method, buffer, buffer_size);
				return buffer.ToString ();
			}

			protected void SetMenu (int method, Menu menu)
			{
				MiscOp_SetR3 (0, method, (menu == null) ? 0 : menu.ID);
			}

			protected Menu GetMenu (int method)
			{
				uint menu_id = MiscOp_GetR0 (0, method);

				Toolbox.Object tb_obj;
				if (!ToolboxTask.AllObjects.TryGetValue (menu_id, out tb_obj))
					throw new UnknownObjectException (menu_id);

				return (Menu)tb_obj;
			}

			public void MiscOp_SetR3 (uint flags, int method, uint r3)
			{
				OS.ThrowOnError (NativeMethods.Toolbox_ObjectMiscOp_SetR3 (flags,
											   ID,
											   method,
											   r3));
			}

			public void MiscOp_SetR3 (uint flags, int method, IntPtr r3)
			{
				OS.ThrowOnError (NativeMethods.Toolbox_ObjectMiscOp_SetR3 (flags,
											   ID,
											   method,
											   r3));
			}

			public void MiscOp_SetR3 (uint flags, int method, string r3)
			{
				OS.ThrowOnError (NativeMethods.Toolbox_ObjectMiscOp_SetR3 (flags,
											   ID,
											   method,
											   r3));
			}

			public void MiscOp_SetR3R4 (uint flags, int method, uint r3, uint r4)
			{
				OS.ThrowOnError (NativeMethods.Toolbox_ObjectMiscOp_SetR3R4 (flags,
											     ID,
											     method,
											     r3,
											     r4));
			}

			public void MiscOp_SetR3R4 (uint flags, int method, uint r3, string r4)
			{
				OS.ThrowOnError (NativeMethods.Toolbox_ObjectMiscOp_SetR3R4 (flags,
											     ID,
											     method,
											     r3,
											     r4));
			}

			public void MiscOp_SetR3R4R5 (uint flags, int method, uint r3, uint r4, uint r5)
			{
				OS.ThrowOnError (NativeMethods.Toolbox_ObjectMiscOp_SetR3R4R5 (flags,
											       ID,
											       method,
											       r3,
											       r4,
											       r5));
			}

			public void MiscOp_SetR3R4R5R6 (uint flags, int method, uint r3, int r4, int r5, int r6)
			{
				OS.ThrowOnError (NativeMethods.Toolbox_ObjectMiscOp_SetR3R4R5R6 (flags,
												 ID,
												 method,
												 r3,
												 r4,
												 r5,
												 r6));
			}

			public uint MiscOp_GetR0 (uint flags, int method)
			{
				uint r0;

				OS.ThrowOnError (NativeMethods.Toolbox_ObjectMiscOp_GetR0 (flags,
											   ID,
											   method,
											   out r0));
				return r0;
			}

			public void MiscOp_GetR0R1 (uint flags, int method, out uint r0, out uint r1)
			{
				OS.ThrowOnError (NativeMethods.Toolbox_ObjectMiscOp_GetR0R1 (flags,
											     ID,
											     method,
											     out r0,
											     out r1));
			}

			public uint MiscOp_SetR3GetR0 (uint flags, int method, uint r3)
			{
				uint r0;

				OS.ThrowOnError (NativeMethods.Toolbox_ObjectMiscOp_SetR3GetR0 (flags,
												ID,
												method,
												r3,
												out r0));
				return r0;
			}

			public void MiscOp_SetR3GetR0R1 (uint flags, int method, uint r3, out uint r0, out uint r1)
			{
				OS.ThrowOnError (NativeMethods.Toolbox_ObjectMiscOp_SetR3GetR0R1 (flags,
												  ID,
												  method,
												  r3,
												  out r0,
												  out r1));
			}

			public void MiscOp_SetR3GetR0R1R2 (uint flags, int method, uint r3,
							   out int r0, out int r1, out int r2)
			{
				OS.ThrowOnError (NativeMethods.Toolbox_ObjectMiscOp_SetR3GetR0R1R2 (flags,
												    ID,
												    method,
												    r3,
												    out r0,
												    out r1,
												    out r2));
			}

			/*! \brief An object that encapsulates the arguments for the event that is raised
			 * just before an object is shown on screen.  */
			public class AboutToBeShownEventArgs : ToolboxEventArgs
			{
				/*! \brief Constants defining event specific data offsets after the header.  */
				public static class EventOffset
				{
					public const int ShowType = 16;
					public const int Buffer = 20;
				}

				/*! \brief Gives details of where the object will be displayed.  */
				public readonly ShowObjectSpec ShowSpec;

				public AboutToBeShownEventArgs (IntPtr unmanagedEventData) : base (unmanagedEventData)
				{
					ShowObjectType show_type = (ShowObjectType)Marshal.ReadInt32 (RawEventData,
												      EventOffset.ShowType);
					switch (show_type)
					{
					case Toolbox.ShowObjectType.FullSpec:
						{
							var ev = (NativeToolbox.ShowObjectFullSpecEvent)
									Marshal.PtrToStructure (RawEventData,
												typeof (NativeToolbox.ShowObjectFullSpecEvent));
							ShowSpec = new ShowObjectFull (new OS.Rect (ev.Spec.Visible),
										       new OS.Coord (ev.Spec.Scroll),
										       ev.Spec.BehindWindow,
										       0, 0, 0);
							break;
						}
					case Toolbox.ShowObjectType.TopLeft:
						{
							var ev = (NativeToolbox.ShowObjectTopLeftEvent)
									Marshal.PtrToStructure (RawEventData,
												typeof (NativeToolbox.ShowObjectTopLeftEvent));
							ShowSpec = new ShowObjectTopLeft (new OS.Coord (ev.Spec.TopLeft));
							break;
						}
					case Toolbox.ShowObjectType.Default:
					case Toolbox.ShowObjectType.Centre:
					case Toolbox.ShowObjectType.AtPointer:
						ShowSpec = new ShowObjectSpec (show_type);
						break;
					default:
						ShowSpec = null;
						break;
					}
				}
			}

			/*! \brief The signature of a normal AboutToBeShown event handler.  */
			public delegate void AboutToBeShownEventHandler (object sender, AboutToBeShownEventArgs e);
		}

		public class ToolboxEventArgs : EventArgs
		{
			public NativeToolbox.EventHeader Header;

			public IntPtr RawEventData;

			public ToolboxEventArgs (IntPtr unmanaged_event_block)
			{
				RawEventData = unmanaged_event_block;

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

		public NativeToolbox.IDBlock GetIDBlock ()
		{
			IntPtr idblock_ptr = NativeMethods.Toolbox_GetIDBlock();

			return (NativeToolbox.IDBlock)Marshal.PtrToStructure (idblock_ptr,
									     typeof (NativeToolbox.IDBlock));
		}

		public override void Dispatch (Wimp.Event event_base)
		{
			NativeToolbox.IDBlock id_block = GetIDBlock();

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
						Toolbox.Gadget tb_cmp;
						if (id_block.SelfCmp != 0xffffffff &&
						    tb_obj.Gadgets.TryGetValue (id_block.SelfCmp, out tb_cmp))
						{
							tb_cmp.Dispatch (event_full);
						}

						tb_obj.Dispatch (event_full);
					}
				}
				break;
			}
		}
	}
}
