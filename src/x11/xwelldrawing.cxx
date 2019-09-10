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
// $Id: xwelldrawing.cxx,v 1.1.1.1 2003/01/04 11:37:22 leo Exp $

/// module description
/// X11 implementation of WellDrawing class - drawing routines for the game.
/// Draw field on pixmap, then maps it to the screen. Uses dirty rectangles 
/// algorithm for mapping areas to screen from pixmap

#include "xwelldrawing.h"
#include "xwellengine.h"

//===========================================================================
/// global XWellDrawingEngine()
///     constructor of the class - init's variables.
/// tags XWellDrawingEngine
XWellDrawingEngine::XWellDrawingEngine(XWellEngine* myengine)
{
  xengine=myengine;
  if(xengine==0)
    xengine=static_cast<XWellEngine*>(default_well_engine);
  mainw=xengine->get_main_window();
  colors=xengine->get_colors();
  scr=DefaultScreen(disp);
}

//===========================================================================
/// global init(int n_fields, int idx, int idy, unsigned il, unsigned ih)
///     init fields - creates x pixmaps and gcs
/// tags XWellDrawingEngine
void XWellDrawingEngine::init(int inum_fields, int idx, int idy,
			      unsigned int il, unsigned int ih)
{
  int i;
  WellDrawingEngine::init(inum_fields,idx,idy,il,ih);
  fields=new Pixmap;
  gcs=new GC;
  maingc=XCreateGC(disp,mainw,0,0);
  XSetBackground(disp,maingc,colors[BackColor]);
  XSetForeground(disp,maingc,colors[GridColor]);
  XSetGraphicsExposures(disp,maingc,False);
  
  for(i=0;i<1;i++)
  {
    fields[i]=XCreatePixmap(disp,mainw,l,h,DefaultDepth(disp,scr));
    if(fields[i]>0)
    {
      gcs[i]=XCreateGC(disp,fields[i],0,0);
      XSetBackground(disp,gcs[i],colors[FreezeColor]);
      clear_field();
    }
  }
}

//===========================================================================
/// global draw_square(int color_idx, int i, int j)
///     Draw square (floor polygon) on all fields with given color
/// tags XWellDrawingEngine
void XWellDrawingEngine::draw_square(int color, int i, int j)
{
  int idx=current_id,sign_x13,sign_y13,sign_x24,sign_y24;

  XSetForeground(disp,*gcs,colors[color]);

  sign_x13= points[idx].base_points[i+1][j+1].x - 
    points[idx].base_points[i][j].x; 
  sign_x13= sign_x13 ? (sign_x13>0 ? 1 : -1) : 0;
  
  sign_y13= points[idx].base_points[i+1][j+1].y - 
    points[idx].base_points[i][j].y;
  sign_y13= sign_y13 ? (sign_y13>0 ? 1 : -1) : 0;

  sign_x24= points[idx].base_points[i][j+1].x - 
    points[idx].base_points[i+1][j].x;
  sign_x24= sign_x24 ? (sign_x24>0 ? 1 : -1) : 0;

  sign_y24= points[idx].base_points[i][j+1].y - 
    points[idx].base_points[i+1][j].y;
  sign_y24= sign_y24 ? (sign_y24>0 ? 1 : -1) : 0;

  trapazoid_list[0].x = points[idx].base_points[i][j].x+sign_x13;
  trapazoid_list[0].y = points[idx].base_points[i][j].y+sign_y13;
  trapazoid_list[1].x = points[idx].base_points[i+1][j].x+sign_x24;
  trapazoid_list[1].y = points[idx].base_points[i+1][j].y+sign_y24;
  trapazoid_list[2].x = points[idx].base_points[i+1][j+1].x-sign_x13;
  trapazoid_list[2].y = points[idx].base_points[i+1][j+1].y-sign_y13;
  trapazoid_list[3].x = points[idx].base_points[i][j+1].x-sign_x24;
  trapazoid_list[3].y = points[idx].base_points[i][j+1].y-sign_y24;
  XFillPolygon (disp, *fields, *gcs, trapazoid_list, MAX_SIDES,
		Convex, CoordModeOrigin);
    //Now draw the grid
  trapazoid_list[0].x = points[idx].base_points[i][j].x;
  trapazoid_list[0].y = points[idx].base_points[i][j].y;
  trapazoid_list[1].x = points[idx].base_points[i+1][j].x;
  trapazoid_list[1].y = points[idx].base_points[i+1][j].y;
  trapazoid_list[2].x = points[idx].base_points[i+1][j+1].x;
  trapazoid_list[2].y = points[idx].base_points[i+1][j+1].y;
  trapazoid_list[3].x = points[idx].base_points[i][j+1].x;
  trapazoid_list[3].y = points[idx].base_points[i][j+1].y;
  XSetForeground(disp,*gcs,colors[GridColor2]);
  XDrawLines(disp, *fields, *gcs, trapazoid_list, MAX_SIDES, CoordModeOrigin);

  new_dirty_rec(delta_x,delta_y);
  dirty_add_xy(trapazoid_list[0].x,trapazoid_list[0].y);
  dirty_add_xy(trapazoid_list[1].x,trapazoid_list[1].y);
  dirty_add_xy(trapazoid_list[2].x,trapazoid_list[2].y);
  dirty_add_xy(trapazoid_list[3].x,trapazoid_list[3].y);
  finish_dirty_rec();
}

