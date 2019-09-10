// docm_prefix(///)
/****************************************************************************
*  Copyright (C) 2002-2003 by Leo Khramov
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
// $Id: sdlwellimagefont.cxx,v 1.2 2003/02/20 15:09:41 leo Exp $

/// module description
/// SDL version of imagefont - displays text with SDL calls, work with pixmaps
/// loaded thought XWellEngine class

#include "sdlwellimagefont.h"

//===========================================================================
/// global WellImageFont(Images id, unsigned int ifl, unsigned int ifh, int idx, int idy)
///   Constructor of base draw text class 
/// tags WellImageFont
SDLWellImageFont::SDLWellImageFont(Images id, unsigned int ifl, unsigned int ifh, 
			       int idx, int idy):
  WellImageFont(id,ifl,ifh,idx,idy)
{
    engine=(SDLWellEngine*) default_well_engine;
    font_surf=engine->get_pixmap_of_image(id);
    mainw=engine->get_main_window();
}

//===========================================================================
/// global clear_region()
///   clear region on screen for making output
/// tags SDLWellImageFont
void SDLWellImageFont::clear_region()
{
    SDL_Rect dest;
    dest.x = screen_x;
    dest.y = screen_y;
    dest.w = screen_l;
    dest.h = screen_h;
    SDL_FillRect(mainw, &dest, 0);
    SDL_UpdateRects(mainw, 1, &dest);
}

//===========================================================================
/// global clear_region()
///   clear region on screen for making output
/// tags SDLWellImageFont
void SDLWellImageFont::clear_region(Colors idx)
{
    SDL_Rect dest;
    Uint32 *cols=engine->get_colors();

    dest.x = screen_x;
    dest.y = screen_y;
    dest.w = screen_l;
    dest.h = screen_h;

    SDL_FillRect(mainw, &dest, cols[idx]);
    SDL_UpdateRects(mainw, 1, &dest);
}

//===========================================================================
/// global draw_symbol(...)
///   draw one symbol from font image to screen
/// tags SDLWellImageFont
void SDLWellImageFont::draw_symbol(int ix, int iy, 
				unsigned int il,
				unsigned int ih,
				char symb)
{
    SDL_Rect src,dest;

    if(symb<=32 || symb>=127)
	return;
    symb-=33;  //Translate to our indexation

    dest.x = ix;
    dest.y = iy;
    dest.w = il;
    dest.h = ih;
    src.x = dx;
    src.y = dy+(int)symb*font_h;
    src.w = il;
    src.h = ih;
    SDL_BlitSurface(font_surf, &src, mainw, &dest);
    /* Update the changed portion of the screen */
    SDL_UpdateRects(mainw, 1, &dest);
}

