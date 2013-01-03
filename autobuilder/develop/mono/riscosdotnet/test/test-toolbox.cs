/*
 * Simple application to test the RISC OS C# bindings and
 * demonstrate what a RISC OS toolbox application might
 * look like when written in C#.
 *
 * RISCOS.Net (the C# binding) is still evolving, so
 * this test app probably will too.
 *
 * A lot of the things done here to manipulate toolbox objects
 * can be done in !ResEd and may not generally be needed in a
 * real application.
 *
 * Author:
 *	Lee Noar <leenoar@sky.com>
 */

using riscos;
using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;

namespace riscos.CSharpBindings.ToolboxTest
{
	public class MyTask : ToolboxTask
	{
		public MainMenu main_menu;

		const string Version = "V1.0 (2nd January 2013)";

		// Could use an enum here, but enums require a cast which is ugly.
		public static class MyEvent
		{
			public const uint IconbarSelect = 0x100;
			public const uint IconbarAdjust = 0x101;
			public const uint Quit = 0x9999;
		}

		public void Init ()
		{
			int[] mess_list = { 0 };
			int[] event_list = { 0 };

			// The message and event lists are copied by the toolbox, so we can
			// discard them after initialisation.
			Initialise (350, mess_list, event_list, "<MonoTestTB$Dir>");

			main_menu = new MainMenu (this);

			InitIconBar();
		}

		public void Run ()
		{
			while (Quit == false)
			{
				try {
					PollIdle (OS.ReadMonotonicTime() + 100);
				}
				catch (OS.ErrorException ex)
				{
					Wimp.ReportError (Wimp.ErrorBoxFlags.OKIcon,
							  "MonoTestTB",
							  ex.OSError.Number,
							  ex.OSError.Message);
				}
				catch (Exception ex)
				{
					Reporter.WriteLine ("{0}", ex.Message);
					string[] lines = ex.StackTrace.Split (new char[]{'\n'});
					foreach (string s in lines)
						Reporter.WriteLine ("{0}", s);
				}
			}
		}

		public static void Main(string[] args)
		{
			MyTask task = new MyTask();

			// Most methods that call a SWI throw an OS.ErrorException if the
			// SWI returns an error.
			try
			{
				task.Init ();
				task.Run ();
			}
			catch (OS.ErrorException ex)
			{
				Reporter.WriteLine ("error number = {0:X8}, error string = {1}",
						    ex.OSError.Number,
						    ex.OSError.Message);
				Reporter.WriteLine ("StackTrace: {0}", Environment.StackTrace);
			}
			finally
			{
			}
		}


		void InitIconBar ()
		{
			var iconbar = Toolbox.Object.CreateInstance<Toolbox.Iconbar> ("AppIcon");

			// Set the text to be displayed under the Iconbar sprite.
			iconbar.Text = "MonoTestTB";
			iconbar.HelpMessage = "Click SELECT to open the main window|MClick ADJUST to open the test dialogue";
			// Set the Iconbar object to return our own event codes when clicked with SELECT/ADJUST.
			iconbar.SelectClickEvent = MyEvent.IconbarSelect;
			iconbar.AdjustClickEvent = MyEvent.IconbarAdjust;
			// Add our own event handlers to be called for the events we set above.
			iconbar.SelectClick += iconbar_Select;
			// Link the main menu to the Iconbar icon.
			iconbar.Menu = main_menu;

			// Display our application icon on the Iconbar.
			iconbar.Show ();

			// Make sure that reading the Iconbar properties works.
			Reporter.WriteLine ("Iconbar");
			Reporter.WriteLine ("-------");
			Reporter.WriteLine ("Icon = {0}", iconbar.IconHandle);
			Reporter.WriteLine ("Text = '{0}', Sprite = '{1}'", iconbar.Text, iconbar.Sprite);
			Reporter.WriteLine ("SelectEvent = {0:X8}, AdjustEvent = {1:X8}", iconbar.SelectClickEvent, iconbar.AdjustClickEvent);
			Reporter.WriteLine ("HelpMessage = '{0}'", iconbar.HelpMessage);
			Reporter.WriteLine ("");
		}

