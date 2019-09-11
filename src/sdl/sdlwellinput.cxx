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
// $Id: sdlwellinput.cxx,v 1.2 2003/02/20 15:09:41 leo Exp $

/// module description
/// SDL implementation of WellInput class - make input text from user
/// display it through WellImageFont, convert SDL Key event to symbols

#include "sdlwellinput.h"
#include "sdl_gfxprimitives.h"
//===========================================================================
/// global SDLWellInput(const char*)
///     constructor - fill name and get geometry
/// tags SDLWellInput
SDLWellInput::SDLWellInput(const char* iname) : WellInput(iname)
{
  engine=(SDLWellEngine*) default_well_engine;
  mainw=engine->get_main_window();
}

//===========================================================================
/// global process_event(wEvent)
///     stub that process events
/// tags SDLWellInput
bool SDLWellInput::process_event(wEvent ev)
{
    SDL_Event *xev=(SDL_Event*)ev.data;
    switch(ev.type)
    {
    case eKeyPress:
	process_key(xev);
	return false;

    }
    return true;
}

//===========================================================================
/// global draw_text()
///     draw text on the screen
/// tags SDLWellInput
void SDLWellInput::draw_text()
{
    fnt->draw_text(buf,buflen,BackColor);
    show_cursor();
}


//===========================================================================
/// global show_cursor()
///     draw cursor on the screen
/// tags SDLWellInput
void SDLWellInput::show_cursor()
{
    SDL_Rect dest;
    Uint32 *colors=engine->get_colors();

    dest.x = geo[1].tox+buflen*FONT2_L;
    dest.y = geo[1].toy;
    dest.w = FONT2_L;
    dest.h = FONT2_H;


    SDL_FillRect(mainw, &dest, colors[BonusColor2]);
    SDL_UpdateRects(mainw, 1, &dest);
}


//===========================================================================
/// global hide_cursor()
///     hide cursor on the screen
/// tags SDLWellInput
void SDLWellInput::hide_cursor()
{
    SDL_Rect dest;
    Uint32 *colors=engine->get_colors();

    dest.x = geo[1].tox+buflen*FONT2_L;
    dest.y = geo[1].toy;
    dest.w = FONT2_L;
    dest.h = FONT2_H;

    SDL_FillRect(mainw, &dest, colors[BackColor]);
    SDL_UpdateRects(mainw, 1, &dest);
}


//===========================================================================
/// global process_key(SDLEvent)
///     draw text on the screen
/// tags SDLWellInput

void SDLWellInput::process_key(SDL_Event *xev)
{
    switch(xev->key.keysym.sym)
    {
    case SDLK_RETURN:
	if(buflen)
	    object_on_enter.call(wEvent(aInputDone,this));
	break;
	
    case SDLK_DELETE:
    case SDLK_BACKSPACE:
	if(buflen>0)
	{
	    hide_cursor();
	    buflen--;
	    buf[buflen]=0;
	    draw_text();
	}
	break;
    default:
	if(buflen<maxlen && 
	   xev->key.keysym.unicode>=32 && 
	   xev->key.keysym.unicode<128)
	{
	    hide_cursor();
	    buf[buflen++]=(char)xev->key.keysym.unicode;
	    buf[buflen]=0;
	    draw_text();
	}
    }
}

