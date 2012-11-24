//
// toolbox/print-dialogue.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;
using System.Runtime.InteropServices;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \brief Allows the user to set a number of print options and then request
		 * that a document be printed given these options.  */
		public class PrintDialogue : Object
		{
			static class Method
			{
				public const int GetWindowID = 0;
				public const int SetPageRange = 1;
				public const int GetPageRange = 2;
				public const int SetCopies = 3;
				public const int GetCopies = 4;
				public const int SetScale = 5;
				public const int GetScale = 6;
				public const int SetOrientation = 7;
				public const int GetOrientation = 8;
				public const int GetTitle = 9;
				public const int SetDraft = 10;
				public const int GetDraft = 11;
			}

			/*! \brief The default Toolbox events that this Print Dialogue object can
			 * raise.  */
			public class EventCode
			{
				public const uint AboutToBeShown = 0x82b00;
				public const uint DialogueCompleted = 0x82b01;
				public const uint SetUpAboutToBeShown = 0x82b02;
				public const uint Save = 0x82b03;
				public const uint SetUp = 0x82b04;
				public const uint Print = 0x82b05;
			}

			/*! \brief Create a %Toolbox Print Dialogue from the named template in the
			 * Resource file.
			 * \param[in] resName The name of the Print Dialogue template to use.  */
			public PrintDialogue (string resName) : base (resName)
			{
			}

			/*! \brief Create a %Toolbox Print Dialogue from the template data given.
			 * \param[in] templateData Pointer to the object template.  */
			public PrintDialogue (IntPtr templateData)
			{
				Create (templateData);
			}

			/*! \brief Create a %Toolbox Print Dialogue from an object that already exists.
			 * \param [in] objectID The Toolbox ID of the existing object.  */
			public PrintDialogue (uint objectID) : base (objectID)
			{
			}

			/*! \brief Gets the ID of the underlying window object.  */
			public uint WindowID
			{
				get { return GetWindowID (); }
			}

			/*! \brief Sets or gets the value of the copies field in the Print Dialogue.  */
			public int Copies
			{
				set { SetCopies (value); }
				get { return GetCopies (); }
			}

			/*! \brief Sets or gets the percentage value to scale by in the Print Dialogue.  */
			public int Scale
			{
				set { SetScale (value); }
				get { return GetScale (); }
			}

			/*! \brief Sets or gets the orientation field in the Print Dialogue.
			 * \e true means Sideways (landscape), \e false means Upright (portrait).  */
			public bool Orientation
			{
				set { SetOrientation (value); }
				get { return GetOrientation (); }
			}

			/*! \brief Gets the string used in the title bar of the Print object.  */
			public string Title
			{
				get { return GetTitle (); }
			}

			/*! \brief Sets or gets whether draft printing is enabled in the Print Dialogue.  */
			public bool Draft
			{
				set { SetDraft (value); }
				get { return GetDraft (); }
			}

			/*! \brief Return the ID of the underlying Window object used to implement this
			 * Print Dialogue.
			 * \return The ID of the Window Object.  */
			public uint GetWindowID ()
			{
				return MiscOp_GetR0 (0, Method.GetWindowID);
			}

			/*! \brief Set the page range for a Print Dialogue.
			 * \param [in] start The first page to start printing from (-1 means 'All')
			 * \param [in] end The last page in the range to be printed.
			 * \return Nothing.  */
			public void SetPageRange (int start, int end)
			{
				MiscOp_SetR3R4 (0, Method.SetPageRange, (uint)start, (uint)end);
			}

			/*! \brief Return the page range for this Print Dialogue.
			 * \param [out] start The first page to start printing from (-1 means 'All')
			 * \param [out] end The last page in the range to be printed.
			 * \return Nothing.  */
			public void GetPageRange (out int start, out int end)
			{
			    uint s, e;

			    MiscOp_GetR0R1 (0, Method.GetPageRange, out s, out e);

			    start = (int)s;
			    end = (int)e;
			}

			/*! \brief Set the number of copies field for the Print Dialogue.
			 * \param [in] copies The number of copies to set.
			 * \return Nothing.  */
			public void SetCopies (int copies)
			{
				MiscOp_SetR3 (0, Method.SetCopies, (uint)copies);
			}

			/*! \brief Return the value of the copies field for this Print Dialogue.
			 * \return The value of the copies field.  */
			public int GetCopies ()
			{
				return (int)MiscOp_GetR0 (0, Method.GetCopies);
			}

			/*! \brief Set the scale factor for the Print Dialogue.
			 * \param [in] scale The percentage value to scale by.
			 * \return Nothing.  */
			public void SetScale (int scale)
			{
				MiscOp_SetR3 (0, Method.SetScale, (uint)scale);
			}

			/*! \brief Return the scale factor for the Print Dialogue.
			 * \return The percentage value to scale by.  */
			public int GetScale ()
			{
				return (int)MiscOp_GetR0 (0, Method.GetScale);
			}

			/*! \brief Set the orientation for the Print Dialogue.
			 * \param [in] landscape \e true means Sideways (landscape), \e false means Upright (portrait).
			 * \return Nothing.  */
			public void SetOrientation (bool landscape)
			{
				MiscOp_SetR3 (0, Method.SetOrientation, landscape ? 1U : 0);
			}

			/*! \brief Return the orientation setting for this Print Dialogue.
			 * \return \e true if Sideways (landscape), \e false if Upright (portrait).  */
			public bool GetOrientation ()
			{
				return MiscOp_GetR0 (0, Method.GetOrientation) == 0 ? false : true;
			}

			/*! \brief Return the current string used in the Print object's title bar.
			 * \return The title string.  */
			public string GetTitle ()
			{
				return GetText (Method.GetTitle);
			}

			/*! \brief Set whether draft printing is used for the Print Dialogue.
			 * \param [in] draft \e true if draft printing is enable, otherwise \e false.
			 * \return Nothing.  */
			public void SetDraft (bool draft)
			{
				MiscOp_SetR3 (0, Method.SetDraft, draft ? 1U : 0);
			}

			/*! \brief Return whether draft printing is used for this Print Dialogue.
			 * \return \e true if draft printing is enable, otherwise \e false.  */
			public bool GetDraft ()
			{
				return MiscOp_GetR0 (0, Method.GetDraft) == 0 ? false : true;
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
			 * The \b OnSetUpAboutToBeShown method also allows derived classes to handle the
			 * event without attaching a delegate. This is the preferred technique for
			 * handling the event in a derived class.
			 * \note  When overriding \b OnSetUpAboutToBeShown in a derived class, be sure to
			 * call the base class's \b OnSetUpAboutToBeShown method so that registered delegates
			 * receive the event.  */
			protected virtual void OnSetUpAboutToBeShown (SetUpAboutToBeShownEventArgs e)
			{
				if (SetUpAboutToBeShown != null)
					SetUpAboutToBeShown (this, e);
			}

			/*! \brief Raising an event invokes the event handler through a delegate.
			 *
			 * The \b OnClickSave method also allows derived classes to handle the
			 * event without attaching a delegate. This is the preferred technique for
			 * handling the event in a derived class.
			 * \note  When overriding \b OnClickSave in a derived class, be sure to
			 * call the base class's \b OnClickSave method so that registered delegates
			 * receive the event.  */
			protected virtual void OnClickSave (PrintSaveEventArgs e)
			{
				if (ClickSave != null)
					ClickSave (this, e);
			}

			/*! \brief Raising an event invokes the event handler through a delegate.
			 *
			 * The \b OnClickPrint method also allows derived classes to handle the
			 * event without attaching a delegate. This is the preferred technique for
			 * handling the event in a derived class.
			 * \note  When overriding \b OnClickPrint in a derived class, be sure to
			 * call the base class's \b OnClickPrint method so that registered delegates
			 * receive the event.  */
			protected virtual void OnClickPrint (PrintSaveEventArgs e)
			{
				if (ClickPrint != null)
					ClickPrint (this, e);
			}

			/*! \brief Raising an event invokes the event handler through a delegate.
			 *
			 * The \b OnClickSetUp method also allows derived classes to handle the
			 * event without attaching a delegate. This is the preferred technique for
			 * handling the event in a derived class.
			 * \note  When overriding \b OnClickSetUp in a derived class, be sure to
			 * call the base class's \b OnClickSetUp method so that registered delegates
			 * receive the event.  */
			protected virtual void OnClickSetUp (ToolboxEventArgs e)
			{
				if (ClickSetUp != null)
					ClickSetUp (this, e);
			}

			/*! \brief Check if the given event is relevant to the Print Dialogue and call the
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
				case EventCode.SetUpAboutToBeShown:
					OnSetUpAboutToBeShown (new SetUpAboutToBeShownEventArgs (e.RawEventData));
					break;
				case EventCode.Save:
					OnClickSave (new PrintSaveEventArgs (e.RawEventData));
					break;
				case EventCode.Print:
					OnClickPrint (new PrintSaveEventArgs (e.RawEventData));
					break;
				case EventCode.SetUp:
					OnClickSetUp (e);
					break;
				}
			}

			/*! \brief The event handlers that will be called just before this Print Dialogue is shown.
			 *
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, AboutToBeShownEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * PrintDialogueObject.AboutToBeShown += handler_name;
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
			 * PrintDialogueObject.DialogueCompleted += handler_name;
			 * \endcode  */
			public event ToolboxEventHandler DialogueCompleted;

			/*! \brief The event handlers that will be called just before the Print module is going
			 * to show its underlying Window object.
			 * 
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, PrintDialogue.SetUpAboutToBeShownEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * PrintDialogueObject.SetUpAboutToBeShown += handler_name;
			 * \endcode  */
			public event SetUpAboutToBeShownEventHandler SetUpAboutToBeShown;

			/*! \brief The event handlers that will be called when the user clicks on the \b Save
			 * button.
			 * 
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, PrintDialogue.SaveClickEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * PrintDialogueObject.ClickSave += handler_name;
			 * \endcode  */
			public event PrintSaveEventHandler ClickSave;

			/*! \brief The event handlers that will be called when the user clicks on the \b Set \b Up
			 * button.
			 * 
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, ToolboxEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * PrintDialogueObject.ClickSetUp += handler_name;
			 * \endcode  */
			public event ToolboxEventHandler ClickSetUp;

			/*! \brief The event handlers that will be called when the user clicks on the \b Print
			 * button or presses Return.
			 * 
			 * Handlers should have the signature:
			 * \code
			 * void handler_name (object sender, PrintDialogue.PrintClickEventArgs e);
			 * \endcode
			 * and can be added to the list with:
			 * \code
			 * PrintDialogueObject.ClickPrint += handler_name;
			 * \endcode  */
			public event PrintSaveEventHandler ClickPrint;

			public delegate void PrintSaveEventHandler (object sender, PrintSaveEventArgs e);
			public delegate void SetUpAboutToBeShownEventHandler (object sender, SetUpAboutToBeShownEventArgs e);

			/*! \brief An object that encapsulates the arguments for the event that is raised
			 * just before the SetUp object is shown on screen.  */
			public class SetUpAboutToBeShownEventArgs : ToolboxEventArgs
			{
				/*! \brief Constants defining event specific data offsets after the header.  */
				public static class EventOffset
				{
					public const int ObjectID = 16;
					public const int ShowType = 20;
					public const int Buffer = 24;
				}

				/*! \brief The Toolbox id of the object that is about to be shown.
				 * \note The \e self \e id in the id block will be for the Print Dialogue object, not the
				 * object which will be shown.  */
				uint ObjectID;

				/*! \brief Gives details of where the object will be displayed.  */
				public readonly ShowObjectSpec ShowSpec;

				public SetUpAboutToBeShownEventArgs (IntPtr unmanagedEventData) : base (unmanagedEventData)
				{
					ShowObjectType show_type = (ShowObjectType)Marshal.ReadInt32 (RawEventData,
												      EventOffset.ShowType);
					ObjectID = (uint)Marshal.ReadInt32 (RawEventData,
									    EventOffset.ObjectID);
					switch (show_type)
					{
					case Toolbox.ShowObjectType.FullSpec:
						{
							var ev = (NativeToolbox.ShowObjectFullSpecEvent)
									Marshal.PtrToStructure (RawEventData,
												typeof (NativeToolbox.ShowObjectFullSpecEvent));
							ShowSpec = new ShowObjectFull (new OS.Rect (ev.Spec.Visible),
										       new OS.Coord (ev.Spec.Scroll),
										       ev.Spec.BehindWindow,
										       0, 0, 0);
							break;
						}
					case Toolbox.ShowObjectType.TopLeft:
						{
							var ev = (NativeToolbox.ShowObjectTopLeftEvent)
									Marshal.PtrToStructure (RawEventData,
												typeof (NativeToolbox.ShowObjectTopLeftEvent));
							ShowSpec = new ShowObjectTopLeft (new OS.Coord (ev.Spec.TopLeft));
							break;
						}
					case Toolbox.ShowObjectType.Default:
					case Toolbox.ShowObjectType.Centre:
					case Toolbox.ShowObjectType.AtPointer:
						ShowSpec = new ShowObjectSpec (show_type);
						break;
					default:
						ShowSpec = null;
						break;
					}
				}

				public T Object<T> () where T : Toolbox.Object
				{
					return Object.CreateInstance<T> (ObjectID);
				}
			}

			/*! \brief This event is raised when the user clicks on the \b Save button.
			 *
			 * The client should save any options associated with this Print Dialogue (usually
			 * in a document which is being edited.  */
			public class PrintSaveEventArgs : ToolboxEventArgs
			{
				/*! \brief Constants defining event specific data offsets after the header.  */
				public static class EventOffset
				{
					public const int PageRangeStart = 16;
					public const int PageRangeEnd = 20;
					public const int NumberOfCopies = 24;
					public const int Scale = 28;
				}

				/*! \brief Constants defining the possible bit flags returned by this event.  */
				public static class Flags
				{
					public const int Sideways = (1 << 0);
					public const int Draft = (1 << 1);
				}

				/*! \brief The first page to start printing from (-1 means 'All').  */
				public readonly int PageRangeStart;
				/*! \brief The last page in the range to print.  */
				public readonly int PageRangeEnd;
				/*! \brief The number of copies to print.  */
				public readonly int NumberOfCopies;
				/*! \brief The value to scale by (as a percentage).  */
				public readonly int Scale;
				/*! \brief \e true means print as Draft (default is non-draft).  */
				public readonly bool Draft;
				/*! \brief \e true means print Sideways (default is Upright).  */
				public readonly bool Orientation;

				public PrintSaveEventArgs (IntPtr unmanagedEventData) : base (unmanagedEventData)
				{
					PageRangeStart = Marshal.ReadInt32 (RawEventData, EventOffset.PageRangeStart);
					PageRangeEnd = Marshal.ReadInt32 (RawEventData, EventOffset.PageRangeEnd);
					NumberOfCopies = Marshal.ReadInt32 (RawEventData, EventOffset.NumberOfCopies);
					Scale = Marshal.ReadInt32 (RawEventData, EventOffset.Scale);
					Draft = (Header.Flags & Flags.Draft) != 0;
					Orientation = (Header.Flags & Flags.Sideways) != 0;
				}
			}
		}
	}
}