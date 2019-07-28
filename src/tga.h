/* tga.h

  Load uncompressed Targa 24 or Targa 32 images.

  Copyright 2004, 2011 Michael Thomas Greer
  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt )

  NOTE This module is only capable of the most basic image handling. If your
  TGA does not load when using this module, go back to your image editor and
  make sure that the following are true:
    No compression
    No colormap data is stored in an RGB(A) file (the TGA must be either
      colormapped or not).
    The x axis is not flipped. (TGA Screen Origin is at left edge of image.)
    No interlacing.
  This algorithm simply assumes that Targa 32 means that the TGA includes
  an alpha channel. If you don't want alpha information, use Targa 24.

  ----------------------------------------------------------------------------
  tTGA
  ----------------------------------------------------------------------------
  type
    Contains the image data.

  members
    long           width   The width of the image in pixels.
    long           height  The height of the image in pixels.
    unsigned char *data    The image data.
    bool           alpha   Does the image data include an alpha channel?

    The data is always stored as
      array[ 0..height-1 ] of
      array[ 0..width -1 ] of pixel

    If alpha is true, then pixel is defined as
      record
        red,
        green,
        blue,
        alpha: byte
        end

    If alpha is false, then pixel is defined as
      record
        red,
        green,
        blue: byte
        end

    The pixel at data[ 0 ][ 0 ] is the bottom-left corner of the image, and
    data[ height-1 ][ width -1 ] is the pixel in the upper-right corner of
    the image.

  ----------------------------------------------------------------------------
  load_TGA
  ----------------------------------------------------------------------------
  function
    Load a TGA file. Cannot load compressed TGAs.

    It will only load Targa 24 and Targa 32 images. The image may be a
    colormapped TGA.

  arguments
    tTGA *        Where to store the image data.
    const char *  The filename to load from.

  returns
    TRUE or FALSE for success or failure.


  ----------------------------------------------------------------------------
  free_TGA
  ----------------------------------------------------------------------------
  function
    Free memory used by a tTGA and clear the structure to 0/NULL.
    
  ----------------------------------------------------------------------------
  load_texture_TGA
  ----------------------------------------------------------------------------
  function
    Load a texture from a TGA file and mipmap it.

  arguments
    filename  The file to load. 
    width     Returns the width of the image in pixels. May be NULL.
    height    Returns the height of the image in pixels. May be NULL.
    wrap_s    Must be either GL_REPEAT or GL_CLAMP.
    wrap_t    Must be either GL_REPEAT or GL_CLAMP.

  returns
    The texture object ID, or zero on failure.

 */

#ifndef TGA_H
#define TGA_H

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct {
    long           width;
    long           height;
    unsigned char *data;
    int            alpha;
    } tTGA;

  int    load_TGA( tTGA *tga, const char *filename );
  void   free_TGA( tTGA *tga );

#ifdef __cplusplus
}
#endif

#endif

/* end tga.h */
      