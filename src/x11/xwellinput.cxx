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
// $Id: xwellinput.cxx,v 1.1.1.1 2003/01/04 11:37:22 leo Exp $

/// module description
/// X11 implementation of WellInput class - make input text from user
/// display it through WellImageFont, convert X11 Key event to symbols

#include "xwellinput.h"
#include "xwellengine.h"

//===========================================================================
/// global XWellInput(char*)
///     constructor - fill name and get geometry
/// tags XWellInput
XWellInput::XWellInput(char* iname) : WellInput(iname)
{
  xengine=(XWellEngine*) default_well_engine;
  mainw=xengine->get_main_window();
  maingc=xengine->get_main_gc();
}

//===========================================================================
/// global process_event(wEvent)
///     stub that process events
/// tags XWellInput
bool XWellInput::process_event(wEvent ev)
{
  XEvent *xev=(XEvent*)ev.data;
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
/// tags XWellInput
void XWellInput::draw_text()
{
  unsigned long *colors=xengine->get_colors();
  fnt->draw_text(buf,buflen,BackColor);
  XSetForeground(disp,maingc,colors[BonusColor2]);
  XFillRectangle(disp,mainw,maingc,geo[1].tox+buflen*FONT2_L,geo[1].toy,
		 FONT2_L,FONT2_H);
}

//===========================================================================
/// global process_key(XEvent)
///     draw text on the screen
/// tags XWellInput
void XWellInput::process_key(XEvent *xev)
{
  KeySym ks;
  XComposeStatus cs;

  char   sym[4]="";
  XLookupString(&xev->xkey, sym, 4, &ks, &cs);
  switch(ks)
  {
  case XK_Return:
    if(buflen)
      object_on_enter.call(wEvent(aInputDone,this));
    break;

  case XK_Delete:
  case XK_BackSpace:
    if(buflen>0)
    {
      buflen--;
      buf[buflen]=0;
      draw_text();
    }
    break;
  default:
    if(buflen<maxlen && sym[0]>=32 && sym[0]<127)
    {
      buf[buflen++]=sym[0];
      buf[buflen]=0;
      draw_text();
    }
  }
}
