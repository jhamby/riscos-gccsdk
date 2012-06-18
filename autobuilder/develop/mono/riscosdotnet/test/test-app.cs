/*
 * Simple application to test the RISC OS C# bindings and
 * demonstrate what a RISC OS application might look like
 * when written in C#.
 *
 * RISC OS Dot Net (the C# binding) is still evolving, so
 * this test app probably will too.
 *
 * In this case, I've used plain WIMP only - no toolbox yet.
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
	public void redraw_main_window (object sender, Wimp.RedrawEventArgs args)
	{
		ColourTrans.SetGCOL (OS.GCOLAction.Replace_FG, ColourTrans.FullRed);
		OS.PlotCircleFill (args.Origin.X + 1000, args.Origin.Y + 1000, 400);
		ColourTrans.SetGCOL (OS.GCOLAction.Replace_FG, ColourTrans.FullGreen);
		OS.PlotLine (args.Origin.X, args.Origin.Y, args.Origin.X + 2000, args.Origin.Y + 2000);
		ColourTrans.SetGCOL (OS.GCOLAction.Replace_FG, ColourTrans.FullBlue);
		OS.PlotLine (args.Origin.X, args.Origin.Y + 2000, args.Origin.X + 2000, args.Origin.Y);

		ColourTrans.SetFontColours (ColourTrans.White, ColourTrans.FullBlue, 7);
		main_font.Paint ("CSharp string",
				 Font.PlotType.OSUnits,
				 args.Origin.X + 100,
				 args.Origin.Y + 1000,
				 0); // Length ignored (paint whole string) if bit 7 of flags not set


		ColourTrans.SetFontColours (ColourTrans.White, ColourTrans.Black, 7);
		main_font.Paint ("CSharp string rotated",
				 0, // Coords are in millipoints
				 (args.Origin.X + 1000) * 400,
				 (args.Origin.Y + 1000) * 400,
				 matrix,
				 0); // Length ignored (paint whole string) if bit 7 of flags not set
	}

	public void close_main_window (object sender, Wimp.CloseEventArgs args)
	{
		Quit = true;
	}

	public void mouse_click (object sender, Wimp.MouseClickEventArgs args)
	{
		if ((args.MouseClickWimpBlock.Buttons & 2) != 0)
			main_menu.Show (args.MouseClickWimpBlock.Pos.X - 64,
					args.MouseClickWimpBlock.Pos.Y);
	}

	/* Each element of the array gives the index of the selected item of the menu at
	 * that level. So selection[0] gives the item selected in the first/root menu.
	 * The last selection is terminated by -1.  */
	public override void MenuSelection (int [] selection)
	{
		if (Wimp.Menu.IsCurrent (main_menu))
		{
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
}

public class Test
{
	public static void Main(string[] args)
	{
		int[] mess_list = { 0 };

		MyTask task = new MyTask();

		// Most methods that call a SWI throw an OS.ErrorException if the
		// SWI returns an error.
		try
		{
			task.Initialise (350, "Test App", mess_list);

			task.main_font = new Font.Instance ("Trinity.Bold", 24 << 4, 24 << 4);

			Wimp.Window window = new Wimp.Window ();

			// Register the event handlers for the window.
			window.RedrawHandler += new Wimp.RedrawEventHandler (task.redraw_main_window);
			window.CloseHandler += new Wimp.CloseEventHandler (task.close_main_window);
			window.MouseClickHandler += new Wimp.MouseClickEventHandler (task.mouse_click);

			window.SetExtent (new OS.Rect (0, 0, 2000, 2000));
			window.Create ("CSharp Window");

			window.CreateIcon (new OS.Rect (150, 150, 350, 350),
					   0x2700313fU,					// Iconflags
					   new Wimp.IconData (new Wimp.IconBuffer ("C# Button"),
							      new Wimp.IconBuffer ("R5,3")),
					   0);
			window.Open (new OS.Rect (100, 100, 500, 500),			// Visible area
				     new OS.Coord (0, 0),				// Scroll offsets
				     Wimp.WindowStackPosition.Top);

			task.rename_menu = new Wimp.Menu ("New name:");
			// Rename writable menu item is created with "Default" as its initial
			// text and a buffer size of 30 characters. The validation string is
			// set to the command to accept digits and upper/lower case letters of
			// the alphabet.
			task.rename_menuitem = task.rename_menu.AddItem ("Default", 30, "A0-9a-zA-Z");
			task.rename_menuitem.Writable = true;

			task.file_menu = new Wimp.Menu ("File");
			task.save_menuitem = task.file_menu.AddItem ("Save");

			task.edit_menu = new Wimp.Menu ("Edit");
			task.edit_menu.AddItem ("Rename").SubMenu = task.rename_menu;

			task.main_menu = new Wimp.Menu ("CSharp Menu");
			task.main_menu.AddItem ("File").SubMenu = task.file_menu;
			Wimp.MenuItem item = task.main_menu.AddItem ("Edit");
			item.SubMenu = task.edit_menu;
			item.DottedLine = true;
			task.main_menu.AddItem ("Quit");

			// Push the text 100 OS units up a 45 degree angle.
			task.matrix.Translate (100, 0);
			task.matrix.Rotate (45);

			task.PollLoop ();
		}
		catch (OS.ErrorException ex)
		{
			Reporter.WriteLine (ex.OSError.errmess);
			Console.WriteLine ("error number = {0}, error string = {1}",ex.OSError.errnum,ex.OSError.errmess);
		}
		finally
		{
			task.CloseDown ();
		}
	}
}

