//
// sprite.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//

using System;
using System.Runtime.InteropServices;

namespace riscos
{
	public static class OSSpriteOp
	{
		/*! \brief The type of sprite area and sprite pointer.  */
		public enum AreaType
		{
			/*! \brief System sprite area using a named sprite.  */
			System = 0,
			/*! \brief User sprite area using a named sprite.  */
			UserWithName = 256,
			/*! \brief User sprite area using a sprite pointer.  */
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
			WithMask,
			/*! \brief Use palette instead of translation table.  */
			UsePalette = 16,
			/*! \brief Translation table is wide.  */
			WideTransTable = 32,
			/*! \brief Dither when plotting a 16 bit or 32 bit sprite into a reduced depth.  */
			UseDither = 64,
			/*! \brief Use colour mapping (16 bit & 32 bit only) \b Select \b 2.  */
			UseColourMapping = 128
		}

		public enum TransformFlags
		{
			DestinationCoords = (1 << 0),
			SourceRectangle = (1 << 1)
		}

		/*! \brief The possible values, in dots per inch, that can be used for the
		 * horizontal and vertical resolutions of a sprite.  */
		public enum SpriteDpi
		{
			Dpi180 = 180,
			Dpi90 = 90,
			Dpi45 = 45,
			DPi23 = 23,
			DPi22 = 22
		}

		/*! \brief The destination coordinate block used by PlotTransformed.
		 * \note Values are in draw units (1/256th OS unit).
		 * \note Method OS.ToDrawUnits can be used to convert values.  */
		[StructLayout(LayoutKind.Sequential)]
		public class DestCoordBlock
		{
			public int x0;
			public int y0;
			public int x1;
			public int y1;
			public int x2;
			public int y2;
			public int x3;
			public int y3;
		}

		/*! \brief Encapsulate the data required for a sprite area.  */
		public class SpriteArea
		{
			/*! \brief The address of the first word of the sprite area.  */
			public IntPtr BaseAddress;

			/*! \brief Create a sprite area.
			 * \param [in] baseAddress The address of a block of memory where the sprite
			 * area should be created.
			 * \param [in] size The size of the new sprite area.  */
			public SpriteArea (IntPtr baseAddress, int size)
			{
				BaseAddress = baseAddress;

				Marshal.WriteInt32 (BaseAddress, size);
				Marshal.WriteInt32 (BaseAddress, 8, 16);

				OSSpriteOp.InitialiseArea (BaseAddress);
			}

			/*! \brief Create sprite in this sprite area.
			 * \param [in] spriteName The name to give the new sprite.
			 * \param [in] createPalette \e true if the new sprite should have a palette.
			 * \param [in] width The width of the new sprite in pixels.
			 * \param [in] height The height of the new sprite in pixels.
			 * \param [in] mode The mode number of sprite type to use for colour depth, DPI, etc.
			 * \note OSSpriteOp.GenerateOldSpriteType () can be used to generate the \e mode
			 * parameter.
			 * \return Nothing.  */
			public void CreateSprite (string spriteName,
						  bool createPalette,
						  int width,
						  int height,
						  int mode)
			{
				OSSpriteOp.CreateSprite (AreaType.UserWithName,
							 BaseAddress,
							 spriteName,
							 createPalette,
							 width,
							 height,
							 mode);
			}

			/*! \brief Create sprite in this sprite area.
			 * \param [in] spriteName The name to give the new sprite.
			 * \param [in] createPalette \e true if the new sprite should have a palette.
			 * \param [in] width The width of the new sprite in pixels.
			 * \param [in] height The height of the new sprite in pixels.
			 * \param [in] mode The mode selector to use for colour depth, DPI, etc.
			 * \return Nothing.  */
			public void CreateSprite (string spriteName,
						  bool createPalette,
						  int width,
						  int height,
						  OS.ModeSelector mode)
			{
				OSSpriteOp.CreateSprite (AreaType.UserWithName,
							 BaseAddress,
							 spriteName,
							 createPalette,
							 width,
							 height,
							 mode);
			}

			/*! \brief Save this sprite area as a file.  */
			public void SaveFile (string filename)
			{
				SaveSpriteFile (AreaType.UserWithName, BaseAddress, filename);
			}
		}

		/*! \brief Encapsulate a sprite via its name.  */
		public class SpriteByName
		{
			protected SpriteArea AreaCB;

			protected string Name;


