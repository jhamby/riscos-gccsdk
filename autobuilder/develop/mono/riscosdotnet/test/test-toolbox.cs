/*
 * Simple application to test the RISC OS C# bindings and
 * demonstrate what a RISC OS toolbox application might
 * look like when written in C#.
 *
 * RISC OS Dot Net (the C# binding) is still evolving, so
 * this test app probably will too.
 *
 * Author:
 *	Lee Noar <leenoar@sky.com>
 */

using System;
using riscos;

public class MyTask : ToolboxTask
{
	private Toolbox.Window MainWindow;
	private Font.Instance MainFont;

	enum MyEvent
	{
		Quit = 0x9999
	}

	public void Init ()
	{
		int[] mess_list = { 0 };
		int[] event_list = { 0 };

		// The message and event lists are copied by the toolbox, so we can
		// discard them after initialisation.
		Initialise (350, mess_list, event_list, "<MonoTestTB$Dir>");

		MainFont = new Font.Instance ("Trinity.Medium", 24 << 4, 24 << 4);
		MainWindow = new Toolbox.Window ("MainWindow");

		// Can also use MainWindow.SetTitle ("CSharp Toolbox Window");
		MainWindow.Title = "CSharp Toolbox Window";

		// Register the event handlers for the window.
		MainWindow.RedrawHandler += new Wimp.RedrawEventHandler (RedrawMainWindow);
		MainWindow.ToolboxHandlers.Add ((uint)MyEvent.Quit, QuitHandler);
	}

	public void Run ()
	{
		MainWindow.Show ();
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
		Quit = true;
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
			Reporter.WriteLine (ex.OSError.errmess);
			Console.WriteLine ("error number = {0}, error string = {1}",ex.OSError.errnum,ex.OSError.errmess);
		}
		finally
		{
		}
	}
}
