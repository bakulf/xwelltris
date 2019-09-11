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
// $Id: wellkey.cxx,v 1.1.1.1 2003/01/04 11:37:22 leo Exp $

/// module description
/// WellKey class implements key (or button) GUI element. It draw key with
/// given images (uses geometry module) then catches mouse events and calls
/// ObjectCaller on press.

#include "globals.h"
#include "wellkey.h"
#include "wellengine.h"

//////////////////////////////WellKey class///////////////////////////////

//===========================================================================
/// global WellKey(const char*)
///     constructor - fill name and get geometry
/// tags WellKey
WellKey::WellKey(const char* iname) : WellObject()
{
  strncpy(object_name,iname,GEO_NAME);
  object_name[GEO_NAME]=0;
  geo=get_geo_by_name(object_name);
}


//===========================================================================
/// global process_event(wEvent)
///     stub that process events
/// tags WellKey
bool WellKey::process_event(wEvent ev)
{
  MouseEvent *pmev;
  switch(ev.type)
  {
  case eKeyPress:
    dbgprintf(("WellKey - pressed\n"));
    object_on_press.call(wEvent(aKeyPressed,this));
    return false;

  case eMousePress:
    pmev=(MouseEvent*)ev.data;
    return !press(pmev->mx,pmev->my);

  case eMouseMove:
    pmev=(MouseEvent*)ev.data;
    return !highlight(pmev->mx,pmev->my);

  }
  return true;
}

//===========================================================================
/// global highlight(int mx, int my)
///     highlights key
/// tags WellKey
bool WellKey::highlight(int mx, int my)
{
  if(mx>=geo[1].tox && my>=geo[1].toy &&
     mx<geo[1].tox+geo[1].l && my<geo[1].toy+geo[1].h)
  {
    if(!highlighted)
    {
      dbgprintf(("WellKey highlighted via mouse\n"));
      highlighted=true;
      default_well_engine->screen_copy(&geo[1]); //highlighted state
      return true;
    }
  } else
  {
    if(highlighted)
    {
      highlighted=false;
      dbgprintf(("WellKey DEhighlighted via mouse\n"));
      default_well_engine->screen_copy(&geo[0]); //normal state
      return true;
    }
  }
  return false;
}

//===========================================================================
/// global press(int mx, int my)
///     press key and call object
/// tags WellKey
bool WellKey::press(int mx, int my)
{
  if(mx>=geo[2].tox && my>=geo[2].toy &&
     mx<geo[2].tox+geo[2].l && my<geo[2].toy+geo[2].h)
  {
    dbgprintf(("WellKey pressed via mouse\n"));
    default_well_engine->screen_copy(&geo[2]); //highlighted state
    object_on_press.call(wEvent(aKeyPressed,this));
    return true;
  }
  return false;
}

//===========================================================================
/// global show()
///     show object
/// tags WellKey
void WellKey::show()
{
  shown=true;
  redraw();
  default_well_engine->add_object(this);
}

//===========================================================================
/// global redraw()
///     redraw object
/// tags WellKey
void WellKey::redraw()
{
  if(shown)
    default_well_engine->screen_copy(&geo[0]);
}

//===========================================================================
/// global hide()
///     hide object
/// tags WellKey
void WellKey::hide()
{
  shown=false;
  default_well_engine->screen_clear(&geo[0]);
  default_well_engine->del_object(this);
}