			/*! \brief Create a sprite that is referenced by its name rather.
			 * \param [in] spriteArea The sprite area in which to create the sprite.
			 * \param [in] name Name of the sprite to create.
			 * \param [in] palette \e true if the sprite should be created with a palette.
			 * \param [in] width Width of sprite in pixels.
			 * \param [in] height Height of sprite in pixels.
			 * \param [in] mode Either a mode number or a sprite type.
			 * \note OSSpriteOp.GenerateOldSpriteType can be used to generate the \e mode
			 * parameter.  */
			public SpriteByName (SpriteArea spriteArea,
					     string name,
					     bool palette,
					     int width,
					     int height,
					     int mode)
			{
				AreaCB = spriteArea;
				Name = name;

				AreaCB.CreateSprite (Name,
						     palette,
						     width,
						     height,
						     mode);
			}

			/*! \brief Create a sprite that is referenced by a pointer rather than its
			 * name.
			 * \param [in] spriteArea The sprite area in which to create the sprite.
			 * \param [in] name Name of the sprite to create.
			 * \param [in] palette \e true if the sprite should be created with a palette.
			 * \param [in] width Width of sprite in pixels.
			 * \param [in] height Height of sprite in pixels.
			 * \param [in] mode A mode selector from which to create the sprite from.  */
			public SpriteByName (SpriteArea spriteArea,
					     string name,
					     bool palette,
					     int width,
					     int height,
					     OS.ModeSelector mode)
			{
				AreaCB = spriteArea;
				Name = name;

				AreaCB.CreateSprite (Name,
						     palette,
						     width,
						     height,
						     mode);
			}

			/*! \brief Render the sprite using the plot type given.  */
			public virtual void Plot (OSSpriteOp.PlotAction plotAction)
			{
				OSSpriteOp.PutSprite (AreaType.UserWithName,
						      AreaCB.BaseAddress,
						      Name,
						      plotAction);
			}

			/*! \brief Render sprite scaled.
			 * \param [in] x X coordinate at which to plot.
			 * \param [in] y Y coordinate at which to plot.
			 * \param [in] action Specifies how to plot the sprite.
			 * \param [in] factors The scale factors to use.
			 * \param [in] transTab A translation table.
			 * \return Nothing.  */
			public virtual void PlotScaled (int x,
							int y,
							OSSpriteOp.PlotAction action,
							OS.ScaleFactors factors,
							IntPtr transTab)
			{
				OSSpriteOp.PutSpriteScaled (AreaType.UserWithName,
							    AreaCB.BaseAddress,
							    Name,
							    x,
							    y,
							    action,
							    factors,
							    transTab);
			}

			/*! \brief Render sprite transformed, with source rectangle and matrix.
			 * \param [in] source Source rectangle in sprite.
			 * \param [in] action Specifies how to plot the sprite.
			 * \param [in] matrix Transformation matrix.
			 * \param [in] transTab A translation table (may be 0).
			 * \return Nothing.  */
			public virtual void PlotTransformed (OS.Rect source,
							     OSSpriteOp.PlotAction action,
							     OS.Matrix matrix,
							     IntPtr transTab)
			{
				TransformFlags t_flags = TransformFlags.SourceRectangle;

				OSSpriteOp.PutSpriteTransformed (AreaType.UserWithName,
								 AreaCB.BaseAddress,
								 Name,
								 t_flags,
								 source,
								 action,
								 matrix,
								 transTab);
			}

			/*! \brief Render sprite transformed, with matrix.
			 * \param [in] source Source rectangle in sprite.
			 * \param [in] action Specifies how to plot the sprite.
			 * \param [in] matrix Transformation matrix.
			 * \param [in] transTab A translation table (may be 0).
			 * \return Nothing.  */
			public virtual void PlotTransformed (OSSpriteOp.PlotAction action,
							     OS.Matrix matrix,
							     IntPtr transTab)
			{
				TransformFlags t_flags = 0;

				OSSpriteOp.PutSpriteTransformed (AreaType.UserWithName,
								 AreaCB.BaseAddress,
								 Name,
								 t_flags,
								 null,
								 action,
								 matrix,
								 transTab);
			}

