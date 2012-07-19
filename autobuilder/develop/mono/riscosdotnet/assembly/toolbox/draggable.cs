//
// toolbox/draggable.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \class Draggable
		 * Consists of a sprite, text or text & sprite which appears in a dialogue
		 * box, and can be dragged using the mouse.  */
		public class Draggable : Gadget
		{
			static class Method
			{
				public const int SetSprite = 640;
				public const int GetSprite = 641;
				public const int SetText = 642;
				public const int GetText = 643;
				public const int SetState = 644;
				public const int GetState = 645;
			}

			/*! \brief The default Toolbox events that a draggable can raise.  */
			public static class EventCode
			{
				public const uint DragStarted = 0x82887;
				public const uint DragEnded = 0x82888;
			}

			/*! \brief Wrap an existing draggable, e.g., from a Resource file created
			 * Window.  */
			public Draggable (Window window, uint cmpID) : base (window, cmpID)
			{
			}

			/*! \brief The name of the sprite used for this draggable.  */
			public string Sprite
			{
				set { SetText (Method.SetSprite, value); }
				get { return GetText (Method.GetSprite); }
			}

			/*! \brief The text which will be displayed in this draggable.  */
			public string Text
			{
				set { SetText (Method.SetText, value); }
				get { return GetText (Method.GetText); }
			}

			/*! \brief The state of this draggable, i.e. whether it is selected or
			 * deselected.  */
			public bool Selected
			{
				set { CallMethod_SetR4 (Method.SetState, (uint)(value ? 1 : 0)); }
				get { return (CallMethod_GetR0 (Method.GetState) & 1) != 0; }
			}
		}
	}
}
