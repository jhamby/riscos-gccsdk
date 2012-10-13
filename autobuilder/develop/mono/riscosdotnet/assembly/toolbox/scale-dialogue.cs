//
// toolbox/scale-dialogue.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;
using System.Runtime.InteropServices;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \class ScaleDialogue
		 * \brief Allows the user to set the scale factors on a document.  */
		public class ScaleDialogue : Object
		{
			static class Method
			{
				public const int GetWindowID = 0;
				public const int SetValue = 1;
				public const int GetValue = 2;
				public const int SetBounds = 3;
				public const int GetBounds = 4;
				public const int SetTitle = 5;
				public const int GetTitle = 6;
			}

			public static class Factor
			{
				public const uint ScaleToFit = 0xffffffff;
			}

			/*! \brief The flags used when calling SetBounds/GetBounds.  */
			public enum BoundsFlags
			{
				/*! \brief Bit 0 set means the lower bound.  */
				Lower = (1 << 0),
				/*! \brief Bit 1 set means the upper bound.  */
				Upper = (1 << 1),
				/*! \brief Bit 2 set means the step size.  */
				StepSize = (1 << 2)
			}

			/*! \brief The default Toolbox events that this Scale Dialogue object can
			 * raise.  */
			public class EventCode
			{
				public const uint AboutToBeShown = 0x82c00;
				public const uint DialogueCompleted = 0x82c01;
				public const uint ApplyFactor = 0x82c02;
			}

			/*! \brief Create a %Toolbox Scale Dialogue from the named template in the
			 * Resource file.
			 * \param[in] resName The name of the Scale Dialogue template to use.  */
			public ScaleDialogue (string resName) : base (resName)
			{
			}

			/*! \brief Create a %Toolbox Scale Dialogue from the template data given.
			 * \param[in] templateData Pointer to the object template.  */
			public ScaleDialogue (IntPtr templateData)
			{
				Create (templateData);
			}

			/*! \brief Create a %Toolbox Scale Dialogue from an object that already exists.
			 * \param [in] objectID The Toolbox ID of the existing object.  */
			public ScaleDialogue (uint objectID) : base (objectID)
			{
			}

			/*! \brief The signature of a ApplyFactor event handler.  */
			public delegate void ApplyFactorEventHandler (object sender, ApplyFactorEventArgs e);

			/*! \brief The event handlers that will be called just before this Scale Dialogue is shown.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, AboutToBeShownEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * ScaleDialogueObject.AboutToBeShown += handler_name;
			 * \endcode  */
			public event AboutToBeShownEventHandler AboutToBeShown;

			/*! \brief The event handlers that will be called when this dialogue is hidden.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, DialogueCompleteEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * ScaleDialogueObject.DialogueComplete += handler_name;
			 * \endcode  */
			public event ToolboxEventHandler DialogueComplete;

			/*! \brief The event handlers that will be called when the use clicks \e OK in the
			 * dialogue box.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, ScaleDialogue.ApplyFactorEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * ScaleDialogueObject.ApplyFactor += handler_name;
			 * \endcode  */
			public event ApplyFactorEventHandler ApplyFactor;

			/*! \brief Gets the ID of the underlying window object.  */
			public uint WindowID
			{
				get { return GetWindowID (); }
			}

			/*! \brief Gets or sets the value displayed in the writable field for this Scale object.  */
			public int Value
			{
				set { SetValue (value); }
				get { return GetValue (); }
			}

			/*! \brief Gets or sets the text string used in the Scale dialogue's title bar.  */
			public string Title
			{
				set { SetTitle (value); }
				get { return GetTitle (); }
			}

			/*! \brief Set or gets the lower bounds of the writable field in the Scale object.  */
			public int LowerBound
			{
				set { SetBounds (BoundsFlags.Lower, value); }
				get { return GetBounds (BoundsFlags.Lower); }
			}

			/*! \brief Set or gets the upper bounds of the writable field in the Scale object.  */
			public int UpperBound
			{
				set { SetBounds (BoundsFlags.Upper, value); }
				get { return GetBounds (BoundsFlags.Upper); }
			}

			/*! \brief Set or gets the step size of the writable field in the Scale object.  */
			public int StepSize
			{
				set { SetBounds (BoundsFlags.StepSize, value); }
				get { return GetBounds (BoundsFlags.StepSize); }
			}

			/*! \brief Return the ID of the underlying Window object used to implement this
			 * Scale Dialogue.
			 * \return The ID of the Window Object.
			 * \note The \e WindowID property can be used for the same purpose.  */
			public uint GetWindowID ()
			{
				return MiscOp_GetR0 (0, Method.GetWindowID);
			}

			/*! \brief Sets the value displayed in the writable field for this Scale object.  */
			public void SetValue (int value)
			{
				MiscOp_SetR3 (0, Method.SetValue, (uint)value);
			}

			/*! \brief Returns the value in the writable field of this Scale object.  */
			public int GetValue ()
			{
				return (int)MiscOp_GetR0 (0, Method.GetValue);
			}

			/*! \brief Sets the lower and upper bounds and step size of the writable field in
			 * the Scale object.
			 * \param [in] lower The lower bounds of the writable field.
			 * \param [in] upper The upper bounds of the writable field.
			 * \param [in] step The step size of the writable field.
			 * \return Nothing.  */
			public void SetBounds (int lower, int upper, int step)
			{
				MiscOp_SetR3R4R5 ((uint)(BoundsFlags.Lower |
							 BoundsFlags.Upper |
							 BoundsFlags.StepSize),
						  Method.SetBounds,
						  (uint)lower,
						  (uint)upper,
						  (uint)step);
			}

			/*! \brief Sets one of the lower and upper bounds and step size of the writable field in
			 * the Scale object according to the flags given.
			 * \param [in] flags Set bit 0, 1 or 2 to indicate which bounds value to set.
			 * \param [in] value The value to set the bounds to.
			 * \return Nothing.  */
			public void SetBounds (BoundsFlags flags, int value)
			{
				int lower = 0;
				int upper = 0;
				int step = 0;

				switch (flags)
				{
				case BoundsFlags.Lower:
					lower = value;
					break;
				case BoundsFlags.Upper:
					upper = value;
					break;
				case BoundsFlags.StepSize:
					step = value;
					break;
				}

				MiscOp_SetR3R4R5 ((uint)flags,
						  Method.SetBounds,
						  (uint)lower,
						  (uint)upper,
						  (uint)step);
			}

			/*! \brief Returns the lower and upper bounds and step size of the 
			 * writable field in the Scale object.
			 * \param [out] lower The lower bounds of the writable field.
			 * \param [out] upper The upper bounds of the writable field.
			 * \param [out] step The step size of the writable field.
			 * \return Nothing.  */
			public void GetBounds (out int lower, out int upper, out int step)
			{
				MiscOp_GetR0R1R2 ((uint)(BoundsFlags.Lower |
							 BoundsFlags.Upper |
							 BoundsFlags.StepSize),
						  Method.GetBounds,
						  out lower,
						  out upper,
						  out step);
			}

			/*! \brief Return one of the lower and upper bounds or step size of the
			 * writable field in the Scale object.
			 * \param [in] flags Flag indicating which bounds to return.
			 * \return The value of the bounds requested.  */
			public int GetBounds (BoundsFlags flags)
			{
				int lower;
				int upper;
				int step;

				MiscOp_GetR0R1R2 ((uint)flags,
						  Method.GetBounds,
						  out lower,
						  out upper,
						  out step);

				switch (flags)
				{
				case BoundsFlags.Lower:
					return (int)lower;
				case BoundsFlags.Upper:
					return (int)upper;
				case BoundsFlags.StepSize:
					return (int)step;
				}

				throw new ArgumentOutOfRangeException ("flags", "Only bits 0, 1 or 2 should be set");
			}

			/*! \brief Sets the text which is to be used in the title bar of the Scale Dialogue box.
			 * \param [in] text The text to use as the title.
			 * \return Nothing.
			 * \note The \e Title property can be used for the same purpose.  */
			public void SetTitle (string text)
			{
				MiscOp_SetR3 (0, Method.SetTitle, text);
			}

			/*! \brief Returns the text string used in the Scale Dialogue box title bar.
			 * \return The text string used in the title bar.
			 * \note The \e Title property can be used for the same purpose.  */
			public string GetTitle ()
			{
				return GetText (Method.GetTitle);
			}

			/*! \brief Check if the given event is relevant to the Scale Dialogue and call the
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
						DialogueComplete (this, ev.ToolboxArgs);
					break;
				case EventCode.ApplyFactor:
					if (ApplyFactor != null)
						ApplyFactor (this, new ApplyFactorEventArgs (ev.ToolboxArgs.RawEventData));
					break;
				}
			}

			/*! \brief An object that encapsulates the arguments for the event that is raised
			 * when the user clicks on the \b Scale button or the \b Scale \b To \b Fit button
			 * (if present), or presses Return.  */
			public class ApplyFactorEventArgs : ToolboxEventArgs
			{
				/*! \brief Constant defining event specific data offset after the header.  */
				public static class EventOffset
				{
					public const int Factor = 16;
				}

				/*! \brief The scale factor to apply as a percentage; 0xffffffff means Scale To Fit.  */
				public readonly uint Factor;

				public ApplyFactorEventArgs (IntPtr unmanagedEventData) : base (unmanagedEventData)
				{
					Factor = (uint)Marshal.ReadInt32 (RawEventData, EventOffset.Factor);
				}
			}
		}
	}
}