			/*! \brief Render sprite transformed, with source rectangle and destination
			 * coordinate block.
			 * \param [in] source Source rectangle in sprite.
			 * \param [in] action Specifies how to plot the sprite.
			 * \param [in] destCoordBlock Destination coordinate block.
			 * \param [in] transTab A translation table (may be 0).
			 * \return Nothing.  */
			public virtual void PlotTransformed (OS.Rect source,
							     OSSpriteOp.PlotAction action,
							     DestCoordBlock destCoordBlock,
							     IntPtr transTab)
			{
				TransformFlags t_flags = TransformFlags.SourceRectangle |
							 TransformFlags.DestinationCoords;

				OSSpriteOp.PutSpriteTransformed (AreaType.UserWithName,
								 AreaCB.BaseAddress,
								 Name,
								 t_flags,
								 source,
								 action,
								 destCoordBlock,
								 transTab);
			}

			/*! \brief Render sprite transformed, with destination coordinate block.
			 * \param [in] action Specifies how to plot the sprite.
			 * \param [in] destCoordBlock Destination coordinate block.
			 * \param [in] transTab A translation table (may be 0).
			 * \return Nothing.  */
			public virtual void PlotTransformed (OSSpriteOp.PlotAction action,
							     DestCoordBlock destCoordBlock,
							     IntPtr transTab)
			{
				TransformFlags t_flags = TransformFlags.DestinationCoords;

				OSSpriteOp.PutSpriteTransformed (AreaType.UserWithName,
								 AreaCB.BaseAddress,
								 Name,
								 t_flags,
								 null,
								 action,
								 destCoordBlock,
								 transTab);
			}

			/*! \brief Redirect %OS graphics operations to this sprite.
			 * \return An object containing values that are required to be passed to
			 * SwitchFrom when redirection is finished with.  */
			public virtual RedirectContext SwitchTo (IntPtr saveArea)
			{
				var context = new OSSpriteOp.RedirectContext ();

				OSSpriteOp.SwitchOutputToSprite (AreaType.UserWithName,
								 AreaCB.BaseAddress,
								 Name,
								 saveArea,
								 out context.r0,
								 out context.r1,
								 out context.r2,
								 out context.r3);
				return context;
			}

			/*! \brief Restore %OS graphics operations back to the previous context.
			 * \param [in] context The object that was previously returned from
			 * SwitchTo.  */
			public void SwitchFrom (RedirectContext context)
			{
				OSSpriteOp.UnswitchOutput (context.r0,
							   context.r1,
							   context.r2,
							   context.r3);
			}
		}

		/*! \brief Encapsulate a sprite via its address.
		 * \note Care must be taken as to where the sprite is created. In managed memory
		 * the sprite area is likely to move making the sprite address invalid. Also, any
		 * sprite operations that change the order of sprites contained within it will
		 * also invalidate a sprite referenced by an object of this type.<br>
		 * SpriteByPointer.Refresh can be used to obtain the sprites new address.
		 * \note As this class derives from SpriteByName, the name of the sprite can also
		 * be used by accessing methods via its base.  */
		public class SpriteByPointer : SpriteByName
		{
			IntPtr SpritePointer;

			/*! \brief Create a sprite that is referenced by a pointer rather than its
			 * name.
			 * \param [in] spriteArea The sprite area in which to create the sprite.
			 * \param [in] name Name of the sprite to create.
			 * \param [in] palette \e true if the sprite should be created with a palette.
			 * \param [in] width Width of sprite in pixels.
			 * \param [in] height Height of sprite in pixels.
			 * \param [in] mode Either a mode number or a sprite type.
			 * \note OSSpriteOp.GenerateOldSpriteType can be used to generate the \e mode
			 * parameter.  */
			public SpriteByPointer (SpriteArea spriteArea, string name, bool palette,
						int width, int height, int mode)
				 : base (spriteArea, name, palette, width, height, mode)
			{
				Refresh ();
			}

			/*! \brief Create a sprite that is referenced by a pointer rather than its
			 * name.
			 * \param [in] spriteArea The sprite area in which to create the sprite.
			 * \param [in] name Name of the sprite to create.
			 * \param [in] palette \e true if the sprite should be created with a palette.
			 * \param [in] width Width of sprite in pixels.
			 * \param [in] height Height of sprite in pixels.
			 * \param [in] mode A mode selector from which to create the sprite from.  */
			public SpriteByPointer (SpriteArea spriteArea, string name, bool palette,
						int width, int height, OS.ModeSelector mode)
				 : base (spriteArea, name, palette, width, height, mode)
			{
				Refresh ();
			}

