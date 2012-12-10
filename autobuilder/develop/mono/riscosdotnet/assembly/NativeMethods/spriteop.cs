//
// NativeMethods/spriteop.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Runtime.InteropServices;
using System.Text;

namespace riscos
{
	internal static partial class NativeMethods
	{
		// OSSpriteOp SWIs
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_screen_save")]
		internal static extern IntPtr OSSpriteOp_ScreenSave (string filename,
								     bool savePalette);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_screen_load")]
		internal static extern IntPtr OSSpriteOp_ScreenLoad (string filename);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_read_area_cb")]
		internal static extern IntPtr OSSpriteOp_ReadAreaControlBlock (OSSpriteOp.AreaType flags,
									       IntPtr spriteAreaCB,
									       out int byteSize,
									       out int spriteCount,
									       out int firstOffset,
									       out int freeOffset);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_clear_sprites")]
		internal static extern IntPtr OSSpriteOp_InitialiseArea (OSSpriteOp.AreaType flags,
									 IntPtr spriteAreaCB);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_load_sprite_file")]
		internal static extern IntPtr OSSpriteOp_LoadSpriteFile (OSSpriteOp.AreaType flags,
									 IntPtr spriteAreaCB,
									 string filename);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_merge_sprite_file")]
		internal static extern IntPtr OSSpriteOp_MergeSpriteFile (OSSpriteOp.AreaType flags,
									  IntPtr spriteAreaCB,
									  string filename);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_save_sprite_file")]
		internal static extern IntPtr OSSpriteOp_SaveSpriteFile (OSSpriteOp.AreaType flags,
									 IntPtr spriteAreaCB,
									 string filename);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_return_name")]
		internal static extern IntPtr OSSpriteOp_ReturnName (OSSpriteOp.AreaType flags,
								     IntPtr spriteAreaCB,
								     StringBuilder buffer,
								     int bufferSize,
								     int spriteNumber,
								     out int used);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_get_sprite")]
		internal static extern IntPtr OSSpriteOp_GetSprite (OSSpriteOp.AreaType flags,
								    IntPtr spriteAreaCB,
								    string spriteName,
								    bool getPalette,
								    out IntPtr spriteAddress);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_create_sprite")]
		internal static extern IntPtr OSSpriteOp_CreateSprite (OSSpriteOp.AreaType flags,
								       IntPtr spriteAreaCB,
								       string spriteName,
								       bool createPalette,
								       int width,
								       int height,
								       int mode);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_create_sprite")]
		internal static extern IntPtr OSSpriteOp_CreateSprite (OSSpriteOp.AreaType flags,
								       IntPtr spriteAreaCB,
								       string spriteName,
								       bool createPalette,
								       int width,
								       int height,
								       OS.ModeSelector mode);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_get_sprite_user_coords")]
		internal static extern IntPtr OSSpriteOp_GetSpriteUserCoords (OSSpriteOp.AreaType flags,
									      IntPtr spriteAreaCB,
									      string spriteName,
									      bool getPalette,
									      int x0,
									      int y0,
									      int x1,
									      int y1);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_verify_area")]
		internal static extern IntPtr OSSpriteOp_VerifyArea (OSSpriteOp.AreaType flags,
								     IntPtr spriteAreaCB);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_select_sprite")]
		internal static extern IntPtr OSSpriteOp_SelectSprite (OSSpriteOp.AreaType flags,
								       IntPtr spriteAreaCB,
								       string spriteName,
								       out IntPtr spriteAddress);

		// Delete using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_delete_sprite")]
		internal static extern IntPtr OSSpriteOp_DeleteSprite (OSSpriteOp.AreaType flags,
								       IntPtr spriteAreaCB,
								       string spriteName);

		// Delete using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_delete_sprite")]
		internal static extern IntPtr OSSpriteOp_DeleteSprite (OSSpriteOp.AreaType flags,
								       IntPtr spriteAreaCB,
								       IntPtr spriteAddress);

		// Rename using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_rename_sprite")]
		internal static extern IntPtr OSSpriteOp_RenameSprite (OSSpriteOp.AreaType flags,
								       IntPtr spriteAreaCB,
								       string spriteName,
								       string newName);

		// Rename using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_rename_sprite")]
		internal static extern IntPtr OSSpriteOp_RenameSprite (OSSpriteOp.AreaType flags,
								       IntPtr spriteAreaCB,
								       IntPtr spriteAddress,
								       string newName);

