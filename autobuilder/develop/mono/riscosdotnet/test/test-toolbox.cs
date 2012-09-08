/*
 * Simple application to test the RISC OS C# bindings and
 * demonstrate what a RISC OS toolbox application might
 * look like when written in C#.
 *
 * RISC OS Dot Net (the C# binding) is still evolving, so
 * this test app probably will too.
 *
 * A lot of the things done here to manipulate toolbox objects
 * can be done in !ResEd and may not generally be needed in a
 * real application.
 *
 * Author:
 *	Lee Noar <leenoar@sky.com>
 */

using System;
using riscos;
using System.Globalization;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;

public class MainMenu : Toolbox.Menu
{
	// Could use an enum here, but enums require a cast which is ugly.
	public class Cmp
	{
		public const uint Quit = 0;
		public const uint Faded = 1;
		public const uint SubMenu = 2;
	}

	// These are the entries that we wish to manipulate.
	public Toolbox.MenuEntry QuitEntry;
	public Toolbox.MenuEntry FadedEntry;
	public Toolbox.MenuEntry SubMenuEntry;

	public MainMenu (MyTask task) : base ("MainMenu")
	{
		Title = "MonoTestTB";
		QuitEntry = new Toolbox.MenuEntry (this, Cmp.Quit);
		QuitEntry.Text = "Quit";
		QuitEntry.Selection += task.QuitHandler;
		QuitEntry.HelpMessage = "Exits the application";

		FadedEntry = new Toolbox.MenuEntry (this, Cmp.Faded);
		FadedEntry.Text = "Faded";
		FadedEntry.Faded = true;

		SubMenuEntry = new Toolbox.MenuEntry (this, Cmp.SubMenu);
		SubMenuEntry.Text = "SubMenu";
		SubMenuEntry.SubMenu += task.SubMenuHandler;
	}
}

public class Dialogue : Toolbox.Window
{
	// These are the C# objects that we use to "wrap" a Toolbox gadget
	// and use to access/manipulate that gadget.
	public Toolbox.ActionButton OKButton;
	public Toolbox.ActionButton CancelButton;
	public Toolbox.DisplayField DisplayField;
	public Toolbox.WritableField WritableField;
	public Toolbox.NumberRange NumberRange;
	public Toolbox.StringSet StringSet;
	public Toolbox.OptionButton OptionButton;
	public Toolbox.RadioButton Radio1;
	public Toolbox.Slider Slider;
	public Toolbox.Draggable Draggable;
	public Toolbox.Button Button;
	public Toolbox.RadioButton Radio2;
	public Toolbox.PopupMenu PopupMenu;
	public Toolbox.AdjusterArrow AdjusterArrow;

	// The Toolbox component IDs of the gadgets that we are interested in.
	public class CmpID
	{
		public const uint CancelButton = 0;
		public const uint OKButton = 1;
		public const uint DisplayField = 3;
		public const uint WritableField = 4;
		public const uint NumberRange = 5;
		public const uint StringSet = 6;
		public const uint OptionButton = 7;
		public const uint Radio1 = 8;
		public const uint Slider = 9;
		public const uint Draggable = 10;
		public const uint Button = 11;
		public const uint Radio2 = 12;
		public const uint PopupMenu = 13;
		public const uint AdjusterArrow = 14;
	}

