//
// toolbox/font-dialogue.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;
using System.Runtime.InteropServices;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \class FontDialogue
		 * \brief A Font Dialogue box shows font, weight and style of the currently
		 * selected font.  */
		public class FontDialogue : Object
		{
			static class Method
			{
				public const int GetWindowID = 0;
				public const int SetFont = 1;
				public const int GetFont = 2;
				public const int SetSize = 3;
				public const int GetSize = 4;
				public const int SetTryString = 5;
				public const int GetTryString = 6;
				public const int SetTitle = 7;
				public const int GetTitle = 8;
			}

			public enum SetSizeFlag
			{
				Height = 1,
				AspectRatio
			}

			/*! \brief The default Toolbox events that this FontDialogue object can
			 * raise.  */
			public class EventCode
			{
				public const uint AboutToBeShown = 0x82a00;
				public const uint DialogueCompleted = 0x82a01;
				public const uint ApplyFont = 0x82a02;
			}

			/*! \brief Create a Toolbox %FontDialogue object from the named resource.
			 * \param [in] resName The name of the template in the resource file.  */
			public FontDialogue (string resName) : base (resName)
			{
			}

			/*! \brief Create a Toolbox %FontDialogue from the template data given.
			 * \param[in] templateData Pointer to the menu template.  */
			public FontDialogue (IntPtr templateData)
			{
				Create (templateData);
			}

			/*! \brief Create a %FontDialogue from a Toolbox object that already exists.
			 * \param [in] objectID The Toolbox ID of the existing object.  */
			public FontDialogue (uint ObjectID) : base (ObjectID)
			{
			}

			/*! \brief Gets the ID of the underlying window object.  */
			public uint WindowID
			{
				get { return GetWindowID (); }
			}

			/*! \brief Gets or sets the current font id for this Font Dialogue.  */
			public string Font
			{
				set { SetFont (value); }
				get { return GetFont(); }
			}

			/*! \brief Gets or sets the height displayed in the Font Dialogue.  */
			public int Height
			{
				set { SetSize (SetSizeFlag.Height, value, 0); }
				get {
					int height, aspect_ration;
					GetSize (out height, out aspect_ration);
					return height;
				}
			}

			/*! \brief Gets or sets the aspect ratio displayed in the Font Dialogue.  */
			public int AspectRatio
			{
				set { SetSize (SetSizeFlag.AspectRatio, 0, value); }
				get {
					int height, aspect_ration;
					GetSize (out height, out aspect_ration);
					return aspect_ration;
				}
			}

			/*! \brief Gets or sets the string currently display in the \b Try writable field
			 * of the Font Dialogue.  */
			public string TryString
			{
				set { SetTryString (value); }
				get { return GetTryString (); }
			}

			/*! \brief Gets or sets the text string used in the title bar of the Font Dialogue box.  */
			public string Title
			{
				set { SetTitle (value); }
				get { return GetTitle (); }
			}

			/*! \brief Return the ID of the underlying Window object used to implement this
			 * Font Dialogue.
			 * \return The ID of the Window Object.
			 * \note The \e WindowID property can be used for the same purpose.  */
			public uint GetWindowID ()
			{
				return MiscOp_GetR0 (0, Method.GetWindowID);
			}

			/*! \brief Selects a font as being the currently selected one for this Font Dialogue
			 * box.
			 * 
			 * The special font id "SystemFont" is used to indicate that the \b System entry
			 * should be selected.
			 * \param [in] font The font id of the font to select (null means none).
			 * \return Nothing.
			 * \note The \e Font property can be used for the same purpose.  */
			public void SetFont (string font)
			{
				MiscOp_SetR3 (0, Method.SetFont, font);
			}

			/*! \brief Return the font id for the font which was last specified either
			 * programmatically or by user choice.
			 * 
			 * The special font id "SystemFont" is used to indicate that the \b System entry
			 * is selected.
			 * \return The font id.
			 * \note The \e Font property can be used for the same purpose.  */
			public string GetFont ()
			{
				return GetText (Method.GetFont);
			}

			/*! \brief Sets the height and/or the aspect ratio displayed in the Font
			 * Dialogue.
			 * \param [in] flags Set SetSizeFlag.Height (bit 0 set) to change font height,<br>
			 *                   Set SetSizeFlag.AspectRatio (bit 1 set) to change aspect ratio.
			 * \param [in] height The height to set the font to (if bit 0 of flags set).
			 * \param [in] aspectRation The aspect ratio to set the font to (if bit 1 of flags set).
			 * \return Nothing.
			 * \note The \e Size and \e AspectRatio properties can be used for the same purpose.  */
			public void SetSize (SetSizeFlag flags, int height, int aspectRatio)
			{
				MiscOp_SetR3R4 ((uint)flags, Method.SetSize, (uint)height, (uint)aspectRatio);
			}

			/*! \brief Gets the height and apsect ratio currently displayed in the Font Dialogue.
			 * \param [out] height The returned height displayed in the Font Dialogue.
			 * \param [out] aspectRatio The returned aspect ratio displayed in the Font Dialogue.
			 * \return Nothing.
			 * \note The \e Size and \e AspectRatio properties can be used for the same purpose.  */
			public void GetSize (out int height, out int aspectRatio)
			{
				uint r0, r1;

				MiscOp_GetR0R1 (0, Method.GetSize, out r0, out r1);

				height = (int)r0;
				aspectRatio = (int)r1;
			}

			/*! \brief Sets the string used in the \b Try writable field of the Font Dialogue.
			 * \param [in] text The text to be used.
			 * \return Nothing.
			 * \exception An OS.ErrorException is thrown if the string is longer than 64 characters.
			 * \note The \e TryString property can be used for the same purpose.  */
			public void SetTryString (string text)
			{
				MiscOp_SetR3 (0, Method.SetTryString, text);
			}

			/*! \brief Return the string currently displayed in the \b Try writable field of the
			 * Font Dialogue.
			 * \return Nothing.
			 * \note The \e TryString property can be used for the same purpose.  */
			public string GetTryString ()
			{
				return GetText (Method.GetTryString);
			}

			/*! \brief Sets the text which is to be used in the title bar of the Font Dialogue box.
			 * \param [in] text The text to use as the title.
			 * \return Nothing.
			 * \note The \e Title property can be used for the same purpose.  */
			public void SetTitle (string text)
			{
				MiscOp_SetR3 (0, Method.SetTitle, text);
			}

			/*! \brief Returns the text string used in the Font Dialogue box title bar.
			 * \return The text string used in the title bar.
			 * \note The \e Title property can be used for the same purpose.  */
			public string GetTitle ()
			{
				return GetText (Method.GetTitle);
			}

			/*! \brief Raising an event invokes the event handler through a delegate.
			 *
			 * The \b OnAboutToBeShown method also allows derived classes to handle the
			 * event without attaching a delegate. This is the preferred technique for
			 * handling the event in a derived class.
			 * \note  When overriding \b OnAboutToBeShown in a derived class, be sure to
			 * call the base class's \b OnAboutToBeShown method so that registered delegates
			 * receive the event.  */
			protected virtual void OnAboutToBeShown (AboutToBeShownEventArgs e)
			{
				if (AboutToBeShown != null)
					AboutToBeShown (this, e);
			}

			/*! \brief Raising an event invokes the event handler through a delegate.
			 *
			 * The \b OnDialogueCompleted method also allows derived classes to handle the
			 * event without attaching a delegate. This is the preferred technique for
			 * handling the event in a derived class.
			 * \note  When overriding \b OnDialogueCompleted in a derived class, be sure to
			 * call the base class's \b OnDialogueCompleted method so that registered delegates
			 * receive the event.  */
			protected virtual void OnDialogueCompleted (ToolboxEventArgs e)
			{
				if (DialogueCompleted != null)
					DialogueCompleted (this, e);
			}

			/*! \brief Raising an event invokes the event handler through a delegate.
			 *
			 * The \b OnApplyFont method also allows derived classes to handle the
			 * event without attaching a delegate. This is the preferred technique for
			 * handling the event in a derived class.
			 * \note  When overriding \b OnApplyFont in a derived class, be sure to
			 * call the base class's \b OnApplyFont method so that registered delegates
			 * receive the event.  */
			protected virtual void OnApplyFont (ApplyFontEventArgs e)
			{
				if (ApplyFont != null)
					ApplyFont (this, e);
			}

			/*! \brief Check if the given event is relevant to the Font Dialogue and call the
			 * associated event handlers.  */
			public override void Dispatch (ToolboxEventArgs e)
			{
				switch (e.Header.EventCode)
				{
				case EventCode.AboutToBeShown:
					OnAboutToBeShown (new AboutToBeShownEventArgs (e.RawEventData));
					break;
				case EventCode.DialogueCompleted:
					OnDialogueCompleted (e);
					break;
				case EventCode.ApplyFont:
					OnApplyFont (new ApplyFontEventArgs (e.RawEventData));
					break;
				}
			}

			/*! \brief The event handlers that will be called just before this Font Dialogue is shown.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, Object.AboutToBeShownEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * FontDialogueObject.AboutToBeShown += handler_name;
			 * \endcode  */
			public event EventHandler<AboutToBeShownEventArgs> AboutToBeShown;

			/*! \brief The event handlers that will be called when this dialogue is hidden.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, ToolboxEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * FontDialogueObject.DialogueCompleted += handler_name;
			 * \endcode  */
			public event EventHandler<ToolboxEventArgs> DialogueCompleted;

			/*! \brief The event handlers that will be called when a selection has been made
			 * in the Font Dialogue.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, FontDialogue.ApplyFontEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * FontDialogueObject.ApplyFont += handler_name;
			 * \endcode  */
			public event EventHandler<ApplyFontEventArgs> ApplyFont;

			/*! \brief An object that encapsulates the arguments for the event that is raised
			 * when a Font Dialogue box selection has been made.  */
			public class ApplyFontEventArgs : ToolboxEventArgs
			{
				/*! \brief Constants defining event specific data offsets after the header.  */
				public static class EventOffset
				{
					public const int FontHeight = 16;
					public const int AspectRatio = 20;
					public const int FontID = 24;
				}

				public readonly int FontHeight;
				public readonly int AspectRatio;
				public readonly string FontID;

				public ApplyFontEventArgs (IntPtr unmanagedEventData) : base (unmanagedEventData)
				{
					FontHeight = Marshal.ReadInt32 (RawEventData, EventOffset.FontHeight);
					AspectRatio = Marshal.ReadInt32 (RawEventData, EventOffset.AspectRatio);
					FontID = Marshal.PtrToStringAnsi (new IntPtr (RawEventData.ToInt32() + EventOffset.FontID));
				}
			}
		}
	}
}