//===========================================================================
/// global draw_trapazoid(int color_idx, int i,int j)
///     draw trapazoid - wall polygon with given color
/// tags XWellDrawingEngine
void XWellDrawingEngine::draw_trapazoid(int color, int i, int j)
{
  Window win;
  GC     gc;
  int    idx=current_id,w,sign_x13,sign_y13,sign_x24,sign_y24;

  unsigned long rcolor=colors[color];
  
  win=*fields;
  gc=*gcs;
  XSetForeground(disp,gc,rcolor);
    
  sign_x13= points[idx].wall_points[i+1][j+1].x - 
    points[idx].wall_points[i][j].x; 
  sign_x13= sign_x13 ? (sign_x13>0 ? 1 : -1) : 0;
  
  sign_y13= points[idx].wall_points[i+1][j+1].y - 
    points[idx].wall_points[i][j].y;
  sign_y13= sign_y13 ? (sign_y13>0 ? 1 : -1) : 0;
  
  sign_x24= points[idx].wall_points[i][j+1].x - 
    points[idx].wall_points[i+1][j].x;
  sign_x24= sign_x24 ? (sign_x24>0 ? 1 : -1) : 0;
  
  sign_y24= points[idx].wall_points[i][j+1].y - 
    points[idx].wall_points[i+1][j].y;
  sign_y24= sign_y24 ? (sign_y24>0 ? 1 : -1) : 0;

    /*
    w=i/MAX_WIDTH;
    dbgprintf(("trapazoid [%2d][%2d] = [%d - %d],[%d - %d]\n",i,j,
	       points[idx].wall_points[i+1][j+1].x,
	       points[idx].wall_points[i][j].x,
	       points[idx].wall_points[i+1][j+1].y,
	       points[idx].wall_points[i][j].y));

    dbgprintf(("trapazoid2[%2d][%2d] = [%d - %d],[%d - %d]\n",i,j,
	       points[idx].wall_points[i][j+1].x,
	       points[idx].wall_points[i+1][j].x,
	       points[idx].wall_points[i][j+1].y,
	       points[idx].wall_points[i+1][j].y));
    dbgprintf(("trapazoid [%2d][%2d] = [%d][%d],[%d][%d]\n",i,j,
	       sign_x13,sign_y13,sign_x24,sign_y24));
    */

    
  trapazoid_list[0].x = points[idx].wall_points[i][j].x+sign_x13;
  trapazoid_list[0].y = points[idx].wall_points[i][j].y+sign_y13;
  trapazoid_list[1].x = points[idx].wall_points[i+1][j].x+sign_x24;
  trapazoid_list[1].y = points[idx].wall_points[i+1][j].y+sign_y24;
  trapazoid_list[2].x = points[idx].wall_points[i+1][j+1].x-sign_x13;
  trapazoid_list[2].y = points[idx].wall_points[i+1][j+1].y-sign_y13;
  trapazoid_list[3].x = points[idx].wall_points[i][j+1].x-sign_x24;
  trapazoid_list[3].y = points[idx].wall_points[i][j+1].y-sign_y24;
  XFillPolygon (disp, win, gc, trapazoid_list, MAX_SIDES,
		Convex, CoordModeOrigin);
    //Now draw the grid
  trapazoid_list[0].x = points[idx].wall_points[i][j].x;
  trapazoid_list[0].y = points[idx].wall_points[i][j].y;
  trapazoid_list[1].x = points[idx].wall_points[i+1][j].x;
  trapazoid_list[1].y = points[idx].wall_points[i+1][j].y;
  trapazoid_list[2].x = points[idx].wall_points[i+1][j+1].x;
  trapazoid_list[2].y = points[idx].wall_points[i+1][j+1].y;
  trapazoid_list[3].x = points[idx].wall_points[i][j+1].x;
  trapazoid_list[3].y = points[idx].wall_points[i][j+1].y;
  XSetForeground(disp,gc,colors[GridColor]);
  XDrawLines(disp, win, gc, trapazoid_list, MAX_SIDES, CoordModeOrigin);
  if(idx==current_id)
  {
    new_dirty_rec(delta_x,delta_y);
    dirty_add_xy(trapazoid_list[0].x,trapazoid_list[0].y);
    dirty_add_xy(trapazoid_list[1].x,trapazoid_list[1].y);
    dirty_add_xy(trapazoid_list[2].x,trapazoid_list[2].y);
    dirty_add_xy(trapazoid_list[3].x,trapazoid_list[3].y);
    finish_dirty_rec();
  }
}

