//
// os.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using riscos;
using System;
using System.Runtime.InteropServices;

namespace riscos
{
	public static class OS
	{
		/*! \enum GCOLAction
		 * \brief Graphics colour actions for use with the OS plotting routines. */
		public enum GCOLAction
		{
			Replace_FG,
			OR_FG,
			AND_FG,
			EOR_FG,
			NOT_FG,
			Leave_FG,
			BIC_FG,
			ORNOT_FG,
			Replace_BG,
			OR_BG,
			AND_BG,
			EOR_BG,
			NOT_BG,
			Leave_BG,
			BIC_BG,
			ORNOT_BG
		}

		public enum DesktopColour
		{
			White,
			Grey1,
			Grey2,
			Grey3,
			Grey4,
			Grey5,
			Grey6,
			Black,
			Blue,
			Yellow,
			Green,
			Red,
			Cream,
			DarkGreen,
			Orange,
			Cyan
		}

		/*! \class PlotType
		 * Defines graphics primitives to be used with \e OS.Plot. */
		public static class PlotType
		{
			public const int SolidLine = 0;
			public const int SolidLineIncEnd = 0;
			public const int SolidLineExcEnd = 8;
			public const int DottedLine = 16;
			public const int DottedLineIncEnd = 16;
			public const int DottedLineExcEnd = 24;
			public const int SolidLineExcFirst = 32;
			public const int SolidLineExcBoth = 40;
			public const int DottedLineExcFirst = 48;
			public const int DottedLineExcBoth = 56;
			public const int Point = 64;
			public const int HFLeftAndRightNonBG = 72; // Horizontal Fill
			public const int TriangleFill = 80;
			public const int HFToRightToBG = 88;
			public const int RectangleFill = 96;
			public const int HFLeftAndRightToFG = 104;
			public const int ParallelogramFill = 112;
			public const int HFToRightNonFG = 120;
			public const int FloodToNonBG = 128;
			public const int FloodToNonFG = 136;
			public const int CircleOutline = 144;
			public const int CircleFill = 152;
			public const int CircleArc = 160;
			public const int Segment = 168;
			public const int Sector = 176;
			public const int BlockCopyMove = 184;
			public const int EllipseOutline = 192;
			public const int EllipseFill = 200;
			public const int SpritePlot = 232;
		}

		/*! \class PlotModifier
		 * Modifies how the graphics primitives are to be drawn \e OS.Plot. */
		public static class PlotModifier
		{
			public const int MoveRelative = 0;
			public const int PlotRelativeFG = 1;
			public const int PlotRelativeInverse = 2;
			public const int PlotRelativeBG = 3;
			public const int MoveAbsolute = 4;
			public const int PlotAbsoluteFG = 5;
			public const int PlotAbsoluteInverse = 6;
			public const int PlotAbsoluteBG = 7;
		}

		public class ErrorException : System.Exception
		{
			public NativeOS.Error OSError;

			public ErrorException(IntPtr error_ptr) : base ()
			{
				OSError = (NativeOS.Error)Marshal.PtrToStructure (error_ptr, typeof(NativeOS.Error));
			}
		}

		public static void ThrowOnError (IntPtr error)
		{
			if (error != IntPtr.Zero)
				throw new ErrorException (error);
		}

		[StructLayout(LayoutKind.Sequential)]
		public class ScaleFactors
		{
			public int XMultiplier;
			public int YMultiplier;
			public int XDivisor;
			public int YDivisor;

			public ScaleFactors (int xm, int ym, int xd, int yd)
			{
				XMultiplier = xm;
				YMultiplier = ym;
				XDivisor = xd;
				YDivisor = yd;
			}
		}

		public class Rect
		{
			public int MinX { get; set; }
			public int MinY { get; set; }
			public int MaxX { get; set; }
			public int MaxY { get; set; }

			public Rect ()
			{
				MinX = MinY = MaxX = MaxY = 0;
			}

			public Rect (int MinX, int MinY, int MaxX, int MaxY)
			{
				this.MinX = MinX;
				this.MinY = MinY;
				this.MaxX = MaxX;
				this.MaxY = MaxY;
			}

			public Rect (NativeOS.Rect rect)
			{
				MinX = rect.MinX;
				MinY = rect.MinY;
				MaxX = rect.MaxX;
				MaxY = rect.MaxY;
			}
		}

		public class Coord
		{
			public int X;
			public int Y;

			public Coord ()
			{
				X = Y = 0;
			}

			public Coord (int X, int Y)
			{
				this.X = X;
				this.Y = Y;
			}

			public Coord (NativeOS.Coord coord)
			{
				X = coord.X;
				Y = coord.Y;
			}
		}

		/*! \class Matrix
		 * \brief Used by \e Font.Instance.ScanString and \e Font.Instance.Paint
		 * to transform, scale and rotate text. */
		public class Matrix : ICloneable
		{
			public int [,] m;

