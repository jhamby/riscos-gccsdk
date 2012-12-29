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

using System;
using riscos;
using System.Diagnostics;

namespace riscos.CSharpBindings.GadgetTest
{
	public class GadgetTask : ToolboxTask
	{
		Dialogue dialogue;
		const string Version = "V1.0 (24th November 2012)";

		public void Init ()
		{
			int[] mess_list = { 0 };
			int[] event_list = { 0 };

			// The message and event lists are copied by the toolbox, so we can
			// discard them after initialisation.
			Initialise (350, mess_list, event_list, "<MonoTestGadget$Dir>");

			dialogue = Toolbox.Object.CreateInstance<Dialogue> ("Dialogue");

			// This is a lambda expression (an anonymous function) used as an
			// event handler.
			dialogue.HasBeenHidden += (sender, e) => Quit = true;

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
					Wimp.ReportError (Wimp.ErrorBoxFlags.OKIcon,
							  "MonoTestGadget",
							  ex.OSError.ErrNum,
							  ex.OSError.ErrMess);
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
			GadgetTask task = new GadgetTask();

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

	public class Dialogue : Toolbox.Window
	{
		// These are the C# objects that we use to contain a Toolbox gadget
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
		public static class CmpID
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

		public Dialogue (uint objID) : base (objID)
		{
			Title = "C# Dialogue Box";
			GainCaret += gain_caret;

			OKButton = new Toolbox.ActionButton (this, CmpID.OKButton);
			OKButton.Text = "OK";
			OKButton.Faded = true;

			CancelButton = new Toolbox.ActionButton (this, CmpID.CancelButton);
			CancelButton.Text = "Cancel";
			CancelButton.HelpMessage = "Help text set in CSharp";
			CancelButton.Click += cancelbtn_Click;

			DisplayField = new Toolbox.DisplayField (this, CmpID.DisplayField);
			DisplayField.Value = String.Format("{0:D}", DateTime.Now);
			DisplayField.SetFont ("Trinity.Bold.Italic", 12 << 4, 12 << 4);

			WritableField = new Toolbox.WritableField (this, CmpID.WritableField);
			WritableField.Value = "CSharp";
			WritableField.Allowable = "A-Za-z0-9 ";
			WritableField.SetFont ("Trinity.Medium.Italic", 12 << 4, 12 << 4);
			WritableField.ValueChange += writable_ValueChange;

			NumberRange = new Toolbox.NumberRange (this, CmpID.NumberRange);
			NumberRange.Value = 1999;
			NumberRange.ValueChange += numberrange_ValueChange;

			StringSet = new Toolbox.StringSet (this, CmpID.StringSet);
			StringSet.Available = "One,Two,Three,Four,Five";
			StringSet.SelectedString = "Three";
			StringSet.ValueChange += stringset_ValueChange;
			StringSet.AboutToBeShown += stringset_AboutToBeShown;

			OptionButton = new Toolbox.OptionButton (this, CmpID.OptionButton);
			OptionButton.Label = "Sugar";
			OptionButton.CurrentState = Toolbox.OptionButton.State.On;
			OptionButton.StateChange += optionbutton_StateChange;

			Radio1 = new Toolbox.RadioButton (this, CmpID.Radio1);
			Radio1.Label = "Coffee";
			Radio1.StateChange += radiobutton_StateChange;

			Radio2 = new Toolbox.RadioButton (this, CmpID.Radio2);
			Radio2.Label = "Tea";
			Radio2.CurrentState = Toolbox.RadioButton.State.On;
			Radio2.StateChange += radiobutton_StateChange;

			Slider = new Toolbox.Slider (this, CmpID.Slider);
			Slider.UpperBound = 1000;
			Slider.LowerBound = 100;
			Slider.StepSize = 20;
			Slider.Value = 200;
			Slider.SetColour (OS.DesktopColour.Red, OS.DesktopColour.Cream);
			Slider.ValueChange += slider_ValueChange;

			Draggable = new Toolbox.Draggable (this, CmpID.Draggable);
			Draggable.Text = "Sprites";
			Draggable.Sprite = "file_ff9";
			Draggable.DragStart += draggable_DragStart;
			Draggable.DragEnd += draggable_DragEnd;

			Button = new Toolbox.Button (this, CmpID.Button);

			PopupMenu = new Toolbox.PopupMenu (this, CmpID.PopupMenu);
//			PopupMenu.Menu = task.main_menu;
			PopupMenu.AboutToBeShown += popupmenu_Show;

			AdjusterArrow = new Toolbox.AdjusterArrow (this, CmpID.AdjusterArrow);
			AdjusterArrow.Click += adjusterarrow_Click;
		}

		protected override void OnAboutToBeShown (Toolbox.Window.AboutToBeShownEventArgs e)
		{
			Reporter.WriteLine ("Dialogue about to be shown, show type = {0}", e.ShowSpec.Type);
			Reporter.WriteLine ("");
		}

		void cancelbtn_Click (object sender, Toolbox.ActionButton.ClickEventArgs e)
		{
			Reporter.WriteLine ("Cancel button was selected.");
			Reporter.WriteLine ("Select clicked = {0}", e.SelectClicked);
			Reporter.WriteLine ("Adjust clicked = {0}", e.AdjustClicked);
			Reporter.WriteLine ("Default button = {0}", e.Default);
			Reporter.WriteLine ("Cancel button = {0}", e.Cancel);
			Reporter.WriteLine ("Local button = {0}", e.Local);
			Reporter.WriteLine ("");
		}

		void numberrange_ValueChange (object sender, Toolbox.NumberRange.ValueChangeEventArgs e)
		{
			Reporter.WriteLine ("NumberRange returned ValueChange event.");
			Reporter.WriteLine ("New value is {0}.", e.NewValue);
			Reporter.WriteLine ("");
		}

		void optionbutton_StateChange (object sender, Toolbox.OptionButton.StateChangeEventArgs e)
		{
			Reporter.WriteLine ("OptionButton returned StateChange event.");
			Reporter.WriteLine ("New state is {0}.", e.NewState);
			Reporter.WriteLine ("Mouse {0} button was used to change the state.", e.Select ? "Select" : "Adjust");
			Reporter.WriteLine ("");
		}

		void radiobutton_StateChange (object sender, Toolbox.RadioButton.StateChangeEventArgs e)
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

		void writable_ValueChange (object sender, Toolbox.WritableField.ValueChangeEventArgs e)
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

		void stringset_ValueChange (object sender, Toolbox.StringSet.ValueChangeEventArgs e)
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

		void stringset_AboutToBeShown (object sender, Toolbox.ToolboxEventArgs e)
		{
			Reporter.WriteLine ("StringSet returned AboutToBeShown event.");
			Reporter.WriteLine ("");
		}

		void draggable_DragStart (object sender, Toolbox.Draggable.DragStartEventArgs e)
		{
			Reporter.WriteLine ("Draggable returned DragStart event.");
			Reporter.WriteLine ("Select drag = {0}", e.Select);
			Reporter.WriteLine ("Adjust drag = {0}", e.Adjust);
			Reporter.WriteLine ("Drag with Shift = {0}", e.Shift);
			Reporter.WriteLine ("Drag with Ctrl = {0}", e.Ctrl);
			Reporter.WriteLine ("");
		}

		void draggable_DragEnd (object sender, Toolbox.Draggable.DragEndEventArgs e)
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

		void slider_ValueChange (object sender, Toolbox.Slider.ValueChangeEventArgs e)
		{
			Reporter.WriteLine ("Slider returned ValueChange event.");
			Reporter.WriteLine ("New value is {0}", e.NewValue);
			Reporter.WriteLine ("Drag start or click = {0}", e.DragStartClick);
			Reporter.WriteLine ("Drag in progress = {0}", e.DragInProgress);
			Reporter.WriteLine ("Drag ended = {0}", e.DragEnded);
			Reporter.WriteLine ("");
		}

		void popupmenu_Show (object sender, Toolbox.PopupMenu.AboutToBeShownEventArgs e)
		{
			Reporter.WriteLine ("PopupMenu returned AboutToBeShown event");
			Reporter.WriteLine ("Menu ID of menu that will be shown is {0:X8}", e.Menu.ID);
			Reporter.WriteLine ("");
		}

		void adjusterarrow_Click (object sender, Toolbox.AdjusterArrow.ClickEventArgs e)
		{
			Reporter.WriteLine ("AdjusterArrow returned Click event");
			Reporter.WriteLine ("Arrow direction is {0}",e.Direction);
			Reporter.WriteLine ("");
		}

		void gain_caret (object sender, Wimp.CaretEventArgs e)
		{
			var caret = (Toolbox.Window.CaretState) e.CaretState;
			Reporter.WriteLine ("caret window: {0:X8}", caret.Window.ID);
			if (caret.Gadget == null)
				Reporter.WriteLine ("Gadget is composite; icon handle: {0}", caret.IconHandle);
			else
				Reporter.WriteLine ("caret gadget: {0}", caret.Gadget.ComponentID);
			Reporter.WriteLine ("caret pos: {0},{1}", caret.Offset.X, caret.Offset.Y);
			Reporter.WriteLine ("caret index: {0}", caret.Index);
			Reporter.WriteLine ("caret invisible: {0}", caret.Invisible);
			Reporter.WriteLine ("");
		}
	}
}