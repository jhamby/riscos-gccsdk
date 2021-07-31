//
// colourtrans.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Runtime.InteropServices;

namespace riscos
{
	public static class ColourTrans
	{
		public const uint FullRed = 0x0000ff00U;
		public const int RedShift = 8;
		public const uint FullGreen = 0x00ff0000U;
		public const int GreenShift = 16;
		public const uint FullBlue = 0xff000000U;
		public const int BlueShift = 24;
		public const uint Black = 0U;
		public const uint White = 0xffffff00U;

		/*! \brief A constant value that can be used by some %ColourTrans methods to indicate
		 * the current display mode.  */
		public const int CurrentMode = -1;
		/*! \brief A constant value that can be used by some %ColourTrans methods to indicate
		 * the default display mode.  */
		public const int DefaultMode = 0;

		// Can't make IntPtrs const, so use static properties to create named literals.
		/*! \brief A constant value that can be used by some %ColourTrans methods to indicate
		 * the current colour palette.  */
		public static IntPtr CurrentPalette
		{
			get { return (IntPtr) (-1); }
		}

		/*! \brief A constant value that can be used by some %ColourTrans methods to indicate
		 * the default colour palette.  */
		public static IntPtr DefaultPalette
		{
			get { return IntPtr.Zero; }
		}

		/*! \brief The flags that can be passed to ColourTrans.GenerateTableForSprite.  */
		public enum TableFlags
		{
			/*! \brief Bit 0 => set pointer to sprite given; else pointer to sprite name.  */
			SpritePointerGiven = (1 << 0),
			/*! \brief Bit 1 => set use current palette if sprite doesn't have one; else use default.  */
			UseCurrentPalette = (1 << 1),
			/*! \brief Bit 2 => set transfer function specified (not currently supported in this library).  */
			UseTransferFunction = (1 << 2),
			/*! \brief Bits 24 - 31 give format of returned table.  */
			ReturnPaletteTable = (1 << 24)
		}

		//! \brief Returns the closest GCOL for the given palette entry (in the form 0xBBGGRR00).
		public static int ReturnGCOL (uint paletteEntry)
		{
			int gcol;

			NativeMethods.ColourTrans_ReturnGCOL (paletteEntry, out gcol);

			return gcol;
		}

		/*! \brief Sets and returns the closest GCOL for a palette entry.
		 * \param [in] paletteEntry A palette entry in the form 0xBBGGRR00.
		 * \param [in] gcolAction The GCOL action.
		 * \return The closest GCOL that was set.  */
		public static int SetGCOL (uint paletteEntry,
					   OS.GCOLAction gcolAction)
		{
			int gcol, log2bpp;

			NativeMethods.ColourTrans_SetGCOL (paletteEntry,
							   0,
							   gcolAction,
							   out gcol,
							   out log2bpp);
			return gcol;
		}

		//! \brief Return the closest colour for the given palette entry.
		public static int ReturnColourNumber (uint paletteEntry)
		{
			int colour;

			NativeMethods.ColourTrans_ReturnColourNumber (paletteEntry, out colour);

			return colour;
		}

		/*! \brief Returns the closest GCOL for a palette entry.
		 * \param [in] paletteEntry A palette entry in the form 0xBBGGRR00.
		 * \param [in] destMode Destination mode, or -1 for current mode.
		 * \param [in] destPalette Destination palette, or -1 for current palette,
		 * or 0 for default for the mode.
		 * \return The closest GCOL.  */
		public static int ReturnGCOLForMode (uint paletteEntry,
						     int destMode,
						     IntPtr destPalette)
		{
			int gcol;

			NativeMethods.ColourTrans_ReturnGCOLForMode (paletteEntry,
								     destMode,
								     destPalette,
								     out gcol);
			return gcol;
		}

