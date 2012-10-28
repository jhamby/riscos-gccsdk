//
// sprite.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

namespace riscos
{
	public static class OSSpriteOp
	{
		public enum AreaType
		{
			System = 0,
			UserWithName = 256,
			UserWithPointer = 512
		}

		/*! \brief Specifies the way in which pixels are plotted onto the screen.  */
		public enum PlotAction
		{
			/*! \brief Overwrite colour on screen with sprite pixel colour.  */
			OverWrite,
			/*! \brief OR colour on screen with sprite pixel colour.  */
			OR,
			/*! \brief And colour on screen with sprite pixel colour.  */
			AND,
			/*! \brief Exclusive OR colour on screen with sprite pixel colour.  */
			EOR,
			/*! \brief Invert colour on screen.  */
			Invert,
			/*! \brief Leave colour on screen unchanged.  */
			Leave,
			/*! \brief AND colour on screen with NOT of sprite pixel colour.  */
			ANDNOT,
			/*! \brief OR colour on screen with NOT of sprite pixel colour.  */
			ORNOT,
			/*! \brief If set, use mask when plotting.  */
			WithMask
		}

		public enum TransformFlags
		{
			DestinationCoords = (1 << 0),
			SourceRectangle = (1 << 1)
		}
	}
}
