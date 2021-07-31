//
// NativeMethods/colourtrans.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Runtime.InteropServices;

namespace riscos
{
	internal static partial class NativeMethods
	{
		// ColourTrans SWIs
		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_return_gcol")]
		internal static extern void ColourTrans_ReturnGCOL (uint paletteEntry,
								    out int gcolOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_set_gcol")]
		internal static extern void ColourTrans_SetGCOL (uint paletteEntry,
								 uint flags,
								 OS.GCOLAction gcolAction,
								 out int gcolOut,
								 out int log2BppOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_return_colour_number")]
		internal static extern void ColourTrans_ReturnColourNumber (uint paletteEntry,
									    out int colourNumberOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_return_gcol_for_mode")]
		internal static extern void ColourTrans_ReturnGCOLForMode (uint paletteEntry,
									   int destMode,
									   IntPtr destPalette,
									   out int gcolOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_return_colour_number_for_mode")]
		internal static extern void ColourTrans_ReturnColourNumberForMode (uint paletteEntry,
										   int destMode,
										   IntPtr destPalette,
										   out int colourNumberOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_return_opp_gcol")]
		internal static extern void ColourTrans_ReturnOppGCOL (uint paletteEntry,
								       out int gcolOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_set_opp_gcol")]
		internal static extern void ColourTrans_SetOppGCOL (uint paletteEntry,
								    uint flags,
								    OS.GCOLAction gcolAction,
								    out int gcolOut,
								    out int log2BppOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_return_opp_colour_number")]
		internal static extern void ColourTrans_ReturnOppColourNumber (uint paletteEntry,
									       out int colourNumberOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_return_opp_gcol_for_mode")]
		internal static extern void ColourTrans_ReturnOppGCOLForMode (uint paletteEntry,
									      int destMode,
									      IntPtr destPalette,
									      out int gcolOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_return_opp_colour_number_for_mode")]
		internal static extern void ColourTrans_ReturnOppColourNumberForMode (uint paletteEntry,
										      int destMode,
										      IntPtr destPalette,
										      out int colourNumberOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_gcol_to_colour_number")]
		internal static extern void ColourTrans_GCOLToColourNumber (int gcol,
									    out int colourNumberOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_colour_number_to_gcol")]
		internal static extern void ColourTrans_ColourNumberToGCOL (int colourNumber,
									    out int gcolOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_return_font_colours")]
		internal static extern void ColourTrans_ReturnFontColours (IntPtr font,
									   uint bgPaletteEntry,
									   uint fgPaletteEntry,
									   int offset,
									   out uint bgLogicalColOut,
									   out uint fgLogicalColOut,
									   out int offsetOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_set_font_colours")]
		internal static extern void ColourTrans_SetFontColours (IntPtr font,
									uint bgPaletteEntry,
									uint fgPaletteEntry,
									int offset,
									out uint bgLogicalColOut,
									out uint fgLogicalColOut,
									out int offsetOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_invalidate_cache")]
		internal static extern void ColourTrans_InvalidateCache ();

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_set_calibration")]
		internal static extern IntPtr ColourTrans_SetCalibration (IntPtr buffer);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_read_calibration")]
		internal static extern IntPtr ColourTrans_ReadCalibration (IntPtr buffer,
									   out int bufferSizeOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_convert_device_colour")]
		internal static extern IntPtr ColourTrans_ConvertDeviceColour (uint paletteEntry,
									       IntPtr calibrationTable,
									       out uint colourOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_convert_device_palette")]
		internal static extern IntPtr ColourTrans_ConvertDevicePalette (int colourCount,
									        IntPtr deviceColourTable,
									        IntPtr colourBuffer,
									        IntPtr calibrationTable);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_convert_rgb_to_cie")]
		internal static extern void ColourTrans_ConvertRGBToCIE (uint r, uint g, uint b,
									 out uint x, out uint y, out uint z);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_convert_cie_to_rgb")]
		internal static extern void ColourTrans_ConvertCIEToRGB (uint x, uint y, uint z,
									 out uint r, out uint g, out uint b);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_write_calibration_to_file")]
		internal static extern IntPtr ColourTrans_WriteCalibrationToFile (int flags,
										  IntPtr fileHandle);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_convert_rgb_to_hsv")]
		internal static extern void ColourTrans_ConvertRGBToHSV (uint r, uint g, uint b,
									 out uint h, out uint s, out uint v);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_convert_hsv_to_rgb")]
		internal static extern IntPtr ColourTrans_ConvertHSVToRGB (uint h, uint s, uint v,
									   out uint r, out uint g, out uint b);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_convert_rgb_to_cmyk")]
		internal static extern void ColourTrans_ConvertRGBToCMYK (uint r, uint g, uint b,
									  out uint c, out uint m, out uint y, out uint k);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_convert_cmyk_to_rgb")]
		internal static extern void ColourTrans_ConvertCMYKToRGB (uint c, uint m, uint y, uint k,
									  out uint r, out uint g, out uint b);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_set_colour")]
		internal static extern void ColourTrans_SetColour (int gcol,
								   int flags,
								   OS.GCOLAction gcolAction);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_set_text_colour")]
		internal static extern IntPtr ColourTrans_SetTextColour (uint paletteEntry,
									 int flags,
									 out int gcolOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_set_opp_text_colour")]
		internal static extern IntPtr ColourTrans_SetOppTextColour (uint paletteEntry,
									    int flags,
									    out int gcolOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_generate_table_for_sprite")]
		internal static extern IntPtr ColourTrans_GenerateTableForSprite (IntPtr spriteArea,
										  IntPtr spritePointer,
										  int destMode,
										  IntPtr destPalette,
										  IntPtr buffer,
										  ColourTrans.TableFlags flags,
										  IntPtr workspace,
										  IntPtr transFunc,
										  out int size);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xcolourtrans_generate_table_for_sprite")]
		internal static extern IntPtr ColourTrans_GenerateTableForSprite (IntPtr spriteArea,
										  string spriteName,
										  int destMode,
										  IntPtr destPalette,
										  IntPtr buffer,
										  ColourTrans.TableFlags flags,
										  IntPtr workspace,
										  IntPtr transFunc,
										  out int size);
	}
}