		// Copy using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_copy_sprite")]
		internal static extern IntPtr OSSpriteOp_CopySprite (OSSpriteOp.AreaType flags,
								     IntPtr spriteAreaCB,
								     string spriteName,
								     string newName);

		// Copy using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_copy_sprite")]
		internal static extern IntPtr OSSpriteOp_CopySprite (OSSpriteOp.AreaType flags,
								     IntPtr spriteAreaCB,
								     IntPtr spriteAddress,
								     string newName);

		// Plot using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_put_sprite")]
		internal static extern IntPtr OSSpriteOp_PutSprite (OSSpriteOp.AreaType flags,
								    IntPtr spriteAreaCB,
								    string spriteName,
								    OSSpriteOp.PlotAction plotAction);

		// Plot using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_put_sprite")]
		internal static extern IntPtr OSSpriteOp_PutSprite (OSSpriteOp.AreaType flags,
								    IntPtr spriteAreaCB,
								    IntPtr spriteAddress,
								    OSSpriteOp.PlotAction plotAction);

		// Create mask using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_create_mask")]
		internal static extern IntPtr OSSpriteOp_CreateMask (OSSpriteOp.AreaType flags,
								     IntPtr spriteAreaCB,
								     string spriteName);

		// Create mask using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_create_mask")]
		internal static extern IntPtr OSSpriteOp_CreateMask (OSSpriteOp.AreaType flags,
								     IntPtr spriteAreaCB,
								     IntPtr spriteAddress);

		// Remove mask using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_remove_mask")]
		internal static extern IntPtr OSSpriteOp_RemoveMask (OSSpriteOp.AreaType flags,
								     IntPtr spriteAreaCB,
								     string spriteName);

		// Remove mask using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_remove_mask")]
		internal static extern IntPtr OSSpriteOp_RemoveMask (OSSpriteOp.AreaType flags,
								     IntPtr spriteAreaCB,
								     IntPtr spriteAddress);

		// Insert row using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_insert_row")]
		internal static extern IntPtr OSSpriteOp_InsertRow (OSSpriteOp.AreaType flags,
								    IntPtr spriteAreaCB,
								    string spriteName,
								    int rowNumber);

		// Insert row using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_insert_row")]
		internal static extern IntPtr OSSpriteOp_InsertRow (OSSpriteOp.AreaType flags,
								    IntPtr spriteAreaCB,
								    IntPtr spriteAddress,
								    int rowNumber);

		// Delete row using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_delete_row")]
		internal static extern IntPtr OSSpriteOp_DeleteRow (OSSpriteOp.AreaType flags,
								    IntPtr spriteAreaCB,
								    string spriteName,
								    int rowNumber);

		// Delete row using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_delete_row")]
		internal static extern IntPtr OSSpriteOp_DeleteRow (OSSpriteOp.AreaType flags,
								    IntPtr spriteAreaCB,
								    IntPtr spriteAddress,
								    int rowNumber);

		// Flip X axis using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_flip_about_xaxis")]
		internal static extern IntPtr OSSpriteOp_FlipAboutXAxis (OSSpriteOp.AreaType flags,
									 IntPtr spriteAreaCB,
									 string spriteName);

		// Flip X axis using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_flip_about_xaxis")]
		internal static extern IntPtr OSSpriteOp_FlipAboutXAxis (OSSpriteOp.AreaType flags,
									 IntPtr spriteAreaCB,
									 IntPtr spriteAddress);

		// Plot using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_put_sprite_user_coords")]
		internal static extern IntPtr OSSpriteOp_PutSpriteUserCoords (OSSpriteOp.AreaType flags,
									      IntPtr spriteAreaCB,
									      string spriteName,
									      int x,
									      int y,
									      OSSpriteOp.PlotAction plotAction);

		// Plot using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_put_sprite_user_coords")]
		internal static extern IntPtr OSSpriteOp_PutSpriteUserCoords (OSSpriteOp.AreaType flags,
									      IntPtr spriteAreaCB,
									      IntPtr spriteAddress,
									      int x,
									      int y,
									      OSSpriteOp.PlotAction plotAction);