			/*! \brief Create the identity matrix. */
			public Matrix ()
			{
				int x, y;

				m = new int [3, 3];

				for (y = 0; y < 3; y++)
					for (x = 0; x < 3; x++)
						m[x,y] = (x == y) ? ToTransformUnits(1.0) : 0;
			}

			/*! \brief Create matrix with rotate, scale and translation elements.
			 * \param [in] m00 x scale factor, or cos(\htmlonly&#952 \endhtmlonly) to rotate.
			 * \param [in] m01 sin(\htmlonly&#952 \endhtmlonly) to rotate.
			 * \param [in] m10 -sin(\htmlonly&#952 \endhtmlonly) to rotate.
			 * \param [in] m11 y scale factor, or cos(\htmlonly&#952 \endhtmlonly) to rotate.
			 * \param [in] m20 x translation.
			 * \param [in] m21 y translation.
			 *
			 * \note
			 * m00 - m11 are in Transform units i.e. 16.16 fixed point.<br>
			 * m20, m21 are in internal Draw units i.e. 24.8 fixed point.<br>
			 * Values are used as is, no conversion is performed. */
			public Matrix (int m00, int m01, int m10, int m11, int m20, int m21)
			{
				m = new int [3, 3];

				Set (m00, m01, m10, m11, m20, m21);
			}

			/*! \brief Create matrix with rotate and scale elements - x,y translation set to zero.
			 * \param [in] m00 x scale factor, or cos(\htmlonly&#952 \endhtmlonly) to rotate.
			 * \param [in] m01 sin(\htmlonly&#952 \endhtmlonly) to rotate.
			 * \param [in] m10 -sin(\htmlonly&#952 \endhtmlonly) to rotate.
			 * \param [in] m11 y scale factor, or cos(\htmlonly&#952 \endhtmlonly) to rotate.
			 *
			 * \note
			 * m00 - m11 are in Transform units i.e. 16.16 fixed point.<br>
			 * m20, m21 are set to zero, i.e. no translation.<br>
			 * Values are used as is, no conversion is performed. */
			public Matrix (int m00, int m01, int m10, int m11)
			{
				m = new int [3, 3];

				Set (m00, m01, m10, m11, 0, 0);
			}

			/*! \brief Create matrix with x,y translation - rotate and scale elements set to identity.
			 * \param [in] m20 x translation.
			 * \param [in] m21 y translation.
			 *
			 * \note
			 * m00 - m11 are set to identity.<br>
			 * m20, m21 are in internal Draw units i.e. 24.8 fixed point.<br>
			 * Values are used as is, no conversion is performed. */
			public Matrix (int m20, int m21)
			{
				m = new int [3, 3];

				Set (ToTransformUnits (1.0), 0, 0, ToTransformUnits (1.0), m20, m21);
			}

			/*! \brief Create matrix from existing 3x3 array. */
			public Matrix (int [,] matrix_array)
			{
				m = (int [,])matrix_array.Clone();
			}

			/*! \brief Copy a matrix. */
			public Object Clone()
			{
				return new Matrix (m);
			}

			/*! \brief Set the individual elements of an existing matrix.
			 * \param [in] m00 x scale factor, or cos(\htmlonly&#952 \endhtmlonly) to rotate.
			 * \param [in] m01 sin(\htmlonly&#952 \endhtmlonly) to rotate.
			 * \param [in] m10 -sin(\htmlonly&#952 \endhtmlonly) to rotate.
			 * \param [in] m11 y scale factor, or cos(\htmlonly&#952 \endhtmlonly) to rotate.
			 * \param [in] m20 x translation.
			 * \param [in] m21 y translation.
			 *
			 * \note
			 * m00 - m11 are in Transform units i.e. 16.16 fixed point.<br>
			 * m20, m21 are in internal Draw units i.e. 24.8 fixed point.<br>
			 * Values are used as is, no conversion is performed. */
			public void Set (int m00, int m01, int m10, int m11, int m20, int m21)
			{
				m[0,0] = m00;
				m[0,1] = m01;
				m[1,0] = m10;
				m[1,1] = m11;
				m[2,0] = m20;
				m[2,1] = m21;
			}

			/*! \brief Multiply the matrix by another one supplied.
			 * \param [in] m2 Matrix to multiply with.
			 * \return Nothing. */
			public void Multiply (Matrix m2)
			{
				Matrix m1 = (Matrix)Clone ();
				int sum = 0, i, j, k;

				for (i = 0; i < 3; i++) {
					for (j = 0; j < 3; j++) {
						for (k = 0; k < 3; k++) {
							sum += (int)(Math.BigMul(m1.m[i,k], m2.m[k,j]) >> 16);
						}
						m[i,j] = sum;
						sum = 0;
					}
				}
			}

			/*! \brief Add a translation to the matrix.
			 * \param [in] x Number of OS units to translate by on the X axis.
			 * \param [in] y Number of OS units to translate by on the Y axis.
			 * \return Nothing.
			 *
			 * \note
			 * Parameters are converted into internal Draw units. */
			public void Translate (int x, int y)
			{
				m[2, 0] += ToDrawUnits (x);
				m[2, 1] += ToDrawUnits (y);
			}