		// For a derived class, it is recommended to override the event notifier rather
		// than subscribe to the event.
		protected override void OnMsgDataLoad (Wimp.DataLoadMessageEventArgs e)
		{
			Reporter.WriteLine ("Received DataLoad Wimp message for the Iconbar icon:");
			Reporter.WriteLine ("  Window handle: {0:X8}", e.DataLoad.WindowHandle);
			Reporter.WriteLine ("  Icon handle: {0}", (int)e.DataLoad.IconHandle);
			Reporter.WriteLine ("  Position: {0},{1}", e.DataLoad.Position.X, e.DataLoad.Position.Y);
			Reporter.WriteLine ("  Estimated file size: {0}", e.DataLoad.EstimatedSize);
			Reporter.WriteLine ("  File type: {0:X3}", e.DataLoad.FileType);
			Reporter.WriteLine ("  Filename: {0}",e.DataLoad.FileName);
			Reporter.WriteLine ("");

			// Make sure that other subscribers receive the event (not that there are any).
			base.OnMsgDataLoad (e);
		}

		protected override void OnObjectAutoCreated (Toolbox.ObjectAutoCreatedEventArgs e)
		{
			Reporter.WriteLine ("Object {0:X8} was auto-created from template '{1}'",
					    e.ObjectID, e.TemplateName);

			if (e.TemplateName.Equals ("ProgInfo"))
			{
				var ProgInfo = new Toolbox.ProgInfoDialogue (e.ObjectID);
				ProgInfo.Version = Version;
			}

			base.OnObjectAutoCreated (e);
		}

		void quitdbox_Quit (object sender, Toolbox.ToolboxEventArgs args)
		{
			Reporter.WriteLine ("Quit handler called - Exiting...");
			Reporter.WriteLine ("");
			Quit = true;

			Toolbox.QuitDialogue quit_dialogue = (Toolbox.QuitDialogue)sender;
			quit_dialogue.Dispose();
		}

		void quitdbox_Cancel (object sender, Toolbox.ToolboxEventArgs args)
		{
			Reporter.WriteLine ("Quit cancelled.");
			Reporter.WriteLine ("");

			Toolbox.QuitDialogue quit_dialogue = (Toolbox.QuitDialogue)sender;
			quit_dialogue.Dispose();
		}

		void quitdbox_Show (object sender, Toolbox.Object.AboutToBeShownEventArgs e)
		{
			Reporter.WriteLine ("Quit Dialogue about to be shown, show type = {0}", e.ShowSpec.Type);
			Reporter.WriteLine ("");
		}

		void QuitHandler (object sender, Toolbox.ToolboxEventArgs args)
		{
			Toolbox.QuitDialogue quit_dialogue;

			quit_dialogue = new Toolbox.QuitDialogue ("Quit");
			quit_dialogue.Message = "Quit handler called, really quit?";
			quit_dialogue.ClickQuit += quitdbox_Quit;
			quit_dialogue.ClickCancel += quitdbox_Cancel;
			quit_dialogue.AboutToBeShown += quitdbox_Show;
			quit_dialogue.ShowCentred ();
		}

		void iconbar_Select (object sender, Toolbox.ToolboxEventArgs args)
		{
			Document doc = Toolbox.Object.CreateInstance<Document> ("MainWindow");
			doc.SetText ("The quick brown fox jumped over the lazy dog.");
			doc.Show();
		}

		public void SubMenuHandler (object sender, Toolbox.MenuEntry.SubMenuEventArgs e)
		{
			Reporter.WriteLine ("SubMenuEvent received.");
			Reporter.WriteLine ("Submenu would be opened at coordinate ({0},{1}).", e.Position.X, e.Position.Y);
			Reporter.WriteLine ("");
		}

		public class MainMenu : Toolbox.Menu
		{
			// Could use an enum here, but enums require a cast which is ugly.
			public static class Cmp
			{
				public const uint Quit = 0;
				public const uint Faded = 1;
				public const uint SubMenu = 2;
				public const uint ProgInfo = 3;
				public const uint GCMemory = 4;
			}

