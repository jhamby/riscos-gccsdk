//
// toolbox/quit-dialogue.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;
using System.Runtime.InteropServices;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \brief Encapsulates a Toolbox Quit Dialogue Box.  */
		public class QuitDialogue : Object
		{
			static class Method
			{
				public const int GetWindowID = 0;
				public const int SetMessage = 1;
				public const int GetMessage = 2;
				public const int SetTitle = 3;
				public const int GetTitle = 4;
			}

			/*! \brief Default Toolbox events that can be generated by the %Toolbox Quit Dialogue object.  */
			public static class EventCode
			{
				public const int AboutToBeShown = 0x82a90;
				public const int QuitQuit = 0x82a91;
				public const int DialogueCompleted = 0x82a92;
				public const int QuitCancel = 0x82a93;
			}

			/*! \brief Create a %Toolbox Quit Dialogue from the named template in the
			 * Resource file.
			 * \param[in] resName The name of the Quit Dialogue template to use.  */
			public QuitDialogue (string resName) : base (resName)
			{
			}

			/*! \brief Create a %Toolbox Quit Dialogue from the template data given.
			 * \param[in] templateData Pointer to the window template.  */
			public QuitDialogue (IntPtr templateData)
			{
				Create (templateData);
			}

			/*! \brief Create a %Toolbox Quit Dialogue from an object that already exists.
			 * \param [in] objectID The Toolbox ID of the existing object.  */
			public QuitDialogue (uint objectID) : base (objectID)
			{
			}

			/*! \brief Get the ID of the underlying Window object.  */
			public uint WindowID
			{
				get { return GetWindowID (); }
			}

			/*! \brief Gets or sets the message used in the Quit Dialogue's Window.  */
			public string Message
			{
				set { SetMessage (value); }
				get { return GetMessage (); }
			}

			/*! \brief Gets or sets the text which is to be used in the title bar
			 * of this Quit Dialogue's Window.  */
			public string Title
			{
				set { SetTitle (value); }
				get { return GetTitle (); }
			}

			/*! \brief Return the ID of the underlying Window object used to implement this
			 * Quit Dialogue.
			 * \return The ID of the Window Object.
			 * \note The \e WindowID property can be used for the same purpose.  */
			public uint GetWindowID ()
			{
				return MiscOp_GetR0 (0, Method.GetWindowID);
			}

			/*! \brief Set the text string which is used in the title bar of the Quit dialogue.
			 * \note The \e Title property can be used for the same purpose.  */
			public void SetTitle (string title)
			{
				SetText (0, Method.SetTitle, title);
			}

			/*! \brief Return the text string which is used in the title bar of the Quit dialogue.
			 * \note The \e Title property can be used for the same purpose.  */
			public string GetTitle ()
			{
				return GetText (Method.GetTitle);
			}

			/*! \brief Set the text string which is used as the message in the Quit dialogue.
			 * \note The \e Message property can be used for the same purpose.  */
			public void SetMessage (string message)
			{
				SetText (0, Method.SetMessage, message);
			}

			/*! \brief Return the text string which is used as the message in the Quit dialogue.
			 * \note The \e Message property can be used for the same purpose.  */
			public string GetMessage ()
			{
				return GetText (Method.GetMessage);
			}

			protected virtual void OnAboutToBeShown (ToolboxEvent e)
			{
				if (AboutToBeShown != null)
					AboutToBeShown (this, new AboutToBeShownEventArgs (e.ToolboxArgs.RawEventData));
			}

			protected virtual void OnDialogueCompleted (ToolboxEvent e)
			{
				if (DialogueCompleted != null)
					DialogueCompleted (this, e.ToolboxArgs);
			}

			protected virtual void OnClickQuit (ToolboxEvent e)
			{
				if (ClickQuit != null)
					ClickQuit (this, e.ToolboxArgs);
			}

			protected virtual void OnClickCancel (ToolboxEvent e)
			{
				if (ClickCancel != null)
					ClickCancel (this, e.ToolboxArgs);
			}

			/*! \brief Check if the given event is relevant to the Quit Dialogue and call the
			 * associated event handlers.  */
			public override void Dispatch (ToolboxEvent e)
			{
				switch (e.ToolboxArgs.Header.EventCode)
				{
				case EventCode.AboutToBeShown:
					OnAboutToBeShown (e);
					break;
				case EventCode.DialogueCompleted:
					OnDialogueCompleted (e);
					break;
				case EventCode.QuitQuit:
					OnClickQuit (e);
					break;
				case EventCode.QuitCancel:
					OnClickCancel (e);
					break;
				}
			}

			/*! \brief The signature of a DialogueCompleted event handler.  */
			public delegate void DialogueCompletedEventHandler (object sender, ToolboxEventArgs e);

			/*! \brief The event handlers that will be called just before this Quit Dialogue is shown.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, Object.AboutToBeShownEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * QuitDialogueObject.AboutToBeShown += handler_name;
			 * \endcode  */
			public event AboutToBeShownEventHandler AboutToBeShown;

			/*! \brief The event handlers that will be called when this dialogue is hidden.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, ToolboxEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * QuitDialogueObject.DialogueCompleted += handler_name;
			 * \endcode  */
			public event ToolboxEventHandler DialogueCompleted;

			/*! \brief The event handlers that will be called when a Quit event is raised.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, ToolboxEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * QuitDialogueObject.ClickQuit += handler_name;
			 * \endcode  */
			public event ToolboxEventHandler ClickQuit;

			/*! \brief The event handlers that will be called when a Cancel event is raised.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, ToolboxEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * QuitDialogueObject.ClickCancel += handler_name;
			 * \endcode  */
			public event ToolboxEventHandler ClickCancel;
		}
	}
}
