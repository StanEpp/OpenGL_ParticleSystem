/* tga.c

  Load uncompressed Targa 24 or Targa 32 images.

  Copyright 2004, 2011 Michael Thomas Greer
  Distributed under the Boost Software License, Version 1.0.
  (See http://www.boost.org/LICENSE_1_0.txt )

  ----------------------------------------------------------------------------
  Simplified TGA Specification
  ----------------------------------------------------------------------------

  The TGA image format was created in 1984 for use by the Truevision TARGA
  video graphics hardware. Owing to its unprecedented popularity, Truevision
  in 1989 updated the specification to TGA 2.0 with a fully backward-
  compatable extension architecture, meaning that decoders written to handle
  the earlier TGA file format could easily read and sometimes even modify TGA
  2.0 files while preserving the extended data.

  This documentation describes type 1 and type 2 TGA 1.0 files.

  Data types used in the following table are
    BYTE    8 bit byte
    WORD    16 bit word
  Multi-byte values are stored least-significant-byte to most-significant-
  byte (little-endian order). For example, the value ABCDh is stored as
  the byte-sequence [CD AB]. This is typical of the PDP-11, VAX, and Intel
  processors.

  The first column of the table lists the offset of the field from the
  beginning of the file in BYTEs. The second column is the size of the data.
  The third is a description of the field.

  ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    Header
  ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
     0 BYTE    Size of Image Identifiaction Field. This field is normally
               zero.

     1 BYTE    Colormap Type. The defined values for this field are:
                 0 - no colormap data is included with this image.
                 1 - colormap data is included with this image.
               This field is always 1 for type 1 TGA images.

     2 BYTE    TGA Type:
                 0  No image data.
                 1  Uncompressed, colormapped image.
                 2  Uncompressed, RGB(A) image.
               There are others, but they are not described here.

  ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    Colormap Specification
  ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
     3 WORD    Colormap Origin. Index of the first colormap entry. In other
               words, if the origin were 3 and the image data had a
               colormap reference of 5, the color would be stored at
               colormap[ 5 -3 = 2 ].

               The colormap origin is normally zero.

     5 WORD    Colormap Length. Number of entries in the colormap array.

     7 BYTE    Colomap Entry Size. The number of bits in each colormap entry.
               See "Colormap" below for more information.

  ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    Image Specification
  ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
     8 WORD    Image X Offset.
    10 WORD    Image Y Offset.
               The number of pixels to offset the LOWER-LEFT corner of the
               image from the LOWER-LEFT corner of the display window.

    12 WORD    Image Width.
    14 WORD    Image Height.
               The size of the image in pixels.

    16 BYTE    Image Pixel Size. The number of bits in each pixel entry.

               If Colormap Type is zero, this field is is the same as the
               Colormap Entry Size field. See "Colormap" below for more
               information.

               If Colormap Type is one, this field is the size of the index
               into the colormap.

               Usually values of 8, 16, 24, or 32 are used, but any value is
               valid. However, the Image Data pixels are always stored in an
               integral number of bytes, no matter what the bit count.

                 bytes_per_pixel <-- (image_pixel_size +7) div 8;

    17 BYTE    Image Descriptor
               bits 0..3  The number of attribute bits associated with each
                          pixel. It is usually safe to assume that the
                          "attribute" data is non-pre-multiplied alpha data.

                          It is best practice to place a value of 8 in this
                          field when using Targa 32.

               bit  4     Screen Origin Bit (Left-Right).
                            0 - Origin is on the left edge.
               bit  5     Screen Origin Bit (Top-Bottom).
                            0 - Origin is on the bottom edge.
                            1 - Origin is on the top edge.
               bits 6..7  Data storage interleave flags. Truevision recommends
                          that these be zero (for non-interleaved data).

               The Screen Origin Bits must not be confused with the Image X
               and Y Offsets. TARGA cards treated the origin at the lower-left
               corner of the display, and so TGA files are naturally biased
               the same way. What this means is that the first line of image
               data refers to the bottom line displayed. If you flip it (bit
               5), then the first line of image data is the top line
               displayed. Please don't flip the X (bit 4), there's no good
               reason to do so.

  ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    Image Identification Field
  ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    18 BYTE[]  Image Identifiction Field. This need not be textual (but it
               normally is).
               Note also that it is NOT NULL-terminated.

  ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    Colormap
  ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    The colormap immediately follows the Image Identification Field.

    The colormap is an array of "Colormap Length" entries. Each entry is
    "Colormap Entry Size" bits. Valid values are 16, 24, and 32 bits (2, 3,
    and 4 bytes).

    Targa 16  bits: a rrrrr ggggg bbbbb (that's byte order: gggbbbbb arrrrrgg)
    Targa 24  byte order: B G R
    Targa 32  byte order: B G R A

  ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    Image Data
  ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    This is an array[ Image Height ] of array[ Image Width ] of pixels.

    See "Image Pixel Size" above for more information on individual pixels.

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tga.h"

#define then
#ifndef TRUE
#define TRUE  (1)
#define FALSE (0)
#endif

/*--------------------------------------------------------------------------+/
  load_TGA
/+--------------------------------------------------------------------------*/
int load_TGA( tTGA *tga, const char *filename ) {

  #define SIZEOF_TGA_HEADER 18

  unsigned char   buffer[ 256 ];

  int             size_of_image_id;
  int             is_colormap;
  int             targa_type;
  int             colormap_origin;
  unsigned int    colormap_length;
  int             colormap_entry_size;
  int             image_pixel_size;
  int             image_descriptor;
  int             is_inverted;

  int             image_width;
  int             image_height;

  unsigned char  *colormap;
  FILE           *f;
  unsigned char  *data;
  int             x, y, i, j;
  int             raster_width;

  if ((f = fopen( filename, "rb" )) == NULL) then return FALSE;

  /* header info */
  if (fread( buffer, 1, SIZEOF_TGA_HEADER, f ) != SIZEOF_TGA_HEADER) then return FALSE;

  size_of_image_id    = buffer[  0 ];
  is_colormap         = buffer[  1 ];
  targa_type          = buffer[  2 ];

  colormap_origin     = buffer[  3 ] +((int)(buffer[  4 ]) << 8);
  colormap_length     = buffer[  5 ] +((int)(buffer[  6 ]) << 8);
  colormap_entry_size = buffer[  7 ];

  image_width         = buffer[ 12 ] +((unsigned)(buffer[ 13 ]) << 8);
  image_height        = buffer[ 14 ] +((unsigned)(buffer[ 15 ]) << 8);
  image_pixel_size    = buffer[ 16 ];
  image_descriptor    = buffer[ 17 ];

  /* valid type? */
  if ((targa_type != 1) && (targa_type != 2)) then return FALSE;

  /* colormap required but missing? */
  if ((targa_type == 1) && !is_colormap) then return FALSE;

  /* cannot load direct-color images */
  if ((targa_type == 2) && is_colormap) then return FALSE;

  /* image id */
  if (size_of_image_id) then
  if ((int)fread( buffer, 1, size_of_image_id, f ) != size_of_image_id) then return FALSE;

  is_inverted = (image_descriptor & 0x10) != 0;

  /* cannot handle interlacing */
  if ((image_descriptor & 0xC0)) then return FALSE;

  /* assume that targa 32 contains alpha (image_descriptor bits 0..3) */

  /* load colormap, if any */
  if (is_colormap)
    then {
      /* must be targa 24 or targa 32 */
      if ((colormap_entry_size != 24) && (colormap_entry_size != 32)) then return FALSE;

      /* convert to number of bytes/color entry */
      colormap_entry_size >>= 3;

      colormap = reinterpret_cast<unsigned char*>(malloc( colormap_length *colormap_entry_size ));
      if (colormap == NULL) then return FALSE;

      if (fread( colormap, colormap_entry_size, colormap_length, f ) != colormap_length)
        then {
          lerror:
          free( colormap );
          return FALSE;
          }

  /* initializations */
      image_pixel_size = (image_pixel_size +7) >> 3;
      raster_width     = image_width *colormap_entry_size;
      }
    else {
      /* must be targa 24 or targa 32 */
      if ((image_pixel_size != 24) && (image_pixel_size != 32)) then return FALSE;
      image_pixel_size >>= 3;
      raster_width     = image_width *image_pixel_size;
      }

  data = reinterpret_cast<unsigned char*>(malloc( raster_width *image_height ));
  if (data == NULL) then goto lerror;

  /* load image data */
  for (y = (is_inverted ? (image_height -1) : 0);
           (is_inverted ? (y >= 0)          : (y < (int)image_height));
           (is_inverted ? (--y)             : (++y)))
  for (x = 0; x < image_width; x++) {

    /* get the next pixel */
    if ((int)fread( buffer, 1, image_pixel_size, f ) != image_pixel_size) then goto lerror;

    /* store it */
    if (is_colormap)
      then {
        /* colormapped */
        i = ((buffer[ 0 ] +((unsigned)(buffer[ 1 ]) << 8)) -colormap_origin)
            *colormap_entry_size;
        j = (y *raster_width) +(x *colormap_entry_size);

        data[ j    ] = colormap[ i +2 ];
        data[ j +1 ] = colormap[ i +1 ];
        data[ j +2 ] = colormap[ i    ];

        if (colormap_entry_size > 3) then data[ j +3 ] = colormap[ i + 3 ];
        }
      else {
        /* non-colormapped */
        j = (y *raster_width) +(x *image_pixel_size);

        data[ j    ] = buffer[ 2 ];
        data[ j +1 ] = buffer[ 1 ];
        data[ j +2 ] = buffer[ 0 ];

        if (image_pixel_size > 3) then data[ j +3 ] = buffer[ 3 ];
        }
    }

  /* free the colormap if we had loaded it */
  if (is_colormap) then free( colormap );

  /* store the result */
  tga->width  = image_width;
  tga->height = image_height;
  tga->data   = data;
  tga->alpha  = (is_colormap ? (colormap_entry_size > 3) : (image_pixel_size > 3));

  #undef SIZEOF_TGA_HEADER

  return TRUE;
  }

/*--------------------------------------------------------------------------+/
  free_TGA
/+--------------------------------------------------------------------------*/
void free_TGA( tTGA *tga ) {

  if (tga->data) then free( tga->data );

  tga->data   = NULL;
  tga->height =
  tga->width  = 0;
  tga->alpha  = 0;
  }

/* end tga.c */