		/*! \brief Returns the closest GCOL for a palette entry.
		 * \param [in] paletteEntry A palette entry in the form 0xBBGGRR00.
		 * \param [in] destMode Destination mode, or -1 for current mode.
		 * \param [in] destPalette Destination palette, or -1 for current palette,
		 * or 0 for default for the mode.
		 * \return The closest colour number.  */
		public static int ReturnColourNumberForMode (uint paletteEntry,
							     int destMode,
							     IntPtr destPalette)
		{
			int colour;

			NativeMethods.ColourTrans_ReturnColourNumberForMode (paletteEntry,
									     destMode,
									     destPalette,
									     out colour);
			return colour;
		}

		//! \brief Returns the furthest GCOL for the given palette entry (in the form 0xBBGGRR00).
		public static int ReturnOppGCOL (uint paletteEntry)
		{
			int gcol;

			NativeMethods.ColourTrans_ReturnOppGCOL (paletteEntry, out gcol);

			return gcol;
		}

		/*! \brief Sets and returns the furthest GCOL for a palette entry.
		 * \param [in] paletteEntry The GCOL action.
		 * \param [in] gcolAction A palette entry in the form 0xBBGGRR00.
		 * \return The furthest GCOL that was set.  */
		public static int SetOppGCOL (uint paletteEntry,
					      OS.GCOLAction gcolAction)
		{
			int gcol, log2bpp;

			NativeMethods.ColourTrans_SetOppGCOL (paletteEntry,
							      0,
							      gcolAction,
							      out gcol,
							      out log2bpp);
			return gcol;
		}

		//! \brief Return the closest colour for the given palette entry.
		public static int ReturnOppColourNumber (uint paletteEntry)
		{
			int colour;

			NativeMethods.ColourTrans_ReturnOppColourNumber (paletteEntry, out colour);

			return colour;
		}

		/*! \brief Returns the furthest GCOL for a palette entry.
		 * \param [in] paletteEntry A palette entry in the form 0xBBGGRR00.
		 * \param [in] destMode Destination mode, or -1 for current mode.
		 * \param [in] destPalette Destination palette, or -1 for current palette,
		 * or 0 for default for the mode.
		 * \return The furthest GCOL.  */
		public static int ReturnOppGCOLForMode (uint paletteEntry,
							int destMode,
							IntPtr destPalette)
		{
			int gcol;

			NativeMethods.ColourTrans_ReturnOppGCOLForMode (paletteEntry,
									destMode,
									destPalette,
									out gcol);
			return gcol;
		}

		/*! \brief Returns the furthest GCOL for a palette entry.
		 * \param [in] paletteEntry A palette entry in the form 0xBBGGRR00.
		 * \param [in] destMode Destination mode, or -1 for current mode.
		 * \param [in] destPalette Destination palette, or -1 for current palette,
		 * or 0 for default for the mode.
		 * \return The furthest colour number.  */
		public static int ReturnOppColourNumberForMode (uint paletteEntry,
								int destMode,
								IntPtr destPalette)
		{
			int colour;

			NativeMethods.ColourTrans_ReturnOppColourNumberForMode (paletteEntry,
										destMode,
										destPalette,
										out colour);
			return colour;
		}

		//! \brief Translate a GCOL to a colour number.
		public static int GCOLToColourNumber (int gcol)
		{
			int colour;

			NativeMethods.ColourTrans_GCOLToColourNumber (gcol, out colour);

			return colour;
		}

		//! \brief Translate a colour number to a GCOL.
		public static int ColourNumberToGCOL (int colour)
		{
			int gcol;

			NativeMethods.ColourTrans_ColourNumberToGCOL (colour, out gcol);

			return gcol;
		}

