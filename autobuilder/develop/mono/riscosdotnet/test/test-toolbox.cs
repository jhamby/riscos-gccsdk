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
		public const uint Faded = 1;
		public const uint SubMenu = 2;
	}

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
		Slider.Value = 85;
		Slider.SetColour (OS.DesktopColour.Red, OS.DesktopColour.Cream);

		Draggable = new Toolbox.Draggable (this, CmpID.Draggable);
		Draggable.Text = "Sprites";
		Draggable.Sprite = "file_ff9";

		Button = new Toolbox.Button (this, CmpID.Button);
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
		Reporter.WriteLine ("New value is '{0}'.", e.NewValue);
		Reporter.WriteLine ("");
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

	public void QuitHandler (object sender, Toolbox.ToolboxEventArgs args)
	{
		Reporter.WriteLine ("Quit handler called - Exiting...");
		Reporter.WriteLine ("");
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
			Reporter.WriteLine ("error number = {0:X8}, error string = {1}",ex.OSError.errnum,ex.OSError.errmess);
			Reporter.WriteLine ("StackTrace: {0}", Environment.StackTrace);
		}
		finally
		{
		}
	}
}