			/*! \brief Refresh the sprite pointer.
			 * Used when the sprite position may have changed due, e.g., to another being
			 * deleted, etc.  */
			public void Refresh ()
			{
				SpritePointer = OSSpriteOp.SelectSprite (AreaType.UserWithName,
									 AreaCB.BaseAddress,
									 Name);
			}

			/*! \brief Render this sprite using the plot type given.  */
			public override void Plot (OSSpriteOp.PlotAction plotAction)
			{
				OSSpriteOp.PutSprite (AreaType.UserWithPointer,
						      AreaCB.BaseAddress,
						      SpritePointer,
						      plotAction);
			}


			/*! \brief Render sprite scaled.
			 * \param [in] x X coordinate at which to plot.
			 * \param [in] y Y coordinate at which to plot.
			 * \param [in] action Specifies how to plot the sprite.
			 * \param [in] factors The scale factors to use.
			 * \param [in] transTab A translation table.
			 * \return Nothing.  */
			public override void PlotScaled (int x,
							 int y,
							 OSSpriteOp.PlotAction action,
							 OS.ScaleFactors factors,
							 IntPtr transTab)
			{
				OSSpriteOp.PutSpriteScaled (AreaType.UserWithPointer,
							    AreaCB.BaseAddress,
							    SpritePointer,
							    x,
							    y,
							    action,
							    factors,
							    transTab);
			}

			/*! \brief Render sprite transformed, with source rectangle and matrix.
			 * \param [in] source Source rectangle in sprite.
			 * \param [in] action Specifies how to plot the sprite.
			 * \param [in] matrix Transformation matrix.
			 * \param [in] transTab A translation table (may be 0).
			 * \return Nothing.  */
			public override void PlotTransformed (OS.Rect source,
							      OSSpriteOp.PlotAction action,
							      OS.Matrix matrix,
							      IntPtr transTab)
			{
				TransformFlags t_flags = TransformFlags.SourceRectangle;

				OSSpriteOp.PutSpriteTransformed (AreaType.UserWithPointer,
								 AreaCB.BaseAddress,
								 SpritePointer,
								 t_flags,
								 source,
								 action,
								 matrix,
								 transTab);
			}

			/*! \brief Render sprite transformed, with matrix.
			 * \param [in] action Specifies how to plot the sprite.
			 * \param [in] matrix Transformation matrix.
			 * \param [in] transTab A translation table (may be 0).
			 * \return Nothing.  */
			public override void PlotTransformed (OSSpriteOp.PlotAction action,
							      OS.Matrix matrix,
							      IntPtr transTab)
			{
				OSSpriteOp.PutSpriteTransformed (AreaType.UserWithPointer,
								 AreaCB.BaseAddress,
								 SpritePointer,
								 0,
								 null,
								 action,
								 matrix,
								 transTab);
			}

			/*! \brief Render sprite transformed, with source rectangle and destination
			 * coordinate block.
			 * \param [in] source Source rectangle in sprite.
			 * \param [in] action Specifies how to plot the sprite.
			 * \param [in] destCoordBlock Destination coordinate block.
			 * \param [in] transTab A translation table (may be 0).
			 * \return Nothing.  */
			public override void PlotTransformed (OS.Rect source,
							      OSSpriteOp.PlotAction action,
							      DestCoordBlock destCoordBlock,
							      IntPtr transTab)
			{
				TransformFlags t_flags = TransformFlags.SourceRectangle |
							 TransformFlags.DestinationCoords;

				OSSpriteOp.PutSpriteTransformed (AreaType.UserWithPointer,
								 AreaCB.BaseAddress,
								 SpritePointer,
								 t_flags,
								 source,
								 action,
								 destCoordBlock,
								 transTab);
			}

			/*! \brief Render sprite transformed, with destination coordinate block.
			 * \param [in] action Specifies how to plot the sprite.
			 * \param [in] destCoordBlock Destination coordinate block.
			 * \param [in] transTab A translation table (may be 0).
			 * \return Nothing.  */
			public override void PlotTransformed (OSSpriteOp.PlotAction action,
							      DestCoordBlock destCoordBlock,
							      IntPtr transTab)
			{
				TransformFlags t_flags = TransformFlags.DestinationCoords;

				OSSpriteOp.PutSpriteTransformed (AreaType.UserWithPointer,
								 AreaCB.BaseAddress,
								 SpritePointer,
								 t_flags,
								 null,
								 action,
								 destCoordBlock,
								 transTab);
			}

