//
// colourtrans.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;

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

		/*! \brief Sets the closest GCOL for a palette entry.
		 * \param [in] gcolAction The GCOL action.
		 * \param [in] paletteEntry A palette entry in the form 0xBBGGRR00.
		 * \return Nothing.  */
		public static void SetGCOL (OS.GCOLAction gcolAction,
					    uint paletteEntry)
		{
			int gcol, log2bpp;

			NativeMethods.ColourTrans_SetGCOL (paletteEntry,
							   0,
							   gcolAction,
							   out gcol,
							   out log2bpp);
		}

		/*! \brief Sets the best range of anti-alias colours to match a pair of palette entries.
		 * \param [in] bgHint Background palette entry.
		 * \param [in] fill Foreground palette entry.
		 * \param [in] offset Maximum foreground colour offset (0 - 14)
		 * \return Nothing.  */
		public static void SetFontColours (uint bgHint, uint fill, int offset)
		{
			uint bg_hint_out, fill_out;
			int offset_out;

			NativeMethods.ColourTrans_SetFontColours (IntPtr.Zero,
								  bgHint,
								  fill,
								  offset,
								  out bg_hint_out,
								  out fill_out,
								  out offset_out);
		}

		/*! \brief Sets the best range of anti-alias colours to match a pair of palette entries.
		 * \param [in] font Font handle, or 0 for the current font.
		 * \param [in] bgHint Background palette entry.
		 * \param [in] fill Foreground palette entry.
		 * \param [in] offset Maximum foreground colour offset (0 - 14)
		 * \return Nothing.  */
		public static void SetFontColours (IntPtr font, uint bgHint, uint fill, int offset)
		{
			uint bg_hint_out, fill_out;
			int offset_out;

			NativeMethods.ColourTrans_SetFontColours (font,
								  bgHint,
								  fill,
								  offset,
								  out bg_hint_out,
								  out fill_out,
								  out offset_out);
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