		/*! \brief Finds the best range of anti-alias colours to match a pair of palette entries.
		 * \param [in] fontHandle Font handle, or 0 for the current font.
		 * \param [in] bgPaletteEntry Background palette entry.
		 * \param [in] fgPaletteEntry Foreground palette entry.
		 * \param [in] maxOffset Maximum foreground colour offset (0 - 14).
		 * \param [out] bgLogicalColourOut The logical background colour.
		 * \param [out] fgLogicalColourOut The logical foreground colour.
		 * \param [out] maxOffsetOut The maximum sensible colour offset (up to maxOffset on entry).
		 * \return Nothing.  */
		public static void ReturnFontColours (IntPtr fontHandle,
						      uint bgPaletteEntry,
						      uint fgPaletteEntry,
						      int maxOffset,
						      out uint bgLogicalColourOut,
						      out uint fgLogicalColourOut,
						      out int maxOffsetOut)
		{
			NativeMethods.ColourTrans_ReturnFontColours (fontHandle,
								     bgPaletteEntry,
								     fgPaletteEntry,
								     maxOffset,
								     out bgLogicalColourOut,
								     out fgLogicalColourOut,
								     out maxOffsetOut);
		}

		/*! \brief Finds the best range of anti-alias colours to match a pair of palette entries
		 * using the current font.
		 * \param [in] bgPaletteEntry Background palette entry.
		 * \param [in] fgPaletteEntry Foreground palette entry.
		 * \param [in] maxOffset Maximum foreground colour offset (0 - 14).
		 * \param [out] bgLogicalColourOut The logical background colour.
		 * \param [out] fgLogicalColourOut The logical foreground colour.
		 * \param [out] maxOffsetOut The maximum sensible colour offset (up to maxOffset on entry).
		 * \return Nothing.  */
		public static void ReturnFontColours (uint bgPaletteEntry,
						      uint fgPaletteEntry,
						      int maxOffset,
						      out uint bgLogicalColourOut,
						      out uint fgLogicalColourOut,
						      out int maxOffsetOut)
		{
			ReturnFontColours (IntPtr.Zero,
					   bgPaletteEntry,
					   fgPaletteEntry,
					   maxOffset,
					   out bgLogicalColourOut,
					   out fgLogicalColourOut,
					   out maxOffsetOut);
		}

		/*! \brief Sets the best range of anti-alias colours to match a pair of palette entries.
		 * \param [in] fontHandle Font handle, or 0 for the current font.
		 * \param [in] bgPaletteEntry Background palette entry.
		 * \param [in] fgPaletteEntry Foreground palette entry.
		 * \param [in] maxOffset Maximum foreground colour offset (0 - 14).
		 * \param [out] bgLogicalColourOut The logical background colour.
		 * \param [out] fgLogicalColourOut The logical foreground colour.
		 * \param [out] maxOffsetOut The maximum sensible colour offset (up to maxOffset on entry).
		 * \return Nothing.  */
		public static void SetFontColours (IntPtr fontHandle,
						   uint bgPaletteEntry,
						   uint fgPaletteEntry,
						   int maxOffset,
						   out uint bgLogicalColourOut,
						   out uint fgLogicalColourOut,
						   out int maxOffsetOut)
		{
			NativeMethods.ColourTrans_SetFontColours (IntPtr.Zero,
								  bgPaletteEntry,
								  fgPaletteEntry,
								  maxOffset,
								  out bgLogicalColourOut,
								  out fgLogicalColourOut,
								  out maxOffsetOut);
		}

		/*! \brief Sets the best range of anti-alias colours to match a pair of palette entries.
		 * \param [in] fontHandle Font handle, or 0 for the current font.
		 * \param [in] bgPaletteEntry Background palette entry.
		 * \param [in] fgPaletteEntry Foreground palette entry.
		 * \param [in] offset Maximum foreground colour offset (0 - 14)
		 * \return Nothing.  */
		public static void SetFontColours (IntPtr fontHandle,
						   uint bgPaletteEntry,
						   uint fgPaletteEntry,
						   int maxOffset)
		{
			uint bg_out, fg_out;
			int offset_out;

			SetFontColours (fontHandle, bgPaletteEntry, fgPaletteEntry, maxOffset,
					out bg_out, out fg_out, out offset_out);
		}