	public Dialogue (MyTask task) : base ("Dialogue")
	{
		Title = "C# Dialogue Box";
		AboutToBeShown += OnAboutToBeShown;

		OKButton = new Toolbox.ActionButton (this, CmpID.OKButton);
		OKButton.Text = "OK";
		OKButton.Faded = true;

		CancelButton = new Toolbox.ActionButton (this, CmpID.CancelButton);
		CancelButton.Text = "Cancel";
		CancelButton.HelpMessage = "Help text set in CSharp";
		CancelButton.Selected += OnCancel;

		DisplayField = new Toolbox.DisplayField (this, CmpID.DisplayField);
		DisplayField.Value = String.Format("{0:D}", DateTime.Now);
		DisplayField.SetFont ("Trinity.Bold.Italic", 12 << 4, 12 << 4);

		WritableField = new Toolbox.WritableField (this, CmpID.WritableField);
		WritableField.Value = "CSharp";
		WritableField.Allowable = "A-Za-z0-9 ";
		WritableField.SetFont ("Trinity.Medium.Italic", 12 << 4, 12 << 4);
		WritableField.ValueChange += OnWritableFieldValueChange;

		NumberRange = new Toolbox.NumberRange (this, CmpID.NumberRange);
		NumberRange.Value = 1999;
		NumberRange.ValueChange += OnNumberRangeValueChange;

		StringSet = new Toolbox.StringSet (this, CmpID.StringSet);
		StringSet.Available = "One,Two,Three,Four,Five";
		StringSet.SelectedString = "Three";
		StringSet.ValueChange += OnStringSetValueChange;
		StringSet.AboutToBeShown += OnStringSetAboutToBeShown;

		OptionButton = new Toolbox.OptionButton (this, CmpID.OptionButton);
		OptionButton.Label = "Sugar";
		OptionButton.CurrentState = Toolbox.OptionButton.State.On;
		OptionButton.StateChange += OnOptionButtonStateChange;

		Radio1 = new Toolbox.RadioButton (this, CmpID.Radio1);
		Radio1.Label = "Coffee";
		Radio1.StateChange += OnRadioButtonStateChange;

		Radio2 = new Toolbox.RadioButton (this, CmpID.Radio2);
		Radio2.Label = "Tea";
		Radio2.CurrentState = Toolbox.RadioButton.State.On;
		Radio2.StateChange += OnRadioButtonStateChange;

		Slider = new Toolbox.Slider (this, CmpID.Slider);
		Slider.UpperBound = 1000;
		Slider.LowerBound = 100;
		Slider.StepSize = 20;
		Slider.Value = 200;
		Slider.SetColour (OS.DesktopColour.Red, OS.DesktopColour.Cream);
		Slider.ValueChange += OnSliderValueChange;

		Draggable = new Toolbox.Draggable (this, CmpID.Draggable);
		Draggable.Text = "Sprites";
		Draggable.Sprite = "file_ff9";
		Draggable.DragStart += OnDraggableDragStart;
		Draggable.DragEnd += OnDraggableDragEnd;

		Button = new Toolbox.Button (this, CmpID.Button);

		PopupMenu = new Toolbox.PopupMenu (this, CmpID.PopupMenu);
		PopupMenu.Menu = task.main_menu;
		PopupMenu.AboutToBeShown += OnPopupMenuAboutToBeShown;

		AdjusterArrow = new Toolbox.AdjusterArrow (this, CmpID.AdjusterArrow);
		AdjusterArrow.Click += OnAdjusterArrowClick;
	}

	void OnAboutToBeShown (object sender, Toolbox.Window.AboutToBeShownEventArgs e)
	{
		Reporter.WriteLine ("Dialogue about to be shown, show type = {0}", e.ShowSpec.Type);
		Reporter.WriteLine ("");
	}

	void OnCancel (object sender, Toolbox.ActionButton.SelectedEventArgs e)
	{
		Reporter.WriteLine ("Cancel button was selected.");
		Reporter.WriteLine ("Select clicked = {0}", e.SelectClicked);
		Reporter.WriteLine ("Adjust clicked = {0}", e.AdjustClicked);
		Reporter.WriteLine ("Default button = {0}", e.Default);
		Reporter.WriteLine ("Cancel button = {0}", e.Cancel);
		Reporter.WriteLine ("Local button = {0}", e.Local);
		Reporter.WriteLine ("");
	}

	void OnNumberRangeValueChange (object sender, Toolbox.NumberRange.ValueChangeEventArgs e)
	{
		Reporter.WriteLine ("NumberRange returned ValueChange event.");
		Reporter.WriteLine ("New value is {0}.", e.NewValue);
		Reporter.WriteLine ("");
	}

	void OnOptionButtonStateChange (object sender, Toolbox.OptionButton.StateChangeEventArgs e)
	{
		Reporter.WriteLine ("OptionButton returned StateChange event.");
		Reporter.WriteLine ("New state is {0}.", e.NewState);
		Reporter.WriteLine ("Mouse {0} button was used to change the state.", e.Select ? "Select" : "Adjust");
		Reporter.WriteLine ("");
	}

