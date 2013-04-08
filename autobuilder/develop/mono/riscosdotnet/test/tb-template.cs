/*
 * Application written to test the C# bindings when creating Toolbox
 * objects/gadgets from templates within memory.
 * 
 * Author:
 *	Lee Noar <leenoar@sky.com>
 */

using System;
using riscos;

namespace riscos.CSharpBindings.TBTemplateTest
{
	public class TBTemplateTask : ToolboxTask
	{
		Dialogue dialogue;
		const string Version = "V1.0 (5th April 2013)";

		public void Init ()
		{
			int[] mess_list = { 0 };
			int[] event_list = { 0 };

			// The message and event lists are copied by the toolbox, so we can
			// discard them after initialisation.
			Initialise (350, mess_list, event_list, "<TBTemplate$Dir>");

			// Define the attributes of a window including the string for the title bar.
			var win_attr = new Wimp.WindowAttributes ("Toolbox Window created in memory using C#");

			// Adjust the default window attributes to suit our needs.
			win_attr.ButtonType = Wimp.WindowButtonType.GainInputFocus;
			win_attr.WorkAreaBGColour = (int)OS.DesktopColour.Grey1;

			using (var win_template = new Toolbox.WindowTemplate ("MyWindow", win_attr))
			{
				// The template exists only in this block scope. When this scope exits,
				// the IDisposable interface ensures that the buffer is immediately freed.

				// All windows created with this template will initially open at the
				// default view rectangle defined here when shown with a default spec.
				win_template.DefaultViewRect = new OS.Rect (100, 100, 1100, 1100);
				win_template.HelpMessage = "Toolbox Window created in memory using C#";
				win_template.Flags = Toolbox.Window.Flags.AutoOpenWindow |
						     Toolbox.Window.Flags.AutoCloseWindow |
						     Toolbox.Window.Flags.GenerateHasBeenHidden;

				// Define an ActionButton gadget in the window template
				Toolbox.GadgetTemplate gadget = new Toolbox.ActionButtonTemplate ("Ok");
				gadget.BoundingBox = new OS.Rect (50, 100, 200, 160);
				win_template.AddGadget (gadget);

				// Define an AdjusterArrow gadget in the window template
				gadget = new Toolbox.AdjusterArrowTemplate ();
				gadget.BoundingBox = new OS.Rect (250, 100, 300, 160);
				((Toolbox.AdjusterArrowTemplate)gadget).SetType (Toolbox.AdjusterArrowTemplate.Direction.Increment,
										 Toolbox.AdjusterArrowTemplate.Orientation.LeftRight);
				win_template.AddGadget (gadget);

				// Define a Button gadget in the window template
				gadget = new Toolbox.ButtonTemplate ("Button", "R5,3");
				gadget.BoundingBox = new OS.Rect (50, 880, 150, 980);
				((Toolbox.ButtonTemplate)gadget).ButtonFlags.ButtonType = Wimp.IconButtonType.Click;
				((Toolbox.ButtonTemplate)gadget).ButtonFlags.FGColour = OS.DesktopColour.Black;
				((Toolbox.ButtonTemplate)gadget).ButtonFlags.BGColour = OS.DesktopColour.Grey1;
				win_template.AddGadget (gadget);

				// Define a DisplayField gadget in the window template
				gadget = new Toolbox.DisplayFieldTemplate ("Display field");
				gadget.BoundingBox = new OS.Rect (50, 800, 350, 860);
				win_template.AddGadget (gadget);

				// Define a Draggable gadget in the window template.
				gadget = new Toolbox.DraggableTemplate ("Draggable", "file_ff9");
				gadget.BoundingBox = new OS.Rect (200, 880, 350, 980);
				((Toolbox.DraggableTemplate)gadget).DropShadow = true;
				win_template.AddGadget (gadget);

				// Define a keyboard shortcut in the window template.
				var shortcut = new Toolbox.KeyboardShortcutTemplate ('A', 0x1234);
				win_template.AddKeyboardShortcut (shortcut);

				// Use the template to create the dialogue window.
				dialogue = new Dialogue (win_template);
			}

			// This is a lambda expression (an anonymous function) used as an
			// event handler.
			dialogue.HasBeenHidden += (sender, e) => Quit = true;

			// Show using default spec.
			dialogue.Show ();
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
					Reporter.WriteLine ("{0}", ex.Message);
					string[] lines = ex.StackTrace.Split (new char[]{'\n'});
					foreach (string s in lines)
						Reporter.WriteLine ("{0}", s);
					Wimp.ReportError (Wimp.ErrorBoxFlags.OKIcon,
							  "TBTemplate",
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
			var task = new TBTemplateTask();

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
		}

		public class Dialogue : Toolbox.Window
		{
			public Dialogue (Toolbox.WindowTemplate template) : base (template)
			{
			}
		}
	}
}
