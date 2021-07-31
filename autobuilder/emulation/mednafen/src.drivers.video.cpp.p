--- src/drivers/video.cpp.orig	2020-08-02 07:57:27.385133438 +0100
+++ src/drivers/video.cpp	2020-08-02 08:08:57.196237524 +0100
@@ -426,7 +426,9 @@
 static ScalerDefinition *CurrentScaler = NULL;
 
 static SDL_Surface *screen = NULL;
+#ifndef __riscos__
 static OpenGL_Blitter *ogl_blitter = NULL;
+#endif
 static SDL_Surface *IconSurface=NULL;
 
 static MDFN_Rect screen_dest_rect;
@@ -456,6 +458,7 @@
 static void ClearBackBuffer(void)
 {
  //printf("WOO: %u\n", Time::MonoMS());
+#ifndef __riscos__
  if(ogl_blitter)
  {
   ogl_blitter->ClearBackBuffer();
@@ -478,9 +481,12 @@
   else
 #endif
   {
+#endif
    SDL_FillRect(screen, NULL, 0);
+#ifndef __riscos__
   }
  }
+#endif
 }
 
 void Video_Kill(void)
@@ -504,13 +510,13 @@
   delete HelpSurface;
   HelpSurface = NULL;
  }
-
+#ifndef __riscos-_
  if(ogl_blitter)
  {
   delete ogl_blitter;
   ogl_blitter = NULL;
  }
-
+#endif
  if(vdriver == VDRIVER_OVERLAY)
   OV_Kill();
 
@@ -830,6 +836,7 @@
 
  vdriver = MDFN_GetSettingI("video.driver");
 
+#ifndef __riscos__
  if(vdriver == VDRIVER_OPENGL)
  {
   if(!sdlhaveogl)
@@ -866,6 +873,10 @@
  {
 
  }
+#else
+   sdlhaveogl = false;
+   vdriver = VDRIVER_SOFTSDL;
+#endif
 
  exs = _fullscreen ? _video.xscalefs : _video.xscale;
  eys = _fullscreen ? _video.yscalefs : _video.yscale;
@@ -972,7 +983,7 @@
   SDL_WM_SetCaption("Mednafen", "Mednafen");
 
  int rs, gs, bs, as;
-
+#ifndef __riscos__
  if(cur_flags & SDL_OPENGL)
  {
   try
@@ -987,6 +998,7 @@
  }
  else
  {
+#endif
   rs = screen->format->Rshift;
   gs = screen->format->Gshift;
   bs = screen->format->Bshift;
@@ -994,8 +1006,9 @@
   as = 0;
   while(as == rs || as == gs || as == bs) // Find unused 8-bits to use as our alpha channel
    as += 8;
+#ifndef __riscos__
  }
-
+#endif
  //printf("%d %d %d %d\n", rs, gs, bs, as);
 
  SDL_ShowCursor(0);
@@ -1075,15 +1088,16 @@
  {
   ClearBackBuffer();
 
+#ifndef __riscos__
   if(cur_flags & SDL_OPENGL)
   {
    SDL_GL_SwapBuffers();
    //ogl_blitter->HardSync();
   }
   else
+#endif
    SDL_Flip(screen);
  }
-
  MarkNeedBBClear();
 }
 
@@ -1108,16 +1122,19 @@
 
 void BlitRaw(MDFN_Surface *src, const MDFN_Rect *src_rect, const MDFN_Rect *dest_rect, int source_alpha)
 {
+#ifndef __riscos__
  if(ogl_blitter)
   ogl_blitter->BlitRaw(src, src_rect, dest_rect, (source_alpha != 0) && osd_alpha_blend);
  else
  {
+#endif
   SDL_to_MDFN_Surface_Wrapper m_surface(screen);
 
   //MDFN_SrcAlphaBlitSurface(src, src_rect, &m_surface, dest_rect);
   MDFN_StretchBlitSurface(src, *src_rect, &m_surface, *dest_rect, (source_alpha > 0) && osd_alpha_blend);
+#ifndef __riscos__
  }
-
+#endif
  bool cond1 = (dest_rect->x < screen_dest_rect.x || (dest_rect->x + dest_rect->w) > (screen_dest_rect.x + screen_dest_rect.w));
  bool cond2 = (dest_rect->y < screen_dest_rect.y || (dest_rect->y + dest_rect->h) > (screen_dest_rect.y + screen_dest_rect.h));
 
@@ -1287,10 +1304,12 @@
     }
 #endif
 
+#ifndef __riscos__
     if(ogl_blitter)
      ogl_blitter->Blit(&bah_surface, &boohoo_rect, &dest_rect, &eff_src_rect, InterlaceField, evideoip, CurGame->rotated);
     else
     {
+#endif
      if(OverlayOK)
      {
       SDL_Rect tr;
@@ -1308,14 +1327,18 @@
 
       MDFN_StretchBlitSurface(&bah_surface, boohoo_rect, &m_surface, dest_rect, false, _video.scanlines, &eff_src_rect, CurGame->rotated, InterlaceField);
      }
+#ifndef __riscos__
     }
+#endif
    }
    else // No special scaler:
    {
+#ifndef __riscos__
     if(ogl_blitter)
      ogl_blitter->Blit(eff_source_surface, &eff_src_rect, &dest_rect, &eff_src_rect, InterlaceField, evideoip, CurGame->rotated);
     else
     {
+#endif
      if(OverlayOK)
      {
       SDL_Rect tr;
@@ -1333,7 +1356,9 @@
 
       MDFN_StretchBlitSurface(eff_source_surface, eff_src_rect, &m_surface, dest_rect, false, _video.scanlines, &eff_src_rect, CurGame->rotated, InterlaceField);
      }
+#ifndef __riscos__
     }
+#endif
    }
 }
 
@@ -1511,11 +1536,12 @@
    if(sr.h > screen->h) sr.h = screen->h;
 
    ib.reset(new MDFN_Surface(NULL, sr.w, sr.h, sr.w, MDFN_PixelFormat(MDFN_COLORSPACE_RGB, real_rs, real_gs, real_bs, real_as)));
-
+#ifndef __riscos__
    if(ogl_blitter)
     ogl_blitter->ReadPixels(ib.get(), &sr);
    else
    {
+#endif
     if(SDL_MUSTLOCK(screen))
      SDL_LockSurface(screen);
 
@@ -1529,8 +1555,9 @@
 
     if(SDL_MUSTLOCK(screen))
      SDL_UnlockSurface(screen);
+#ifndef __riscos__
    }
-
+#endif
 
    tr.x = tr.y = 0;
    tr.w = ib->w;
