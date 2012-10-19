//
// toolbox/adjuster-arrows.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;
using System.Runtime.InteropServices;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \class AdjusterArrow
		 * \brief Displayed as an up, down, left or right arrow icon.
		 *
		 * Clicking on the arrow will raise an AdjusterClicked Toolbox event with an
		 * indication of whether the change is up or down.  */
		public class AdjusterArrow : Gadget
		{
			/*! \brief The default Toolbox event that an %AdjusterArrow can raise.  */
			public static class EventCode
			{
				public const uint AdjusterClicked = 0x8288c;
			}

			/*! \brief The possible values that the Click event can return.  */
			public enum ClickDirection
			{
				Down = 0,
				Up = 1,
				Left = 0,
				Right = 1
			}

			/*! \brief An object that encapsulates the arguments for the event that is raised when an
			 * %AdjusterArrow is clicked.  */
			public class ClickEventArgs : ToolboxEventArgs
			{
				/*! \brief Constant defining event specific data offset after the header.  */
				public static class EventOffset
				{
					public const int Direction = 16;
				}

				/*! \brief Indicates the direction of the arrow that was clicked.
				 *
				 * \note Down is equivalent to Left and Up is equivalent to Right.  */
				public ClickDirection Direction;

				/*! \brief Create the arguments for a Click event from the raw event data.  */
				public ClickEventArgs (IntPtr unmanagedEventBlock) : base (unmanagedEventBlock)
				{
					Direction = (ClickDirection)Marshal.ReadInt32 (RawEventData, EventOffset.Direction);
				}
			}

			/*! \brief The signature of a Click event handler.  */
			public delegate void ClickEventHandler (object sender, ClickEventArgs e);

			/*! \brief The event handlers that will be called when this %AdjusterArrow is clicked.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, ClickEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * AdjusterArrowObject.Click += handler_name;
			 * \endcode  */
			public event ClickEventHandler Click;

			/*! \brief Wrap an existing %AdjusterArrow, e.g., from a Resource file created
			 * Window.  */
			public AdjusterArrow (Window window, uint cmpID) : base (window, cmpID)
			{
			}

			protected virtual void OnClick (ClickEventArgs e)
			{
				if (Click != null)
					Click (this, e);
			}

			public override void Dispatch (ToolboxEventArgs e)
			{
				if (e.Header.EventCode == EventCode.AdjusterClicked)
					OnClick (new ClickEventArgs (e.RawEventData));
			}
		}
	}
}
