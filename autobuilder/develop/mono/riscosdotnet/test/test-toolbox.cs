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

public class MainMenu : Toolbox.Menu
{
	// Could use an enum here, but enums require a cast which is ugly.
	public class Cmp
	{
		public const uint Quit = 0;
	}

	public MainMenu () : base ("MainMenu")
	{
	}
}

public class Dialogue : Toolbox.Window
{
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
	}

	public Dialogue () : base ("Dialogue")
	{
		Title = "C# Dialogue Box";

		OKButton = new Toolbox.ActionButton (this, CmpID.OKButton);
		OKButton.Text = "OK";
		OKButton.Faded = true;

		CancelButton = new Toolbox.ActionButton (this, CmpID.CancelButton);
		CancelButton.Text = "Cancel";
		CancelButton.HelpMessage = "Help text set in CSharp";

		DisplayField = new Toolbox.DisplayField (this, CmpID.DisplayField);
		DisplayField.Value = String.Format("{0:D}", DateTime.Now);
		DisplayField.SetFont ("Trinity.Bold.Italic", 12 << 4, 12 << 4);

		WritableField = new Toolbox.WritableField (this, CmpID.WritableField);
		WritableField.Value = "CSharp";
		WritableField.Allowable = "A-Za-z0-9 ";
		WritableField.SetFont ("Trinity.Medium.Italic", 12 << 4, 12 << 4);

		NumberRange = new Toolbox.NumberRange (this, CmpID.NumberRange);
		NumberRange.Value = 1999;

		StringSet = new Toolbox.StringSet (this, CmpID.StringSet);
		StringSet.Available = "One,Two,Three,Four,Five";
		StringSet.SelectedString = "Three";

		OptionButton = new Toolbox.OptionButton (this, CmpID.OptionButton);
		OptionButton.Label = "Sugar";
		OptionButton.CurrentState = Toolbox.OptionButton.State.On;

		Radio1 = new Toolbox.RadioButton (this, CmpID.Radio1);
		Radio1.Label = "Coffee";

		Radio2 = new Toolbox.RadioButton (this, CmpID.Radio2);
		Radio2.Label = "Tea";
		Radio2.CurrentState = Toolbox.RadioButton.State.On;

		Slider = new Toolbox.Slider (this, CmpID.Slider);
		Slider.Value = 85;
		Slider.SetColour (OS.DesktopColour.Red, OS.DesktopColour.Cream);

		Draggable = new Toolbox.Draggable (this, CmpID.Draggable);
		Draggable.Text = "Sprites";
		Draggable.Sprite = "file_ff9";

		Button = new Toolbox.Button (this, CmpID.Button);
	}
}

public class MyTask : ToolboxTask
{
	private Toolbox.Iconbar Iconbar;
	private Toolbox.Window MainWindow;
	private Dialogue dialogue;
	private Font.Instance MainFont;

	private MainMenu main_menu;

	// Could use an enum here, but enums require a cast which is ugly.
	class MyEvent
	{
		public const uint IconbarSelect = 0x100;
		public const uint IconbarAdjust = 0x101;
		public const uint Quit = 0x9999;
	}

	void InitMainMenu ()
	{
		// Create a menu for the Iconbar icon.
		main_menu = new MainMenu ();
		main_menu.Title = "MonoTestTB";
		// Set the text of the menu component with ID number "MainMenu.Cmp.Quit".
		main_menu.SetEntryText (MainMenu.Cmp.Quit, "Quit");
		// Set the event code that will be raised by the Toolbox when the Quit entry is clicked
		main_menu.SetClickEvent (MainMenu.Cmp.Quit, MyEvent.Quit);
		// Add a handler that will act on the raised Quit event when it occurs.
		main_menu.ToolboxHandlers.Add (MyEvent.Quit, QuitHandler);
	}

	void InitIconBar ()
	{
		Iconbar = new Toolbox.Iconbar ("AppIcon");
		// Set the text to be displayed under the Iconbar sprite.
		Iconbar.Text = "MonoTestTB";
		Iconbar.HelpMessage = "Click SELECT to toggle the main window open/closed|MClick ADJUST to open dialogue";
		// Set the Iconbar object to return our own events when clicked with SELECT/ADJUST.
		Iconbar.SelectEvent = MyEvent.IconbarSelect;
		Iconbar.AdjustEvent = MyEvent.IconbarAdjust;
		// Add our own event handlers to be called for the events we set above.
		Iconbar.ToolboxHandlers.Add (MyEvent.IconbarSelect, IconbarSelectHandler);
		Iconbar.ToolboxHandlers.Add (MyEvent.IconbarAdjust, IconbarAdjustHandler);
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
	}

	public void Init ()
	{
		int[] mess_list = { 0 };
		int[] event_list = { 0 };

		// The message and event lists are copied by the toolbox, so we can
		// discard them after initialisation.
		Initialise (350, mess_list, event_list, "<MonoTestTB$Dir>");

		InitMainMenu ();
		InitIconBar();

		MainFont = new Font.Instance ("Trinity.Medium", 24 << 4, 24 << 4);
		MainWindow = new Toolbox.Window ("MainWindow");

		MainWindow.Title = "CSharp Toolbox Window";

		// Register the event handlers for the window.
		MainWindow.RedrawHandler += new Wimp.RedrawEventHandler (RedrawMainWindow);
		MainWindow.ToolboxHandlers.Add (MyEvent.Quit, QuitHandler);

		dialogue = new Dialogue ();
		dialogue.Show ();
	}

	public void Run ()
	{
		PollLoop ();
	}

	private void RedrawMainWindow (object sender, Wimp.RedrawEventArgs args)
	{
		ColourTrans.SetFontColours (ColourTrans.White, ColourTrans.Black, 7);
		MainFont.Paint ("CSharp Toolbox Task",
				Font.PlotType.OSUnits,
				args.Origin.X + 10,
				args.Origin.Y - 100,
				0); // Length ignored (paint whole string) if bit 7 of flags not set
	}

	private void QuitHandler (object sender, Toolbox.ToolboxEventArgs args)
	{
		Reporter.WriteLine ("Quit handler called - Exiting...");
		Quit = true;
	}

	private void IconbarSelectHandler (object sender, Toolbox.ToolboxEventArgs args)
	{
		// Toggle the main window open/closed when SELECT clicked on the Iconbar icon.
		if (MainWindow.IsOnScreen ())
			MainWindow.Hide ();
		else
			MainWindow.Show ();
	}

	private void IconbarAdjustHandler (object sender, Toolbox.ToolboxEventArgs args)
	{
		dialogue.Show ();
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
			Reporter.WriteLine ("error number = {0:X8}, error string = {1}",ex.OSError.errnum,ex.OSError.errmess);
			Reporter.WriteLine ("StackTrace: {0}", Environment.StackTrace);
		}
		finally
		{
		}
	}
}
