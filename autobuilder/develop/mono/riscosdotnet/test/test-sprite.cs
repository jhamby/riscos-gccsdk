/*
 * Simple application to test the OSSpriteOp and dynamic area
 * RISC OS C# bindings.
 *
 * RISC OS Dot Net (the C# binding) is still evolving, so
 * this test app probably will too.
 *
 * In this case, I've used plain WIMP.
 *
 * Author:
 *	Lee Noar <leenoar@sky.com>
 */

using System;
using System.Collections.Generic;
using riscos;

namespace riscos.CSharpBindings.SpriteTest
{
	public class SpriteTask : WimpTask
	{
		OS.DynamicAreaHeap Heap;

		OSSpriteOp.SpriteArea SpriteArea;

		List<OSSpriteOp.SpriteByPointer>  Sprites = new List<OSSpriteOp.SpriteByPointer> ();

		void CreateSprite ()
		{
			try {
				int sprite_type = OSSpriteOp.GenerateOldSpriteType (OS.PixelDepth.BPP24,
										    OSSpriteOp.SpriteDpi.Dpi90,
										    OSSpriteOp.SpriteDpi.Dpi90,
										    false);
				var sprite = new OSSpriteOp.SpriteByPointer (SpriteArea,
									     "sprite1",
									     false,
									     50,
									     50,
									     sprite_type);

				OSSpriteOp.RedirectContext context = sprite.SwitchTo (IntPtr.Zero);

				// Make sure that if any errors occur, output is directed back to the
				// screen.
				try {
					for (int x = 0; x < 100; x++)
					{
						uint red = (uint)((256 * x) / 256) + 30;

						ColourTrans.SetGCOL (OS.GCOLAction.Replace_FG,
								     red << ColourTrans.RedShift);
						OS.PlotLine (x, 0, x, 100);
					}

					Sprites.Add (sprite);
				}
				catch {
					throw;
				}
				finally {
					sprite.SwitchFrom (context);
				}
			}
			catch {
				throw;
			}
		}

		void Close (object sender, Wimp.CloseEventArgs e)
		{
			Quit = true;
		}

		void Init ()
		{
			int[] mess_list = { 0 };

			Initialise (350, "Test App", mess_list);

			Heap = new OS.DynamicAreaHeap ("C# Sprites",
						       4 * 1024,		// 4KB Initial
						       10 * 1024 * 1024);	// 10MB Max

			// Allocate 1MB of memory from the dynamic area to use as a sprite area.
			var sprite_area = Heap.Alloc (1 * 1024 * 1024);
			SpriteArea = new OSSpriteOp.SpriteArea (sprite_area, 1 * 1024 * 1024);

			CreateSprite ();

			Wimp.WindowAttributes attributes = new Wimp.WindowAttributes ("CSharp Sprites");
			attributes.WorkArea = new OS.Rect (0, 0, 2000, 2000);
			attributes.UserRedrawn = true;

			SpriteWindow window = new SpriteWindow (Sprites,
								attributes);
			window.Closed += Close;

			window.Open (new OS.Rect (100, 100, 500, 500),			// Visible area
				     new OS.Coord (0, 0),				// Scroll offsets
				     Wimp.WindowStackPosition.Top);
		}

		void Run ()
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
							  ex.OSError.ErrNum,
							  ex.OSError.ErrMess);
				}
				catch (Exception ex)
				{
					Reporter.WriteLine (ex.Message);
					// Split the stacktrace into individual lines and feed each one to
					// Reporter.
					string[] lines = ex.StackTrace.Split (new char[]{'\n'});
					foreach (string s in lines)
						Reporter.WriteLine (s);
				}
			}
		}

		public static void Main(string[] args)
		{
			SpriteTask task = new SpriteTask();

			try
			{
				task.Init ();
				task.Run ();
			}
			catch (OS.ErrorException ex)
			{
				Reporter.WriteLine (ex.Message);
				Reporter.WriteLine (ex.OSError.ErrMess);
				// Split the stacktrace into individual lines and feed each one to
				// Reporter.
				string[] lines = ex.StackTrace.Split (new char[]{'\n'});
				foreach (string s in lines)
					Reporter.WriteLine (s);
			}
			finally
			{
				task.CloseDown ();
			}
		}

		class SpriteWindow : Wimp.Window
		{
			List<OSSpriteOp.SpriteByPointer> Sprites;

			int WindowHeight;

			// Initialise to the identity matrix.
			OS.Matrix matrix = new OS.Matrix ();

			public SpriteWindow (List<OSSpriteOp.SpriteByPointer> sprites,
					     Wimp.WindowAttributes attr) : base (attr)
			{
				Sprites = sprites;

				WindowHeight = attr.WorkArea.MaxY - attr.WorkArea.MinY;

				matrix.Rotate (45);
			}

			protected override void OnPaint (Wimp.RedrawEventArgs e)
			{
				// Render as is.
				OS.Move (e.Origin.X + 40, e.Origin.Y + WindowHeight - 170);
				Sprites[0].Plot (OSSpriteOp.PlotAction.OverWrite);

				// Render scaled by factor of 2.
				Sprites[0].PlotScaled (e.Origin.X + 180,
						       e.Origin.Y + WindowHeight - 220,
						       OSSpriteOp.PlotAction.OverWrite,
						       new OS.ScaleFactors (2, 2, 1, 1),
						       IntPtr.Zero);

				// Render transformed.
				// Transformed sprites are not plotted at the current graphics position;
				// the matrix/destination rectangle is used to place the sprite at the
				// required position.
				// The translation has to be applied here because we don't know ahead
				// of time where the screen position is and it is likely to change
				// as the window is moved.
				// A copy of the matrix has to be used so that the translation is applied
				// only once to the original matrix rather than multiple times.
				var local_matrix = (OS.Matrix)matrix.Clone ();
				local_matrix.Translate (e.Origin.X + 480, e.Origin.Y + WindowHeight - 170);
				Sprites[0].PlotTransformed (OSSpriteOp.PlotAction.OverWrite,
							    local_matrix,
							    IntPtr.Zero);
			}
		}
	}
}
