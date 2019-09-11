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
// $Id: wellinput.cxx,v 1.2 2003/02/21 11:40:33 leo Exp $
#include "globals.h"
#include "wellinput.h"
#include "wellengine.h"

//////////////////////////////WellInput class///////////////////////////////

//===========================================================================
/// global WellInput(char*)
///     constructor - fill name and get geometry
/// tags WellInput
WellInput::WellInput(const char* iname) : WellObject()
{
  strncpy(object_name,iname,GEO_NAME);
  object_name[GEO_NAME]=0;
  geo=get_geo_by_name(object_name);
  fnt=default_well_engine->new_well_image_font(imFont1,FONT2_L,FONT2_H,
					  FONT2_DX,FONT2_DY);
  fnt->set_screen_region(geo[1].tox, geo[1].toy, geo[1].l, geo[1].h);
}


//===========================================================================
/// global process_event(wEvent)
///     stub that process events
/// tags WellInput
bool WellInput::process_event(wEvent ev)
{
  switch(ev.type)
  {
  case eKeyPress:
    dbgprintf(("WellInput - done\n"));
    object_on_enter.call(wEvent(aInputDone,this));
    return false;

  }
  return true;
}

//===========================================================================
/// global set_text(const char*)
///     set test on the buffer
/// tags WellInput
void WellInput::set_text(const char* txt)
{
  if(txt)
  {
    strcpy(buf,txt);
    buflen=strlen(buf);
    if(buflen>maxlen)
      buflen=maxlen;
    buf[buflen]=0;
    pos=buflen;
  } else
  {
    buflen=pos=0;
    buf[buflen]=0;
  }
}


//===========================================================================
/// global draw_text()
///     draw text on the screen
/// tags WellInput
void WellInput::draw_text()
{
  fnt->draw_text(buf,buflen,BackColor);
}

//===========================================================================
/// global show()
///     show object
/// tags WellInput
void WellInput::show()
{
  shown=true;
  redraw();
  default_well_engine->add_object(this);
}

//===========================================================================
/// global redraw()
///     redraw object
/// tags WellInput
void WellInput::redraw()
{
  if(shown)
  {
    default_well_engine->screen_copy(&geo[0]);
    draw_text();
  }
}

//===========================================================================
/// global hide()
///     hide object
/// tags WellInput
void WellInput::hide()
{
  shown=false;
  default_well_engine->screen_clear(&geo[0]);
  default_well_engine->del_object(this);
}


