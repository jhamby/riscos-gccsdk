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
			public const int ScaleDbox = 0x82c00;
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

		//! \brief The numbers of the possible events that the Toolbox can raise.
		public static class EventCode
		{
			public const int Error = 0x44ec0;
			public const int ObjectAutoCreated = 0x44ec1;
			public const int ObjectDeleted = 0x44ec2;
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

			/*! \brief The Toolbox id of the object to be shown.  */
			public uint ObjectID;

			/*! \brief Flags used to indicate whether a component that can show an object when
			 * clicked should show the object persistently or transiently.  */
			public uint Flags;

			/*! \brief Create a ClickShow object.
			 * \param [in] objID The ID of the Toolbox object that will be shown.
			 * \param [in] flags Set to \e ClickShow.Persistent or \e ClickShow.Transient.
			 * \exception UnknownObjectException Thrown if the class of the given object is not known.  */
			public ClickShow (uint objID, uint flags)
			{
				ObjectID = objID;
				Flags = flags;
			}

			/*! \brief Create a ClickShow object.
			 * \param [in] tbObj The Toolbox object that will be shown.
			 * \param [in] flags Set to \e ClickShow.Persistent or \e ClickShow.Transient.  */
			public ClickShow (Object tbObj, uint flags)
			{
				ObjectID = tbObj.ID;
				Flags = flags;
			}

			/*! \brief Return the Toolbox object that the ClickShow object refers to.  */
			public T Object<T> () where T : Toolbox.Object
			{
				return Object.CreateInstance<T> (ObjectID);
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

		/*! \brief The base object of all Toolbox objects.  */
		public class Object : IDisposable, IEquatable<Object>
		{
			public uint ID { get; protected set; }

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
				Register ();
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

			/*! \brief Return \e true if the Toolbox object given is the same as this
			 * one.
			 * \note This method is part of the IEquatable interface,  */
			public bool Equals (Toolbox.Object other)
			{
				if (other == null)
					return false;

				return (ID == other.ID);
			}

			/*! \brief Create a toolbox object using the named template.
			 * \param [in] objectName Name of object in resource file to create.  */
			protected void Create (string objectName)
			{
				ID = Toolbox.CreateObject (objectName);
				Register ();
			}

			/*! \brief Create a toolbox object using the template pointed to.
			 * \param [in] templateData Pointer to template data to create object from.  */
			protected void Create (IntPtr templateData)
			{
				ID = Toolbox.CreateObject (templateData);
				Register ();
			}

			/*! \brief Delete the object.  */
			private void Delete ()
			{
				Deregister ();
				Toolbox.DeleteObject (0, ID);
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

			/*! \brief Sets the value of the client handle for this object.  */
			public void SetClientHandle (int handle)
			{
				Toolbox.SetClientHandle (ID, handle);
			}

			/*! \brief Return the value of the client handle for this object.  */
			public int GetClientHandle ()
			{
				return Toolbox.GetClientHandle (ID);
			}

			public T GetAncestor<T> () where T : Toolbox.Object
			{
				return Object.CreateInstance<T> (Toolbox.GetAncestor (ID));
			}

			/*! \brief Sets or gets the client handle for this object.  */
			public int ClientHandle
			{
				set { SetClientHandle (value); }
				get { return GetClientHandle (); }
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

			public virtual void Dispatch (Wimp.EventArgs e)
			{
			}

			public virtual void Dispatch (ToolboxEventArgs e)
			{
				var id_block = ToolboxTask.GetIDBlock();
				var gadget = GetGadget (id_block.SelfCmp);

				if (gadget == null)
					return;

				gadget.Dispatch (e);
			}

			public Toolbox.Gadget GetGadget (uint id)
			{
				if (id == 0xffffffff)
					return null;

				Gadget gadget;

				return Gadgets.TryGetValue (id, out gadget) ? gadget : null;
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

			static Dictionary<uint, Toolbox.Object> AllObjects =
							new Dictionary<uint, Toolbox.Object>();

			public static T CreateInstance<T> (string templateName) where T : Toolbox.Object
			{
				uint tb_obj_id;
				Toolbox.Object obj;

				tb_obj_id = Toolbox.CreateObject (templateName);
				if (!AllObjects.TryGetValue (tb_obj_id, out obj))
				{
					return (T)Activator.CreateInstance (typeof(T), tb_obj_id);
				}
				else
				{
					Toolbox.DeleteObject (0, tb_obj_id);
					return (T)obj;
				}
			}

			public static T CreateInstance<T> (uint objectID) where T : Toolbox.Object
			{
				T obj_ret = null;

				if (objectID != 0)
				{
					Toolbox.Object obj;

					obj_ret = (T)(AllObjects.TryGetValue (objectID, out obj) ?
						      obj :
						      Activator.CreateInstance (typeof(T), objectID));
				}

				return obj_ret;
			}

			public static T CreateInstance<T> (IntPtr templateData) where T : Toolbox.Object
			{
				uint tb_obj_id;
				Toolbox.Object obj;

				tb_obj_id = Toolbox.CreateObject (templateData);
				if (!AllObjects.TryGetValue (tb_obj_id, out obj))
				{
					return (T)Activator.CreateInstance (typeof(T), tb_obj_id);
				}
				else
				{
					Toolbox.DeleteObject (0, tb_obj_id);
					return (T)obj;
				}
			}

			/*! \brief Add a Toolbox object to the list of known objects.  */
			void Register ()
			{
				AllObjects.Add (ID, this);
			}

			/*! \brief Remove a Toolbox object from the list of known objects.  */
			void Deregister ()
			{
				AllObjects.Remove (ID);
			}

			/*! \brief Test the Toolbox object id to see if it has been registered and
			 * return the corresponding managed object if so.
			 * \param [in] id The Toolbox object id to test.
			 * \param [out] obj The managed Toolbox object if the object is known.
			 * \return \e true if the object was found.  */
			public static bool TryGetValue (uint id, out Toolbox.Object obj)
			{
				return AllObjects.TryGetValue (id, out obj);
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
				if (!Object.TryGetValue (menu_id, out tb_obj))
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

			public void MiscOp_GetR0R1R2 (uint flags, int method, out int r0, out int r1, out int r2)
			{
				OS.ThrowOnError (NativeMethods.Toolbox_ObjectMiscOp_GetR0R1R2 (flags,
											     ID,
											     method,
											     out r0,
											     out r1,
											     out r2));
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
							ShowSpec = new ShowObjectFull (new OS.Rect (ref ev.Spec.Visible),
										       new OS.Coord (ref ev.Spec.Scroll),
										       ev.Spec.BehindWindow,
										       0, 0, 0);
							break;
						}
					case Toolbox.ShowObjectType.TopLeft:
						{
							var ev = (NativeToolbox.ShowObjectTopLeftEvent)
									Marshal.PtrToStructure (RawEventData,
												typeof (NativeToolbox.ShowObjectTopLeftEvent));
							ShowSpec = new ShowObjectTopLeft (new OS.Coord (ref ev.Spec.TopLeft));
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

			/*! \brief An object that encapsulates the arguments for the event that is raised
			 * just before the Menu object is shown on screen.  */
			public class MenuAboutToBeShownEventArgs : ToolboxEventArgs
			{
				/*! \brief Constants defining event specific data offsets after the header.  */
				public static class EventOffset
				{
					public const int ShowType = 16;
					public const int Buffer = 20;
				}

				/*! \brief Gives details of where the menu will be displayed.
				 * \note FullSpec is the same as TopLeft in the case of a Menu. DefaultSpec
				 * means that the menu will open 64 OS units to the left of the mouse pointer.  */
				public readonly ShowObjectSpec ShowSpec;

				/*! \brief Create the event arguments from the raw event data.  */
				public MenuAboutToBeShownEventArgs (IntPtr unmanagedEventData) : base (unmanagedEventData)
				{
					ShowObjectType show_type = (ShowObjectType)Marshal.ReadInt32 (RawEventData,
												      EventOffset.ShowType);
					switch (show_type)
					{
					case Toolbox.ShowObjectType.FullSpec:
					case Toolbox.ShowObjectType.TopLeft:
						var ev = (NativeToolbox.ShowObjectTopLeftEvent)
								Marshal.PtrToStructure (RawEventData,
											typeof (NativeToolbox.ShowObjectTopLeftEvent));
						ShowSpec = new ShowObjectTopLeft (new OS.Coord (ref ev.Spec.TopLeft));
						break;
					case Toolbox.ShowObjectType.Default:
						ShowSpec = new ShowObjectSpec (Toolbox.ShowObjectType.Default);
						break;
					default:
						ShowSpec = null;
						break;
					}
				}
			}
		}

		public class ToolboxEventArgs : Wimp.EventArgs
		{
			public NativeToolbox.EventHeader Header;

			public ToolboxEventArgs (IntPtr unmanagedEventBlock) : base (unmanagedEventBlock)
			{
				Header = (NativeToolbox.EventHeader)Marshal.PtrToStructure(
						unmanagedEventBlock, typeof(NativeToolbox.EventHeader));
			}
		}

		/*! \brief Provide data for the event that is raised when a Toolbox object is
		 * automatically created.  */
		public class ObjectAutoCreatedEventArgs : ToolboxEventArgs
		{
			//! \brief Name of template from which the object was created.
			public readonly string TemplateName;

			//! \brief Toolbox id of the object that was created.
			public readonly uint ObjectID;

			/*! \brief Create the arguments for the event raised when a Toolbox object is
			 * automatically created.  */
			public ObjectAutoCreatedEventArgs (IntPtr unmanagedEventData) :
								base (unmanagedEventData)
			{
				TemplateName = Marshal.PtrToStringAnsi (new IntPtr (RawEventData.ToInt32() +
										    EventOffset.TemplateName));
				ObjectID = ToolboxTask.IDBlock.SelfID;
			}

			public static class EventOffset
			{
				public const int TemplateName = 16;
			}
		}

		/*! \brief Provide data for the event that is raised when the Toolbox returns an error
		 * indirectly to the task.  */
		public class ErrorEventArgs : ToolboxEventArgs
		{
			public OS.Error Error;

			public ErrorEventArgs (IntPtr unmanagedEventData) : base (unmanagedEventData)
			{
				IntPtr error_ptr = new IntPtr (unmanagedEventData.ToInt32 () + EventOffset.OSError);
				Error = (OS.Error)Marshal.PtrToStructure (error_ptr, typeof (OS.Error));
			}

			public static class EventOffset
			{
				public const int OSError = 16;
				public const int ErrorNumber = 16;
				public const int ErrorMessage = 20;
			}
		}

		/*! \brief Create the named Toolbox object.
		 * \param resName The name of the template in the Resource file.
		 * \return The Toolbox id of the created object.  */
		public static uint CreateObject (string resName)
		{
			uint id;

			OS.ThrowOnError (NativeMethods.Toolbox_CreateObject (0, resName, out id));

			return id;
		}

		/*! \brief Create a Toolbox object a template held in memory.
		 * \param resData Pointer to the data required to create the object.
		 * \return The Toolbox id of the created object.  */
		public static uint CreateObject (IntPtr resData)
		{
			uint id;

			OS.ThrowOnError (NativeMethods.Toolbox_CreateObject (CreateObjectFlags.FromMemory,
									     resData, out id));
			return id;

		}

		/*! \brief Delete the Toolbox object whose ID is given.
		 * \param [in] flags Bit 0 set means do not delete recursively.
		 * \param [in] objectID The Toolbox id of the object.
		 * \return Nothing.  */
		public static void DeleteObject (uint flags, uint objectID)
		{
			OS.ThrowOnError (NativeMethods.Toolbox_DeleteObject (flags, objectID));
		}

		/*! \brief Given a template name within  the Resource file, return a pointer to
		 * a block suitable to create an object from.
		 * \param [in] resName The name of the resource template to look up.
		 * \return A pointer to a block that represents the object.  */
		public static IntPtr TemplateLookup (string resName)
		{
			IntPtr template;

			OS.ThrowOnError (NativeMethods.Toolbox_TemplateLookUp (0, resName, out template));

			return template;
		}

		/*! \brief Return the object id which was passed as the parent to a Show method (even
		 * if the parent has been deleted since).
		 * An object that has not yet been shown will have a parent object id of 0 and a
		 * component id of 0xffffffff (-1).
		 * \param [in] objectID The id of the object to return the parent of.
		 * \param [out] parentCmp The id of the parent component.
		 * \return The object id of the parent object.  */
		public static uint GetParent (uint objectID, out uint parentCmp)
		{
			uint parent_id;

			OS.ThrowOnError (NativeMethods.Toolbox_GetParent (0,
									  objectID,
									  out parent_id,
									  out parentCmp));
			return parent_id;
		}

		/*! \brief Return the object id which was passed as the parent to a Show method (even
		 * if the parent has been deleted since).
		 * An object that has not yet been shown will have a parent object id of 0.
		 * \param [in] objectID The id of the object to return the parent of.
		 * \return The object id of the parent object.  */
		public static uint GetParent (uint objectID)
		{
			uint parent_cmp;

			return GetParent (objectID, out parent_cmp);
		}

		/*! \brief Return the id of the Ancestor of the given object.
		 * An object which has not yet been shown will have an ancestor object id of 0 and
		 * component id of 0xffffffff (-1).
		 * \note The Ancestor may have been deleted since this object was shown.
		 * \param [in] objectID The id of the object to return the ancestor of.
		 * \param [out] ancestorCmp The id of the ancestor component.
		 * \return The object id of the ancestor object.  */
		public static uint GetAncestor (uint objectID, out uint ancestorCmp)
		{
			uint ancestor_id;

			OS.ThrowOnError (NativeMethods.Toolbox_GetAncestor (0,
									  objectID,
									  out ancestor_id,
									  out ancestorCmp));
			return ancestor_id;
		}

		/*! \brief Return the id of the Ancestor of the given object.
		 * An object which has not yet been shown will have an ancestor object id of 0.
		 * \note The Ancestor may have been deleted since this object was shown.
		 * \param [in] objectID The id of the object to return the ancestor of.
		 * \return The object id of the ancestor object.  */
		public static uint GetAncestor (uint objectID)
		{
			uint ancestor_cmp;

			return GetAncestor (objectID, out ancestor_cmp);
		}

		/*! \brief Sets the value of the client handle for the object with the given id.  */
		public static void SetClientHandle (uint objectID, int handle)
		{
			OS.ThrowOnError (NativeMethods.Toolbox_SetClientHandle (0, objectID, handle));
		}

		/*! \brief Return the value of the client handle for the object with the given id.  */
		public static int GetClientHandle (uint objectID)
		{
			int handle;

			OS.ThrowOnError (NativeMethods.Toolbox_GetClientHandle (0, objectID, out handle));

			return handle;
		}
	}

	public class ToolboxTask : Task
	{
		public IntPtr SpriteArea;

		public static NativeToolbox.IDBlock IDBlock;

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

		public static NativeToolbox.IDBlock GetIDBlock ()
		{
			IntPtr idblock_ptr = NativeMethods.Toolbox_GetIDBlock();

			return (NativeToolbox.IDBlock)Marshal.PtrToStructure (idblock_ptr,
									     typeof (NativeToolbox.IDBlock));
		}

		public override void Dispatch (Wimp.EventArgs e)
		{
			Toolbox.Object tb_obj;

			IDBlock = GetIDBlock();

			if (Toolbox.Object.TryGetValue (IDBlock.SelfID, out tb_obj))
			{
				if (e.Type == Wimp.PollCode.ToolboxEvent)
				{
					// A Toolbox event for a specific object, so dispatch it to that object.
					tb_obj.Dispatch ((Toolbox.ToolboxEventArgs)e);
				}
				else
				{
					// A Wimp event for a specific object, so dispatch it to that object.
					tb_obj.Dispatch (e);
				}
			}
			else if (e.Type != Wimp.PollCode.ToolboxEvent)
			{
				if (e.Type == Wimp.PollCode.UserMessage ||
				    e.Type == Wimp.PollCode.UserMessageRecorded)
				{
					// Some messages are window specific, but the Toolbox ID Block
					// isn't updated. In this case, we use the Wimp handle to find the
					// Toolbox object.
					uint cmp_id;

					tb_obj = Toolbox.Window.WimpToToolbox (e.GetWindowHandle (),
									       -1,
									       out cmp_id);
					if (tb_obj != null)
						tb_obj.Dispatch (e);
					else
					{
						// The destination of the DataLoad could be the Iconbar, in
						// which case WimpToToolbox will return null, so let
						// Task.Dispatch deal with it.
						base.Dispatch (e);
					}
				}
				else
				{
					// A Wimp event that is not specific to an object, so let
					// Task.Dispatch deal with it.
					base.Dispatch (e);
				}
			}
			else
			{
				var toolbox_event = (Toolbox.ToolboxEventArgs) e;

				switch (toolbox_event.Header.EventCode)
				{
				case Toolbox.EventCode.ObjectAutoCreated:
					OnObjectAutoCreated (new Toolbox.ObjectAutoCreatedEventArgs (e.RawEventData));
					break;
				case Toolbox.EventCode.Error:
					OnError (new Toolbox.ErrorEventArgs (e.RawEventData));
					break;
				}
			}
		}

		//! \brief Create a CaretEventArgs object specific to Toolbox windows and gadgets.
		protected override Wimp.CaretEventArgs CreateCaretEventArgs (IntPtr rawEventData)
		{
			return new Toolbox.Window.CaretEventArgs (rawEventData);
		}

		//! \brief Create a PointerEventArgs object specific to Toolbox windows and gadgets.
		protected override Wimp.PointerEventArgs CreatePointerEventArgs (IntPtr rawEventData)
		{
			return new Toolbox.Window.PointerEventArgs (rawEventData);
		}

		/*! \brief Raising an event invokes the event handler through a delegate.
		 *
		 * The \b OnObjectAutoCreated method also allows derived classes to handle the
		 * event without attaching a delegate. This is the preferred technique for
		 * handling the event in a derived class.
		 * \note  When overriding \b OnObjectAutoCreated in a derived class, be sure to
		 * call the base class's \b OnObjectAutoCreated method so that registered delegates
		 * receive the event.  */
		protected virtual void OnObjectAutoCreated (Toolbox.ObjectAutoCreatedEventArgs e)
		{
			if (ObjectAutoCreated != null)
				ObjectAutoCreated (this, e);
		}

		/*! \brief Raising an event invokes the event handler through a delegate.
		 *
		 * The \b OnError method also allows derived classes to handle the
		 * event without attaching a delegate. This is the preferred technique for
		 * handling the event in a derived class.
		 * \note  When overriding \b OnError in a derived class, be sure to
		 * call the base class's \b OnError method so that registered delegates
		 * receive the event.  */
		protected virtual void OnError (Toolbox.ErrorEventArgs e)
		{
			if (Error != null)
				Error (this, e);
		}

		/*! \brief The event handlers that will be called when a Toolbox object is automatically
		 * created.  */
		public event EventHandler<Toolbox.ObjectAutoCreatedEventArgs> ObjectAutoCreated;

		/*! \brief The event handlers that will be called when the Toolbox indirectly returns
		 * an error to the task.  */
		public event EventHandler<Toolbox.ErrorEventArgs> Error;
	}
}