			/*! \brief Add a scale to the matrix.
			 * \param [in] x Scale factor to apply to the X axis.
			 * \param [in] y Scale factor to apply to the Y axis.
			 * \return Nothing.
			 *
			 * \note
			 * Parameters are converted into transform units. */
			public void Scale (double x, double y)
			{
				m[0, 0] += ToTransformUnits (x);
				m[1, 1] += ToTransformUnits (y);
			}

			/*! \brief Rotate the matrix.
			 * \param [in] angle_degrees Number of degrees with which to rotate by.
			 * \return Nothing. */
			public void Rotate (double angle_degrees)
			{
				double rads = DegreeToRadian (angle_degrees % 360);
				Matrix matrix = new Matrix (ToTransformUnits (Math.Cos (rads)),
							    ToTransformUnits (Math.Sin (rads)),
							    ToTransformUnits (-Math.Sin (rads)),
							    ToTransformUnits (Math.Cos (rads)));
				Multiply (matrix);
			}
		}

		public class DateTime
		{
			public byte [] Utc = new byte [5];

			public DateTime ()
			{
			}

			public void GetNow ()
			{
				Utc[0] = 3;
				NativeMethods.OSWord_ReadClockUtc (Utc);
			}

			public static DateTime Now
			{
				get
				{
					DateTime date = new DateTime ();
					date.GetNow ();
					return date;
				}
			}
		}

		/*! \brief Convert an angle in degrees into radians.
		 * \param [in] angle Angle in degrees.
		 * \return Angle in radians. */
		public static double DegreeToRadian (double angle)
		{
			return Math.PI * angle / 180.0;
		}

		/*! \brief Convert a value into transform units where 1 is equal to &10000 transform units.
		 * \param value Value to convert.
		 * \return Converted value. */
		public static int ToTransformUnits (double value)
		{
			return (int)(value * (double)0x10000);
		}

		/*! \brief Convert a value into internal Draw units where 1 is equal to 256 transform units.
		 * \param value Value to convert.
		 * \return Converted value. */
		public static int ToDrawUnits (double value)
		{
			return (int)(value * 256);
		}

		/*! \brief Straight through to SWI OS_Plot. */
		public static void Plot (int type, int x, int y)
		{
			NativeMethods.OS_Plot (type, x, y);
		}

		/*! \brief Equivalent to MOVE in BASIC. */
		public static void Move (int x, int y)
		{
			Plot (PlotType.SolidLine + PlotModifier.MoveAbsolute, x, y);
		}

		/*! \brief Equivalent to DRAW in BASIC. */
		public static void Draw (int x, int y)
		{
			Plot (PlotType.SolidLine + PlotModifier.PlotAbsoluteFG, x, y);
		}

		/*! \brief Equivalent to MOVE BY in BASIC. */
		public static void MoveBy (int x, int y)
		{
			Plot (PlotType.SolidLine + PlotModifier.MoveRelative, x, y);
		}

		/*! \brief Equivalent to DRAW BY in BASIC. */
		public static void DrawBy (int x, int y)
		{
			Plot (PlotType.SolidLine + PlotModifier.PlotRelativeFG, x, y);
		}

		/*! \brief Plot a straight line between the given coordinates. */
		public static void PlotLine (int x1, int y1, int x2, int y2)
		{
			Move (x1, y1);
			Draw (x2, y2);
		}

		/*! \brief Plot a straight line between the given coordinates. */
		public static void PlotLine (Coord c1, Coord c2)
		{
			Move (c1.X, c1.Y);
			Draw (c2.X, c2.Y);
		}

		public static void PlotCircleOutline (int centre_x, int centre_y, int radius)
		{
			Move (centre_x, centre_y);
			Plot (PlotType.CircleOutline + PlotModifier.PlotRelativeFG, radius, 0);
		}

		public static void PlotCircleFill (int centre_x, int centre_y, int radius)
		{
			Move (centre_x, centre_y);
			Plot (PlotType.CircleFill + PlotModifier.PlotRelativeFG, radius, 0);
		}

		public static void PlotRectangleFill (Coord c1, Coord c2)
		{
			Move (c1.X, c1.Y);
			Plot (PlotType.RectangleFill + PlotModifier.PlotAbsoluteFG, c2.X, c2.Y);
		}

		public static void PlotRectangleFill (Rect rect)
		{
			Move (rect.MinX, rect.MinY);
			Plot (PlotType.RectangleFill + PlotModifier.PlotAbsoluteFG, rect.MaxX, rect.MaxY);
		}

		/*! \brief Return the number of centi-seconds since the last hard reset.  */
		public static uint ReadMonotonicTime ()
		{
			uint time;

			NativeMethods.OS_ReadMonotonicTime (out time);

			return time;
		}
	}
}