			public MainMenu (MyTask task) : base ("MainMenu")
			{
				Title = "MonoTestTB";

				Toolbox.MenuEntry entry;

				entry = new Toolbox.MenuEntry (this, Cmp.Quit);
				entry.Text = "Quit";
				entry.Selection += task.QuitHandler;
				entry.HelpMessage = "Exits the application";

				entry = new Toolbox.MenuEntry (this, Cmp.Faded);
				entry.Text = "Faded";
				entry.Faded = true;

				entry = new Toolbox.MenuEntry (this, Cmp.SubMenu);
				entry.Text = "SubMenu";
				entry.SubMenu += task.SubMenuHandler;

				entry = new Toolbox.MenuEntry (this, Cmp.GCMemory);
				entry.Selection += DumpGCStats;
			}

			public void DumpGCStats (object sender, Toolbox.ToolboxEventArgs args)
			{
				long memory = GC.GetTotalMemory(true);

				Reporter.WriteLine ("Approximate number of bytes allocated to managed memory = {0}", memory);
				Reporter.WriteLine ("");
			}

			protected override void OnAboutToBeShown (Toolbox.Object.MenuAboutToBeShownEventArgs e)
			{
				Reporter.WriteLine ("Menu about to be shown, show type = {0}", e.ShowSpec.Type);
				Reporter.WriteLine ("");
			}
		}

		// A sample data type that might be used to represent a file in an editor.
		public class Document : Toolbox.Window
		{
			// We derive a new object from the SaveAs dialogue so that we can add some extra
			// data that is to be associated with it.
			public class DocumentSaveAs : Toolbox.SaveAsDialogue
			{
				// The size of the buffer we're going to use for RAM transfer. We could
				// do the transfer in one go, but that wouldn't be much of a test, so we
				// divide it up into chunks of 10 bytes.
				public const int RamTransBufferSize = 10;

				// The memory used to buffer the RAM transfer. We need some memory with
				// a fixed address, i.e., something that the GC wont move. There are number
				// of ways to do this, e.g., Marshal.AllocHGlobal, but that is unmanaged memory.
				// Here we use managed memory, but tell the GC to keep its hands off.
				public byte [] RamTransBuffer;
				public GCHandle RamTransBufferHandle;

				// Keep track of the position in the data to transfer.
				public int RamTransBufferIndex;

				public DocumentSaveAs (uint objectID) : base (objectID)
				{
				}

				// As a derived object, it's recommended to override the event notifier rather
				// than subscribe to the event.
				protected override void OnAboutToBeShown (Toolbox.SaveAsDialogue.AboutToBeShownEventArgs e)
				{
					// Allocate some managed memory.
					RamTransBuffer = new byte [RamTransBufferSize];
					// Tell the GC not to move it. This is a burden to the GC, so as soon as
					// the SaveAs is closed, we give back control of this memory to the GC.
					RamTransBufferHandle = GCHandle.Alloc(RamTransBuffer, GCHandleType.Pinned);
					RamTransBufferIndex = 0;
					// Tell the SaveAs dialogue box how big the file is.
					FileSize = 45;

					// Technically, we should call the base method to ensure that event subscribers
					// are also notified of this event, but I don't think that's necessary in this
					// case.

					// base.OnAboutToBeShown (e);
				}

				// As a derived object, it's recommended to override the event notifier rather
				// than subscribe to the event.
				protected override void OnDialogueCompleted (Toolbox.SaveAsDialogue.DialogueCompletedEventArgs e)
				{
					// Give back control of the memory we allocated to the GC.
					RamTransBufferHandle.Free();

					// Technically, we should call the base method to ensure that event subscribers
					// are also notified of this event, but I don't think that's necessary in this
					// case.

					// base.OnDialogueCompleted (e);
				}
			}

			// We derive a new object from the Toolbox menu so that we can add some extra
			// data that is to be associated with it.
			public class DocumentMenu : Toolbox.Menu
			{
				public static class Cmp
				{
					public const uint SaveAs = 0;
					public const uint FileInfo = 1;
					public const uint ColourDBox = 2;
					public const uint ColourMenu = 3;
					public const uint FontDBox = 4;
					public const uint FontMenu = 5;
					public const uint ScaleDBox = 6;
					public const uint PrintDBox = 7;
				}

				public Toolbox.MenuEntry ColourDBoxEntry;
				public Toolbox.MenuEntry ColourMenuEntry;