//===========================================================================
/// global draw_grid()
///     draw grid - all wall and floor polygons 
/// tags XWellDrawingEngine
void XWellDrawingEngine::draw_grid()
{
  int idx,i,j,k;
  Window win;
  GC gc;
  idx=current_id;

  gc=*gcs;
  win=*fields;
  XSetForeground(disp,gc,colors[GridColor]);
  for(i=0;i<MAX_PERIMETER;i++)
    for(j=0;j<MAX_DEPTH;j++)
    {
      XDrawLine(disp,win,gc,points[idx].wall_points[i][j].x,
		points[idx].wall_points[i][j].y,
		points[idx].wall_points[i][j+1].x,
		points[idx].wall_points[i][j+1].y);
      XDrawLine(disp,win,gc,points[idx].wall_points[i][j].x,
		points[idx].wall_points[i][j].y,
		points[idx].wall_points[i+1][j].x,
		points[idx].wall_points[i+1][j].y);
    }
  for(i=0;i<MAX_WIDTH+1;i++)
    for(j=0;j<MAX_WIDTH+1;j++)
    {
      if(j==0 || j==MAX_WIDTH)
	XSetForeground(disp,gc,colors[GridColor2]);
      else
	XSetForeground(disp,gc,colors[GridColor]);
      
      if(i<MAX_WIDTH)
	XDrawLine(disp,win,gc,points[idx].base_points[i][j].x,
		  points[idx].base_points[i][j].y,
		  points[idx].base_points[i+1][j].x,
		  points[idx].base_points[i+1][j].y);
      
      if(i==0 || i==MAX_WIDTH)
	XSetForeground(disp,gc,colors[GridColor2]);
      else
	XSetForeground(disp,gc,colors[GridColor]);
      
      if(j<MAX_WIDTH)
	XDrawLine(disp,win,gc,points[idx].base_points[i][j].x,
		  points[idx].base_points[i][j].y,
		  points[idx].base_points[i][j+1].x,
		  points[idx].base_points[i][j+1].y);
    }
}

//===========================================================================
/// global clear_field()
///     clear field with given index with BackColor (black) 
/// tags XWellDrawingEngine
void XWellDrawingEngine::clear_field()
{
  XSetForeground(disp,*gcs,colors[BackColor]);
  XFillRectangle(disp,*fields,*gcs,0,0,l,h);
}

//===========================================================================
/// global flush_all()
///     copy all the field with current_id to the screen
/// tags XWellDrawingEngine
void XWellDrawingEngine::flush_all()
{
  XCopyArea(disp,*fields,mainw,maingc,0,0,l,h,delta_x,delta_y);
  sync();
}

//===========================================================================
/// global sync()
///     flush event in X queue
/// tags XWellDrawingEngine
void XWellDrawingEngine::sync()
{
  XFlush(disp);
}

