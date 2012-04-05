--- src/drivers/video.cpp.orig	2012-03-31 09:12:47.000000000 +0100
+++ src/drivers/video.cpp	2012-03-31 09:12:54.000000000 +0100
@@ -119,14 +119,18 @@

 void ClearBackBuffer(void)
 {
+#ifndef __riscos__
  if(cur_flags & SDL_OPENGL)
  {
   ClearBackBufferOpenGL(screen);
  }
  else
  {
+#endif
   SDL_FillRect(screen, NULL, 0);
+#ifndef __riscos__
  }
+#endif
 }

 /* Return 1 if video was killed, 0 otherwise(video wasn't initialized). */
@@ -175,8 +179,10 @@
   NetSurface = NULL;
  }

+#ifndef __riscos__
  if(cur_flags & SDL_OPENGL)
   KillOpenGL();
+#endif

  VideoGI = NULL;
  cur_flags = 0;
@@ -354,6 +360,7 @@
   exit(-1);
  }

+#ifndef __riscos__
  if(vdriver == 0)
  {
   if(!sdlhaveogl)
@@ -374,8 +381,10 @@
   }
  }

+
  if(vdriver == 0)
  {
+#endif
   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1 );

   if(MDFN_GetSettingB("glvsync"))
@@ -384,8 +393,10 @@
    SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
    #endif
   }
+#ifndef __riscos__
  }
  else
+#endif
   flags |= SDL_DOUBLEBUF;

  exres = _video.xres;
@@ -463,6 +474,7 @@
   MDFN_indent(-1);
   return(0);
  }
+#ifndef __riscos__
  MDFN_printf(_("OpenGL: %s\n"), (cur_flags & SDL_OPENGL) ? _("Yes") : _("No"));

  if(cur_flags & SDL_OPENGL)
@@ -471,6 +483,7 @@
   MDFN_printf(_("Pixel shader: %s\n"), _video.pixshader.c_str());
   MDFN_indent(-1);
  }
+#endif

  MDFN_printf(_("Fullscreen: %s\n"), _fullscreen ? _("Yes") : _("No"));
  MDFN_printf(_("Special Scaler: %s\n"), _video.special ? Scalers[_video.special - 1].name : _("None"));
@@ -483,9 +496,12 @@
  if(dest_rect.x < 0 || dest_rect.y < 0 || (dest_rect.x + dest_rect.w) > screen->w || (dest_rect.y + dest_rect.h) > screen->h)
  {
   MDFN_indent(1);
+#ifndef __riscos__
   if(cur_flags & SDL_OPENGL)
    MDFN_printf(_("Warning:  Destination rectangle exceeds screen dimensions.  This is ok if you really do want the clipping...\n"));
-  else // NonOpenGL mode doesn't support this kind of screen overflow clipping
+  else
+#endif
+   // NonOpenGL mode doesn't support this kind of screen overflow clipping
    MDFN_printf(_("Warning:  Destination rectangle exceeds screen dimensions.  The rectangle will be adjusted to fit within the screen area.\n"));
   MDFN_indent(-1);
  }
@@ -509,6 +525,7 @@
  #endif
  SDL_WM_SetIcon(IconSurface,0);

+#ifndef __riscos__
  if(cur_flags & SDL_OPENGL)
  {
   if(!InitOpenGL(evideoip, _video.scanlines, _video.pixshader, screen))
@@ -524,12 +541,14 @@


  }
+#endif

  MDFN_indent(-1);
  SDL_ShowCursor(0);

  int rs, gs, bs, as;

+#ifndef __riscos__
  if(cur_flags & SDL_OPENGL)
  {
   #ifdef LSB_FIRST
@@ -546,6 +565,7 @@
  }
  else
  {
+#endif
   rs = screen->format->Rshift;
   gs = screen->format->Gshift;
   bs = screen->format->Bshift;
@@ -553,7 +573,9 @@
   as = 0;
   while(as == rs || as == gs || as == bs) // Find unused 8-bits to use as our alpha channel
    as += 8;
+#ifndef __riscos__
  }
+#endif

  real_rs = rs;
  real_gs = gs;
@@ -619,9 +641,11 @@
  {
   ClearBackBuffer();

+#ifndef __riscos__
   if(cur_flags & SDL_OPENGL)
    FlipOpenGL();
   else
+#endif
    SDL_Flip(screen);
  }

@@ -644,14 +668,18 @@

 void BlitRaw(SDL_Surface *src, SDL_Rect *src_rect, SDL_Rect *dest_rect)
 {
+#ifndef __riscos__
  if(cur_flags & SDL_OPENGL)
   BlitOpenGLRaw(src, src_rect, dest_rect);
  else
  {
+#endif
   BlitNonGL(src, src_rect, src_rect, dest_rect, screen, 0, 0);

   //SDL_BlitSurface(src, src_rect, screen, dest_rect);
+#ifndef __riscos__
  }
+#endif
  NeedClear = 2;
 }

@@ -780,18 +808,22 @@
      }
     }

+#ifndef __riscos__
     if(cur_flags & SDL_OPENGL)
      BlitOpenGL(bah_surface, &boohoo_rect, &dest_rect, &eff_src_rect, alpha_blend);
     else
+#endif
      BlitNonGL(bah_surface, &boohoo_rect, &eff_src_rect, &dest_rect, screen, _video.scanlines, CurGame->rotated);

     SDL_FreeSurface(bah_surface);
    }
    else // No special scaler:
    {
+#ifndef __riscos__
     if(cur_flags & SDL_OPENGL)
      BlitOpenGL(eff_source_surface, &eff_src_rect, &dest_rect, &eff_src_rect, alpha_blend);
     else
+#endif
      BlitNonGL(eff_source_surface, &eff_src_rect, &eff_src_rect, &dest_rect, screen, _video.scanlines, CurGame->rotated);
    }

@@ -981,10 +1013,14 @@

  FPS_Draw(screen, real_rs, real_gs, real_bs, real_as);

+#ifndef __riscos__
  if(!(cur_flags & SDL_OPENGL))
+#endif
   SDL_Flip(screen);
+#ifndef __riscos__
  else
   FlipOpenGL();
+#endif
 }

 void PtoV(int *x, int *y)
