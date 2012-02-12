//
// colourtrans.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
namespace riscos
{
	public static class ColourTrans
	{
		public const uint FullRed = 0x0000ff00U;
		public const uint FullGreen = 0x00ff0000U;
		public const uint FullBlue = 0xff000000U;

		public static void SetGCOL (OS.GCOLAction gcol_action,
					    uint palette_entry)
		{
			NativeMethods.ColourTrans_SetGCOL (gcol_action,
							   palette_entry);
		}
	}
}
