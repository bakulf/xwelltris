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
// $Id: wellengine.cxx,v 1.2 2003/02/18 17:01:30 leo Exp $

/// module description
/// WellEngine is a game engine that contains basic game routins for drawing,
/// processing events, makeing queues of object, delivering events to them,
/// timeouting with given threshold, loading images needed for the game
/// and more

#include "wellengine.h"
#include "wellclass.h"

WellEngine *default_well_engine=0;

//===========================================================================
/// global WellEngine()
///     Constructor of the class - init display size
/// tags WellEngine
WellEngine::WellEngine(int argc, char** argv)
{
  mainl=800;
  mainh=600;
}

WellEngine::~WellEngine()
{
}

//===========================================================================
/// global process_time_event()
///     go throuht the list of time objects, decsrease ticks_left and call
///     objects on timeout
/// tags WellEngine
bool WellEngine::process_time_event()
{
  bool processed=false;
  TimeList* ptimelist=timelist.get_next();
  while(ptimelist)
  {
    TimeObject& obj=ptimelist->get_object();
    obj.ticks_left--;
    if(obj.ticks_left<=0)
    {
      processed=true;
      obj.restart_ticks();
      if(!obj.obj->process_event(wEvent(eTimeOut))) 
	//if we return false -> don't process any objects
	break;
    }
    ptimelist=ptimelist->get_next();
  }
  return processed;
}

//===========================================================================
/// global add_timer(WellObject*, int N_ticks)
///     add object to time list for ticking every N_ticks
/// tags WellEngine
bool    WellEngine::add_timer(WellObject* wo, int ticks)
{
  TimeObject obj(wo,ticks);
  TimeList *ptobject=new TimeList(obj);
  timelist.add(ptobject);
  return true;
}

//===========================================================================
/// global del_timer(WellObject*)
///     delete object from time list
/// tags WellEngine
bool    WellEngine::del_timer(WellObject* wo)
{
  TimeObject obj(wo);
  TimeList *plist=timelist.del(obj);
  if(plist)
  {
    delete plist;
    return true;
  }
  return false;
}

//===========================================================================
/// global new_well_drawing_engine()
///     creates drawing engine for well board - must be overloaded in childs
/// tags WellEngine
WellDrawingEngine* WellEngine::new_well_drawing_engine()
{
  return 0;
}


//===========================================================================
/// global process_event_for_all(wEvent)
///     go throuht the list of objects, and call process_event for each
///     if object return false -> break the loop
/// tags WellEngine
bool WellEngine::process_event_for_all(wEvent ev)
{
  bool processed=true;
  ObjectList* plist=objectlist.get_next();
  while(plist)
  {
    if(!plist->get_object()->process_event(ev)) 
	//if we return false -> don't process any objects
	return false;
    plist=plist->get_next();
  }
  return processed;
}

//===========================================================================
/// global add_object(WellObject*)
///     add object to list of objects that want events
/// tags WellEngine
bool    WellEngine::add_object(WellObject* wo)
{
  ObjectList *ptobject=new ObjectList(wo);
  objectlist.add(ptobject);
  return true;
}

//===========================================================================
/// global del_object(WellObject*, int N_ticks)
///     delete object from list of objects that want events
/// tags WellEngine
bool    WellEngine::del_object(WellObject* wo)
{
  ObjectList *plist=objectlist.del(wo);
  if(plist)
  {
    delete plist;
    return true;
  }
  return false;
}

//===========================================================================
/// global clear_objectlist()
///     clear all list of objects
/// tags WellEngine
void    WellEngine::clear_objectlist()
{
  ObjectList *plist;
  do 
  {
    plist=objectlist.get_next();
    if(plist)
    {
      plist->del_self();
      delete plist;
    }
  } while(plist);
}

//===========================================================================
/// global load_images()
///     load all images for the game
/// tags WellEngine
void WellEngine::load_images()
{
  fprintf(stderr, "Loading resources..");
  load_image(imBoardBG,  "board2.gif");
  fprintf(stderr, "..");
  load_image(imFont1,    "font2.gif");
  fprintf(stderr, "..");
  load_image(imIntroBG,  "wellintro.gif");
  fprintf(stderr, "..");
  load_image(imScoreBG,  "topnine.gif");
  fprintf(stderr, "..");
  load_image(imCuts,     "wellcuts.gif");
  fprintf(stderr, "..done\n");
}

//===========================================================================
/// global new_well_intro()
///     creates new introduction object - version for SDL Window
/// tags WellEngine
WellIntro* WellEngine::new_well_intro()
{
  return new WellIntro;
}

//===========================================================================
/// global new_well_top_nine()
///     creates new top nine object
/// tags WellEngine
WellTopNine* WellEngine::new_well_top_nine()
{
  return new WellTopNine;
}


//===========================================================================
/// global new_well_key(char*)
///     creates new key object
/// tags WellEngine
WellKey* WellEngine::new_well_key(char* name)
{
  return new WellKey(name);
}

//===========================================================================
/// global new_well_switch(char*)
///     creates new switch object
/// tags WellEngine
WellSwitch* WellEngine::new_well_switch(char* name)
{
  return new WellSwitch(name);
}


//===========================================================================
/// global new_well_base()
///     creates new game object
/// tags WellEngine
WellBase* WellEngine::new_well_base()
{
  return new WellBase;
}