				public Toolbox.FontMenu FontMenu;
				public Toolbox.ScaleDialogue ScaleDBox;
				public Toolbox.PrintDialogue PrintDBox;
				public Toolbox.FileInfoDialogue FileInfoDBox;
				public Toolbox.FontDialogue FontDBox;
				public DocumentSaveAs SaveAsDBox;

				public DocumentMenu (uint objectID) : base (objectID)
				{
					ColourDBoxEntry = new Toolbox.MenuEntry (this, Cmp.ColourDBox);
					ColourMenuEntry = new Toolbox.MenuEntry (this, Cmp.ColourMenu);

					Toolbox.MenuEntry entry;

					entry = new Toolbox.MenuEntry (this, Cmp.FontMenu);
					FontMenu = entry.GetSubMenuShow<Toolbox.FontMenu> ();

					entry = new Toolbox.MenuEntry (this, Cmp.ScaleDBox);
					ScaleDBox = entry.GetSubMenuShow<Toolbox.ScaleDialogue> ();

					entry = new Toolbox.MenuEntry (this, Cmp.PrintDBox);
					PrintDBox = entry.ClickShow.Object<Toolbox.PrintDialogue>();

					entry = new Toolbox.MenuEntry (this, Cmp.FileInfo);
					FileInfoDBox = entry.GetSubMenuShow<Toolbox.FileInfoDialogue> ();

					entry = new Toolbox.MenuEntry (this, Cmp.FontDBox);
					FontDBox = entry.ClickShow.Object<Toolbox.FontDialogue>();

					SaveAsDBox = Toolbox.Object.CreateInstance<DocumentSaveAs> ("SaveAs");
					entry = new Toolbox.MenuEntry (this, Cmp.SaveAs);
					entry.SubMenuShow = SaveAsDBox;
				}
			}

			public DocumentMenu DocMenu;

			string FontID = "Trinity.Medium";
			int FontHeight = 24;
			int FontAspectRatio = 100;
			Font.Instance font;
			// A palette entry
			uint font_palette_colour = ColourTrans.Black;
			Toolbox.ColourMenu.WimpColour font_wimp_colour;

			enum ColourSetter
			{
				Dialogue,
				Menu
			}

			ColourSetter ColourSetBy = ColourSetter.Dialogue;

			// Scale is a percentage.
			uint Scale = 100;

			// The text to be displayed in this window.
			public string Text = "";

			public string FileName = null;

			public Document (uint objectID) : base (objectID)
			{
				// Set the window title via its Title property.
				Title = "CSharp Toolbox Window - Scale: " + Scale + "%";

				DocMenu = Toolbox.Object.CreateInstance<DocumentMenu> ("WindowMenu");
				// Attach the menu to the window.
				Menu = DocMenu;

				font = new Font.Instance (FontID,
							  ((FontHeight * FontAspectRatio) / 100) << 4,
							  FontHeight << 4);

				DocMenu.SaveAsDBox.FileType = 0xfff;
				DocMenu.SaveAsDBox.SelectionAvailable = false;
				DocMenu.SaveAsDBox.SaveToFile += save_data;
				DocMenu.SaveAsDBox.FillBuffer += fill_buffer;

				DocMenu.FileInfoDBox.AboutToBeShown += fileinfo_Show;

				// The AboutToBeShown events for ColourMenu and ColourDialogue don't seem to
				// be reliable, so use the SubMenuShow events instead. This is a Toolbox issue,
				// not a Mono/C# issue.
				DocMenu.ColourDBoxEntry.SubMenu += colourdbox_SubMenuShow;
				DocMenu.ColourMenuEntry.SubMenu += colourmenu_SubMenuShow;

				Toolbox.ColourDialogue colour_dbox =
					DocMenu.ColourDBoxEntry.GetSubMenuShow<Toolbox.ColourDialogue> ();
				colour_dbox.ColourSelected += colourdbox_Selected;

				Toolbox.ColourMenu colour_menu =
					DocMenu.ColourMenuEntry.GetSubMenuShow<Toolbox.ColourMenu> ();
				colour_menu.Selection += colourmenu_Selected;

				DocMenu.FontDBox.AboutToBeShown += fontdbox_Show;
				DocMenu.FontDBox.ApplyFont += fontdbox_ApplyFont;

				DocMenu.FontMenu.AboutToBeShown += fontmenu_Show;
				DocMenu.FontMenu.FontSelection += fontmenu_Selection;

				DocMenu.ScaleDBox.AboutToBeShown += scaledbox_Shown;
				DocMenu.ScaleDBox.ApplyFactor += scaledbox_ApplyFactor;

				DocMenu.PrintDBox.ClickSave += printdbox_SavePrint;
				DocMenu.PrintDBox.ClickPrint += printdbox_SavePrint;
			}