			/*! \brief Switch %OS graphics operations to this sprite.
			 * \return An object containing values that are required to be passed to
			 * SwitchFrom when redirection is finished with.  */
			public override RedirectContext SwitchTo (IntPtr saveArea)
			{
				var context = new OSSpriteOp.RedirectContext ();

				OSSpriteOp.SwitchOutputToSprite (AreaType.UserWithPointer,
								 AreaCB.BaseAddress,
								 SpritePointer,
								 saveArea,
								 out context.r0,
								 out context.r1,
								 out context.r2,
								 out context.r3);
				return context;
			}
		}

		/*! \brief Convenience class that can be used to collect the output from
		 * OSSpriteOp.SwitchOutputToSprite.  */
		public class RedirectContext
		{
			public IntPtr r0;
			public IntPtr r1;
			public IntPtr r2;
			public IntPtr r3;
		}

		/*! \brief Initialise a sprite area control block.  */
		public static void InitialiseArea (IntPtr spriteAreaCB)
		{
			OS.ThrowOnError (NativeMethods.OSSpriteOp_InitialiseArea (AreaType.UserWithName,
										  spriteAreaCB));
		}

		/*! \brief Create a sprite using a mode number of sprite type.
		 * \param [in] flags Indicates the type of sprite area and sprite pointer given.
		 * \param [in] spriteAreaCB Pointer to the sprite area control block.
		 * \param [in] spriteName Name of the sprite.
		 * \param [in] createPalette \e true if the sprite should be created with a palette.
		 * \param [in] width Width of sprite in pixels.
		 * \param [in] height Height of sprite in pixels.
		 * \param [in] mode Either a mode number or a sprite type.
		 * \return Nothing.
		 * \note OSSpriteOp.GenerateOldSpriteType () can be used to generate the \e mode
		 * parameter.  */
		public static void CreateSprite (OSSpriteOp.AreaType flags,
						 IntPtr spriteAreaCB,
						 string spriteName,
						 bool createPalette,
						 int width,
						 int height,
						 int mode)
		{
			OS.ThrowOnError (NativeMethods.OSSpriteOp_CreateSprite (flags,
										spriteAreaCB,
										spriteName,
										createPalette,
										width,
										height,
										mode));
		}

		/*! \brief Create a sprite using a mode selector.
		 * \param [in] flags Indicates the type of sprite area and sprite pointer given.
		 * \param [in] spriteAreaCB Pointer to the sprite area control block.
		 * \param [in] spriteName Name of the sprite.
		 * \param [in] createPalette \e true if the sprite should be created with a palette.
		 * \param [in] width Width of sprite in pixels.
		 * \param [in] height Height of sprite in pixels.
		 * \param [in] mode A mode selector from which to create the sprite from.
		 * \return Nothing.  */
		public static void CreateSprite (OSSpriteOp.AreaType flags,
						 IntPtr spriteAreaCB,
						 string spriteName,
						 bool createPalette,
						 int width,
						 int height,
						 OS.ModeSelector mode)
		{
			OS.ThrowOnError (NativeMethods.OSSpriteOp_CreateSprite (flags,
										spriteAreaCB,
										spriteName,
										createPalette,
										width,
										height,
										mode));
		}

		/*! \brief Save the given sprite area as a file.
		 * \param [in] flags Indicates the type of sprite area and sprite pointer given.
		 * \param [in] spriteAreaCB Pointer to the sprite area control block.
		 * \param [in] filename Name of file to save sprites as.
		 * \return Nothing.  */
		public static void SaveSpriteFile (OSSpriteOp.AreaType flags,
						   IntPtr spriteAreaCB,
						   string filename)
		{
			OS.ThrowOnError (NativeMethods.OSSpriteOp_SaveSpriteFile (flags,
										  spriteAreaCB,
										  filename));
		}