		// Append using sprite names
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_append_sprite")]
		internal static extern IntPtr OSSpriteOp_FlipAboutXAxis (OSSpriteOp.AreaType flags,
									 IntPtr spriteAreaCB,
									 string spriteName1,
									 string spriteName2,
									 bool appendVertically);

		// Append using sprite addresses
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_append_sprite")]
		internal static extern IntPtr OSSpriteOp_FlipAboutXAxis (OSSpriteOp.AreaType flags,
									 IntPtr spriteAreaCB,
									 IntPtr spriteAddress1,
									 IntPtr spriteAddress2,
									 bool appendVertically);

		// Set pointer shape using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_set_pointer_shape")]
		internal static extern IntPtr OSSpriteOp_SetPointerShape (OSSpriteOp.AreaType flags,
									  IntPtr spriteAreaCB,
									  string spriteName,
									  uint flags,
									  int activeX,
									  int activeY,
									  OS.ScaleFactors factors,
									  IntPtr transTable);

		// Set pointer shape using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_set_pointer_shape")]
		internal static extern IntPtr OSSpriteOp_SetPointerShape (OSSpriteOp.AreaType flags,
									  IntPtr spriteAreaCB,
									  IntPtr spriteAddress,
									  uint flags,
									  int activeX,
									  int activeY,
									  OS.ScaleFactors factors,
									  IntPtr transTable);

		// Read palette using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_read_palette_size")]
		internal static extern IntPtr OSSpriteOp_ReadPaletteSize (OSSpriteOp.AreaType flags,
									  IntPtr spriteAreaCB,
									  string spriteName,
									  out int size,
									  out IntPtr palette,
									  out int mode);

		// Read palette shape using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_read_palette_size")]
		internal static extern IntPtr OSSpriteOp_ReadPaletteSize (OSSpriteOp.AreaType flags,
									  IntPtr spriteAreaCB,
									  IntPtr spriteAddress,
									  out int size,
									  out IntPtr palette,
									  out int mode);

		// Remove palette using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_remove_palette")]
		internal static extern IntPtr OSSpriteOp_RemovePalette (OSSpriteOp.AreaType flags,
									IntPtr spriteAreaCB,
									string spriteName);

		// Remove palette using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_remove_palette")]
		internal static extern IntPtr OSSpriteOp_RemovePalette (OSSpriteOp.AreaType flags,
									IntPtr spriteAreaCB,
									IntPtr spriteAddress);

		// Create palette using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_create_palette")]
		internal static extern IntPtr OSSpriteOp_CreatePalette (OSSpriteOp.AreaType flags,
									IntPtr spriteAreaCB,
									string spriteName);

		// Create palette using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_create_palette")]
		internal static extern IntPtr OSSpriteOp_CreatePalette (OSSpriteOp.AreaType flags,
									IntPtr spriteAreaCB,
									IntPtr spriteAddress);

		// Create palette using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_create_true_palette")]
		internal static extern IntPtr OSSpriteOp_CreateTruePalette (OSSpriteOp.AreaType flags,
									    IntPtr spriteAreaCB,
									    string spriteName);

		// Create palette using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_create_true_palette")]
		internal static extern IntPtr OSSpriteOp_CreateTruePalette (OSSpriteOp.AreaType flags,
									    IntPtr spriteAreaCB,
									    IntPtr spriteAddress);

		// Read info using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_read_sprite_info")]
		internal static extern IntPtr OSSpriteOp_ReadSpriteInfo (OSSpriteOp.AreaType flags,
									 IntPtr spriteAreaCB,
									 string spriteName,
									 out int width,
									 out int height,
									 out bool hasMask,
									 out int mode);

		// Read info using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_read_sprite_info")]
		internal static extern IntPtr OSSpriteOp_ReadSpriteInfo (OSSpriteOp.AreaType flags,
									 IntPtr spriteAreaCB,
									 IntPtr spriteAddress,
									 out int width,
									 out int height,
									 out bool hasMask,
									 out int mode);

		// Read pixel using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_read_pixel_colour")]
		internal static extern IntPtr OSSpriteOp_ReadPixelColour (OSSpriteOp.AreaType flags,
									  IntPtr spriteAreaCB,
									  string spriteName,
									  int x,
									  int y,
									  out int gcol,
									  out int tint);

		// Read pixel using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_read_pixel_colour")]
		internal static extern IntPtr OSSpriteOp_ReadPixelColour (OSSpriteOp.AreaType flags,
									  IntPtr spriteAreaCB,
									  IntPtr spriteAddress,
									  int x,
									  int y,
									  out int gcol,
									  out int tint);

