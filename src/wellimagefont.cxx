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
// $Id: wellimagefont.cxx,v 1.1.1.1 2003/01/04 11:37:22 leo Exp $

#include "wellimagefont.h"

//===========================================================================
/// global WellImageFont(Images id, unsigned int ifl, unsigned int ifh, int idx, int idy)
///   Constructor of base draw text class 
/// tags WellImageFont
WellImageFont::WellImageFont(Images id, unsigned int ifl, unsigned int ifh, 
			     int idx, int idy)
{
  font_id=id;
  font_l=ifl;
  font_h=ifh;
  dx=idx;
  dy=idy;
  text_l=0;
}

//===========================================================================
/// global set_font(...)
///   setting font params
/// tags WellImageFont
void WellImageFont::set_font(unsigned int ifl, unsigned int ifh, 
			     int idx, int idy)
{
  font_l=ifl;
  font_h=ifh;
  dx=idx;
  dy=idy;
}

//===========================================================================
/// global set_text(char*)
///   setting text for output
/// tags WellImageFont
void WellImageFont::set_text(char* itext)
{
  strcpy(text,itext);
  text_l=strlen(text);
}


//===========================================================================
/// global set_screen_region(int ix, int iy, int il, int ih)
///   setting text for output
/// tags WellImageFont
void WellImageFont::set_screen_region(int ix, int iy, int il, int ih)
{
  screen_x=ix;
  screen_y=iy;
  screen_l=il;
  screen_h=ih;
}


//===========================================================================
/// global draw_text(Justify just)
///   draw text with given justification
/// tags WellImageFont
void WellImageFont::draw_text(Justify just)
{
  draw_text(text,text_l,just);
}

//===========================================================================
/// global draw_text(char *text, int text_l, Justify just)
///   draw text with given justification
/// tags WellImageFont
void WellImageFont::draw_text(char* txt, int txt_l, Justify just)
{
  int i,draw_x;
  unsigned int draw_h;

  clear_region();
  if(txt_l<=0)
    return;

  switch(just)
  {
  case LeftJust:
    draw_x=screen_x;
    break;
  case RightJust:
    draw_x=screen_x+screen_l-txt_l*font_l;
    break;
  case CenterJust:
    draw_x=screen_x+(screen_l-txt_l*font_l)/2;
    break;
  }
  draw_h=(screen_h<font_h ? screen_h : font_h); 
  for(i=0;i<txt_l;i++)
    draw_symbol(draw_x+font_l*i,screen_y,font_l,draw_h,txt[i]);
}

//===========================================================================
/// global draw_text(char *text, int text_l, Justify just)
///   draw text with given justification
/// tags WellImageFont
void WellImageFont::draw_text(char* txt, int txt_l, Colors idx, Justify just)
{
  int i,draw_x;
  unsigned int draw_h;

  clear_region(idx);
  if(txt_l<=0)
    return;

  switch(just)
  {
  case LeftJust:
    draw_x=screen_x;
    break;
  case RightJust:
    draw_x=screen_x+screen_l-txt_l*font_l;
    break;
  case CenterJust:
    draw_x=screen_x+(screen_l-txt_l*font_l)/2;
    break;
  }
  draw_h=(screen_h<font_h ? screen_h : font_h); 
  for(i=0;i<txt_l;i++)
    draw_symbol(draw_x+font_l*i,screen_y,font_l,draw_h,txt[i]);
}


