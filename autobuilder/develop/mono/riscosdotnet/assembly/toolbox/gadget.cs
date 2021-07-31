//
// toolbox/gadget.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;
using System.Text;
using System.Runtime.InteropServices;

namespace riscos
{
	public static partial class Toolbox
	{
		//! \brief The Toolbox flags that can be applied to all types of gadget.
		public static class GadgetFlags
		{
			public const uint CreatedFirst = (1 << 30);
			public const uint Faded = (1U << 31);
		}

		/*! \class Gadget
		 * \brief Base object of all gadget types.  */
		public class Gadget
		{
			static class Method
			{
				public const int GetFlags = 64;
				public const int SetFlags = 65;
				public const int SetHelpMessage = 66;
				public const int GetHelpMessage = 67;
				public const int GetIconList = 68;
				public const int SetFocus = 69;
				new public const int GetType = 70;
				public const int MoveGadget = 71;
				public const int GetBBox = 72;
			}

			public static class ComponentType
			{
				public const int ActionButton = 128;
				public const int OptionButton = 192;
				public const int LabelledBox = 256;
				public const int Label = 320;
				public const int RadioButton = 384;
				public const int DisplayField = 448;
				public const int WritableField = 512;
				public const int Slider = 576;
				public const int Draggable = 640;
				public const int PopupMenu = 704;
				public const int AdjusterArrow = 768;
				public const int NumberRange = 832;
				public const int StringSet = 896;
				public const int Button = 960;
			}

			/*! \brief The window/menu containing this gadget.  */
			public Toolbox.Object Object { get; private set; }
			/*! \brief The Toolbox ID of this gadget.  */
			public readonly uint ComponentID;

			/*! \brief Wrap an existing gagdet.  */
			public Gadget (Object tbObj, uint cmpID)
			{
				Object = tbObj;
				ComponentID = cmpID;

				Object.Gadgets.Add (ComponentID, this);
			}

			~Gadget ()
			{
				Object.Gadgets.Remove (ComponentID);
			}

			/*! \brief Gets the flags for this gadget. The available flags are
			 * dependent on the type of gadget.  */
			public uint Flags
			{
				get { return Object.MiscOp_SetR3GetR0 (0, Method.GetFlags, ComponentID); }
				// Only the faded flag can usefully be changed. Modifying the other
				// flags is undefined, so allow only the Faded property write access.
				private set {
					Object.MiscOp_SetR3R4 (0, Method.SetFlags, ComponentID, value);
				}
			}

			/*! \brief Determines whether the gadget is faded or not.  */
			public virtual bool Faded
			{
				get { return (Flags & GadgetFlags.Faded) != 0; }
				set
				{
					uint flags = Flags;
					Flags = value ?
						flags | GadgetFlags.Faded :
						flags &= ~GadgetFlags.Faded;
				}
			}

			/*! \brief The help message which will be returned when a Help Request
			 * is received for this gadget.  */
			public virtual string HelpMessage
			{
				get { return GetText (Method.GetHelpMessage); }
				set { SetText (Method.SetHelpMessage, value); }
			}

			/*! \brief The type of this gadget.  */
			public virtual uint Type
			{
				get { return Object.MiscOp_SetR3GetR0 (0, Method.GetType, ComponentID); }
			}

			/*! \brief The bounding box of the gadget.<br>
			 * Setting allows the gadget to be moved/resized within the window.  */
			public virtual OS.Rect BoundingBox
			{
				get {
					NativeOS.Rect extent = new NativeOS.Rect ();
					OS.ThrowOnError (NativeMethods.Gadget_GetBBox (0,
										       Object.ID,
										       Method.GetBBox,
										       ComponentID,
										       out extent));
					return new OS.Rect (ref extent);
				}
				set {
					NativeOS.Rect bbox = new NativeOS.Rect (value);
					OS.ThrowOnError (NativeMethods.Gadget_MoveGadget (0,
											  Object.ID,
											  Method.MoveGadget,
											  ComponentID,
											  ref bbox));
				}
			}

