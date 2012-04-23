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

	public OS.Matrix matrix;

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

		main_font.Paint ("CSharp string",
				 1U << 4, // Coords are in OS units
				 args.Origin.X + 100,
				 args.Origin.Y + 1000,
				 0); // Length ignored (paint whole string) if bit 7 of flags not set


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
			window.RedrawHandler += new Wimp.RedrawEventHandler (task.redraw_main_window);
			window.CloseHandler += new Wimp.CloseEventHandler (task.close_main_window);
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

			// Push the text 100 OS units up a 45 degree angle.
			task.matrix.Translate (100, 0);
			task.matrix.Rotate (45);

			task.PollLoop ();
		}
		catch (OS.ErrorException ex)
		{
			Reporter.Output (ex.OSError.errmess);
			Console.WriteLine ("error number = {0}, error string = {1}",ex.OSError.errnum,ex.OSError.errmess);
		}
		finally
		{
			task.CloseDown ();
		}
	}
}

