using System;
using System.Runtime.InteropServices;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \brief Encapsulates a Toolbox SaveAs Dialogue Box.  */
		public class SaveAsDialogue : Object
		{
			static class Method
			{
				public const int GetWindowID = 0;
				public const int SetTitle = 1;
				public const int GetTitle = 2;
				public const int SetFileName = 3;
				public const int GetFileName = 4;
				public const int SetFileType = 5;
				public const int GetFileType = 6;
				public const int SetFileSize = 7;
				public const int GetFileSize = 8;
				public const int SelectionAvailable = 9;
				public const int SetDataAddress = 10;
				public const int BufferFilled = 11;
				public const int FileSaveCompleted = 12;
			}

			/*! \brief Default Toolbox events that can be generated by the Toolbox SaveAs object.  */
			public static class EventCode
			{
				public const int AboutToBeShown = 0x82bc0;
				public const int DialogueCompleted = 0x82bc1;
				public const int SaveToFile = 0x82bc2;
				public const int FillBuffer = 0x82bc3;
				public const int SaveCompleted = 0x82bc4;
			}


			/*! \brief Create a Toolbox SaveAs Dialogue from the named template in the
			 * Resource file.
			 * \param[in] resName The name of the SaveAs template to use.  */
			public SaveAsDialogue (string resName) : base (resName)
			{
			}

			/*! \brief Create a Toolbox SaveAs from the template data given.
			 * \param[in] templateData Pointer to the window template.  */
			public SaveAsDialogue (IntPtr templateData)
			{
				Create (templateData);
			}

			/*! \brief Create a Toolbox SaveAs from an object that already exists.
			 * \param [in] objectID The Toolbox ID of the existing object.  */
			public SaveAsDialogue (uint objectID) : base (objectID)
			{
			}

			/*! \brief Get the ID of the underlying Window object.  */
			public uint WindowID
			{
				get { return GetWindowID (); }
			}

			/*! \brief Gets or sets the text which is to be used in the title bar
			 * of this SaveAs dialogue.  */
			public string Title
			{
				set { SetTitle (value); }
				get { return GetTitle (); }
			}

			/*! \brief Gets or sets the filename displayed in the writable
			 * field of the SaveAs dialogue.  */
			public string FileName
			{
				set { SetFileName (value); }
				get { return GetFileName (); }
			}

			/*! \brief Gets or sets the filetype for this SaveAs dialogue, and therefore
			 * the sprite which will be displayed.  */
			public int FileType
			{
				set { SetFileType (value); }
				get { return GetFileType (); }
			}

			/*! \brief Gets or sets the estimated file size in bytes for this SaveAs dialogue.  */
			public int FileSize
			{
				set { SetFileSize (value); }
				get { return GetFileSize (); }
			}

			/*! \brief Indicate whether there is a current selection in existence.  */
			public bool SelectionAvailable
			{
				set { SetSelectionAvailable (value); }
			}

			/*! \brief Return the ID of the underlying Window object used to implement this
			 * SaveAs Dialogue.
			 * \return The ID of the Window Object.
			 * \note The \e WindowID property can be used for the same purpose.  */
			public uint GetWindowID ()
			{
				return MiscOp_GetR0 (0, Method.GetWindowID);
			}

			/*! \brief Set the text string which is used in the title bar of the SaveAs dialogue.
			 * \note The \e Title property can be used for the same purpose.  */
			public void SetTitle (string title)
			{
				SetText (0, Method.SetTitle, title);
			}

			/*! \brief Return the text string which is used in the title bar of the SaveAs dialogue.
			 * \note The \e Title property can be used for the same purpose.  */
			public string GetTitle ()
			{
				return GetText (Method.GetTitle);
			}

			/*! \brief Set the filename which is to be used in the SaveAs dialogue's writable field.
			 * \note The \e FileName property can be used for the same purpose.  */
			public void SetFileName (string filename)
			{
				SetText (0, Method.SetTitle, filename);
			}

			/*! \brief Return the filename displayed in the SaveAs dialogue's writable field.
			 * \note The \e FileName property can be used for the same purpose.  */
			public string GetFileName ()
			{
				return GetText (Method.GetTitle);
			}

			/*! \brief Set the filetype for this SaveAs dialogue, and hence the sprite which will
			 * be displayed in the dialogue box.
			 * \note The \e FileType property can be used for the same purpose.  */
			public void SetFileType (int filetype)
			{
				MiscOp_SetR3 (0, Method.SetFileType, (uint)filetype);
			}

			/*! \brief Return the filetype of this SaveAs object.
			 * \note The \e FileType property can be used for the same purpose.  */
			public int GetFileType ()
			{
				return (int)MiscOp_GetR0 (0, Method.GetFileType);
			}

			/*! \brief Set the estimated file size in bytes for this SaveAs dialogue.
			 * \note The \e FileSize property can be used for the same purpose.  */
			public void SetFileSize (int filesize)
			{
				MiscOp_SetR3 (0, Method.SetFileSize, (uint)filesize);
			}

			/*! \brief Return the file size of this SaveAs object.
			 * \note The \e FileSize property can be used for the same purpose.  */
			public int GetFileSize ()
			{
				return (int)MiscOp_GetR0 (0, Method.GetFileSize);
			}

			/*! \brief Indicate to the SaveAs module whether there is a current selection in
			 * existence.
			 * If there is a selection, then the \b Selection option button will be enabled,
			 * if not the \b Selection option button will be greyed out.<br>
			 * \throw OS.ErrorException Thrown if the SaveAs object has no \b Selection option
			 * button.
			 * \note The \e SelectionAvailable property can be used for the same purpose.  */
			public void SetSelectionAvailable (bool selection)
			{
				MiscOp_SetR3 (0, Method.SelectionAvailable, selection ? 1U : 0);
			}

			/*! \brief Indicate to the SaveAs module the address of a contiguous block of
			 * memory containing data to be saved.  */
			public void SetDataAddress (IntPtr data, int dataSize,
						    IntPtr selection, int selectionSize)
			{
				MiscOp_SetR3R4R5R6 (0, Method.SetDataAddress,
						    (uint)data.ToInt32 (),
						    dataSize,
						    selection.ToInt32 (),
						    selectionSize);
			}

			/*! \brief Used to respond to a FillBuffer event; it confirms that the requested buffer
			 * fill has taken place, and states the number of bytes written to the buffer.  */
			public void BufferFilled (IntPtr buffer, int bytesWritten)
			{
				MiscOp_SetR3R4 (0, Method.BufferFilled, (uint)buffer.ToInt32(), (uint)bytesWritten);
			}

			/*! \brief Report whether an attempt to save the data to a file as a result of a
			 * SaveToFile event was successful or not.
			 * \throw OS.ErrorException Thrown if \e wasSuccessful is \e false.  */
			public void FileSaveCompleted (string filename, bool wasSuccessful)
			{
				MiscOp_SetR3 (wasSuccessful ? 1U : 0, Method.FileSaveCompleted, filename);
			}

			protected virtual void OnAboutToBeShown (ToolboxEvent e)
			{
				if (AboutToBeShown != null)
					AboutToBeShown (this, new AboutToBeShownEventArgs (e.ToolboxArgs.RawEventData));
			}

			protected virtual void OnDialogueCompleted (ToolboxEvent e)
			{
				if (DialogueCompleted != null)
					DialogueCompleted (this, new DialogueCompletedEventArgs (e.ToolboxArgs.RawEventData));
			}

			protected virtual void OnSaveToFile (ToolboxEvent e)
			{
				if (SaveToFile != null)
					SaveToFile (this, new SaveToFileEventArgs (e.ToolboxArgs.RawEventData));
			}

			protected virtual void OnFillBuffer (ToolboxEvent e)
			{
				if (FillBuffer != null)
					FillBuffer (this, new FillBufferEventArgs (e.ToolboxArgs.RawEventData));
			}

			/*! \brief Check if the given event is relevant to the SaveAs dialogue and call the
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
				case EventCode.SaveToFile:
					OnSaveToFile (e);
					break;
				case EventCode.FillBuffer:
					OnFillBuffer (e);
					break;
				}
			}

			/*! \brief The signature of a DialogueCompleted event handler.  */
			public delegate void DialogueCompletedEventHandler (object sender, DialogueCompletedEventArgs e);

			/*! \brief The signature of a SaveToFile event handler.  */
			public delegate void SaveToFileEventHandler (object sender, SaveToFileEventArgs e);

			/*! \brief The signature of a FillBuffer event handler.  */
			public delegate void FillBufferEventHandler (object sender, FillBufferEventArgs e);

			/*! \brief The event handlers that will be called just before this %SaveAs is shown.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, Object.AboutToBeShownEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * SaveAsObject.AboutToBeShown += handler_name;
			 * \endcode  */
			public event AboutToBeShownEventHandler AboutToBeShown;

			/*! \brief The event handlers that will be called when this dialogue is hidden.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, DialogueCompletedEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * SaveAsObject.DialogueComplete += handler_name;
			 * \endcode  */
			public event DialogueCompletedEventHandler DialogueCompleted;

			/*! \brief The event handlers that will be called when the client is required to
			 * save its data to a file.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, SaveToFileEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * SaveAsObject.SaveToFile += handler_name;
			 * \endcode  */
			public event SaveToFileEventHandler SaveToFile;

			/*! \brief The event handlers that will be called when the client is required to
			 * fill the given buffer.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, FillBufferEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * SaveAsObject.FillBuffer += handler_name;
			 * \endcode  */
			public event FillBufferEventHandler FillBuffer;

			/*! \brief An object that encapsulates the arguments for the event that is raised
			 * after the SaveAs dialogue has been hidden.  */
			public class DialogueCompletedEventArgs : ToolboxEventArgs
			{
				/*! \brief Constant defining the possible bit flag returned by this event.  */
				public static class Flags
				{
					public const int SaveSuccessful = (1 << 0);
				}

				public readonly bool SaveSuccessful;

				public DialogueCompletedEventArgs (IntPtr unmanagedEventData) : base (unmanagedEventData)
				{
					SaveSuccessful = (Header.Flags & Flags.SaveSuccessful) != 0;
				}
			}

			/*! \brief An object that encapsulates the arguments for the event that is raised
			 * by a SaveAs dialogue to request that the client save its data to the given filename.  */
			public class SaveToFileEventArgs : ToolboxEventArgs
			{
				/*! \brief Constant defining event specific data offset after the header.  */
				public static class EventOffset
				{
					public const int Filename = 16;
				}

				/*! \brief Constant defining the possible bit flag returned by this event.  */
				public static class Flags
				{
					public const int SelectionOnly = (1 << 0);
				}

				/*! \brief The name of the file where the data should be saved.  */
				public readonly string Filename;
				/*! \brief \e true if a selection is being saved.  */
				public readonly bool SelectionOnly;

				public SaveToFileEventArgs (IntPtr unmanagedEventData) : base (unmanagedEventData)
				{
					SelectionOnly = (Header.Flags & Flags.SelectionOnly) != 0;
					Filename = Marshal.PtrToStringAnsi (new IntPtr (RawEventData.ToInt32 () +
											EventOffset.Filename));
				}
			}

			/*! \brief An object that encapsulates the arguments for the event that is raised
			 * by a SaveAs dialogue to request that the client fill the given buffer.  */
			public class FillBufferEventArgs : ToolboxEventArgs
			{
				/*! \brief Constants defining event specific data offsets after the header.  */
				public static class EventOffset
				{
					public const int BufferSize = 16;
					public const int BufferAddress = 20;
					public const int BytesTransmitted = 24;
				}

				/*! \brief Constant defining the possible bit flag returned by this event.  */
				public static class Flags
				{
					public const int SelectionOnly = (1 << 0);
				}

				/*! \brief \e true if a selection is being saved.  */
				public readonly bool SelectionOnly;
				/*! \brief The size of the buffer being used.  */
				public readonly int BufferSize;
				/*! \brief The address of the buffer.  */
				public readonly IntPtr BufferAddress;
				/*! \brief The number of bytes already transmitted.  */
				public readonly int BytesTransmitted;

				public FillBufferEventArgs (IntPtr unmanagedEventData) : base (unmanagedEventData)
				{
					SelectionOnly = (Header.Flags & Flags.SelectionOnly) != 0;
					BufferSize = Marshal.ReadInt32 (RawEventData, EventOffset.BufferSize);
					BufferAddress = Marshal.ReadIntPtr (RawEventData, EventOffset.BufferAddress);
					BytesTransmitted = Marshal.ReadInt32 (RawEventData, EventOffset.BytesTransmitted);
				}
			}
		}
	}
}