	void OnRadioButtonStateChange (object sender, Toolbox.RadioButton.StateChangeEventArgs e)
	{
		// We're using the same handler for two buttons, so use the sender object to determine which
		// one we've been called for.
		Toolbox.RadioButton button = (Toolbox.RadioButton)sender;

		Reporter.WriteLine ("RadioButton {0} returned StateChange event.", button.Label);
		Reporter.WriteLine ("New state is {0}.", e.NewState);
		Reporter.WriteLine ("Mouse {0} button was used to change the state.", e.Select ? "Select" : "Adjust");
		Reporter.WriteLine ("Previously selected radio button was {0}.", e.Previous.Label);
		Reporter.WriteLine ("");
	}

	void OnWritableFieldValueChange (object sender, Toolbox.WritableField.ValueChangeEventArgs e)
	{
		Reporter.WriteLine ("WritableField returned ValueChange event.");
		if (e.StringTooLong)
		{
			// This can't actually happen in this example, but we should still check.
			Reporter.WriteLine ("Could not read new value, string was too long.");
		}
		else
			Reporter.WriteLine ("New value is '{0}'.", e.NewValue);
		Reporter.WriteLine ("");
	}

	void OnStringSetValueChange (object sender, Toolbox.StringSet.ValueChangeEventArgs e)
	{
		Reporter.WriteLine ("StringSet returned ValueChange event.");
		if (e.StringTooLong)
		{
			// This can't actually happen in this example, but we should still check.
			Reporter.WriteLine ("Could not read new value, string was too long.");
		}
		else
			Reporter.WriteLine ("New value is '{0}'.", e.NewValue);
		Reporter.WriteLine ("");
	}

	void OnStringSetAboutToBeShown (object sender, Toolbox.ToolboxEventArgs e)
	{
		Reporter.WriteLine ("StringSet returned AboutToBeShown event.");
		Reporter.WriteLine ("");
	}

	void OnDraggableDragStart (object sender, Toolbox.Draggable.DragStartEventArgs e)
	{
		Reporter.WriteLine ("Draggable returned DragStart event.");
		Reporter.WriteLine ("Select drag = {0}", e.Select);
		Reporter.WriteLine ("Adjust drag = {0}", e.Adjust);
		Reporter.WriteLine ("Drag with Shift = {0}", e.Shift);
		Reporter.WriteLine ("Drag with Ctrl = {0}", e.Ctrl);
		Reporter.WriteLine ("");
	}

	void OnDraggableDragEnd (object sender, Toolbox.Draggable.DragEndEventArgs e)
	{
		Reporter.WriteLine ("Draggable returned DragEnd event.");
		if (e.ToolboxWindow != null)
		{
			Reporter.WriteLine ("Drag ended on a Toolbox Window with ID {0:X8}", e.ToolboxWindow.ID);
			if (e.ToolboxCmpID != 0xffffffff)
				Reporter.WriteLine ("and gadget with ID {0}", e.ToolboxCmpID);
		}
		else
		{
			Reporter.WriteLine ("Drag ended on a WIMP window with handle {0:X8}", e.WimpWindowHandle);
			if (e.WimpIconHandle != -1)
				Reporter.WriteLine ("and the icon with handle {0}", e.WimpIconHandle);
		}
		Reporter.WriteLine ("The mouse pointer was at position {0},{1}", e.MousePosition.X, e.MousePosition.Y);
		Reporter.WriteLine ("");
	}

	void OnSliderValueChange (object sender, Toolbox.Slider.ValueChangeEventArgs e)
	{
		Reporter.WriteLine ("Slider returned ValueChange event.");
		Reporter.WriteLine ("New value is {0}", e.NewValue);
		Reporter.WriteLine ("Drag start or click = {0}", e.DragStartClick);
		Reporter.WriteLine ("Drag in progress = {0}", e.DragInProgress);
		Reporter.WriteLine ("Drag ended = {0}", e.DragEnded);
		Reporter.WriteLine ("");
	}