			public void SetText (string text)
			{
				Text = text;
				if (IsOnScreen ())
					ForceRedraw (Extent);
			}

			// For a derived class, it is recommended to override the event notifier rather
			// than subscribe to the event.
			protected override void OnMsgDataLoad (Wimp.DataLoadMessageEventArgs e)
			{
				Reporter.WriteLine ("Received DataLoad Wimp message:");
				Reporter.WriteLine ("  Window handle: {0:X8}", e.DataLoad.WindowHandle);
				Reporter.WriteLine ("  Icon handle: {0}", (int)e.DataLoad.IconHandle);
				Reporter.WriteLine ("  Position: {0},{1}", e.DataLoad.Position.X, e.DataLoad.Position.Y);
				Reporter.WriteLine ("  Estimated file size: {0}", e.DataLoad.EstimatedSize);
				Reporter.WriteLine ("  File type: {0:X3}", e.DataLoad.FileType);
				Reporter.WriteLine ("  Filename: {0}",e.DataLoad.FileName);
				Reporter.WriteLine ("");

				// Make sure that other subscribers receive the event (not that there are any).
				base.OnMsgDataLoad (e);
			}

			void printdbox_SavePrint (object sender, Toolbox.PrintDialogue.PrintSaveEventArgs e)
			{
				var print_dbox = (Toolbox.PrintDialogue)sender;

				// Print dialogue is a shared toolbox object, so check that this is the
				// intended target of the event.
				if (!this.Equals (print_dbox.GetAncestor<Document>()))
					return;

				Reporter.WriteLine ("Print Dialogue Save/Print button clicked:");
				if (e.PageRangeStart == -1)
					Reporter.WriteLine ("  Print all pages");
				else
					Reporter.WriteLine ("  Print pages from {0} to {1}", e.PageRangeStart, e.PageRangeEnd);
				Reporter.WriteLine ("  Print {0} copies to a scale of {1}%", e.NumberOfCopies, e.Scale);
				Reporter.WriteLine ("  Draft printing = {0}",e.Draft);
				Reporter.WriteLine ("  Print sideways = {0}", e.Orientation);
				Reporter.WriteLine ("");
			}

			void scaledbox_Shown (object sender, Toolbox.Object.AboutToBeShownEventArgs args)
			{
				var scale_dbox = (Toolbox.ScaleDialogue)sender;

				// Scale dialogue is a shared toolbox object, so check that this is the
				// intended target of the event.
				if (this.Equals (scale_dbox.GetAncestor<Document>()))
					scale_dbox.Value = (int)Scale;
			}

			void scaledbox_ApplyFactor (object sender, Toolbox.ScaleDialogue.ApplyFactorEventArgs args)
			{
				var scale_dbox = (Toolbox.ScaleDialogue)sender;

				// Scale dialogue is a shared toolbox object, so check that this is the
				// intended target of the event.
				if (!this.Equals (scale_dbox.GetAncestor<Document>()))
					return;

				Scale = args.Factor;

				font.Lose ();

				int scaled_height = (FontHeight * (int)Scale) / 100;

				font.Find (FontID,
					  ((scaled_height * FontAspectRatio) / 100) << 4,
					  scaled_height << 4, 0, 0);
				ForceRedraw (Extent);

				// Set the window title via its Title property.
				Title = "CSharp Toolbox Window - Scale: " + Scale + "%";
			}

			void fontmenu_Show (object sender, Toolbox.Object.MenuAboutToBeShownEventArgs args)
			{
				var font_menu = (Toolbox.FontMenu)sender;

				// Font menu is a shared toolbox object, so check that this is the
				// intended target of the event.
				if (this.Equals (font_menu.GetAncestor<Document>()))
					font_menu.Font = FontID;
			}

