/*
 * Simple application to test the RISC OS C# bindings and
 * demonstrate what a RISC OS application might look like
 * when written in C#.
 *
 * RISC OS Dot Net (the C# binding) is still evolving, so
 * this test app probably will too.
 *
 * In this case, I've used plain WIMP only - no toolbox.
 *
 * Author:
 *	Lee Noar <leenoar@sky.com>
 */

using System;
using riscos;

public class MyTask : WimpTask
{
	public Font.Instance main_font;

	public Wimp.Menu main_menu, file_menu, edit_menu, rename_menu;
	public Wimp.MenuItem save_menuitem, rename_menuitem;

	public OS.Matrix matrix;

	public Wimp.Icon WritableField;
	public Wimp.Icon Button;

	enum MainMenu
	{
		Root
	}

	enum MainMenuItem
	{
		FileSubMenu,
		EditSubMenu,
		Quit
	}

	enum FileMenuItem
	{
		Save
	}

	enum EditMenuItem
	{
		RenameSubMenu
	}

	enum RenameMenuItem
	{
		Rename
	}

	public MyTask () : base ()
	{
		// Initialise to the identity matrix.
		matrix = new OS.Matrix ();
	}

	// These are C# 'event' handlers.
	public void redraw_main_window (object sender, Wimp.RedrawEventArgs e)
	{
		ColourTrans.SetGCOL (ColourTrans.FullRed, OS.GCOLAction.Replace_FG);
		OS.PlotCircleFill (e.Origin.X + 1000, e.Origin.Y + 1000, 400);
		ColourTrans.SetGCOL (ColourTrans.FullGreen, OS.GCOLAction.Replace_FG);
		OS.PlotLine (e.Origin.X, e.Origin.Y, e.Origin.X + 2000, e.Origin.Y + 2000);
		ColourTrans.SetGCOL (ColourTrans.FullBlue, OS.GCOLAction.Replace_FG);
		OS.PlotLine (e.Origin.X, e.Origin.Y + 2000, e.Origin.X + 2000, e.Origin.Y);

		ColourTrans.SetFontColours (ColourTrans.White, ColourTrans.FullBlue, 7);
		main_font.Paint ("CSharp string",
				 Font.PlotType.OSUnits,
				 e.Origin.X + 100,
				 e.Origin.Y + 1000,
				 0); // Length ignored (paint whole string) if bit 7 of flags not set


		ColourTrans.SetFontColours (ColourTrans.White, ColourTrans.Black, 7);
		main_font.Paint ("CSharp string rotated",
				 0, // Coords are in millipoints
				 (e.Origin.X + 1000) * 400,
				 (e.Origin.Y + 1000) * 400,
				 matrix,
				 0); // Length ignored (paint whole string) if bit 7 of flags not set
	}

	public void close_main_window (object sender, Wimp.CloseEventArgs e)
	{
		Quit = true;
	}

	public void mouse_click (object sender, Wimp.MouseClickEventArgs e)
	{
		if ((e.MouseClickWimpBlock.Buttons & 2) != 0)
			main_menu.Show (e.MouseClickWimpBlock.Pos.X - 64,
					e.MouseClickWimpBlock.Pos.Y);
		else if (e.MouseClickWimpBlock.Buttons == 64)
		{
			var window = (Wimp.Window)sender;
			var drag = new NativeWimp.DragStartBlock (e.MouseClickWimpBlock.WindowHandle,
								  Wimp.DragType.Rubber,
								  new OS.Rect (e.MouseClickWimpBlock.Pos.X,
									       e.MouseClickWimpBlock.Pos.Y,
									       e.MouseClickWimpBlock.Pos.X,
									       e.MouseClickWimpBlock.Pos.Y),
								  window.VisibleArea);
			Wimp.DragBox (ref drag);
		}
	}

	public void drag_end (object sender, Wimp.UserDragEventArgs e)
	{
		Reporter.WriteLine ("User drag ended.");
		Reporter.WriteLine ("  Resulting box was minx: {0}, miny: {1}, maxx: {2}, maxy: {3}",
				    e.Box.MinX, e.Box.MinY, e.Box.MaxX, e.Box.MaxY);
	}

	// Because we're a derived object, override the method in the WimpTask base class
	// rather than subscribe to the events.
	protected override void OnMenuSelection (Wimp.MenuSelectionEventArgs e)
	{
		if (Wimp.Menu.IsCurrent (main_menu))
		{
			int[] selection = e.MenuSelectionWimpBlock.Selection;
			/* Each element of the array gives the index of the selected item of the menu at
			 * that level. So selection[0] gives the item selected in the first/root menu.
			 * The last selection is terminated by -1.  */
			switch (selection[(int)MainMenu.Root])
			{
				case (int)MainMenuItem.FileSubMenu:
					if (selection[1] == (int)FileMenuItem.Save)
						Reporter.WriteLine ("Save menu item selected");
					break;

				case (int)MainMenuItem.EditSubMenu:
					if (selection[1] == (int)EditMenuItem.RenameSubMenu &&
					    selection[2] == (int)RenameMenuItem.Rename)
						Reporter.WriteLine ("Rename menu item selected - new text is {0}",
								    rename_menuitem.GetText());
					break;

				case (int)MainMenuItem.Quit:
					Reporter.WriteLine ("Quit menu item selected");
					Quit = true;
					break;
			}
		}
	}