		// Write pixel using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_write_pixel_colour")]
		internal static extern IntPtr OSSpriteOp_WritePixelColour (OSSpriteOp.AreaType flags,
									   IntPtr spriteAreaCB,
									   string spriteName,
									   int x,
									   int y,
									   int gcol,
									   int tint);

		// Write pixel using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_write_pixel_colour")]
		internal static extern IntPtr OSSpriteOp_WritePixelColour (OSSpriteOp.AreaType flags,
									   IntPtr spriteAreaCB,
									   IntPtr spriteAddress,
									   int x,
									   int y,
									   int gcol,
									   int tint);

		// Read pixel using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_read_pixel_mask")]
		internal static extern IntPtr OSSpriteOp_ReadPixelMask (OSSpriteOp.AreaType flags,
									IntPtr spriteAreaCB,
									string spriteName,
									int x,
									int y,
									out bool solid);

		// Read pixel using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_read_pixel_mask")]
		internal static extern IntPtr OSSpriteOp_ReadPixelMask (OSSpriteOp.AreaType flags,
									IntPtr spriteAreaCB,
									IntPtr spriteAddress,
									int x,
									int y,
									out bool solid);

		// Read pixel using sprite name - RISC OS Select
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_read_pixel_mask_alpha")]
		internal static extern IntPtr OSSpriteOp_ReadPixelMaskAlpha (OSSpriteOp.AreaType flags,
									     IntPtr spriteAreaCB,
									     string spriteName,
									     int x,
									     int y,
									     out int opacity);

		// Read pixel using sprite address - RISC OS Select
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_read_pixel_mask_alpha")]
		internal static extern IntPtr OSSpriteOp_ReadPixelMaskAlpha (OSSpriteOp.AreaType flags,
									     IntPtr spriteAreaCB,
									     IntPtr spriteAddress,
									     int x,
									     int y,
									     out int opacity);

		// Write pixel using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_write_pixel_mask")]
		internal static extern IntPtr OSSpriteOp_WritePixelMask (OSSpriteOp.AreaType flags,
									 IntPtr spriteAreaCB,
									 string spriteName,
									 int x,
									 int y,
									 bool solid);

		// Write pixel using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_write_pixel_mask")]
		internal static extern IntPtr OSSpriteOp_WritePixelMask (OSSpriteOp.AreaType flags,
									 IntPtr spriteAreaCB,
									 IntPtr spriteAddress,
									 int x,
									 int y,
									 bool solid);

		// Write pixel using sprite name - RISC OS Select
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_write_pixel_mask_alpha")]
		internal static extern IntPtr OSSpriteOp_WritePixelMaskAlpha (OSSpriteOp.AreaType flags,
									      IntPtr spriteAreaCB,
									      string spriteName,
									      int x,
									      int y,
									      int opacity);

		// Write pixel using sprite address - RISC OS Select
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_write_pixel_mask_alpha")]
		internal static extern IntPtr OSSpriteOp_WritePixelMaskAlpha (OSSpriteOp.AreaType flags,
									      IntPtr spriteAreaCB,
									      IntPtr spriteAddress,
									      int x,
									      int y,
									      int opacity);

		// Insert column using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_insert_column")]
		internal static extern IntPtr OSSpriteOp_InsertColumn (OSSpriteOp.AreaType flags,
								       IntPtr spriteAreaCB,
								       string spriteName,
								       int columnNumber);

		// Insert column using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_insert_column")]
		internal static extern IntPtr OSSpriteOp_InsertColumn (OSSpriteOp.AreaType flags,
								       IntPtr spriteAreaCB,
								       IntPtr spriteAddress,
								       int columnNumber);

		// Delete column using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_delete_column")]
		internal static extern IntPtr OSSpriteOp_DeleteColumn (OSSpriteOp.AreaType flags,
								       IntPtr spriteAreaCB,
								       string spriteName,
								       int columnNumber);

		// Delete column using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_delete_column")]
		internal static extern IntPtr OSSpriteOp_DeleteColumn (OSSpriteOp.AreaType flags,
								       IntPtr spriteAreaCB,
								       IntPtr spriteAddress,
								       int columnNumber);