		/*! \brief Sets the best range of anti-alias colours to match a pair of palette entries
		 * using the current font.
		 * \param [in] bgPaletteEntry Background palette entry.
		 * \param [in] fgPaletteEntry Foreground palette entry.
		 * \param [in] maxOffset Maximum foreground colour offset (0 - 14)
		 * \return Nothing.  */
		public static void SetFontColours (uint bgPaletteEntry,
						   uint fgPaletteEntry,
						   int maxOffset)
		{
			uint bg_out, fg_out;
			int offset_out;

			SetFontColours (IntPtr.Zero, bgPaletteEntry, fgPaletteEntry, maxOffset,
					out bg_out, out fg_out, out offset_out);
		}

		//! \brief Informs ColourTrans that the palette has been changed by some other means.
		public static void InvalidateCache ()
		{
			NativeMethods.ColourTrans_InvalidateCache ();
		}

		//! \brief Sets the calibration table for the screen.
		public static void SetCalibration (int [] calibrationTable)
		{
			GCHandle pinned_table = GCHandle.Alloc (calibrationTable, GCHandleType.Pinned);
			try {
				IntPtr raw_data_ptr = pinned_table.AddrOfPinnedObject ();
				OS.ThrowOnError (NativeMethods.ColourTrans_SetCalibration (raw_data_ptr));
			}
			catch {
				// Rethrow any exceptions for the caller to deal with.
				throw;
			}
			finally {
				pinned_table.Free ();
			}
		}

		//! \brief Reads the calibration table for the screen.
		public static int [] ReadCalibration ()
		{
			int buffer_size;
			IntPtr buffer = IntPtr.Zero;

			try {
				OS.ThrowOnError (NativeMethods.ColourTrans_ReadCalibration (IntPtr.Zero,
											    out buffer_size));
				buffer = Marshal.AllocHGlobal (buffer_size);
				OS.ThrowOnError (NativeMethods.ColourTrans_ReadCalibration (buffer,
											    out buffer_size));
				int [] table = new int [buffer_size >> 2];
				Marshal.Copy (buffer, table, 0, buffer_size >> 2);
				return table;
			}
			catch {
				// Rethrow any exceptions for the caller to deal with.
				throw;
			}
			finally {
				if (buffer != IntPtr.Zero)
					Marshal.FreeHGlobal (buffer);
			}
		}

		/*! \brief Converts a device colour to a standard colour
		 * \param [in] colour A 24-bit device colour.
		 * \param [in] calibrationTable 0 to use the current screen calibration, or an
		 * array containing a calibration table.
		 * \return A 24-bit standard colour (0xBBGGRR00).  */
		public static uint ConvertDeviceColour (uint colour, int [] calibrationTable)
		{
			GCHandle pinned_table = GCHandle.Alloc (calibrationTable, GCHandleType.Pinned);
			uint colour_result;
			try {
				IntPtr raw_data_ptr = pinned_table.AddrOfPinnedObject ();
				OS.ThrowOnError (NativeMethods.
						 ColourTrans_ConvertDeviceColour (colour,
										  raw_data_ptr,
										  out colour_result));
				return colour_result;
			}
			catch {
				// Rethrow any exceptions for the caller to deal with.
				throw;
			}
			finally {
				pinned_table.Free ();
			}
		}