	void OnPopupMenuAboutToBeShown (object sender, Toolbox.PopupMenu.AboutToBeShownEventArgs e)
	{
		Reporter.WriteLine ("PopupMenu returned AboutToBeShown event");
		Reporter.WriteLine ("Menu ID of menu that will be shown is {0:X8}", e.Menu.ID);
		Reporter.WriteLine ("");
	}

	void OnAdjusterArrowClick (object sender, Toolbox.AdjusterArrow.ClickEventArgs e)
	{
		Reporter.WriteLine ("AdjusterArrow returned Click event");
		Reporter.WriteLine ("Arrow direction is {0}",e.Direction);
		Reporter.WriteLine ("");
	}
}

public class MyTask : ToolboxTask
{
	// A sample data type that might be used to represent a file in an editor.
	public class TextFile : Toolbox.Window
	{
		// We derive a new object from the SaveAs dialogue so that we can add some extra
		// data that is to be associated with it.
		public class TextFileSaveAs : Toolbox.SaveAs
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

			public TextFileSaveAs () : base ("SaveAs")
			{
				AboutToBeShown += OnAboutToBeShown;
				DialogueComplete += OnDialogueComplete;
			}

			void OnAboutToBeShown (object o, Toolbox.SaveAs.AboutToBeShownEventArgs e)
			{
				// Allocate some managed memory.
				RamTransBuffer = new byte [RamTransBufferSize];
				// Tell the GC not to move it. This is a burden to the GC, so as soon as
				// the SaveAs is closed, we give back control of this memory to the GC.
				RamTransBufferHandle = GCHandle.Alloc(RamTransBuffer, GCHandleType.Pinned);
				RamTransBufferIndex = 0;
				// Tell the SaveAs dialogue box how big the file is.
				FileSize = 45;
			}

			void OnDialogueComplete (object o, Toolbox.SaveAs.DialogueCompleteEventArgs e)
			{
				// Give back control of the memory we allocated to the GC.
				RamTransBufferHandle.Free();
			}
		}

		// We derive a new object from the Toolbox menu so that we can add some extra
		// data that is to be associated with it.
		public class TextFileMenu : Toolbox.Menu
		{
			public static class Cmp
			{
				public const uint SaveAs = 0;
			}

			public Toolbox.MenuEntry SaveAsEntry;
			public TextFileSaveAs SaveAsDBox;

			public TextFileMenu () : base ("WindowMenu")
			{
				SaveAsEntry = new Toolbox.MenuEntry (this, Cmp.SaveAs);
				SaveAsDBox = new TextFileSaveAs ();
				SaveAsEntry.SubMenuShow = SaveAsDBox;
			}
		}

		public TextFileMenu WindowMenu;

		private Font.Instance font;

		// The text to be displayed in this window.
		public string Text;

		public TextFile (string text) : base ("MainWindow")
		{
			Text = text;
			WindowMenu = new TextFileMenu ();
			// Attach the menu to the window.
			Menu = WindowMenu;
			RedrawHandler += OnRedraw;

			font = new Font.Instance ("Trinity.Medium", 24 << 4, 24 << 4);

			WindowMenu.SaveAsDBox.FileType = 0xfff;
			WindowMenu.SaveAsDBox.SelectionAvailable = false;
			WindowMenu.SaveAsDBox.SaveToFile += save_data;
			WindowMenu.SaveAsDBox.FillBuffer += fill_buffer;
		}

		private void OnRedraw (object sender, Wimp.RedrawEventArgs args)
		{
			ColourTrans.SetFontColours (ColourTrans.White, ColourTrans.Black, 7);
			font.Paint (Text,
				    Font.PlotType.OSUnits,
				    args.Origin.X + 10,
				    args.Origin.Y - 100,
				    0); // Length ignored (paint whole string) if bit 7 of flags not set
		}