		// Flip Y axis using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_flip_about_yaxis")]
		internal static extern IntPtr OSSpriteOp_FlipAboutYAxis (OSSpriteOp.AreaType flags,
									 IntPtr spriteAreaCB,
									 string spriteName);

		// Flip Y axis using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_flip_about_yaxis")]
		internal static extern IntPtr OSSpriteOp_FlipAboutYAxis (OSSpriteOp.AreaType flags,
									 IntPtr spriteAreaCB,
									 IntPtr spriteAddress);

		// plot mask using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_plot_mask")]
		internal static extern IntPtr OSSpriteOp_PlotMask (OSSpriteOp.AreaType flags,
								   IntPtr spriteAreaCB,
								   string spriteName);

		// plot mask using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_plot_mask")]
		internal static extern IntPtr OSSpriteOp_PlotMask (OSSpriteOp.AreaType flags,
								   IntPtr spriteAreaCB,
								   IntPtr spriteAddress);

		// plot mask using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_plot_mask_user_coords")]
		internal static extern IntPtr OSSpriteOp_PlotMaskUserCoords (OSSpriteOp.AreaType flags,
									     IntPtr spriteAreaCB,
									     string spriteName,
									     int x,
									     int y);

		// plot mask using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_plot_mask_user_coords")]
		internal static extern IntPtr OSSpriteOp_PlotMaskUserCoords (OSSpriteOp.AreaType flags,
									     IntPtr spriteAreaCB,
									     IntPtr spriteAddress,
									     int x,
									     int y);

		// plot mask using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_plot_mask_scaled")]
		internal static extern IntPtr OSSpriteOp_PlotMaskScaled (OSSpriteOp.AreaType flags,
									 IntPtr spriteAreaCB,
									 string spriteName,
									 int x,
									 int y,
									 OS.ScaleFactors factors);

		// plot mask using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_plot_mask_scaled")]
		internal static extern IntPtr OSSpriteOp_PlotMaskScaled (OSSpriteOp.AreaType flags,
									 IntPtr spriteAreaCB,
									 IntPtr spriteAddress,
									 int x,
									 int y,
									 OS.ScaleFactors factors);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_paint_char_scaled")]
		internal static extern IntPtr OSSpriteOp_PaintCharScaled (int c,
									  int x,
									  int y,
									  OS.ScaleFactors factors);

		// plot sprite using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_put_sprite_scaled")]
		internal static extern IntPtr OSSpriteOp_PutSpriteScaled (OSSpriteOp.AreaType flags,
									  IntPtr spriteAreaCB,
									  string spriteName,
									  int x,
									  int y,
									  OSSpriteOp.PlotAction action,
									  OS.ScaleFactors factors,
									  IntPtr transTab);

		// plot sprite using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_put_sprite_scaled")]
		internal static extern IntPtr OSSpriteOp_PutSpriteScaled (OSSpriteOp.AreaType flags,
									  IntPtr spriteAreaCB,
									  IntPtr spriteAddress,
									  int x,
									  int y,
									  OSSpriteOp.PlotAction action,
									  OS.ScaleFactors factors,
									  IntPtr transTab);

		// plot sprite using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_put_sprite_scaled_mapped")]
		internal static extern IntPtr OSSpriteOp_PutSpriteScaledMapped (OSSpriteOp.AreaType flags,
										IntPtr spriteAreaCB,
										string spriteName,
										int x,
										int y,
										OSSpriteOp.PlotAction action,
										OS.ScaleFactors factors,
										IntPtr mapping);

		// plot sprite using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_put_sprite_scaled_mapped")]
		internal static extern IntPtr OSSpriteOp_PutSpriteScaledMapped (OSSpriteOp.AreaType flags,
										IntPtr spriteAreaCB,
										IntPtr spriteAddress,
										int x,
										int y,
										OSSpriteOp.PlotAction action,
										OS.ScaleFactors factors,
										IntPtr mapping);

		// plot sprite using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_put_sprite_grey_scaled")]
		internal static extern IntPtr OSSpriteOp_PutSpriteGreyScaled (OSSpriteOp.AreaType flags,
									      IntPtr spriteAreaCB,
									      string spriteName,
									      int x,
									      int y,
									      OS.ScaleFactors factors,
									      IntPtr transTab);

