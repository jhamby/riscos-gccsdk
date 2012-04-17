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
using riscos.Wimp;

public class MyTask : WimpTask
{
	// These are C# 'event' handlers.
	public void redraw_main_window (object sender, RedrawEventArgs args)
	{
		ColourTrans.SetGCOL (OS.GCOLAction.Replace_FG, ColourTrans.FullRed);
		OS.PlotCircleFill (args.Origin.X + 1000, args.Origin.Y + 1000, 400);
		ColourTrans.SetGCOL (OS.GCOLAction.Replace_FG, ColourTrans.FullGreen);
		OS.PlotLine (args.Origin.X, args.Origin.Y, args.Origin.X + 2000, args.Origin.Y + 2000);
		ColourTrans.SetGCOL (OS.GCOLAction.Replace_FG, ColourTrans.FullBlue);
		OS.PlotLine (args.Origin.X, args.Origin.Y + 2000, args.Origin.X + 2000, args.Origin.Y);
	}

	public void close_main_window (object sender, CloseEventArgs args)
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

			Window window = new Window ();
			window.RedrawHandler += new RedrawEventHandler (task.redraw_main_window);
			window.CloseHandler += new CloseEventHandler (task.close_main_window);
			window.SetExtent (new OS.Rect (0, 0, 2000, 2000));
			window.Create ("CSharp Window");

			window.CreateIcon (new OS.Rect (150, 150, 350, 350),
					   0x2700313fU,					// Iconflags
					   new IconData (new IconBuffer ("C# Button"),
							 new IconBuffer ("R5,3")),
					   0);
			window.Open (new OS.Rect (100, 100, 500, 500),			// Visible area
				     new OS.Coord (0, 0),				// Scroll offsets
				     WindowStackPosition.Top);

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

