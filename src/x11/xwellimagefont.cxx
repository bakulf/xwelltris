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
// $Id: xwellimagefont.cxx,v 1.1.1.1 2003/01/04 11:37:22 leo Exp $

/// module description
/// X11 version of imagefont - displays text with X11 calls, work with pixmaps
/// loaded thought XWellEngine class

#include "xwellimagefont.h"

//===========================================================================
/// global XWellImageFont(Images id, unsigned int ifl, unsigned int ifh, int idx, int idy)
///   Constructor of base draw text class 
/// tags XWellImageFont
XWellImageFont::XWellImageFont(Images id, unsigned int ifl, unsigned int ifh, 
			       int idx, int idy):
  WellImageFont(id,ifl,ifh,idx,idy)
{
  xengine=(XWellEngine*) default_well_engine;
  font_pixmap=xengine->get_pixmap_of_image(id);
  mainw=xengine->get_main_window();
  maingc=xengine->get_main_gc();
}

//===========================================================================
/// global clear_region()
///   clear region on screen for making output
/// tags XWellImageFont
void XWellImageFont::clear_region()
{
  XClearArea(disp,mainw,screen_x,screen_y,screen_l,screen_h,False);
}

//===========================================================================
/// global clear_region()
///   clear region on screen for making output
/// tags XWellImageFont
void XWellImageFont::clear_region(Colors idx)
{
  unsigned long *cols=xengine->get_colors();
  XSetForeground(disp,maingc,cols[idx]);
  XFillRectangle(disp,mainw,maingc,screen_x,screen_y,screen_l,screen_h);
}

//===========================================================================
/// global draw_symbol(...)
///   draw one symbol from font image to screen
/// tags XWellImageFont
void XWellImageFont::draw_symbol(int ix, int iy, 
				unsigned int il,
				unsigned int ih,
				char symb)
{
  if(symb<=32 || symb>=127)
    return;
  symb-=33;  //Translate to our indexation
  XCopyArea(disp,font_pixmap,mainw,maingc,
	    dx,dy+(int)symb*font_h,
	    il,ih,ix,iy);
}