		// plot sprite using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_put_sprite_grey_scaled")]
		internal static extern IntPtr OSSpriteOp_PutSpriteGreyScaled (OSSpriteOp.AreaType flags,
									      IntPtr spriteAreaCB,
									      IntPtr spriteAddress,
									      int x,
									      int y,
									      OS.ScaleFactors factors,
									      IntPtr transTab);

		// Remove lefthand wastage using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_remove_left_hand_wastage")]
		internal static extern IntPtr OSSpriteOp_RemoveLeftHandWastage (OSSpriteOp.AreaType flags,
										IntPtr spriteAreaCB,
										string spriteName);

		// Remove lefthand wastage using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_remove_left_hand_wastage")]
		internal static extern IntPtr OSSpriteOp_RemoveLeftHandWastage (OSSpriteOp.AreaType flags,
										IntPtr spriteAreaCB,
										IntPtr spriteAddress);

		// plot mask using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_plot_mask_trfm")]
		internal static extern IntPtr OSSpriteOp_PlotMaskTransformed (OSSpriteOp.AreaType flags,
									      IntPtr spriteAreaCB,
									      string spriteName,
									      OSSpriteOp.TransformFlags flags,
									      OS.Rect source,
									      [In, MarshalAs(UnmanagedType.Struct)]
										   ref NativeOS.Matrix matrix);

		// plot mask using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_plot_mask_trfm")]
		internal static extern IntPtr OSSpriteOp_PlotMaskTransformed (OSSpriteOp.AreaType flags,
									      IntPtr spriteAreaCB,
									      IntPtr spriteAddress,
									      OSSpriteOp.TransformFlags flags,
									      OS.Rect source,
									      [In, MarshalAs(UnmanagedType.Struct)]
										   ref NativeOS.Matrix matrix);

		// plot sprite using sprite name and transformation matrix.
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_put_sprite_trfm")]
		internal static extern IntPtr OSSpriteOp_PutSpriteTransformed (OSSpriteOp.AreaType flags,
									       IntPtr spriteAreaCB,
									       string spriteName,
									       OSSpriteOp.TransformFlags transformFlags,
									       OS.Rect source,
									       OSSpriteOp.PlotAction action,
									       [In, MarshalAs(UnmanagedType.Struct)]
										    ref NativeOS.Matrix matrix,
									       IntPtr transTab);


		// plot sprite using sprite name and destination coordinate block.
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_put_sprite_trfm")]
		internal static extern IntPtr OSSpriteOp_PutSpriteTransformed (OSSpriteOp.AreaType flags,
									       IntPtr spriteAreaCB,
									       string spriteName,
									       OSSpriteOp.TransformFlags transformFlags,
									       OS.Rect source,
									       OSSpriteOp.PlotAction action,
									       OSSpriteOp.DestCoordBlock destCoordBlock,
									       IntPtr transTab);

		// plot sprite using sprite address and transformation matrix.
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_put_sprite_trfm")]
		internal static extern IntPtr OSSpriteOp_PutSpriteTransformed (OSSpriteOp.AreaType flags,
									       IntPtr spriteAreaCB,
									       IntPtr spriteAddress,
									       OSSpriteOp.TransformFlags flags,
									       OS.Rect source,
									       OSSpriteOp.PlotAction action,
									       [In, MarshalAs(UnmanagedType.Struct)]
										    ref NativeOS.Matrix matrix,
									       IntPtr transTab);


		// plot sprite using sprite address and destination coordinate block.
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_put_sprite_trfm")]
		internal static extern IntPtr OSSpriteOp_PutSpriteTransformed (OSSpriteOp.AreaType flags,
									       IntPtr spriteAreaCB,
									       IntPtr spriteAddress,
									       OSSpriteOp.TransformFlags flags,
									       OS.Rect source,
									       OSSpriteOp.PlotAction action,
									       OSSpriteOp.DestCoordBlock destCoordBlock,
									       IntPtr transTab);

		// plot sprite using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_put_sprite_trfm_mapped")]
		internal static extern IntPtr OSSpriteOp_PutSpriteTransformedMapped (OSSpriteOp.AreaType flags,
										     IntPtr spriteAreaCB,
										     string spriteName,
										     OSSpriteOp.TransformFlags flags,
										     OS.Rect source,
										     OSSpriteOp.PlotAction action,
										     [In, MarshalAs(UnmanagedType.Struct)]
											  ref NativeOS.Matrix matrix,
										     IntPtr mapping);

