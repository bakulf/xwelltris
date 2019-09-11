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
// $Id: wellswitch.cxx,v 1.1.1.1 2003/01/04 11:37:22 leo Exp $

/// module description
/// WellSwitch class implements Switch GUI element, it can be set or reset,
/// uses for switching options

#include "globals.h"
#include "wellswitch.h"
#include "wellengine.h"

//////////////////////////////WellSwitch class///////////////////////////////

//===========================================================================
/// global WellSwitch(const char*)
///     constructor - fill name and get geometry
/// tags WellSwitch
WellSwitch::WellSwitch(const char* iname) : WellObject()
{
  strncpy(object_name,iname,GEO_NAME);
  object_name[GEO_NAME]=0;
  geo=get_geo_by_name(object_name);
  value=false;
  mode=SetReset;
}


//===========================================================================
/// global process_event(wEvent)
///     stub that process events
/// tags WellSwitch
bool WellSwitch::process_event(wEvent ev)
{
  MouseEvent *pmev;
  switch(ev.type)
  {
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
/// tags WellSwitch
bool WellSwitch::highlight(int mx, int my)
{
  if(mx>=geo[1].tox && my>=geo[1].toy &&
     mx<geo[1].tox+geo[1].l && my<geo[1].toy+geo[1].h)
  {
    if(!highlighted)
    {
      dbgprintf(("WellSwitch highlighted via mouse\n"));
      highlighted=true;
      default_well_engine->screen_copy(&geo[1]); //highlighted state
      return true;
    }
  } else
  {
    if(highlighted)
    {
      highlighted=false;
      dbgprintf(("WellSwitch DEhighlighted via mouse\n"));
      redraw();
      return true;
    }
  }
  return false;
}

//===========================================================================
/// global press(int mx, int my)
///     press key and call object
/// tags WellSwitch
bool WellSwitch::press(int mx, int my)
{
  if(mx>=geo[1].tox && my>=geo[1].toy &&
     mx<geo[1].tox+geo[1].l && my<geo[1].toy+geo[1].h)
  {
    dbgprintf(("WellSwitch pressed via mouse\n"));
    value= value ? false : true;
    switch(mode)
    {
    case OnlySet:
      value=true;
      break;
    case OnlyReset:
      value=false;
      break;
    }
    redraw();
    object_on_switch.call(wEvent(aSwitchChanged,this));
    return true;
  }
  return false;
}

//===========================================================================
/// global show()
///     show object
/// tags WellSwitch
void WellSwitch::show()
{
  shown=true;
  redraw();
  default_well_engine->add_object(this);
}

//===========================================================================
/// global redraw()
///     redraw object
/// tags WellSwitch
void WellSwitch::redraw()
{
  if(shown)
  {
    default_well_engine->screen_copy(&geo[0]); //normal state
    if(value)
      default_well_engine->screen_copy(&geo[2]); //value state
    else
      default_well_engine->screen_clear(&geo[2]); //value state
  }
}

//===========================================================================
/// global hide()
///     hide object
/// tags WellSwitch
void WellSwitch::hide()
{
  shown=false;
  default_well_engine->screen_clear(&geo[0]);
  default_well_engine->del_object(this);
}