		/*! \brief Redirect OS graphics operations to the given sprite.
		 * \param [in] flags Indicates the type of sprite area and sprite pointer given.
		 * \param [in] spriteAreaCB Pointer to the sprite area control block.
		 * \param [in] spriteName Name of the sprite.
		 * \param [in] saveArea Pointer to save area (0 for no save area, 1 for system save area).
		 * \param [out] r0_out Value of r0 on exit.
		 * \param [out] r1_out Value of r1 on exit.
		 * \param [out] r2_out Value of r2 on exit.
		 * \param [out] r3_out Value of r3 on exit.
		 * \return Nothing.  */
		public static void SwitchOutputToSprite (OSSpriteOp.AreaType flags,
							 IntPtr spriteAreaCB,
							 string spriteName,
							 IntPtr saveArea,
							 out IntPtr r0_out,
							 out IntPtr r1_out,
							 out IntPtr r2_out,
							 out IntPtr r3_out)
		{
			OS.ThrowOnError (NativeMethods.OSSpriteOp_SwitchOutputToSprite (flags,
											spriteAreaCB,
											spriteName,
											saveArea,
											out r0_out,
											out r1_out,
											out r2_out,
											out r3_out));
		}

		/*! \brief Redirect OS graphics operations to the given sprite.
		 * \param [in] flags Indicates the type of sprite area and sprite pointer given.
		 * \param [in] spriteAreaCB Pointer to the sprite area control block.
		 * \param [in] spriteAddress Pointer to the sprite.
		 * \param [in] saveArea Pointer to save area (0 for no save area, 1 for system save area).
		 * \param [out] r0_out Value of r0 on exit.
		 * \param [out] r1_out Value of r1 on exit.
		 * \param [out] r2_out Value of r2 on exit.
		 * \param [out] r3_out Value of r3 on exit.
		 * \return Nothing.  */
		public static void SwitchOutputToSprite (OSSpriteOp.AreaType flags,
							 IntPtr spriteAreaCB,
							 IntPtr spriteAddress,
							 IntPtr saveArea,
							 out IntPtr r0_out,
							 out IntPtr r1_out,
							 out IntPtr r2_out,
							 out IntPtr r3_out)
		{
			OS.ThrowOnError (NativeMethods.OSSpriteOp_SwitchOutputToSprite (flags,
											spriteAreaCB,
											spriteAddress,
											saveArea,
											out r0_out,
											out r1_out,
											out r2_out,
											out r3_out));
		}

		/*! \brief Redirect OS graphics operations back to a previous context (i.e., another
		 * sprite or the screen).
		 * \param [in] r0 Value of r0 previously saved by OSSpriteOp.SwitchOutputToSprite.
		 * \param [in] r1 Value of r1 previously saved by OSSpriteOp.SwitchOutputToSprite.
		 * \param [in] r2 Value of r2 previously saved by OSSpriteOp.SwitchOutputToSprite.
		 * \param [in] r3 Value of r3 previously saved by OSSpriteOp.SwitchOutputToSprite.
		 * \return Nothing.  */
		public static void UnswitchOutput (IntPtr r0, IntPtr r1, IntPtr r2, IntPtr r3)
		{
			OS.ThrowOnError (NativeMethods.OSSpriteOp_UnswitchOutput (r0, r1, r2, r3));
		}

		/*! \brief Select a sprite.
		 * \param [in] flags Indicates the type of sprite area and sprite pointer given.
		 * \param [in] spriteAreaCB Pointer to the sprite area control block.
		 * \param [in] spriteName Name of the sprite.
		 * \return The address of the named sprite.  */
		public static IntPtr SelectSprite (OSSpriteOp.AreaType flags,
						   IntPtr spriteAreaCB,
						   string spriteName)
		{
			IntPtr sprite_address;

			OS.ThrowOnError (NativeMethods.OSSpriteOp_SelectSprite (flags,
										spriteAreaCB,
										spriteName,
										out sprite_address));
			return sprite_address;
		}

		/*! \brief Render a sprite.
		 * \param [in] flags Indicates the type of sprite area and sprite pointer given.
		 * \param [in] spriteAreaCB Pointer to the sprite area control block.
		 * \param [in] spriteName Name of the sprite.
		 * \param [in] plotAction Indicates how the sprite should be plotted.
		 * \return Nothing.  */
		public static void PutSprite (OSSpriteOp.AreaType flags,
					      IntPtr spriteAreaCB,
					      string spriteName,
					      OSSpriteOp.PlotAction plotAction)
		{
			OS.ThrowOnError (NativeMethods.OSSpriteOp_PutSprite (flags,
									     spriteAreaCB,
									     spriteName,
									     plotAction));
		}

