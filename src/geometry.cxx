// docm_prefix(///)
/****************************************************************************
*  Copyright (C) 2002 by Leo Khramov
*  email:   leo@xnc.dubna.su
* 
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
 ****************************************************************************/
// $Id: geometry.cxx,v 1.1.1.1 2003/01/04 11:37:22 leo Exp $

#include "globals.h"
#include "geometry.h"


//imNone means simple clear this area with background image :)

Geo      top_key_exit[]= {
  //   image  srcx srcy dstx dsty  len  hei
  {    imNone,   0,   0, 734, 550,  57,  40 }, //Normal view
  {    imCuts,   0, 365, 734, 550,  57,  39 }, //Highlighted
  {    imNone,   0,   0, 734, 550,  57,  40 }, //Pressed
};

Geo      intro_key_exit[]= {
  //   image  srcx srcy dstx dsty  len  hei
  {    imNone,   0,   0, 734, 550,  57,  40 }, //Normal view
  {    imCuts,   0, 365, 734, 550,  57,  40 }, //Highlighted
  {    imNone,   0,   0, 734, 550,  57,  40 }, //Pressed
};

Geo      intro_key_new_game[]= {
  //   image  srcx srcy dstx dsty  len  hei
  {    imNone,   0,   0, 332, 500, 130,  40 }, //Normal view
  {    imCuts,   0, 215, 332, 500, 130,  40 }, //Highlighted
  {    imNone,   0,   0, 332, 500, 130,  40 }, //Pressed
};

Geo      intro_key_top_nine[]= {
  //   image  srcx srcy dstx dsty  len  hei
  {    imNone,   0,   0, 333, 544, 123,  41 }, //Normal view
  {    imCuts,   0, 255, 333, 544, 123,  41 }, //Highlighted
  {    imNone,   0,   0, 333, 544, 123,  41 }, //Pressed
};

Geo      intro_key_plus[]= {
  //   image  srcx srcy dstx dsty  len  hei
  {    imNone,   0,   0, 720, 400,  25,  25 }, //Normal view
  {    imNone,   0,   0, 720, 400,  25,  25 }, //Highlighted
  {    imNone,   0,   0, 720, 400,  25,  25 }, //Pressed
};

Geo      intro_key_minus[]= {
  //   image  srcx srcy dstx dsty  len  hei
  {    imNone,   0,   0, 640, 405,  25,  17 }, //Normal view
  {    imNone,   0,   0, 640, 405,  25,  17 }, //Highlighted
  {    imNone,   0,   0, 640, 405,  25,  17 }, //Pressed
};

Geo      game_elements[]= {
  //   image  srcx srcy dstx dsty  len  hei
  {    imCuts, 160,   0, 130, 120, 296, 215 }, //Pause image
  {    imCuts, 120,   0, 603, 469,  30,  26 }, //Rotation image
  {    imCuts, 120,  38, 608, 509,  26,  16 }, //Next piece image
  {    imCuts, 120,  76, 610, 539,  22,  24 }, //Mixed image
};

Geo      top_nine_elements[]= {
  //   image  srcx srcy dstx dsty  len  hei
  {    imCuts,  90, 365, 704, 119,  25,  26 }, //Rotation image
  {    imCuts, 115, 365, 729, 119,  33,  26 }, //Next piece image
  {    imCuts, 148, 365, 762, 119,  18,  26 }, //Mixed image
};

Geo      intro_sw_rotation[]= {
  //   image  srcx srcy dstx dsty  len  hei
  {    imNone,   0,   0, 640, 225,  93,  34 }, //Normal view
  {    imCuts,   0, 298, 640, 225,  93,  34 }, //Highlighted
  {    imCuts, 120, 296, 739, 228,  25,  25 }, //Pressed
};

Geo      intro_sw_next_piece[]= {
  //   image  srcx srcy dstx dsty  len  hei
  {    imNone,   0,   0, 616, 268, 116,  35 }, //Normal view
  {    imCuts,   0, 330, 616, 268, 116,  35 }, //Highlighted
  {    imCuts, 120, 330, 739, 270,  25,  25 }, //Pressed
};

