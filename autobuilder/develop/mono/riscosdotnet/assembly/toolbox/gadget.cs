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
			class Method
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

			public class Flags
			{
				public const uint Faded = ((uint)1 << 31);
			}

			/*! \brief The Toolbox ID of the window containing this gadget.  */
			protected uint WindowID;
			/*! \brief The Toolbox ID of this gadget.  */
			protected uint ComponentID;

			/*! \brief Wrap an existing gagdet.  */
			public Gadget (Window window, uint cmpID)
			{
				WindowID = window.ID;
				ComponentID = cmpID;
			}

			/*! \brief Determines whether the gadget is faded or not.  */
			public bool Faded
			{
				get
				{
					uint flags;
					OS.ThrowOnError (NativeMethods.Component_GetR0 (0,
											WindowID,
											Method.GetFlags,
											ComponentID,
											out flags));
					return (flags & Flags.Faded) != 0;
				}
				set
				{
					uint flags;
					OS.ThrowOnError (NativeMethods.Component_GetR0 (0,
											WindowID,
											Method.GetFlags,
											ComponentID,
											out flags));
					if (value)
						flags |= Flags.Faded;
					else
						flags &= ~Flags.Faded;
					OS.ThrowOnError (NativeMethods.Component_SetR4 (0,
											WindowID,
											Method.SetFlags,
											ComponentID,
											flags));
				}
			}

			/*! \brief The help message which will be returned when a Help Request
			 * is received for this gadget.  */
			public string HelpMessage
			{
				get { return GetText (Method.GetHelpMessage); }
				set { SetText (Method.SetHelpMessage, value); }
			}

			/*! \brief The type of this gadget.  */
			public uint Type
			{
				get
				{
					uint type;

					OS.ThrowOnError (NativeMethods.Component_GetR0 (0, 
											WindowID,
											Method.GetType,
											ComponentID,
											out type));
					return type;
				}
			}

			/*! \brief The bounding box of the gadget.<br>
			 * Setting allows the gadget to be moved/resized within the window.  */
			public OS.Rect BoundingBox
			{
				get
				{
					NativeOS.Rect extent = new NativeOS.Rect ();
					OS.ThrowOnError (NativeMethods.Gadget_GetBBox (0,
										       WindowID,
										       Method.GetBBox,
										       ComponentID,
										       out extent));
					return new OS.Rect (extent);
				}
				set
				{
					NativeOS.Rect bbox = new NativeOS.Rect (value);
					OS.ThrowOnError (NativeMethods.Gadget_MoveGadget (0,
											  WindowID,
											  Method.MoveGadget,
											  ComponentID,
											  ref bbox));
				}
			}

			/*! \brief Set the input focus to this component.
			 * \note Component must be a writable field, or a composite gadget which
			 * includes a writable field such as a number range.  */
			public void SetFocus ()
			{
				OS.ThrowOnError (NativeMethods.Gadget_SetFocus (0, WindowID, ComponentID, 0));
			}
/*
			TODO:
			void GetIconList();
*/

			/* Generic functions that implement common methods used by gadgets.
			 * Gadgets can use these to call their specific methods.  */
			protected void SetText (int method, string text)
			{
				OS.ThrowOnError (NativeMethods.Component_SetText (0,
										  WindowID,
										  method,
										  ComponentID,
										  text));
			}

			protected string GetText (int method)
			{
				int buffer_size;

				OS.ThrowOnError (NativeMethods.Component_GetText (0,
										  WindowID,
										  method,
										  ComponentID,
										  null,
										  0,
										  out buffer_size));
				StringBuilder buffer = new StringBuilder (buffer_size);
				OS.ThrowOnError (NativeMethods.Component_GetText (0,
										  WindowID,
										  method,
										  ComponentID,
										  buffer,
										  buffer_size,
										  out buffer_size));
				return buffer.ToString();
			}

			protected void SetEvent (int method, uint eventCode)
			{
				OS.ThrowOnError (NativeMethods.Component_SetR4 (0,
										WindowID,
										method,
										ComponentID,
										eventCode));
			}

			protected uint GetEvent (int method)
			{
				uint event_code;
				OS.ThrowOnError (NativeMethods.Component_GetR0 (0,
										WindowID,
										method,
										ComponentID,
										out event_code));
				return event_code;
			}

			// if clickShow is null, object is detached.
			protected void SetClickShow (int method, Toolbox.ClickShow clickShow)
			{
				uint tb_obj_id, flags;

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

				OS.ThrowOnError (NativeMethods.Component_SetR4R5 (0,
										  WindowID,
										  method,
										  ComponentID,
										  tb_obj_id,
										  flags));
			}

			// If no object attached, then returns null.
			protected Toolbox.ClickShow GetClickShow (int method)
			{
				uint tb_obj_id, flags;

				OS.ThrowOnError (NativeMethods.Component_GetR0R1 (0,
										  WindowID,
										  method,
										  ComponentID,
										  out tb_obj_id,
										  out flags));
				return (tb_obj_id == 0) ? null : new Toolbox.ClickShow (tb_obj_id, flags);
			}
		}
	}
}