	public void window_DataLoad (object sender, Wimp.DataLoadMessageEventArgs e)
	{
		// The sender is the Wimp.Window object.
		Reporter.WriteLine ("Received DataLoad Wimp message:");
		Reporter.WriteLine ("  Window handle: {0:X8}", e.DataLoad.WindowHandle);
		Reporter.WriteLine ("  Icon handle: {0}", (int)e.DataLoad.IconHandle);
		Reporter.WriteLine ("  Position: {0},{1}", e.DataLoad.Position.X, e.DataLoad.Position.Y);
		Reporter.WriteLine ("  Estimated file size: {0}", e.DataLoad.EstimatedSize);
		Reporter.WriteLine ("  File type: {0:X3}", e.DataLoad.FileType);
		Reporter.WriteLine ("  Filename: {0}",e.DataLoad.FileName);
		Reporter.WriteLine ("");
	}

	public void gain_caret (object sender, Wimp.CaretEventArgs e)
	{
		var caret = (Wimp.Window.CaretEventArgs) e;
		Reporter.WriteLine ("caret window: {0:X8}", caret.Window.WimpHandle);
		Reporter.WriteLine ("caret icon: {0}", caret.Icon.Handle);
		Reporter.WriteLine ("caret pos: {0},{1}", caret.Offset.X, caret.Offset.Y);
		Reporter.WriteLine ("caret index: {0}", caret.Index);
		Reporter.WriteLine ("caret invisible: {0}", caret.Invisible);
		Reporter.WriteLine ("");
	}

	public void pointer_enter (object sender, Wimp.PointerEventArgs e)
	{
		// Note that the window handle is provided in the base class 'e' so
		// we don't really need to go through the window object to find it.
		// However, this demonstrates that a higher level object exists containing
		// a reference to the Window object if required.
		var enter = (Wimp.Window.PointerEventArgs) e;

		Reporter.WriteLine ("Pointer entered window with handle: {0:X8}", enter.Window.WimpHandle);
	}

	public void pointer_leave (object sender, Wimp.PointerEventArgs e)
	{
		// Note that the window handle is provided in the base class 'e' so
		// we don't really need to go through the window object to find it.
		// However, this demonstrates that a higher level object exists containing
		// a reference to the Window object if required.
		var leave = (Wimp.Window.PointerEventArgs) e;

		Reporter.WriteLine ("Pointer left window with handle: {0:X8}", leave.Window.WimpHandle);
	}

	public void Init ()
	{
		int[] mess_list = { 0 };

		Initialise (350, "Test App", mess_list);

		main_font = new Font.Instance ("Trinity.Bold", 24 << 4, 24 << 4);

		var win_attr = new Wimp.WindowAttributes ("CSharp Window");

		win_attr.WorkArea = new OS.Rect (0, 0, 2000, 2000);
		win_attr.UserRedrawn = true;
		win_attr.ButtonType = Wimp.WindowButtonType.ClickDrag;

		var window = new Wimp.Window (win_attr);

		// Register the event handlers for the window.
		window.Paint += redraw_main_window;
		window.Closed += close_main_window;
		window.MouseClick += mouse_click;
		window.MsgDataLoad += window_DataLoad;
		window.GainCaret += gain_caret;
		window.PointerEnter += pointer_enter;
		window.PointerLeave += pointer_leave;

		UserDragEnd += drag_end;

		// Create an icon in the window.
		var icon_attr = new Wimp.IconAttributes ("C# Button", "R5,3");

		icon_attr.BoundingBox = new OS.Rect (150, 12, 350, 60);
		icon_attr.ButtonType = Wimp.IconButtonType.Click;
		Button = window.CreateIcon (icon_attr);

		icon_attr = new Wimp.IconAttributes ("", 30);
		icon_attr.BoundingBox = new OS.Rect (400, 12, 700, 60);
		icon_attr.ButtonType = Wimp.IconButtonType.GainInputFocus;
		icon_attr.BGColour = OS.DesktopColour.White;
		WritableField = window.CreateIcon (icon_attr);

		window.Open (new OS.Rect (100, 100, 1000, 1000),	// Visible area
			     new OS.Coord (0, 0),			// Scroll offsets
			     Wimp.WindowStackPosition.Top);

		WritableField.SetText (DateTime.Now.ToString ());
		WritableField.SetCaretPosition (3);

		rename_menu = new Wimp.Menu ("New name:");
		// Rename writable menu item is created with "Default" as its initial
		// text and a buffer size of 30 characters. The validation string is
		// set to the command to accept digits and upper/lower case letters of
		// the alphabet.
		rename_menuitem = rename_menu.AddItem ("Default", 30, "A0-9a-zA-Z");
		rename_menuitem.Writable = true;

		file_menu = new Wimp.Menu ("File");
		save_menuitem = file_menu.AddItem ("Save");

		edit_menu = new Wimp.Menu ("Edit");
		edit_menu.AddItem ("Rename").SubMenu = rename_menu;

		main_menu = new Wimp.Menu ("CSharp Menu");
		main_menu.AddItem ("File").SubMenu = file_menu;
		Wimp.MenuItem item = main_menu.AddItem ("Edit");
		item.SubMenu = edit_menu;
		item.DottedLine = true;
		main_menu.AddItem ("Quit");

		// Push the text 100 OS units up a 45 degree angle.
		matrix.Translate (100, 0);
		matrix.Rotate (45);
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
						  "MonoTestApp",
						  ex.OSError.Number,
						  ex.OSError.Message);
			}
			catch (Exception ex)
			{
				Reporter.WriteLine ("{0}", ex.Message);
				// Split the stacktrace into individual lines and feed each one to
				// Reporter.
				string[] lines = ex.StackTrace.Split (new char[]{'\n'});
				foreach (string s in lines)
					Reporter.WriteLine ("{0}", s);
			}
		}
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
			Reporter.WriteLine (ex.OSError.Message);
			Console.WriteLine ("error number = {0}, error string = {1}",
					   ex.OSError.Number,
					   ex.OSError.Message);
		}
		finally
		{
			task.CloseDown ();
		}
	}
}

