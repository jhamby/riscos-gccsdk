--- coders/spr.c.orig	2012-11-17 10:42:45.226837622 +0000
+++ coders/spr.c	2012-11-16 17:51:18.770000000 +0000
@@ -0,0 +1,460 @@
+/*
+%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
+%                                                                             %
+%                                                                             %
+%                                                                             %
+%                            SSSSS PPPP  RRRR                                 %
+%                            SS    P   P R   R                                %
+%                             SSS  PPPP  RRRR                                 %
+%                               SS P     R R                                  %
+%                            SSSSS P     R  R                                 %
+%                                                                             %
+%                                                                             %
+%                        Read/Write SPR RISC OS Sprite Image Format           %
+%                                                                             %
+%                              Software Design                                %
+%                                John Cristy                                  %
+%                                 July 1992                                   %
+%                                                                             %
+%                                                                             %
+%  Copyright 1999-2012 ImageMagick Studio LLC, a non-profit organization      %
+%  dedicated to making software imaging solutions freely available.           %
+%                                                                             %
+%  You may not use this file except in compliance with the License.  You may  %
+%  obtain a copy of the License at                                            %
+%                                                                             %
+%    http://www.imagemagick.org/script/license.php                            %
+%                                                                             %
+%  Unless required by applicable law or agreed to in writing, software        %
+%  distributed under the License is distributed on an "AS IS" BASIS,          %
+%  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   %
+%  See the License for the specific language governing permissions and        %
+%  limitations under the License.                                             %
+%                                                                             %
+%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
+%
+%
+*/
+
+/*
+  Include declarations.
+*/
+#include "magick/studio.h"
+#include "magick/blob.h"
+#include "magick/blob-private.h"
+#include "magick/cache.h"
+#include "magick/colorspace.h"
+#include "magick/colorspace-private.h"
+#include "magick/exception.h"
+#include "magick/exception-private.h"
+#include "magick/image.h"
+#include "magick/image-private.h"
+#include "magick/list.h"
+#include "magick/magick.h"
+#include "magick/memory_.h"
+#include "magick/monitor.h"
+#include "magick/monitor-private.h"
+#include "magick/quantum-private.h"
+#include "magick/static.h"
+#include "magick/string_.h"
+#include "magick/module.h"
+
+/*
+  Forward declarations.
+*/
+static MagickBooleanType
+  WriteSPRImage(const ImageInfo *,Image *);
+
+
+/*
+%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
+%                                                                             %
+%                                                                             %
+%                                                                             %
+%   R e a d S P R I m a g e                                                   %
+%                                                                             %
+%                                                                             %
+%                                                                             %
+%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
+%
+%  ReadSPRImage() reads an SPR X image file and returns it.  It
+%  allocates the memory necessary for the new Image structure and returns a
+%  pointer to the new image.
+%
+%  The format of the ReadSPRImage method is:
+%
+%      Image *ReadSPRImage(const ImageInfo *image_info,ExceptionInfo *exception)
+%
+%  A description of each parameter follows:
+%
+%    o image_info: the image info.
+%
+%    o exception: return any errors or warnings in this structure.
+%
+*/
+static Image *ReadSPRImage(const ImageInfo *image_info,ExceptionInfo *exception)
+{
+
+  typedef struct _SPRInfo
+  {
+    int sprite_count;
+    int first;
+    int used;
+
+    int size;
+    char name[12];
+    int width;
+    int height;
+    int left_bit;
+    int right_bit;
+    int image;
+    int mask;
+    int mode;
+  } SPRInfo;
+
+  Image
+    *image;
+
+  MagickBooleanType
+    status;
+
+  register PixelPacket
+    *q;
+
+  register ssize_t
+    x;
+
+  register unsigned char
+    *p;
+
+  size_t
+    height,
+    length,
+    width;
+
+  ssize_t
+    count,
+    y;
+
+  SPRInfo
+    spr_info;
+
+  unsigned char
+    *pixels;
+
+  /*
+    Open image file.
+  */
+  assert(image_info != (const ImageInfo *) NULL);
+  assert(image_info->signature == MagickSignature);
+  if (image_info->debug != MagickFalse)
+    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",
+      image_info->filename);
+  assert(exception != (ExceptionInfo *) NULL);
+  assert(exception->signature == MagickSignature);
+  image=AcquireImage(image_info);
+  status=OpenBlob(image_info,image,ReadBinaryBlobMode,exception);
+  if (status == MagickFalse)
+    {
+      image=DestroyImageList(image);
+      return((Image *) NULL);
+    }
+  /*
+    Read SPR header information.
+  */
+  spr_info.sprite_count=ReadBlobLSBLong(image);
+  spr_info.first=ReadBlobLSBLong(image);
+  spr_info.used=ReadBlobLSBLong(image);
+  spr_info.size=ReadBlobLSBLong(image);
+  count=ReadBlob(image, 12, spr_info.name);
+  spr_info.width=ReadBlobLSBLong(image);
+  spr_info.height=ReadBlobLSBLong(image);
+  spr_info.left_bit=ReadBlobLSBLong(image);
+  spr_info.right_bit=ReadBlobLSBLong(image);
+  spr_info.image=ReadBlobLSBLong(image);
+  spr_info.mask=ReadBlobLSBLong(image);
+  spr_info.mode=ReadBlobLSBLong(image);
+
+   if (EOFBlob(image) != MagickFalse)
+    ThrowReaderException(CorruptImageError,"ImproperImageHeader");
+  if ((spr_info.width == 0UL) || (spr_info.height == 0UL))
+    ThrowReaderException(CorruptImageError,"ImproperImageHeader");
+
+    /*
+      Convert SPR raster image to pixel packets.
+    */
+    image->columns=spr_info.width + 1;
+    image->rows=spr_info.height + 1;
+    image->depth=8;
+    pixels=(unsigned char *) AcquireQuantumMemory(image->columns,
+      4*sizeof(*pixels));
+    if (pixels == (unsigned char *) NULL)
+      ThrowReaderException(ResourceLimitError,"MemoryAllocationFailed");
+    length=(size_t) 4*image->columns;
+    for (y=0; y < (ssize_t) image->rows; y++)
+    {
+      count=ReadBlob(image,length,pixels);
+      if ((size_t) count != length)
+        ThrowReaderException(CorruptImageError,"UnableToReadImageData");
+      p=pixels;
+      q=QueueAuthenticPixels(image,0,y,image->columns,1,exception);
+      if (q == (PixelPacket *) NULL)
+        break;
+      for (x=0; x < (ssize_t) image->columns; x++)
+      {
+        SetPixelRed(q,ScaleCharToQuantum(*p++));
+        SetPixelGreen(q,ScaleCharToQuantum(*p++));
+        SetPixelBlue(q,ScaleCharToQuantum(*p++));
+        *p++;
+ //       SetPixelAlpha(q,ScaleCharToQuantum(*p++));
+
+//        if (q->opacity != OpaqueOpacity)
+//          image->matte=MagickTrue;
+        q++;
+      }
+      if (SyncAuthenticPixels(image,exception) == MagickFalse)
+        break;
+      if (image->previous == (Image *) NULL)
+        {
+          status=SetImageProgress(image,LoadImageTag,(MagickOffsetType) y,
+            image->rows);
+          if (status == MagickFalse)
+            break;
+        }
+    }
+    pixels=(unsigned char *) RelinquishMagickMemory(pixels);
+    if (EOFBlob(image) != MagickFalse)
+        ThrowFileException(exception,CorruptImageError,"UnexpectedEndOfFile",
+          image->filename);
+
+  (void) CloseBlob(image);
+  return(GetFirstImageInList(image));
+}
+
+/*
+%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
+%                                                                             %
+%                                                                             %
+%                                                                             %
+%   R e g i s t e r S P R I m a g e                                           %
+%                                                                             %
+%                                                                             %
+%                                                                             %
+%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
+%
+%  RegisterSPRImage() adds attributes for the SPR X image format to the list
+%  of supported formats.  The attributes include the image format tag, a
+%  method to read and/or write the format, whether the format supports the
+%  saving of more than one frame to the same file or blob, whether the format
+%  supports native in-memory I/O, and a brief description of the format.
+%
+%  The format of the RegisterSPRImage method is:
+%
+%      size_t RegisterSPRImage(void)
+%
+*/
+ModuleExport size_t RegisterSPRImage(void)
+{
+  MagickInfo
+    *entry;
+
+  entry=SetMagickInfo("SPR");
+  entry->decoder=(DecodeImageHandler *) ReadSPRImage;
+  entry->encoder=(EncodeImageHandler *) WriteSPRImage;
+  entry->description=ConstantString("SPR RISC OS sprite");
+  entry->module=ConstantString("SPR");
+  (void) RegisterMagickInfo(entry);
+  return(MagickImageCoderSignature);
+}
+
+/*
+%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
+%                                                                             %
+%                                                                             %
+%                                                                             %
+%   U n r e g i s t e r S P R I m a g e                                       %
+%                                                                             %
+%                                                                             %
+%                                                                             %
+%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
+%
+%  UnregisterSPRImage() removes format registrations made by the
+%  SPR module from the list of supported formats.
+%
+%  The format of the UnregisterSPRImage method is:
+%
+%      UnregisterSPRImage(void)
+%
+*/
+ModuleExport void UnregisterSPRImage(void)
+{
+  (void) UnregisterMagickInfo("SPR");
+}
+
+/*
+%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
+%                                                                             %
+%                                                                             %
+%                                                                             %
+%   W r i t e S P R I m a g e                                                 %
+%                                                                             %
+%                                                                             %
+%                                                                             %
+%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
+%
+%  WriteSPRImage() writes an image to a file in SPR X image format.
+%
+%  The format of the WriteSPRImage method is:
+%
+%      MagickBooleanType WriteSPRImage(const ImageInfo *image_info,Image *image)
+%
+%  A description of each parameter follows.
+%
+%    o image_info: the image info.
+%
+%    o image:  The image.
+%
+*/
+static MagickBooleanType WriteSPRImage(const ImageInfo *image_info,Image *image)
+{
+  MagickBooleanType
+    status;
+
+  MagickOffsetType
+    scene;
+
+  register const PixelPacket
+    *restrict p;
+
+  register ssize_t
+    x;
+
+  register unsigned char
+    *restrict q;
+
+  ssize_t
+    count,
+    y;
+
+  unsigned char
+    *pixels;
+
+  const char
+    *value;
+
+  char spritename[12];
+
+  int dpi, xdpi, ydpi;
+
+  /*
+    Open output image file.
+  */
+  assert(image_info != (const ImageInfo *) NULL);
+  assert(image_info->signature == MagickSignature);
+  assert(image != (Image *) NULL);
+  assert(image->signature == MagickSignature);
+  if (image->debug != MagickFalse)
+    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
+  status=OpenBlob(image_info,image,WriteBinaryBlobMode,&image->exception);
+  if (status == MagickFalse)
+    return(status);
+  value=GetImageProperty(image,"comment");
+  if (value != (const char *) NULL)
+    strncpy(spritename, value, 12);
+  else
+    strncpy(spritename, "screen", 12);
+
+  dpi = 0;
+
+  value=GetImageProperty(image,"spr:dpi");
+  if (value != (const char *) NULL)
+    dpi=atoi(value);
+
+  if (dpi==1)
+    {
+      xdpi = 90;
+      ydpi = 45;
+    }
+  else if (dpi == 3)
+    {
+      xdpi = 180;
+      ydpi = 180;
+    }
+  else
+    {
+      xdpi = 90;
+      ydpi = 90;
+    }
+
+  scene=0;
+  do
+  {
+    /*
+      Write SPR header.
+    */
+    if (IssRGBCompatibleColorspace(image->colorspace) == MagickFalse)
+      (void) TransformImageColorspace(image,sRGBColorspace);
+//    (void) WriteBlobMSBLong(image,(unsigned int) image->columns); // Width
+//    (void) WriteBlobMSBLong(image,(unsigned int) image->rows); // Height
+
+    (void) WriteBlobLSBLong(image,(unsigned int) 1);
+    (void) WriteBlobLSBLong(image,(unsigned int) 12 + 4);
+    (void) WriteBlobLSBLong(image,(unsigned int) 12 + 4 + (image->columns * 4 * image->rows) + 44);
+
+    (void) WriteBlobLSBLong(image,(unsigned int) (image->columns * 4 * image->rows) + 44); // size
+    (void) WriteBlob(image,12,(unsigned char*) spritename); // name
+    (void) WriteBlobLSBLong(image,(unsigned int) image->columns - 1); // width
+    (void) WriteBlobLSBLong(image,(unsigned int) image->rows - 1); // height
+    (void) WriteBlobLSBLong(image,(unsigned int) 0); // left_bit
+    (void) WriteBlobLSBLong(image,(unsigned int) 31); // right_bit
+    (void) WriteBlobLSBLong(image,(unsigned int) 44); // image
+    (void) WriteBlobLSBLong(image,(unsigned int) 44); // mask
+    (void) WriteBlobLSBLong(image,(unsigned int) ( 6 << 27 ) + ( xdpi << 14 ) + ( ydpi << 1 ) + 1); // mode
+
+    /*
+      Allocate memory for pixels.
+    */
+    pixels=(unsigned char *) AcquireQuantumMemory((size_t) image->columns,
+      4*sizeof(*pixels));
+    if (pixels == (unsigned char *) NULL)
+      ThrowWriterException(ResourceLimitError,"MemoryAllocationFailed");
+    /*
+      Convert MIFF to SPR raster pixels.
+    */
+    for (y=0; y < (ssize_t) image->rows; y++)
+    {
+      p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
+      if (p == (PixelPacket *) NULL)
+        break;
+      q=pixels;
+      for (x=0; x < (ssize_t) image->columns; x++)
+      {
+        *q++=ScaleQuantumToChar(GetPixelRed(p));
+        *q++=ScaleQuantumToChar(GetPixelGreen(p));
+        *q++=ScaleQuantumToChar(GetPixelBlue(p));
+        *q++=0;
+        p++;
+      }
+      count=WriteBlob(image,(size_t) (q-pixels),pixels);
+      if (count != (ssize_t) (q-pixels))
+        break;
+      if (image->previous == (Image *) NULL)
+        {
+          status=SetImageProgress(image,SaveImageTag,(MagickOffsetType) y,
+            image->rows);
+          if (status == MagickFalse)
+            break;
+        }
+    }
+    pixels=(unsigned char *) RelinquishMagickMemory(pixels);
+    if (GetNextImageInList(image) == (Image *) NULL)
+      break;
+    image=SyncNextImageInList(image);
+    status=SetImageProgress(image,SaveImagesTag,scene++,
+      GetImageListLength(image));
+    if (status == MagickFalse)
+      break;
+  } while (image_info->adjoin != MagickFalse);
+  (void) CloseBlob(image);
+  return(MagickTrue);
+}
