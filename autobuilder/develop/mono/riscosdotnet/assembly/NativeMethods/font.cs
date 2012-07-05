//
// NativeMethods/font.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Runtime.InteropServices;
using System.Text;

namespace riscos
{
	public static class NativeFont
	{
		[StructLayout(LayoutKind.Sequential)]
		public struct ScanCoordBlock
		{
			public NativeOS.Coord Space;
			public NativeOS.Coord Letter;
			public int SplitChar;
			public NativeOS.Rect BoundingBox;

			public ScanCoordBlock (Font.ScanCoordBlock block)
			{
				Space = new NativeOS.Coord (block.Space);
				Letter = new NativeOS.Coord (block.Letter);
				SplitChar = block.SplitChar;
				BoundingBox = new NativeOS.Rect (block.BoundingBox);
			}
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct PaintCoordBlock
		{
			public NativeOS.Coord Space;
			public NativeOS.Coord Letter;
			public NativeOS.Rect RubOut;

			public PaintCoordBlock (Font.PaintCoordBlock block)
			{
				Space = new NativeOS.Coord (block.Space);
				Letter = new NativeOS.Coord (block.Letter);
				RubOut = new NativeOS.Rect (block.RubOut);
			}
		}
	}

	static partial class NativeMethods
	{
		// Font SWIs
		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_cache_addr")]
		internal static extern IntPtr Font_CacheAddr (out int version,
							      out int cacheSize,
							      out int cacheUsed);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_find_font")]
		internal static extern IntPtr Font_FindFont (string fontName,
							    int xSize,
							    int ySize,
							    int xRes,
							    int yRes,
							    out IntPtr font,
							    out int xResOut,
							    out int yResOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_lose_font")]
		internal static extern IntPtr Font_LoseFont (IntPtr font);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_read_full_defn")]
		internal static extern IntPtr Font_ReadFullDefn (IntPtr font,
								 StringBuilder fontID,
								 out int xSize,
								 out int ySize,
								 out int xRes,
								 out int yRes,
								 out int age,
								 out int usageCount);

		// Allows a buffer to be passed in the 2nd parameter.
		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_read_identifier")]
		internal static extern IntPtr Font_ReadIdentifier (IntPtr font,
								   StringBuilder fontID,
								   out int used);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_read_info")]
		internal static extern IntPtr Font_ReadInfo (IntPtr font,
							     out int x0,
							     out int y0,
							     out int x1,
							     out int y1);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_string_width")]
		internal static extern IntPtr Font_StringWidth (string s,
								int xmax,
								int ymax,
								int split,
								int length,
								// The returned splitPoint address will be in the unmanaged
								// buffer, which is useless here, so ignore it - we have the
								// index below.
								IntPtr splitPoint,
								out int x,
								out int y,
								out int lengthOut,
								out int indexOut);

		// SWI "XFont_Paint" with coord block and transformation matrix
		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_paint")]
		internal static extern IntPtr Font_Paint (IntPtr font,
							  string s,
							  Font.PlotType flags,
							  int xPos,
							  int yPos,
							  [In, MarshalAs(UnmanagedType.Struct)]
							       ref NativeFont.PaintCoordBlock CoordBlock,
							  [In, MarshalAs(UnmanagedType.Struct)]
							       ref NativeOS.Matrix matrix,
							  int length);

		// SWI "XFont_Paint" with transformation matrix, no coord block
		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_paint")]
		internal static extern IntPtr Font_Paint (IntPtr font,
							  string s,
							  Font.PlotType flags,
							  int xPos,
							  int yPos,
							  IntPtr CoordBlock,
							  [In, MarshalAs(UnmanagedType.Struct)]
							       ref NativeOS.Matrix matrix,
							  int length);

		// SWI "XFont_Paint" with coord block, no transformation matrix
		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_paint")]
		internal static extern IntPtr Font_Paint (IntPtr font,
							  string s,
							  Font.PlotType flags,
							  int xPos,
							  int yPos,
							  [In, MarshalAs(UnmanagedType.Struct)]
							       ref NativeFont.PaintCoordBlock CoordBlock,
							  IntPtr matrix,
							  int length);

		// SWI "XFont_Paint" with no transformation matrix, no coord block
		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_paint")]
		internal static extern IntPtr Font_Paint (IntPtr font,
							  string s,
							  Font.PlotType flags,
							  int xPos,
							  int yPos,
							  IntPtr CoordBlock,
							  IntPtr matrix,
							  int length);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_caret")]
		internal static extern IntPtr Font_Caret (uint colourNumber,
							  int height,
							  uint unitFlags,
							  int x,
							  int y);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_convertto_os")]
		internal static extern IntPtr Font_ConvertToOS (int y, int y, out int xOut, out int yOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_converttopoints")]
		internal static extern IntPtr Font_ConvertToPoints (int y, int y, out int xOut, out int yOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_set_font")]
		internal static extern IntPtr Font_SetFont (IntPtr font);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_current_font")]
		internal static extern IntPtr Font_CurrentFont (out IntPtr font,
								out uint bgHint,
								out uint fill,
								out int offset);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_future_font")]
		internal static extern IntPtr Font_FutureFont (out IntPtr font,
							       out uint bgHint,
							       out uint fill,
							       out int offset);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_find_caret")]
		internal static extern IntPtr Font_FindCaret (string s,
							      int x,
							      int y,
							      // FIXME: Difficult to return split point when using a string type
							      IntPtr splitPoint,
							      out int xOut,
							      out int yOut,
							      out int lengthOut,
							      out int indexOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_char_bbox")]
		internal static extern IntPtr Font_CharBBox (IntPtr font,
							     int c,
							     Font.PlotType unitFlags,
							     out int x0,
							     out int y0,
							     out int x1,
							     out int y1);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_read_scale_factor")]
		internal static extern IntPtr Font_ReadScaleFactor (out int xFactor, out int yFactor);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_set_scale_factor")]
		internal static extern IntPtr Font_SetScaleFactor (int xFactor, int yFactor);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_list_fonts")]
		internal static extern IntPtr Font_ListFonts (IntPtr buffer1,
							      int context,
							      int size1,
							      IntPtr buffer2,
							      int size2,
							      IntPtr tickFont,
							      out int contextOut,
							      out int buffer1SizeOut,
							      out int buffer2SizeOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_list_fonts")]
		internal static extern IntPtr Font_ListFonts (StringBuilder buffer1,
							      int context,
							      int size1,
							      StringBuilder buffer2,
							      int size2,
							      IntPtr tickFont,
							      out int contextOut,
							      out int buffer1SizeOut,
							      out int buffer2SizeOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_set_font_colours")]
		internal static extern IntPtr Font_SetFontColours (IntPtr font,
								   uint bgHint,
								   uint fill,
								   int offset);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_set_palette")]
		internal static extern IntPtr Font_SetPalette (uint bgHint,
							       uint fill, 
							       int offset,
							       uint bgHintOut,
							       uint fillOut);


		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_set_true_palette")]
		internal static extern IntPtr Font_SetTruePalette (uint bgHint,
								   uint fill,
								   int offset,
								   uint bgHintOut,
								   uint fillOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_read_thresholds")]
		internal static extern IntPtr Font_ReadThresholds (byte [] buffer);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_set_thresholds")]
		internal static extern IntPtr Font_SetThresholds (byte [] buffer);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_find_caretj")]
		internal static extern IntPtr Font_FindCaretj (string s,
							       int x,
							       int y,
							       int xJust,
							       int yJust,
							       // FIXME: Difficult to return split point when using a string type
							       IntPtr splitPoint,
							       out int xOut,
							       out int yOut,
							       out int lengthOut,
							       out int indexOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_string_bbox")]
		internal static extern IntPtr Font_StringBBox (string s,
							       out int x0,
							       out int y0,
							       out int x1,
							       out int y1);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_read_colour_table")]
		internal static extern IntPtr Font_ReadColourTable (byte [] colourTable); // 16 byte buffer

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_make_bitmap")]
		internal static extern IntPtr Font_MakeBitmap (string fontName,
							       int xsize,
							       int ysize,
							       int xres,
							       int yres,
							       uint flags);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_un_cache_file")]
		internal static extern IntPtr Font_UncacheFile (string fileName, bool recache);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_set_font_max")]
		internal static extern IntPtr Font_SetFontMax (int fontMax,
							       int fontMax1,
							       int fontMax2,
							       int fontMax3,
							       int fontMax4,
							       int fontMax5);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_read_font_max")]
		internal static extern IntPtr Font_ReadFontMax (out int fontMax,
							      out int fontMax1,
							      out int fontMax2,
							      out int fontMax3,
							      out int fontMax4,
							      out int fontMax5);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_read_font_prefix")]
		internal static extern IntPtr Font_ReadFontPrefix (IntPtr font,
								   StringBuilder buffer,
								   int size,
								   // For now, set to zero and ignore
								   IntPtr end);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_switch_output_to_buffer")]
		internal static extern IntPtr Font_SwitchOutputToBuffer (uint flags,
									 byte [] buffer,
									 // For now, set to zero and ignore
									 IntPtr end);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_switch_output_to_buffer_flags")]
		internal static extern IntPtr Font_SwitchOutputToBufferFlags (out uint flags);

		// SWI "XFont_ScanString" with coordinate block and transformation matrix
		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Font_ScanString")]
		internal static extern IntPtr Font_ScanString (IntPtr font,
							       string s,
							       Font.PlotType flags,
							       int x,
							       int y,
							       [In,Out, MarshalAs(UnmanagedType.Struct)]
								    ref NativeFont.ScanCoordBlock coordBlock,
							       [In,Out, MarshalAs(UnmanagedType.Struct)]
								    ref NativeOS.Matrix matrix,
							       int length,
							       out int splitIndexOut,
							       out int xOut,
							       out int yOut,
							       out int splitCountOut);

		// SWI "XFont_ScanString" with coordinate block, but no transformation matrix
		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Font_ScanString")]
		internal static extern IntPtr Font_ScanString (IntPtr font,
							       string s,
							       Font.PlotType flags,
							       int x,
							       int y,
							       [In,Out, MarshalAs(UnmanagedType.Struct)]
								    ref NativeFont.ScanCoordBlock block,
							       IntPtr matrix,
							       int length,
							       out int splitIndexOut,
							       out int xOut,
							       out int yOut,
							       out int splitCountOut);

		// SWI "XFont_ScanString" with transformation matrix, but no coordinate block
		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Font_ScanString")]
		internal static extern IntPtr Font_ScanString (IntPtr font,
							       string s,
							       Font.PlotType flags,
							       int x,
							       int y,
							       IntPtr coordBlock,
							       [In,Out, MarshalAs(UnmanagedType.Struct)]
								    ref NativeOS.Matrix matrix,
							       int length,
							       out int splitIndexOut,
							       out int xOut,
							       out int yOut,
							       out int splitCountOut);

		// SWI "XFont_ScanString" with no coordinate block or transformation matrix
		[DllImport("libriscosdotnet.so.1", EntryPoint="rdn_Font_ScanString")]
		internal static extern IntPtr Font_ScanString (IntPtr font,
							       string s,
							       Font.PlotType flags,
							       int x,
							       int y,
							       IntPtr coordBlock,
							       IntPtr matrix,
							       int length,
							       out int splitIndexOut,
							       out int xOut,
							       out int yOut,
							       out int splitCountOut);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_current_rgb")]
		internal static extern IntPtr Font_CurrentRGB (IntPtr font,
							       out uint bgHint,
							       out uint fill,
							       out int offset);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_future_rgb")]
		internal static extern IntPtr Font_FutureRGB (IntPtr font,
							      out uint bgHint,
							      out uint fill,
							      out int offset);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_read_encoding_filename")]
		internal static extern IntPtr Font_ReadEncodingFilename (IntPtr font,
									 StringBuilder buffer,
									 int size,
									 // For now, set to zero and ignore
									 IntPtr end);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_find_field")]
		internal static extern IntPtr Font_FindField (string identifier,
							      int qualifier,
							      // FIXME: This may have to be an index rather than a pointer.
							      IntPtr field,
							      out int found);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_apply_fields")]
		internal static extern IntPtr Font_ApplyFields (string identifier,
								string fields,
								StringBuilder buffer,
								int size,
								out int spare);

		// Pass IntPtr.Zero to buffer to return required buffer size
		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_apply_fields")]
		internal static extern IntPtr Font_ApplyFields (string identifier,
								string fields,
								IntPtr buffer,
								int size,
								out int spare);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_lookup_font")]
		internal static extern IntPtr Font_LookupFont (IntPtr font,
							       out uint flags);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xfont_enumerate_characters")]
		internal static extern IntPtr Font_LookupFont (IntPtr font,
							       int character,
							       out int next_character,
							       out int internal_character_code);

	}
}