		/*! \brief Converts a device palette to standard colours.
		 * \param [in] deviceColourTable An array of 24-bit device colours.
		 * \param [in] calibrationTable 0 to use the current screen calibration, or an
		 * array containing a calibration table.
		 * \return An array containing the converted palette.
		 * \note The number of colour to convert is taken from the number of elements
		 * in the deviceColourTable array.  */
		public static int [] ConvertDevicePalette (uint [] deviceColourTable,
							   int [] calibrationTable)
		{
			// FixMe: Do we need to pin integer arrays or can we pass them straight to native methods?
			int colourCount = deviceColourTable.Length;
			int [] buffer = new int [colourCount];

			// Exception handling is a grey area here for me. If the 2nd or 3rd pin fail, I don't
			// think the finally block will be called, so will the pinned arrays be leaked?
			// We can't pin them in the try block because either they're out of scope or they're
			// used unintialised.
			GCHandle pinned_calibration_table = GCHandle.Alloc (calibrationTable, GCHandleType.Pinned);
			GCHandle pinned_buffer = GCHandle.Alloc (buffer, GCHandleType.Pinned);
			GCHandle pinned_device_table = GCHandle.Alloc (deviceColourTable, GCHandleType.Pinned);

			try {
				OS.ThrowOnError (NativeMethods.
						 ColourTrans_ConvertDevicePalette (colourCount,
										   pinned_device_table.
										     AddrOfPinnedObject (),
										   pinned_buffer.
										     AddrOfPinnedObject (),
										   pinned_calibration_table.
										     AddrOfPinnedObject ()));
				return buffer;
			}
			catch {
				throw;
			}
			finally {
				pinned_calibration_table.Free ();
				pinned_buffer.Free ();
				pinned_device_table.Free ();
			}
		}

		/*! \brief Converts RISC %OS RGB colours to industry standard CIE colours.
		 * \param [in] r Red component.
		 * \param [in] g Green component.
		 * \param [in] b Blue component.
		 * \param [out] x CIE X tristimulus value.
		 * \param [out] y CIE Y tristimulus value.
		 * \param [out] z CIE Z tristimulus value.
		 * \return Nothing.
		 * \note All parameters are passed as fixed point 32 bit numbers, with 16 bits below
		 * the point and 16 bits above the point. It is suggested that numbers in the range
		 * 0 - 1 are used for compatibility with other conversion methods.  */
		public static void ConvertRGBToCIE (uint r, uint g, uint b,
						    out uint x, out uint y, out uint z)
		{
			NativeMethods.ColourTrans_ConvertRGBToCIE (r, g, b, out x, out y, out z);
		}

		/*! \brief Converts RISC %OS RGB colours to industry standard CIE colours.
		 * \param [in] x CIE X tristimulus value.
		 * \param [in] y CIE Y tristimulus value.
		 * \param [in] z CIE Z tristimulus value.
		 * \param [out] r Red component.
		 * \param [out] g Green component.
		 * \param [out] b Blue component.
		 * \return Nothing.
		 * \note All parameters are passed as fixed point 32 bit numbers, with 16 bits below
		 * the point and 16 bits above the point. It is suggested that numbers in the range
		 * 0 - 1 are used for compatibility with other conversion methods.  */
		public static void ConvertCIEToRGB (uint x, uint y, uint z,
						    out uint r, out uint g, out uint b)
		{
			NativeMethods.ColourTrans_ConvertCIEToRGB (x, y, z, out r, out g, out b);
		}

		/*! \brief Save the current calibration to a file.
		 * \param [in] fileHandle Handle of file to save to.
		 * \param [in] alwaysSave \e true if the calibration should be saved even if it is the
		 * default calibration.
		 * \return Nothing.  */ 
		public static void WriteCalibrationToFile (IntPtr fileHandle, bool alwaysSave)
		{
			OS.ThrowOnError (NativeMethods.ColourTrans_WriteCalibrationToFile (alwaysSave ? 1 : 0,
											   fileHandle));
		}

		/*! \brief Converts RISC %OS RGB colours into corresponding hue, saturation and value.
		 * \param [in] r Red component.
		 * \param [in] g Green component.
		 * \param [in] b Blue component.
		 * \param [out] h Hue.
		 * \param [out] s Saturation.
		 * \param [out] v Value.
		 * \return Nothing.
		 * \note All parameters are passed as fixed point 32 bit numbers, with 16 bits below
		 * the point and 16 bits above the point. Hue ranges from 0 - 360 with no fractional
		 * element, whilst the remaining parameters are in the range 0 - 1 and may have
		 * fractional elements.  */
		public static void ConvertRGBToHSV (uint r, uint g, uint b,
						    out uint h, out uint s, out uint v)
		{
			NativeMethods.ColourTrans_ConvertRGBToHSV (r, g, b, out h, out s, out v);
		}

