//
// toolbox/colour-dialogue.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;
using System.Runtime.InteropServices;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \class ColourDialogue
		 * \brief Allows the user to specify a colour using a variety of colour models.  */
		public class ColourDialogue : Object
		{
			public enum ColourModel
			{
				RGB,
				CMYK,
				HSV
			}

			static class Method
			{
				public const int GetWimpHandle = 0;
				public const int GetDialogueHandle = 1;
				public const int SetColour = 2;
				public const int GetColour = 3;
				public const int SetColourModel = 4;
				public const int GetColourModel = 5;
				public const int SetNoneAvailable = 6;
				public const int GetNoneAvailable = 7;
			}

			/*! \brief The default Toolbox events that this ColourDialogue object can
			 * raise.  */
			public class EventCode
			{
				public const uint AboutToBeShown = 0x829c0;
				public const uint DialogueCompleted = 0x829c1;
				public const uint ColourSelected = 0x829c2;
			}

			/*! \brief The event handlers that will be called just before this Colour Dialogue is shown.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, AboutToBeShownEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * ColourDialogueObject.AboutToBeShown += handler_name;
			 * \endcode  */
			public event AboutToBeShownEventHandler AboutToBeShown;

			/*! \brief An object that encapsulates the arguments for the event that is raised
			 * after the Colour Dialogue has been hidden.  */
			public class DialogueCompleteEventArgs : ToolboxEventArgs
			{
				/*! \brief Constant defining the possible bit flag returned by this event.  */
				public static class Flags
				{
					public const int SelectionWasMade = (1 << 0);
				}

				/*! \brief \e true if a colour selection was made during this dialogue.  */
				public readonly bool SelectionWasMade;

				public DialogueCompleteEventArgs (IntPtr unmanagedEventData) : base (unmanagedEventData)
				{
					SelectionWasMade = (Header.Flags & Flags.SelectionWasMade) != 0;
				}
			}

			/*! \brief The signature of a DialogueComplete event handler.  */
			public delegate void DialogueCompleteEventHandler (object sender, DialogueCompleteEventArgs e);

			/*! \brief The event handlers that will be called when this dialogue is hidden.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, DialogueCompleteEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * ColourDialogueObject.DialogueComplete += handler_name;
			 * \endcode  */
			public event DialogueCompleteEventHandler DialogueComplete;

			/*! \brief An object that encapsulates the arguments for the event that is raised
			 * when the user clicks \e OK in the dialogue box.
			 * 
			 * \note If th \e None button is set, a colour value is still returned, reflecting the
			 * current state of the dialogue box.  */
			public class ColourSelectedEventArgs : ToolboxEventArgs
			{
				/*! \brief Constant defining event specific data offset after the header.  */
				public static class EventOffset
				{
					public const int ColourBlock = 16;
				}

				/*! \brief Constant defining the possible bit flag returned by this event.  */
				public static class Flags
				{
					public const int NoneSelected = (1 << 0);
				}

				public readonly int [] ColourBlock;

				/*! \brief \e true if the \e None button was selected.  */
				public readonly bool NoneSelected;

				public ColourSelectedEventArgs (IntPtr unmanagedEventData) : base (unmanagedEventData)
				{
					NoneSelected = (Header.Flags & Flags.NoneSelected) != 0;

					ColourBlock = new int [212 / 4];
					Marshal.Copy (new IntPtr (RawEventData.ToInt32() + EventOffset.ColourBlock),
						      ColourBlock,
						      0,
						      212 / 4);
				}
			}

			/*! \brief The signature of a ColourSelected event handler.  */
			public delegate void ColourSelectedEventHandler (object sender, ColourSelectedEventArgs e);

			/*! \brief The event handlers that will be called when the use clicks \e OK in the
			 * dialogue box.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, ColourSelectedEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * ColourDialogueObject.ColourSelected += handler_name;
			 * \endcode  */
			public event ColourSelectedEventHandler ColourSelected;

			/*! \brief Create a %Toolbox Colour Dialogue from the named template in the
			 * Resource file.
			 * \param[in] resName The name of the Colour Dialogue template to use.  */
			public ColourDialogue (string resName) : base (resName)
			{
			}

			/*! \brief Create a %Toolbox Colour Dialogue from the template data given.
			 * \param[in] templateData Pointer to the object template.  */
			public ColourDialogue (IntPtr templateData)
			{
				Create (templateData);
			}

			/*! \brief Create a %Toolbox Colour Dialogue from an object that already exists.
			 * \param [in] objectID The Toolbox ID of the existing object.  */
			public ColourDialogue (uint objectID) : base (objectID)
			{
			}

			/*! \brief The handle of the WIMP window used by the underlying Colour
			 * Picker module.
			 * 
			 * Only valid when the Colour Dialogue box is showing.  */
			public uint WimpHandle
			{
				get { return CallMethod_GetR0 (Method.GetWimpHandle); }
			}

			/*! \brief The handle of the dialogue box used by the underlying Colour
			 * Picker module.
			 * 
			 * Only valid when the Colour Dialogue box is showing.  */
			public uint DialogueHandle
			{
				get { return CallMethod_GetR0 (Method.GetDialogueHandle); }
			}

			/*! \brief Determines whether a \b None option appears in the Colour Dialogue.  */
			public bool NoneAvailable
			{
				get
				{
					return (CallMethod_GetR0 (Method.GetNoneAvailable) & 1) != 0;
				}
				set
				{
					CallMethod_SetR3 (Method.SetNoneAvailable, value ? 1U : 0U);
				}
			}

			/*! \brief Gets or sets the colour model used in the Colour Dialogue.  */
			public ColourModel Model
			{
				get
				{
					GCHandle pinned_array;
					int buffer_size;
					try {
						OS.ThrowOnError (NativeMethods.Object_GetBuffer (0,
												 ID,
												 Method.GetColourModel,
												 IntPtr.Zero,
												 0,
												 out buffer_size));
						int [] model_block = new int [buffer_size >> 2];
						// Prevent the GC from moving the memory while we use its address
						pinned_array = GCHandle.Alloc (model_block, GCHandleType.Pinned);
						OS.ThrowOnError (NativeMethods.Object_GetBuffer (0,
												 ID,
												 Method.GetColourModel,
												 pinned_array.AddrOfPinnedObject(),
												 buffer_size,
												 out buffer_size));
						return (ColourModel)model_block [1];
					}
					catch {
						throw;
					}
					finally {
						pinned_array.Free ();
					}
				}
				set
				{
					GCHandle pinned_array;
					try {
						int [] model_block = new int [2];
						model_block[0] = 4;
						model_block[1] = (int)value;
						// Prevent the GC from moving the memory while we use its address
						pinned_array = GCHandle.Alloc (model_block, GCHandleType.Pinned);
						CallMethod_SetR3 (0,
								  Method.SetColourModel,
								  pinned_array.AddrOfPinnedObject());
					}
					catch {
						throw;
					}
					finally {
						pinned_array.Free ();
					}
				}
			}

			/*! \brief Sets the colour currently being display in the Colour Dialogue.
			 * \param [in] colourBlock The colour block describing the colour to display.
			 * \param [in] noneSelected \e true if the None option should be selected.
			 * \return Nothing.
			 * 
			 * \note See AllocStandardColourBlock for methods that allow a colour block to
			 * be created easily from a palette entry or from individual red, green, blue
			 * values.  */
			public void SetColour (int [] colourBlock, bool noneSelected)
			{
				GCHandle pinned_array;
				try {
					// Prevent the GC from moving the memory while we use its address
					pinned_array = GCHandle.Alloc (colourBlock, GCHandleType.Pinned);
					CallMethod_SetR3 (noneSelected ? 1U : 0,
							  Method.SetColour,
							  pinned_array.AddrOfPinnedObject());
				}
				catch {
					throw;
				}
				finally {
					pinned_array.Free ();
				}
			}

			/*! \brief Retrieves the colour block used to describe the colour currently being
			 * displayed in the Colour Dialogue.
			 * \param [out] noneSelected \e true if the None option is selected.
			 * \return An integer array containing the colour block.
			 * 
			 * \note See ColourFromColourBlock for an easy way to find the colour as a palette
			 * entry from the array returned from this method.  */
			public int [] GetColour (out bool noneSelected)
			{
				int [] colour_block;
				GCHandle pinned_array;
				int none_selected = 0;
				int buffer_size;

				try {
					OS.ThrowOnError (NativeMethods.Object_GetBufferWithR0 (0,
											       ID,
											       Method.GetColour,
											       IntPtr.Zero,
											       0,
											       out none_selected,
											       out buffer_size));
					if (buffer_size == 0)
						return null;

					colour_block = new int [buffer_size >> 2];
					pinned_array = GCHandle.Alloc (colour_block, GCHandleType.Pinned);
					OS.ThrowOnError (NativeMethods.Object_GetBufferWithR0 (0,
											       ID,
											       Method.GetColour,
											       pinned_array.AddrOfPinnedObject(),
											       buffer_size,
											       out none_selected,
											       out buffer_size));
					return colour_block;
				}
				catch {
					throw;
				}
				finally {
					noneSelected = none_selected != 0 ? true : false;

					pinned_array.Free ();
				}
			}

			/*! \brief Create a colour block from the individual red, green and blue values of
			 * a colour.
			 * \param [in] r The Red component.
			 * \param [in] g The green component.
			 * \param [in] b The blue component.
			 * \param [in] model The colour model to use in this colour block.
			 * \return An integer array containing the generated colour block.
			 *
			 * \note The red, green and blue values should be within the range 0-255 and this
			 * method truncates them to ensure that they are.  */
			public static int [] AllocStandardColourBlock (int r, int g, int b, ColourModel model)
			{
				  return AllocStandardColourBlock ((uint)((r & 0xff) << ColourTrans.RedShift |
									  (g & 0xff) << ColourTrans.GreenShift |
									  (b & 0xff) << ColourTrans.BlueShift),
									  model);
			}

			/*! \brief Create a colour block from a palette entry.
			 * \param [in] colour The colour as a palette entry in the form 0xBBGGRR00.
			 * \return An integer array containing the generated colour block.  */
			public static int [] AllocStandardColourBlock (uint colour, ColourModel model)
			{
				int [] colour_block = new int [3];

				// The Acorn User Interface Manual seems to have the red and blue values the wrong
				// way around.
				colour_block[0] = (int)colour;
				colour_block[1] = 0;
				colour_block[2] = (int)model;

				return colour_block;
			}

			/*! \brief Retrieve a colour from a colour block.
			 * \param [in] colourBlock The colour block containing the colour.
			 * \return The colour as a palette entry in the form 0xBBGGRR00.  */
			public static uint ColourFromColourBlock (int [] colourBlock)
			{
				return (uint)colourBlock[0];
			}

			/*! \brief Check if the given event is relevant to the %SaveAs dialogue and call the
			 * associated event handlers.  */
			public override void Dispatch (ToolboxEvent ev)
			{
				switch (ev.ToolboxArgs.Header.EventCode)
				{
				case EventCode.AboutToBeShown:
					if (AboutToBeShown != null)
						AboutToBeShown (this, new AboutToBeShownEventArgs (ev.ToolboxArgs.RawEventData));
					break;
				case EventCode.DialogueCompleted:
					if (DialogueComplete != null)
						DialogueComplete (this, new DialogueCompleteEventArgs (ev.ToolboxArgs.RawEventData));
					break;
				case EventCode.ColourSelected:
					if (ColourSelected != null)
						ColourSelected (this, new ColourSelectedEventArgs (ev.ToolboxArgs.RawEventData));
					break;
				}
			}
		}
	}
}
