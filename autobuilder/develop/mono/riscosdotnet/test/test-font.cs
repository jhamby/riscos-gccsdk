/*
 * Test non graphical aspects of the font C# bindings.
 *
 * Author:
 *	Lee Noar <leenoar@sky.com>
 */

using System;
using System.Text;
using riscos;

public class FontTest
{
	static Font.Instance open_non_existent_font ()
	{
		Font.Instance myfont = null;

		try
		{
			Console.WriteLine ("Deliberately opening non-existent font.");

			// Deliberate typo. Test failure to find font. 
			myfont = new Font.Instance ("Trinity.edium", 16 << 4, 16 << 4);

			Console.WriteLine ("Test failed. Exception was not thrown.\n");
		}
		catch (OS.ErrorException ex)
		{
			Console.WriteLine ("Test passed. Exception was thrown, error details are:");
			Console.WriteLine ("error number = {0}, error string = {1}\n",
					   ex.OSError.ErrNum,
					   ex.OSError.ErrMess);
		}

		return myfont;
	}

	static void call_cache_addr ()
	{
		int cache_size, cache_used;
		double version;

		Console.WriteLine ("Calling SWI \"XFont_CacheAddr\"");
		version = Font.ReadCacheAddr (out cache_size, out cache_used);
		Console.WriteLine ("Version: {0:N}, Cache Size: {1} bytes, Cache Used: {2} bytes\n",
				   version,
				   cache_size,
				   cache_used);
	}

	static void read_full_def (Font.Instance font)
	{
		OS.Coord size, res;
		int age, usage_count;
		string ID;

		size = new OS.Coord ();
		res = new OS.Coord ();

		Console.WriteLine ("Calling SWI \"XFont_ReadDefn\" to read full definition of font:");
		ID = font.ReadFullDefn (size, res, out age, out usage_count);
		Console.WriteLine ("Result is:");
		Console.WriteLine ("Identifier \"{0}\"",ID);
		Console.WriteLine ("Size: {0}x{1} points", size.X >> 4, size.Y >> 4);
		Console.WriteLine ("Resolution: {0}x{1} dpi", res.X, res.Y);
		Console.WriteLine ("Age: {0}", age);
		Console.WriteLine ("Usage count: {0}\n", usage_count);
	}

	static void read_info (Font.Instance font)
	{
		OS.Rect bbox;

		Console.WriteLine ("Calling SWI \"XFont_ReadInfo\" to read font bounding box:");
		bbox = font.ReadInfo ();
		Console.WriteLine ("Result is:");
		Console.WriteLine ("Minimum X {0}", bbox.MinX);
		Console.WriteLine ("Minimum Y {0}", bbox.MinY);
		Console.WriteLine ("Maximum X {0}", bbox.MaxX);
		Console.WriteLine ("Maximum Y {0}\n", bbox.MaxY);
	}

	static void string_width (Font.Instance font)
	{
		OS.Coord result_offset = new OS.Coord ();
		int result_index, result_count;

		Console.WriteLine ("Calling SWI \"Font_SetFont\"");
		Font.SetFont (font);

		string string_to_split = "The quick brown fox jumped over the lazy dog.";
		Console.WriteLine ("Calling SWI \"Font_StringWidth\" to split a string");
		Console.WriteLine ("String to split is \"{0}\"", string_to_split);
		Console.WriteLine ("Splitting on a space character at 300 OS units.");
		result_index = Font.StringWidth (string_to_split,
						 Font.ConvertToPoints (300, 0),
						 ' ', // Split on a space
						 string_to_split.Length, // Consider all the string
						 result_offset,
						 out result_count);
		Font.ConvertToOS (result_offset, result_offset);
		Console.WriteLine ("Result is:");
		Console.WriteLine ("Split at offset {0},{1} OS units", result_offset.X, result_offset.Y);
		Console.WriteLine ("Number of split characters is {0}", result_count);
		Console.WriteLine ("Split was at index {0} between characters '{1}' and '{2}'\n",
				   result_index,
				   string_to_split[result_index - 1],
				   string_to_split[result_index + 1]);
	}