		/*! \brief Converts hue, saturation and value into corresponding RISC %OS RGB colours.
		 * \param [in] h Hue.
		 * \param [in] s Saturation.
		 * \param [in] v Value.
		 * \param [out] r Red component.
		 * \param [out] g Green component.
		 * \param [out] b Blue component.
		 * \return Nothing.
		 * \note All parameters are passed as fixed point 32 bit numbers, with 16 bits below
		 * the point and 16 bits above the point. Hue ranges from 0 - 360 with no fractional
		 * element, whilst the remaining parameters are in the range 0 - 1 and may have
		 * fractional elements.
		 * \throw OS.ErrorException Thrown if hue and saturation are both 0.  */
		public static void ConvertHSVToRSB (uint h, uint s, uint v,
						    out uint r, out uint g, out uint b)
		{
			OS.ThrowOnError (NativeMethods.ColourTrans_ConvertHSVToRGB (h, s, v, out r, out g, out b));
		}

		/*! \brief Converts RISC %OS RGB colours into the CMYK model.
		 * \param [in] r Red component.
		 * \param [in] g Green component.
		 * \param [in] b Blue component.
		 * \param [out] c Cyan component.
		 * \param [out] m Magenta component.
		 * \param [out] y Yellow component.
		 * \param [out] k Key (black) component.
		 * \return Nothing.
		 * \note All parameters are passed as fixed point 32 bit numbers in the range 0 - 1,
		 * with 16 bits below the point and 16 bits above the point.  */
		public static void ConvertRGBToCMYK (uint r, uint g, uint b,
						     out uint c, out uint m, out uint y, out uint k)
		{
			NativeMethods.ColourTrans_ConvertRGBToCMYK (r, g, b, out c, out m, out y, out k);
		}

		/*! \brief Converts the CMYK model into RISC %OS RGB colours.
		 * \param [out] c Cyan component.
		 * \param [out] m Magenta component.
		 * \param [out] y Yellow component.
		 * \param [out] k Key (black) component.
		 * \param [in] r Red component.
		 * \param [in] g Green component.
		 * \param [in] b Blue component.
		 * \return Nothing.
		 * \note All parameters are passed as fixed point 32 bit numbers in the range 0 - 1,
		 * with 16 bits below the point and 16 bits above the point.  */
		public static void ConvertCMYKToRGB (uint c, uint m, uint y, uint k,
						     out uint r, out uint g, out uint b)
		{
			NativeMethods.ColourTrans_ConvertCMYKToRGB (c, m, y, k, out r, out g, out b);
		}

		/*! \brief Changes the foreground or background colour to a GCOL number.
		 * \param [in] gcol GCOL number.
		 * \param [in] gcolAction GCOL action.
		 * \param [in] background \e true to set the background colour instead of the foreground.
		 * \param [in] text \e true to set the text colour instead of the graphics colour.
		 * \return Nothing.  */
		public static void SetColour (int gcol,
					      OS.GCOLAction gcolAction,
					      bool background,
					      bool text)
		{
			int flags = 0;

			if (background)
				flags |= (1 << 7);
			if (text)
				flags |= (1 << 9);
			NativeMethods.ColourTrans_SetColour (gcol, flags, gcolAction);
		}

		/*! \brief Changes the foreground colour to a GCOL number.
		 * \param [in] gcol GCOL number.
		 * \param [in] gcolAction GCOL action.
		 * \return Nothing.  */
		public static void SetColour (int gcol, OS.GCOLAction gcolAction)
		{
			SetColour (gcol, gcolAction, false, false);
		}