		/*! \brief Render a sprite.
		 * \param [in] flags Indicates the type of sprite area and sprite pointer given.
		 * \param [in] spriteAreaCB Pointer to the sprite area control block.
		 * \param [in] spriteAddress Pointer to the sprite.
		 * \param [in] plotAction Indicates how the sprite should be plotted.
		 * \return Nothing.  */
		public static void PutSprite (OSSpriteOp.AreaType flags,
					      IntPtr spriteAreaCB,
					      IntPtr spriteAddress,
					      OSSpriteOp.PlotAction plotAction)
		{
			OS.ThrowOnError (NativeMethods.OSSpriteOp_PutSprite (flags,
									     spriteAreaCB,
									     spriteAddress,
									     plotAction));
		}

		/*! \brief Render sprite scaled.
		 * \param [in] flags Indicates the type of sprite area and sprite pointer given.
		 * \param [in] spriteAreaCB Pointer to the sprite area control block.
		 * \param [in] spriteAddress Pointer to the sprite.
		 * \param [in] x X coordinate at which to plot.
		 * \param [in] y Y coordinate at which to plot.
		 * \param [in] action Specifies how to plot the sprite.
		 * \param [in] factors The scale factors to use.
		 * \param [in] transTab A translation table.
		 * \return Nothing.  */
		public static void PutSpriteScaled (OSSpriteOp.AreaType flags,
						    IntPtr spriteAreaCB,
						    IntPtr spriteAddress,
						    int x,
						    int y,
						    OSSpriteOp.PlotAction action,
						    OS.ScaleFactors factors,
						    IntPtr transTab)
		{
			OS.ThrowOnError (NativeMethods.OSSpriteOp_PutSpriteScaled (flags,
										   spriteAreaCB,
										   spriteAddress,
										   x,
										   y,
										   action,
										   factors,
										   transTab));
		}


		/*! \brief Render sprite scaled.
		 * \param [in] flags Indicates the type of sprite area and sprite pointer given.
		 * \param [in] spriteAreaCB Pointer to the sprite area control block.
		 * \param [in] spriteName Name of the sprite.
		 * \param [in] x X coordinate at which to plot.
		 * \param [in] y Y coordinate at which to plot.
		 * \param [in] action Specifies how to plot the sprite.
		 * \param [in] factors The scale factors to use.
		 * \param [in] transTab A translation table.
		 * \return Nothing.  */
		public static void PutSpriteScaled (OSSpriteOp.AreaType flags,
						    IntPtr spriteAreaCB,
						    string spriteName,
						    int x,
						    int y,
						    OSSpriteOp.PlotAction action,
						    OS.ScaleFactors factors,
						    IntPtr transTab)
		{
			OS.ThrowOnError (NativeMethods.OSSpriteOp_PutSpriteScaled (flags,
										   spriteAreaCB,
										   spriteName,
										   x,
										   y,
										   action,
										   factors,
										   transTab));
		}

		/*! \brief Render sprite transformed.
		 * \param [in] flags Indicates the type of sprite area and sprite pointer given.
		 * \param [in] spriteAreaCB Pointer to the sprite area control block.
		 * \param [in] spriteAddress Pointer to the sprite.
		 * \param [in] transformflags Flags indicating how r4 and r6 should be interpreted.
		 * \param [in] source Source rectangle in sprite (if R3 bit 1 set).
		 * \param [in] action Specifies how to plot the sprite.
		 * \param [in] matrix Transformation matrix (if R3 bit 0 clear).
		 * \param [in] transTab A translation table.
		 * \return Nothing.  */
		public static void PutSpriteTransformed (OSSpriteOp.AreaType flags,
							 IntPtr spriteAreaCB,
							 IntPtr spriteAddress,
							 TransformFlags transformFlags,
							 OS.Rect source,
							 OSSpriteOp.PlotAction action,
							 OS.Matrix matrix,
							 IntPtr transTab)
		{
			var native_matrix = new NativeOS.Matrix (matrix);

			OS.ThrowOnError (NativeMethods.OSSpriteOp_PutSpriteTransformed (flags,
											spriteAreaCB,
											spriteAddress,
											transformFlags,
											source,
											action,
											ref native_matrix,
											transTab));
		}