			void fontmenu_Selection (object sender, Toolbox.FontMenu.FontSelectionEventArgs args)
			{
				var font_menu = (Toolbox.FontMenu)sender;

				// Font menu is a shared toolbox object, so check that this is the
				// intended target of the event.
				if (!this.Equals (font_menu.GetAncestor<Document>()))
					return;

				font.Lose ();

				// The FontMenu returns a full font ID, i.e., one where the name begins with "\F".
				// The FontDialogue doesn't recognise names like this, so strip the "\F" off.
				FontID = args.FontID.StartsWith ("\\F") ?
					args.FontID.Remove (0, 2) :
					args.FontID;

				font.Find (FontID,
					  ((FontHeight * FontAspectRatio) / 100) << 4,
					  FontHeight << 4, 0, 0);
				ForceRedraw (Extent);
			}

			void fontdbox_Show (object sender, Toolbox.Object.AboutToBeShownEventArgs args)
			{
				var font_dbox = (Toolbox.FontDialogue)sender;

				// Font dialogue is a shared toolbox object, so check that this is the
				// intended target of the event.
				if (this.Equals (font_dbox.GetAncestor<Document>()))
				{
					font_dbox.Font = FontID;
					font_dbox.Height = FontHeight;
					font_dbox.AspectRatio = FontAspectRatio;
				}
			}

			void fontdbox_ApplyFont (object sender, Toolbox.FontDialogue.ApplyFontEventArgs args)
			{
				var font_dbox = (Toolbox.FontDialogue)sender;

				// Font dialogue is a shared toolbox object, so check that this is the
				// intended target of the event.
				if (!this.Equals (font_dbox.GetAncestor<Document>()))
					return;

				font.Lose ();

				FontID = args.FontID;
				FontHeight = args.FontHeight;
				FontAspectRatio = args.AspectRatio;

				font.Find (FontID,
					  ((FontHeight * FontAspectRatio) / 100) << 4,
					  FontHeight << 4, 0, 0);
				ForceRedraw (Extent);
			}

			void colourmenu_SubMenuShow (object sender, Toolbox.MenuEntry.SubMenuEventArgs args)
			{
				var menu_entry = (Toolbox.MenuEntry)sender;

				Toolbox.ColourMenu colour_menu = menu_entry.GetSubMenuShow<Toolbox.ColourMenu> ();

				// The ColourMenu is a shared toolbox object, so check that this is the
				// intended target of the event.
				if (!this.Equals (colour_menu.GetAncestor<Document>()))
					return;

				// If the ColourDialogue was the last to set the colour then we leave the
				// menu unselected.
				colour_menu.Colour = (ColourSetBy != ColourSetter.Menu) ?
							Toolbox.ColourMenu.WimpColour.Unselected :
							font_wimp_colour;
			}

			void colourmenu_Selected (object sender, Toolbox.ColourMenu.SelectionEventArgs args)
			{
				var colour_menu = (Toolbox.ColourMenu)sender;

				if (this.Equals (colour_menu.GetAncestor<Document>()))
				{
					font_wimp_colour = args.Colour;
					ColourSetBy = ColourSetter.Menu;
					ForceRedraw (Extent);
				}
			}

			void colourdbox_SubMenuShow (object sender, Toolbox.MenuEntry.SubMenuEventArgs args)
			{
				var menu_entry = (Toolbox.MenuEntry)sender;

				Toolbox.ColourDialogue colour_dbox = menu_entry.GetSubMenuShow<Toolbox.ColourDialogue> ();

				// The menu is a shared toolbox object, so check that this is the
				// intended target of the event.
				if (!this.Equals (colour_dbox.GetAncestor<Document>()))
					return;

				// Create the necessary colour block from the font colour
				int [] colour_block = Toolbox.ColourDialogue.AllocStandardColourBlock (font_palette_colour, Toolbox.ColourDialogue.ColourModel.RGB);
				// If the ColourMenu was the last to set the colour, then we set the current
				// colour to None.
				colour_dbox.SetColour (colour_block, ColourSetBy != ColourSetter.Dialogue);
			}