		// plot sprite using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_put_sprite_trfm_mapped")]
		internal static extern IntPtr OSSpriteOp_PutSpriteTransformedMapped (OSSpriteOp.AreaType flags,
										     IntPtr spriteAreaCB,
										     IntPtr spriteAddress,
										     OSSpriteOp.TransformFlags flags,
										     OS.Rect source,
										     OSSpriteOp.PlotAction action,
										     [In, MarshalAs(UnmanagedType.Struct)]
											  ref NativeOS.Matrix matrix,
										     IntPtr mapping);

		// Insert/delete rows using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_insert_delete_rows")]
		internal static extern IntPtr OSSpriteOp_InsertDeleteRows (OSSpriteOp.AreaType flags,
									   IntPtr spriteAreaCB,
									   string spriteName,
									   int rowNumber,
									   int rowCount);

		// Insert/delete rows using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_insert_delete_rows")]
		internal static extern IntPtr OSSpriteOp_InsertDeleteRows (OSSpriteOp.AreaType flags,
									   IntPtr spriteAreaCB,
									   IntPtr spriteAddress,
									   int rowNumber,
									   int rowCount);

		// Insert/delete columns using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_insert_delete_columns")]
		internal static extern IntPtr OSSpriteOp_InsertDeleteColumns (OSSpriteOp.AreaType flags,
									      IntPtr spriteAreaCB,
									      string spriteName,
									      int columnNumber,
									      int columnCount);

		// Insert/delete columns using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_insert_delete_columns")]
		internal static extern IntPtr OSSpriteOp_InsertDeleteColumns (OSSpriteOp.AreaType flags,
									      IntPtr spriteAreaCB,
									      IntPtr spriteAddress,
									      int columnNumber,
									      int columnCount);

		// Switch output using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_switch_output_to_sprite")]
		internal static extern IntPtr OSSpriteOp_SwitchOutputToSprite (OSSpriteOp.AreaType flags,
									       IntPtr spriteAreaCB,
									       string spriteName,
									       IntPtr saveArea,
									       out IntPtr r0_out,
									       out IntPtr r1_out,
									       out IntPtr r2_out,
									       out IntPtr r3_out);

		// Switch output using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_switch_output_to_sprite")]
		internal static extern IntPtr OSSpriteOp_SwitchOutputToSprite (OSSpriteOp.AreaType flags,
									       IntPtr spriteAreaCB,
									       IntPtr spriteAddress,
									       IntPtr saveArea,
									       out IntPtr r0_out,
									       out IntPtr r1_out,
									       out IntPtr r2_out,
									       out IntPtr r3_out);

		// Switch output using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_switch_output_to_mask")]
		internal static extern IntPtr OSSpriteOp_SwitchOutputToMask (OSSpriteOp.AreaType flags,
									     IntPtr spriteAreaCB,
									     string spriteName,
									     IntPtr saveArea,
									     out IntPtr r0_out,
									     out IntPtr r1_out,
									     out IntPtr r2_out,
									     out IntPtr r3_out);

		// Switch output using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_switch_output_to_mask")]
		internal static extern IntPtr OSSpriteOp_SwitchOutputToMask (OSSpriteOp.AreaType flags,
									     IntPtr spriteAreaCB,
									     IntPtr spriteAddress,
									     IntPtr saveArea,
									     out IntPtr r0_out,
									     out IntPtr r1_out,
									     out IntPtr r2_out,
									     out IntPtr r3_out);

		// Read using sprite name
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_read_save_area_size")]
		internal static extern IntPtr OSSpriteOp_ReadSaveAreaSize (OSSpriteOp.AreaType flags,
									   IntPtr spriteAreaCB,
									   string spriteName,
									   out int size);

		// Read using sprite address
		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_read_save_area_size")]
		internal static extern IntPtr OSSpriteOp_ReadSaveAreaSize (OSSpriteOp.AreaType flags,
									   IntPtr spriteAreaCB,
									   IntPtr spriteAddress,
									   out int size);

		[DllImport("libriscosdotnet.so.1", EntryPoint="xosspriteop_unswitch_output")]
		internal static extern IntPtr OSSpriteOp_UnswitchOutput (IntPtr r0,
									 IntPtr r1,
									 IntPtr r2,
									 IntPtr r3);
	}
}