Geo      intro_sw_mixed[]= {
  //   image  srcx srcy dstx dsty  len  hei
  {    imNone,   0,   0,  65, 379,  71,  35 }, //Normal view
  {    imCuts,   0, 180,  65, 379,  71,  35 }, //Highlighted
  {    imCuts,  90, 180,  35, 385,  25,  25 }, //Pressed
};

Geo      intro_sw_square0[]= {
  //   image  srcx srcy dstx dsty  len  hei
  {    imNone,   0,   0,  65, 115,  75,  38 }, //Normal view
  {    imCuts,   0,   0,  65, 115,  75,  38 }, //Highlighted
  {    imCuts,  90,   0,  35, 122,  25,  25 }, //Pressed
};

Geo      intro_sw_square1[]= {
  //   image  srcx srcy dstx dsty  len  hei
  {    imNone,   0,   0,  65, 158,  77,  38 }, //Normal view
  {    imCuts,   0,  38,  65, 158,  77,  38 }, //Highlighted
  {    imCuts,  90,  38,  35, 167,  25,  25 }, //Pressed
};

Geo      intro_sw_square2[]= {
  //   image  srcx srcy dstx dsty  len  hei
  {    imNone,   0,   0,  65, 205,  70,  34 }, //Normal view
  {    imCuts,   0,  76,  65, 205,  70,  34 }, //Highlighted
  {    imCuts,  90,  76,  35, 209,  25,  25 }, //Pressed
};

Geo      intro_sw_square3[]= {
  //   image  srcx srcy dstx dsty  len  hei
  {    imNone,   0,   0,  65, 248,  89,  35 }, //Normal view
  {    imCuts,   0, 109,  65, 248,  89,  35 }, //Highlighted
  {    imCuts,  90, 109,  35, 254,  25,  25 }, //Pressed
};

Geo      intro_sw_square4[]= {
  //   image  srcx srcy dstx dsty  len  hei
  {    imNone,   0,   0,  65, 292,  78,  35 }, //Normal view
  {    imCuts,   0, 145,  65, 292,  78,  35 }, //Highlighted
  {    imCuts,  90, 145,  35, 296,  25,  25 }, //Pressed
};

Geo      intro_sw_square5[]= {
  //   image  srcx srcy dstx dsty  len  hei
  {    imNone,   0,   0,  65, 392,  78,  35 }, //Normal view
  {    imCuts,   0, 145,  65, 392,  78,  35 }, //Highlighted
  {    imCuts,  90, 145,  35, 396,  25,  25 }, //Pressed
};

Geo      intro_inp_player_name[]= {
  //   image  srcx srcy dstx dsty  len  hei
  {    imCuts, 170, 215, 211, 205, 357, 154 }, //full view
  {    imCuts,   0,   0, 240, 297, 299,  30 }, //input area
  {    imCuts,  90, 145,  35, 296,  25,  25 }, //Pressed
};



// Array of all objects geometry
GeoObject all_objects[]= {
  { "top_key_exit"      , top_key_exit },
  { "intro_key_exit"    , intro_key_exit },
  { "intro_key_new_game", intro_key_new_game },
  { "intro_key_top_nine", intro_key_top_nine },
  { "intro_key_minus"   , intro_key_minus },
  { "intro_key_plus"    , intro_key_plus },
  { "the_game"          , game_elements },
  { "top_nine_elements" , top_nine_elements },
  { "intro_sw_rotation" , intro_sw_rotation },
  { "intro_sw_next"     , intro_sw_next_piece },
  { "intro_sw_mixed"    , intro_sw_mixed },
  { "intro_sw_square0"  , intro_sw_square0 },
  { "intro_sw_square1"  , intro_sw_square1 },
  { "intro_sw_square2"  , intro_sw_square2 },
  { "intro_sw_square3"  , intro_sw_square3 },
  { "intro_sw_square4"  , intro_sw_square4 },
  { "intro_sw_square5"  , intro_sw_square5 },
  { "intro_inp_player"  , intro_inp_player_name },
  { "empty",      0}
};



//===========================================================================
/// global get_geo_by_name(const char* fname)
///     find geometry structures for object by given name
/// tags geometry
Geo* get_geo_by_name(const char* name)
{
  int i=0;
  while(all_objects[i].geo)
  {
    if(!strcmp(all_objects[i].name,name))
      return all_objects[i].geo;
    i++;
  }
  return 0;
}