			void colourdbox_Selected (object sender, Toolbox.ColourDialogue.ColourSelectedEventArgs args)
			{
				var colour_dbox = (Toolbox.ColourDialogue)sender;

				// The colour dialogue is a shared toolbox object, so check that this is the
				// intended target of the event.
				if (this.Equals (colour_dbox.GetAncestor<Document>()))
				{
					// We use the colour regardless of whether 'None' was selected.
					font_palette_colour = Toolbox.ColourDialogue.
									ColourFromColourBlock (args.ColourBlock);
					ColourSetBy = ColourSetter.Dialogue;
					ForceRedraw (Extent);
				}
			}

			void fileinfo_Show (object sender, Toolbox.Object.AboutToBeShownEventArgs args)
			{
				var file_info_dbox = (Toolbox.FileInfoDialogue)sender;

				// The FileInfo dialogue is a shared toolbox object, so check that this is the
				// intended target of the event.
				if (this.Equals (file_info_dbox.GetAncestor<Document>()))
				{
					file_info_dbox.Date = OS.DateTime.Now;
					file_info_dbox.FileSize = Text.Length;
					file_info_dbox.Modified = false;
					file_info_dbox.FileType = 0xfff;
					file_info_dbox.FileName = (FileName != null) ? FileName : "<Untitled>";
				}
			}

			// For a derived class, it is recommended to override the event notifier rather
			// than subscribe to the event.
			protected override void OnPaint (Wimp.RedrawEventArgs e)
			{
				if (ColourSetBy == ColourSetter.Dialogue)
					ColourTrans.SetFontColours (ColourTrans.White, font_palette_colour, 7);
				else if (ColourSetBy == ColourSetter.Menu)
					Wimp.SetFontColours ((int)OS.DesktopColour.White, (int)font_wimp_colour);
				font.Paint (Text,
					    Font.PlotType.OSUnits,
					    e.Origin.X + 10,
					    e.Origin.Y - 100,
					    0); // Length ignored (paint whole string) if bit 7 of flags not set

				// Technically, we should call the base method to ensure that event subscribers
				// are also notified of this event, but I don't think that's necessary in this
				// case.

				// base.OnPaint (e);
			}

			// Type 2 client
			// Client participates in disc save.
			void save_data (object sender, Toolbox.SaveAsDialogue.SaveToFileEventArgs e)
			{
				try
				{
					var saveas = (DocumentSaveAs)sender;

					// The SaveAs dialogue is a shared toolbox object, so check that this is the
					// intended target of the event.
					if (this.Equals (saveas.GetAncestor<Document>()))
					{
						FileName = e.Filename;
						System.IO.File.WriteAllText(e.Filename, Text);
						saveas.FileSaveCompleted (e.Filename, true);
					}
				}
				catch (Exception ex)
				{
					Wimp.ReportError (Wimp.ErrorBoxFlags.OKIcon,
							  "MonoTestTB",
							  0,
							  ex.Message);
				}
			}

			// Type 3 client
			// Client participates in RAM transfer.
			// 
			// FIXME: This doesn't quite work yet - only the first buffer fill is being sent.
			// I suspect this is due to me misunderstanding the Toolbox SaveAs object, but I
			// guess it could be a C# issue.
			void fill_buffer (object sender, Toolbox.SaveAsDialogue.FillBufferEventArgs e)
			{
				var saveas = (DocumentSaveAs)sender;

				// The SaveAs dialogue is a shared toolbox object, so check that this is the
				// intended target of the event.
				if (!this.Equals (saveas.GetAncestor<Document>()))
					return;

				if (saveas.RamTransBufferIndex < Text.Length)
				{
					ASCIIEncoding ascii = new ASCIIEncoding();
					int amount_to_transfer = Math.Min (DocumentSaveAs.RamTransBufferSize,
									  Text.Length - saveas.RamTransBufferIndex);
					ascii.GetBytes (Text, saveas.RamTransBufferIndex, amount_to_transfer, saveas.RamTransBuffer, 0);
					saveas.RamTransBufferIndex += amount_to_transfer;
					IntPtr address = saveas.RamTransBufferHandle.AddrOfPinnedObject ();
					saveas.BufferFilled (address, amount_to_transfer);
				}
			}
		}
	}
}