		/*! \brief Changes the text foreground or background colour to a GCOL number.
		 * \param [in] paletteEntry A palette entry in the form 0xBBGGRR00.
		 * \param [in] background \e true if the background colour should be set instead of
		 * the foreground.
		 * \return The GCOL that was found to be nearest to the palette entry given and set.  */
		public static int SetTextColour (uint paletteEntry, bool background)
		{
			int flags = background ? (1 << 7) : 0;
			int gcol;

			NativeMethods.ColourTrans_SetTextColour (paletteEntry, flags, out gcol);

			return gcol;
		}

		/*! \brief Changes the text foreground or background colour to a GCOL number.
		 * \param [in] paletteEntry A palette entry in the form 0xBBGGRR00.
		 * \param [in] background \e true if the background colour should be set instead of
		 * the foreground.
		 * \return The GCOL that was found to be furthest to the palette entry given and set.  */
		public static int SetOppTextColour (uint paletteEntry, bool background)
		{
			int flags = background ? (1 << 7) : 0;
			int gcol;

			NativeMethods.ColourTrans_SetOppTextColour (paletteEntry, flags, out gcol);

			return gcol;
		}

		/*! \brief Sets up a translation table in a buffer for the given sprite.
		 * \param [in] spriteArea The sprite area that contains the sprite.
		 * \param [in] spritePointer Pointer to the sprite to generate the table for.
		 * \param [in] destMode Destination mode, or -1 for current mode.
		 * \param [in] destPalette Destination palette, or -1 for current palette,
		 * or 0 for default palette for the mode.
		 * \param [in] buffer Pointer to buffer to contain table, or 0 to return size
		 * of buffer required.
		 * \param [in] flags Flags (not all flags are settable).
		 * \return Size of buffer required if \e buffer on input is 0.
		 * \note The transfer function is not currently supported, therefor bit 2 of
		 * the flags is ignored.
		 * \note This method is specifically for a sprite pointer, therefor bit 0 of
		 * the flags is ignored.  */
		public static int GenerateTableForSprite (IntPtr spriteArea,
							  IntPtr spritePointer,
							  int destMode,
							  IntPtr destPalette,
							  IntPtr buffer,
							  TableFlags flags)
		{
			int buffer_size;

			// We don't currently support the transfer function.
			flags &= (~TableFlags.UseTransferFunction);
			flags |= TableFlags.SpritePointerGiven;

			OS.ThrowOnError (NativeMethods.ColourTrans_GenerateTableForSprite (spriteArea,
											   spritePointer,
											   destMode,
											   destPalette,
											   buffer,
											   flags,
											   IntPtr.Zero,
											   IntPtr.Zero,
											   out buffer_size));
			return buffer_size;
		}

		/*! \brief Sets up a translation table in a buffer for the given sprite.
		 * \param [in] spriteArea The sprite area that contains the sprite.
		 * \param [in] spriteName Name of the sprite to generate the table for.
		 * \param [in] destMode Destination mode, or -1 for current mode.
		 * \param [in] destPalette Destination palette, or -1 for current palette,
		 * or 0 for default palette for the mode.
		 * \param [in] buffer Pointer to buffer to contain table, or 0 to return size
		 * of buffer required.
		 * \param [in] flags Flags (not all flags are settable).
		 * \return Size of buffer required if \e buffer on input is 0.
		 * \note The transfer function is not currently supported, therefor bit 2 of
		 * the flags is ignored.
		 * \note This method is specifically for a sprite name, therefor bit 0 of
		 * the flags is ignored.  */
		public static int GenerateTableForSprite (IntPtr spriteArea,
							  string spriteName,
							  int destMode,
							  IntPtr destPalette,
							  IntPtr buffer,
							  TableFlags flags)
		{
			int buffer_size;

			// We don't currently support the transfer function.
			flags &= (~(TableFlags.UseTransferFunction | TableFlags.SpritePointerGiven));

			OS.ThrowOnError (NativeMethods.ColourTrans_GenerateTableForSprite (spriteArea,
											   spriteName,
											   destMode,
											   destPalette,
											   buffer,
											   flags,
											   IntPtr.Zero,
											   IntPtr.Zero,
											   out buffer_size));
			return buffer_size;
		}
	}
}