//===========================================================================
/// global flush_dirty()
///     copy list of dirty rectangle areas to the screen
/// tags XWellDrawingEngine
void XWellDrawingEngine::flush_dirty()
{
  DirtyList *plist;

  Window win=*fields;

  do 
  {
    plist=dirty_list.get_next();
    if(plist)
    {
      DirtyRect& drec=plist->get_object();
      //Here we need to draw dirty rec on the screen -> will be in overloaded childs
      XCopyArea(disp,win,mainw,maingc,drec.get_src_x(),drec.get_src_y(),
		drec.l,drec.h,
		drec.get_dest_x(),drec.get_dest_y());

      plist->del_self();
      delete plist;
    }
  } while(plist);  
  XFlush(disp);
}

//===========================================================================
/// global key_to_action(void* event)
///     convert keys to actions in the game
/// tags XWellDrawingEngine
Actions XWellDrawingEngine::key_to_action(void* event)
{
  XEvent *ev=(XEvent*)event;
  KeySym ks;

  ks=XLookupKeysym(&ev->xkey, 0);
  switch(ks)
  {
  case XK_KP_Left:
  case XK_KP_4:
  case XK_Left:
  case XK_j:
  case XK_J:
    return RIGHT;

  case XK_KP_Right:
  case XK_KP_6:
  case XK_Right:
  case XK_l:
  case XK_L:
    return LEFT;

  case XK_Up:
  case XK_Down:
  case XK_KP_Begin:
  case XK_KP_5:
  case XK_k:
  case XK_K:
    return ROTATE;
    
  case XK_space:
  case XK_KP_0:
  case XK_KP_Insert:
    return DROP;

  case XK_Pause:
  case XK_p:
  case XK_P:
    return PAUSE_GAME;

  case XK_F10:
  case XK_q:
  case XK_Q:
  case XK_Escape:
    return END_GAME;

  case XK_KP_Multiply:
    return OUTER_ROTATION;

  case XK_KP_Add:
    return LEVEL_UP;

  case XK_KP_Subtract:
    return LEVEL_DOWN;

  }

  return NOTHING;
}


//===========================================================================
/// global draw_line(...)
///     draw line with given coords and color
/// tags XWellDrawingEngine
void XWellDrawingEngine::draw_line(int x1, int y1, int x2, int y2, int color_idx,
				   Canvas where)
{
  switch(where)
  {
  case screen:
    XSetForeground(disp,maingc,colors[color_idx]);
    XDrawLine(disp,mainw,maingc,x1,y1,x2,y2);
    break;
  case pixmap:
    XSetForeground(disp,*gcs,colors[color_idx]);
    XDrawLine(disp,*fields,*gcs,x1,y1,x2,y2);
    break;
  }
}

//===========================================================================
/// global draw_rect(...)
///     draw rectangle with given coords and color
/// tags XWellDrawingEngine
void XWellDrawingEngine::draw_rect(int x1, int y1, 
				   unsigned int il, 
				   unsigned int ih, int color_idx,
				   Canvas where)
{
  switch(where)
  {
  case screen:
    XSetForeground(disp,maingc,colors[color_idx]);
    XDrawRectangle(disp,mainw,maingc,x1,y1,il,ih);
    break;
  case pixmap:
    XSetForeground(disp,*gcs,colors[color_idx]);
    XDrawRectangle(disp,*fields,*gcs,x1,y1,il,ih);
    break;
  }
}

//===========================================================================
/// global fill_rect(...)
///     fill rectangle with given coords and color
/// tags XWellDrawingEngine
void XWellDrawingEngine::fill_rect(int x1, int y1, 
				   unsigned int il, 
				   unsigned int ih, int color_idx,
				   Canvas where)
{
  switch(where)
  {
  case screen:
    XSetForeground(disp,maingc,colors[color_idx]);
    XFillRectangle(disp,mainw,maingc,x1,y1,il,ih);
    break;
  case pixmap:
    XSetForeground(disp,*gcs,colors[color_idx]);
    XFillRectangle(disp,*fields,*gcs,x1,y1,il,ih);
    break;
  }
}

//===========================================================================
/// global pixmap_copy(Geo*)
///     copy part of image to the screen
/// tags XWellDrawingEngine
void XWellDrawingEngine::pixmap_copy(Geo *pgeo)
{
  if(pgeo->im==imNone)
    return;
  XCopyArea(disp,xengine->get_pixmap_of_image(pgeo->im),*fields,*gcs,
	    pgeo->fromx,pgeo->fromy,pgeo->l,pgeo->h,
	    pgeo->tox,pgeo->toy);
}