		/*! \brief Render sprite transformed.
		 * \param [in] flags Indicates the type of sprite area and sprite pointer given.
		 * \param [in] spriteAreaCB Pointer to the sprite area control block.
		 * \param [in] spriteName Name of the sprite.
		 * \param [in] transformflags Flags indicating how r4 and r6 should be interpreted.
		 * \param [in] source Source rectangle in sprite (if R3 bit 1 set).
		 * \param [in] action Specifies how to plot the sprite.
		 * \param [in] matrix Transformation matrix (if R3 bit 0 clear).
		 * \param [in] transTab A translation table.
		 * \return Nothing.  */
		public static void PutSpriteTransformed (OSSpriteOp.AreaType flags,
							 IntPtr spriteAreaCB,
							 string spriteName,
							 TransformFlags transformFlags,
							 OS.Rect source,
							 OSSpriteOp.PlotAction action,
							 OS.Matrix matrix,
							 IntPtr transTab)
		{
			var native_matrix = new NativeOS.Matrix (matrix);

			OS.ThrowOnError (NativeMethods.OSSpriteOp_PutSpriteTransformed (flags,
											spriteAreaCB,
											spriteName,
											transformFlags,
											source,
											action,
											ref native_matrix,
											transTab));
		}

		/*! \brief Render sprite transformed.
		 * \param [in] flags Indicates the type of sprite area and sprite pointer given.
		 * \param [in] spriteAreaCB Pointer to the sprite area control block.
		 * \param [in] spriteAddress Pointer to the sprite.
		 * \param [in] transformflags Flags indicating how r4 and r6 should be interpreted.
		 * \param [in] source Source rectangle in sprite (if R3 bit 1 set).
		 * \param [in] action Specifies how to plot the sprite.
		 * \param [in] destCoordBlock Destination coordinate block (if R3 bit 0 set).
		 * \param [in] transTab A translation table.
		 * \return Nothing.  */
		public static void PutSpriteTransformed (OSSpriteOp.AreaType flags,
							 IntPtr spriteAreaCB,
							 IntPtr spriteAddress,
							 TransformFlags transformFlags,
							 OS.Rect source,
							 OSSpriteOp.PlotAction action,
							 DestCoordBlock destCoordBlock,
							 IntPtr transTab)
		{
			OS.ThrowOnError (NativeMethods.OSSpriteOp_PutSpriteTransformed (flags,
											spriteAreaCB,
											spriteAddress,
											transformFlags,
											source,
											action,
											destCoordBlock,
											transTab));
		}

		/*! \brief Render sprite transformed.
		 * \param [in] flags Indicates the type of sprite area and sprite pointer given.
		 * \param [in] spriteAreaCB Pointer to the sprite area control block.
		 * \param [in] spriteName Name of the sprite.
		 * \param [in] transformflags Flags indicating how r4 and r6 should be interpreted.
		 * \param [in] source Source rectangle in sprite (if R3 bit 1 set).
		 * \param [in] action Specifies how to plot the sprite.
		 * \param [in] destCoordBlock Destination coordinate block (if R3 bit 0 set).
		 * \param [in] transTab A translation table.
		 * \return Nothing.  */
		public static void PutSpriteTransformed (OSSpriteOp.AreaType flags,
							 IntPtr spriteAreaCB,
							 string spriteName,
							 TransformFlags transformFlags,
							 OS.Rect source,
							 OSSpriteOp.PlotAction action,
							 DestCoordBlock destCoordBlock,
							 IntPtr transTab)
		{
			OS.ThrowOnError (NativeMethods.OSSpriteOp_PutSpriteTransformed (flags,
											spriteAreaCB,
											spriteName,
											transformFlags,
											source,
											action,
											destCoordBlock,
											transTab));
		}

		/*! \brief Generate an old style sprite type word that can be used to create
		 * a new sprite.
		 * \param [in] depth The number of bits per pixel.
		 * \param [in] horizontal The number of dots per inch horizontally.<br>
		 *		Possible values are 22, 23, 45, 90 and 180.
		 * \param [in] vertical The number of dots per inch vertically.<br>
		 *		Possible values are 22, 23, 45, 90 and 180.
		 * \param [in] wideMask \e true to enabled an 8 bit mask.
		 * \return The generated sprite type.  */
		// The OS StrongHelp manual is a bit confusing on this matter.
		public static int GenerateOldSpriteType (OS.PixelDepth depth,
							 SpriteDpi horizontal,
							 SpriteDpi vertical,
							 bool wideMask)
		{
			uint sprite_type = 1 | (((uint)depth + 1) << 27);

			sprite_type |= ((uint)horizontal & 0xfff) << 1;
			sprite_type |= ((uint)vertical & 0xfff) << 14;
			if (wideMask)
				sprite_type |= (1 << 32);

			return (int)sprite_type;
		}
	}
}