		// Type 2 client
		// Client participates in disc save.
		void save_data (object sender, Toolbox.SaveAs.SaveToFileEventArgs e)
		{
			try
			{
				TextFileSaveAs saveas = (TextFileSaveAs)sender;
				System.IO.File.WriteAllText(e.Filename, Text);
				saveas.FileSaveCompleted (e.Filename, true);
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
		void fill_buffer (object sender, Toolbox.SaveAs.FillBufferEventArgs e)
		{
			TextFileSaveAs saveas = (TextFileSaveAs)sender;
			if (saveas.RamTransBufferIndex < Text.Length)
			{
				ASCIIEncoding ascii = new ASCIIEncoding();
				int amount_to_transfer = Math.Min (TextFileSaveAs.RamTransBufferSize,
								   Text.Length - saveas.RamTransBufferIndex);
				ascii.GetBytes (Text, saveas.RamTransBufferIndex, amount_to_transfer, saveas.RamTransBuffer, 0);
				saveas.RamTransBufferIndex += amount_to_transfer;
				IntPtr address = saveas.RamTransBufferHandle.AddrOfPinnedObject ();
				saveas.BufferFilled (address, amount_to_transfer);
			}
		}
	}

	private Toolbox.Iconbar Iconbar;
	private Dialogue dialogue;

	public TextFile CurrentFile;

	public MainMenu main_menu;

	// Could use an enum here, but enums require a cast which is ugly.
	public static class MyEvent
	{
		public const uint IconbarSelect = 0x100;
		public const uint IconbarAdjust = 0x101;
		public const uint Quit = 0x9999;
	}

	void InitIconBar ()
	{
		Iconbar = new Toolbox.Iconbar ("AppIcon");
		// Set the text to be displayed under the Iconbar sprite.
		Iconbar.Text = "MonoTestTB";
		Iconbar.HelpMessage = "Click SELECT to toggle the main window open/closed|MClick ADJUST to open dialogue";
		// Set the Iconbar object to return our own event codes when clicked with SELECT/ADJUST.
		Iconbar.SelectEvent = MyEvent.IconbarSelect;
		Iconbar.AdjustEvent = MyEvent.IconbarAdjust;
		// Add our own event handlers to be called for the events we set above.
		Iconbar.SelectClicked += IconbarSelectHandler;
		Iconbar.AdjustClicked += IconbarAdjustHandler;
		// Link the main menu to the Iconbar icon.
		Iconbar.Menu = main_menu;

		// Display our application icon on the Iconbar.
		Iconbar.Show ();

		// Make sure that reading the Iconbar properties works.
		Reporter.WriteLine ("Iconbar");
		Reporter.WriteLine ("-------");
		Reporter.WriteLine ("Icon = {0}", Iconbar.IconHandle);
		Reporter.WriteLine ("Text = '{0}', Sprite = '{1}'", Iconbar.Text, Iconbar.Sprite);
		Reporter.WriteLine ("SelectEvent = {0:X8}, AdjustEvent = {1:X8}", Iconbar.SelectEvent, Iconbar.AdjustEvent);
		Reporter.WriteLine ("HelpMessage = '{0}'", Iconbar.HelpMessage);
		Reporter.WriteLine ("");
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

		CurrentFile = new TextFile ("The quick brown fox jumped over the lazy dog.");
		CurrentFile.Title = "CSharp Toolbox Window";

		dialogue = new Dialogue (this);
		dialogue.Show ();
	}

	public void Run ()
	{
		PollLoop ();
	}

	public void QuitHandler (object sender, Toolbox.ToolboxEventArgs args)
	{
		Reporter.WriteLine ("Quit handler called - Exiting...");
		Reporter.WriteLine ("");
		Quit = true;
	}

	private void IconbarSelectHandler (object sender, Toolbox.ToolboxEventArgs args)
	{
		CurrentFile.Show();
	}

	private void IconbarAdjustHandler (object sender, Toolbox.ToolboxEventArgs args)
	{
		dialogue.Show ();
	}

	public void SubMenuHandler (object sender, Toolbox.MenuEntry.SubMenuEventArgs e)
	{
		Reporter.WriteLine ("SubMenuEvent received.");
		Reporter.WriteLine ("Submenu would be opened at coordinate ({0},{1}).", e.Position.X, e.Position.Y);
		Reporter.WriteLine ("");
	}
}

public class Test
{
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
			Reporter.WriteLine ("error number = {0:X8}, error string = {1}",ex.OSError.ErrNum,ex.OSError.ErrMess);
			Reporter.WriteLine ("StackTrace: {0}", Environment.StackTrace);
		}
		finally
		{
		}
	}
}
