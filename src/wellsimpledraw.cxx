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
// $Id: wellsimpledraw.cxx,v 1.2 2003/02/19 09:51:39 leo Exp $

/// module description
/// WellSimpleDrawing - class that implements simple drawing algorithm
/// with dirty rectangles

#include "wellsimpledraw.h"

//===========================================================================
/// global WellSimpleDraw()
///     constructor of the class - init's variables.
/// tags WellSimpleDraw
WellSimpleDraw::WellSimpleDraw()
{
    max_dirties=0;
}

//===========================================================================
/// global ~WellSimpleDraw()
///     destructor of the class.
/// tags WellSimpleDraw
WellSimpleDraw::~WellSimpleDraw()
{
  clear_dirty_list();
}

//===========================================================================
/// global new_dirty_rec(int dx, int dy)
///     create new dirty rec and remember delta position for main screen
/// tags WellSimpleDraw
void WellSimpleDraw::new_dirty_rec(int dx, int dy)
{
  current_dirty.delta_x=dx;
  current_dirty.delta_y=dy;
  max_x=max_y=-1;
  min_x=min_y=99999; //Hope we will never have screen bigger :)  
}

//===========================================================================
/// global dirty_add_xy(int x, int y)
///     we add dot to current dirty rec - need detect max and min corners
/// tags WellSimpleDraw
void WellSimpleDraw::dirty_add_xy(int x, int y)
{
  if(x<min_x)
    min_x=x;
  if(x>max_x)
    max_x=x;
  if(y<min_y)
    min_y=y;
  if(y>max_y)
    max_y=y;
}

//===========================================================================
/// global finish_dirty_rec()
///     finish dots definition of current dirty rec - add it to the dirty queue
/// tags WellSimpleDraw
void WellSimpleDraw::finish_dirty_rec()
{
  current_dirty.x=min_x;
  current_dirty.y=min_y;
  current_dirty.l=max_x-min_x+1;
  current_dirty.h=max_y-min_y+1;
  DirtyList *plist=new DirtyList(current_dirty);
  dirty_list.add(plist);
  max_dirties++;
}

//===========================================================================
/// global clear_dirty_list()
///     clear all the list and free allocated memory
/// tags WellSimpleDraw
void WellSimpleDraw::clear_dirty_list()
{
  DirtyList *plist;
  do 
  {
    plist=dirty_list.get_next();
    if(plist)
    {
      plist->del_self();
      delete plist;
    }
  } while(plist);
  max_dirties=0;
}

//===========================================================================
/// global dirty_add_rec(DirtyRect r)
///     directly add the whole rec to the dirty queue
/// tags WellSimpleDraw
void  WellSimpleDraw::dirty_add_rec(DirtyRect r)
{
  DirtyList *plist=new DirtyList(r);
  dirty_list.add(plist);
  max_dirties++;
}