			/*! \brief Set the input focus to this component.
			 * \note Component must be a writable field, or a composite gadget which
			 * includes a writable field such as a number range.  */
			public virtual void SetFocus ()
			{
				Object.MiscOp_SetR3R4 (0, Method.SetFocus, ComponentID, 0);
			}

			/*! \brief Returns a list of WIMP icon numbers for the icons used to implement
			 * this gadget.
			 * 
			 * If the gadget is composite (i.e. made up of other gadgets), then \e null is returned.
			 * \return A managed integer array containing the icon numbers or null for a
			 * composite gadget.  */
			public int [] GetIconList()
			{
				int buffer_size;
				IntPtr buffer = IntPtr.Zero;

				try
				{
					OS.ThrowOnError (NativeMethods.Gadget_GetIconList (0,
											   Object.ID,
											   Method.GetIconList,
											   ComponentID,
											   IntPtr.Zero,
											   0,
											   out buffer_size));
					if (buffer_size == 0)
						return null;

					// The returned buffer size is in bytes.
					buffer = Marshal.AllocHGlobal (buffer_size);
					OS.ThrowOnError (NativeMethods.Gadget_GetIconList (0,
											   Object.ID,
											   Method.GetIconList,
											   ComponentID,
											   buffer,
											   buffer_size,
											   out buffer_size));
					int [] icon_list = new int [buffer_size >> 2];
					Marshal.Copy (buffer, icon_list, 0, buffer_size >> 2);
					return icon_list;
				}
				catch
				{
					// Rethrow any exceptions
					throw;
				}
				finally
				{
					// Free the temporary unmanaged buffer memory
					Marshal.FreeHGlobal (buffer);
				}
			}

			public virtual void Dispatch (ToolboxEventArgs e)
			{
			}

			/*! \brief Get the type of an unknown gadget.
			 * \param [in] ObjectID The Toolbox ID of the object.
			 * \param [in] cmpID The Toolbox ID of the gadget component.
			 * \return The type of the given gadget.  */
			public static int GetType (uint ObjectID, uint cmpID)
			{
				uint type;

				OS.ThrowOnError (NativeMethods.Toolbox_ObjectMiscOp_SetR3GetR0 (0,
												ObjectID,
												Method.GetType,
												cmpID,
												out type));
				return (int)type;
			}

			/* Generic functions that implement common methods used by gadgets.
			 * Gadgets can use these to call their specific methods.  */
			protected void SetText (int method, string text)
			{
				Object.MiscOp_SetR3R4 (0, method, ComponentID, text);
			}

			protected int GetTextBuffer (uint flags, int method, StringBuilder buffer, int buffer_size)
			{
				int used_out;

				OS.ThrowOnError (NativeMethods.Toolbox_ObjectMiscOp_SetR3R4R5GetR5 (flags,
												    Object.ID,
												    method,
												    ComponentID,
												    buffer,
												    buffer_size,
												    out used_out));
				return used_out;
			}

			protected string GetText (int method)
			{
				int buffer_size = GetTextBuffer (0, method, null, 0);
				StringBuilder buffer = new StringBuilder (buffer_size);
				GetTextBuffer (0, method, buffer, buffer_size);
				return buffer.ToString ();
			}

			// if clickShow is null, object is detached.
			protected void SetClickShow (int method, Toolbox.ClickShow clickShow)
			{
				uint tb_obj_id;
				uint flags;

				if (clickShow == null)
				{
					tb_obj_id = 0;
					flags = 0;
				}
				else
				{
					tb_obj_id = clickShow.ObjectID;
					flags = clickShow.Flags;
				}

				Object.MiscOp_SetR3R4R5 (0, method, ComponentID, tb_obj_id, flags);
			}

			// If no object attached, then returns null.
			protected Toolbox.ClickShow GetClickShow (int method)
			{
				uint tb_obj_id;
				uint flags;

				Object.MiscOp_SetR3GetR0R1 (0, method, ComponentID, out tb_obj_id, out flags);

				return (tb_obj_id == 0) ? null : new Toolbox.ClickShow (tb_obj_id, flags);
			}
		}
	}
}
