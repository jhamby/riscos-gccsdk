//
// toolbox/gadget.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;
using System.Text;

namespace riscos
{
	public static partial class Toolbox
	{
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

			public static class Flags
			{
				public const uint Faded = ((uint)1 << 31);
			}

			public static class ComponentType
			{
				public const uint ActionButton = 128;
				public const uint OptionButton = 192;
				public const uint LabelledBox = 256;
				public const uint Label = 320;
				public const uint RadioButton = 384;
				public const uint DisplayField = 448;
				public const uint WritableField = 512;
				public const uint Slider = 576;
				public const uint Draggable = 640;
				public const uint PopupMenu = 704;
				public const uint AdjusterArrow = 768;
				public const uint NumberRange = 832;
				public const uint StringSet = 896;
				public const uint Button = 960;
			}

			/*! \brief The window/menu containing this gadget.  */
			protected Toolbox.Object Object;
			/*! \brief The Toolbox ID of this gadget.  */
			protected uint ComponentID;

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

			/*! \brief Determines whether the gadget is faded or not.  */
			public virtual bool Faded
			{
				get { return (CallMethod_GetR0 (Method.GetFlags) & Flags.Faded) != 0; }
				set
				{
					uint flags = CallMethod_GetR0 (Method.GetFlags);
					if (value)
						flags |= Flags.Faded;
					else
						flags &= ~Flags.Faded;
					CallMethod_SetR4 (Method.SetFlags, flags);
				}
			}

			/*! \brief The help message which will be returned when a Help Request
			 * is received for this gadget.  */
			public virtual string HelpMessage
			{
				virtual get { return GetText (Method.GetHelpMessage); }
				virtual set { SetText (Method.SetHelpMessage, value); }
			}

			/*! \brief The type of this gadget.  */
			public virtual uint Type
			{
				virtual get { return CallMethod_GetR0 (Method.GetType); }
			}

			/*! \brief The bounding box of the gadget.<br>
			 * Setting allows the gadget to be moved/resized within the window.  */
			public virtual OS.Rect BoundingBox
			{
				virtual get
				{
					NativeOS.Rect extent = new NativeOS.Rect ();
					OS.ThrowOnError (NativeMethods.Gadget_GetBBox (0,
										       Object.ID,
										       Method.GetBBox,
										       ComponentID,
										       out extent));
					return new OS.Rect (extent);
				}
				virtual set
				{
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
				CallMethod_SetR4 (Method.SetFocus, 0);
			}

			/*! \todo Method to return an array of icons used by the gadget.
			 * Would be nice if there were a StringBuilder class for integer
			 * arrays.  */
			public void GetIconList()
			{
			}

			public virtual void Dispatch (ToolboxEvent ev)
			{
			}

			/* Generic functions that implement common methods used by gadgets.
			 * Gadgets can use these to call their specific methods.  */
			protected void SetText (int method, string text)
			{
				OS.ThrowOnError (NativeMethods.Component_SetText (0,
										  Object.ID,
										  method,
										  ComponentID,
										  text));
			}

			protected string GetText (int method)
			{
				int buffer_size;

				OS.ThrowOnError (NativeMethods.Component_GetText (0,
										  Object.ID,
										  method,
										  ComponentID,
										  null,
										  0,
										  out buffer_size));
				StringBuilder buffer = new StringBuilder (buffer_size);
				OS.ThrowOnError (NativeMethods.Component_GetText (0,
										  Object.ID,
										  method,
										  ComponentID,
										  buffer,
										  buffer_size,
										  out buffer_size));
				return buffer.ToString();
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
					tb_obj_id = clickShow.Object.ID;
					flags = clickShow.Flags;
				}

				CallMethod_SetR4R5 (method, tb_obj_id, flags);
			}

			// If no object attached, then returns null.
			protected Toolbox.ClickShow GetClickShow (int method)
			{
				uint tb_obj_id;
				uint flags;

				CallMethod_GetR0R1 (method, out tb_obj_id, out flags);

				return (tb_obj_id == 0) ? null : new Toolbox.ClickShow (tb_obj_id, flags);
			}

			protected void CallMethod_SetR4 (int method, uint r4)
			{
				CallMethod_SetR4 (0, method, r4);
			}

			protected void CallMethod_SetR4 (uint flags, int method, uint r4)
			{
				OS.ThrowOnError (NativeMethods.Component_SetR4 (0,
										Object.ID,
										method,
										ComponentID,
										r4));
			}

			protected void CallMethod_SetR4R5 (int method, uint r4, uint r5)
			{
				OS.ThrowOnError (NativeMethods.Component_SetR4R5 (0,
										  Object.ID,
										  method,
										  ComponentID,
										  r4,
										  r5));
			}

			protected void CallMethod_GetR0R1 (int method, out uint r0, out uint r1)
			{
				OS.ThrowOnError (NativeMethods.Component_GetR0R1 (0,
										  Object.ID,
										  method,
										  ComponentID,
										  out r0,
										  out r1));
			}

			protected uint CallMethod_GetR0 (uint flags, int method)
			{
				uint value;

				OS.ThrowOnError (NativeMethods.Component_GetR0 (flags,
										Object.ID,
										method,
										ComponentID,
										out value));
				return value;
			}

			protected uint CallMethod_GetR0 (int method)
			{
				return CallMethod_GetR0 (0, method);
			}
		}
	}
}