	static void char_bbox (Font.Instance font)
	{
		OS.Rect bbox;

		Console.WriteLine ("Calling SWI \"Font_CharBBox\" to find bounding box of character 'A'");
		bbox = font.CharBBox ('A', Font.PlotType.OSUnits); // return OS units
		Console.WriteLine ("Result is:");
		Console.WriteLine ("Minimum X {0}", bbox.MinX);
		Console.WriteLine ("Minimum Y {0}", bbox.MinY);
		Console.WriteLine ("Maximum X {0}", bbox.MaxX);
		Console.WriteLine ("Maximum Y {0}\n", bbox.MaxY);
	}

	static void scan_string (Font.Instance font)
	{
		OS.Coord result_offset = new OS.Coord ();
		int result_index, result_count;

		string string_to_scan = "The quick brown fox jumped over the lazy dog.";

		Console.WriteLine ("Calling SWI \"Font_ScanString\" to find caret position using no matrix or coordinate block");
		Console.WriteLine ("String to scan is \"{0}\"", string_to_scan);
		Console.WriteLine ("Mouse position is 300, 10 OS units.");
		result_index = font.ScanString (string_to_scan,
						Font.PlotType.GivenLength |
						Font.PlotType.ReturnSplitCount |
						Font.PlotType.ReturnCaretPos,
						Font.ConvertToPoints (300, 10),
						string_to_scan.Length, // Consider all the string
						result_offset,
						out result_count);
		Font.ConvertToOS (result_offset, result_offset);
		Console.WriteLine ("Result is:");
		Console.WriteLine ("Caret is at offset {0},{1} OS units", result_offset.X, result_offset.Y);
		Console.WriteLine ("Caret is at index {0}, character '{1}'\n",
				   result_index,
				   string_to_scan[result_index]);

		Console.WriteLine ("Calling SWI \"Font_ScanString\" to find string bounding box using no matrix.");
		Console.WriteLine ("Coordinate block used for returned results.");
		Font.ScanCoordBlock coord_block = new Font.ScanCoordBlock ();
		result_index = font.ScanString (string_to_scan,
						Font.PlotType.GivenLength |
						Font.PlotType.ReturnBBox,
						new OS.Coord (-1, -1),
						coord_block,
						string_to_scan.Length, // Consider all the string
						result_offset,
						out result_count);
		Console.WriteLine ("Bounding box (in millipoints) is:");
		Console.WriteLine ("Minimum X {0}", coord_block.BoundingBox.MinX);
		Console.WriteLine ("Minimum Y {0}", coord_block.BoundingBox.MinY);
		Console.WriteLine ("Maximum X {0}", coord_block.BoundingBox.MaxX);
		Console.WriteLine ("Maximum Y {0}\n", coord_block.BoundingBox.MaxY);
	}

	static void list_fonts ()
	{
		Console.WriteLine ("Calling SWI \"Font_ListFonts\"");

		StringBuilder font_id, font_name;
		int context = Font.ListFonts (out font_id, out font_name, (1 << 16) | (1 <<17));
		while (context != -1)
		{
			Console.WriteLine ("{0}, {1}", font_id, font_name);
			context = Font.ListFonts (out font_id, out font_name, context);
		}
	}

	public static void Main (string[] args)
	{
		Font.Instance myfont;

		try
		{
			call_cache_addr ();

			open_non_existent_font ();

			Console.WriteLine ("Calling SWI \"XFont_FindFont\" for Trinity.Medium.Italic.");
			myfont = new Font.Instance ("Trinity.Medium.Italic");
			Console.WriteLine ("Font was found and given the handle {0}\n", myfont.Handle);

			Console.WriteLine ("Calling SWI \"XFont_ReadDefn\" to read back font identifier:");
			Console.WriteLine ("Result is \"{0}\"\n", myfont.ReadIdentifier ());

			read_full_def (myfont);

			read_info (myfont);

			string_width (myfont);

			char_bbox (myfont);

			scan_string (myfont);

			list_fonts ();
		}
		catch (OS.ErrorException ex)
		{
			Console.WriteLine ("error number = {0}, error string = {1}",
					   ex.OSError.ErrNum,
					   ex.OSError.ErrMess);
		}

	}
}
