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
// $Id: welldrawing.cxx,v 1.1.1.1 2003/01/04 11:37:22 leo Exp $
#include "globals.h"
#include "welldrawing.h"

//===========================================================================
/// global init(int n_fields, int dx, int dy, unsigned l, unsigned h)
///     init indexes and allocate FieldPoints
/// tags WellDrawingEngine
void WellDrawingEngine::init(int inum_fields, int idx, int idy,
			     unsigned int il, unsigned int ih)
{
  l=il;h=ih;
  delta_x=idx;
  delta_y=idy;
  current_id=0;
  num_fields=inum_fields;
  points=new FieldPoints[num_fields];
  inner_max_alpha=0.0;
  f_step=1;
}


//===========================================================================
/// global flush_dirty()
///     pseudo function that need flush dirty rec to the screen,
///     here it only delete dirty rect from queue
/// tags WellDrawingEngine
void WellDrawingEngine::flush_dirty()
{
  DirtyList *plist;
  do 
  {
    plist=dirty_list.get_next();
    if(plist)
    {
      //Here we need to draw dirty rec on the screen -> will be in overloaded childs

      plist->del_self();
      delete plist;
    }
  } while(plist);  

}


void WellDrawingEngine::draw_square(int color, int x, int y)
{

}

void WellDrawingEngine::draw_trapazoid(int color, int x, int y)
{

}

void WellDrawingEngine::sync()
{

}

void WellDrawingEngine::flush_all()
{

}

void WellDrawingEngine::bell(int)
{

}


//===========================================================================
/// global outer_rotation()
///     make shift current field id -> switch to next field -> rotate it
/// tags WellDrawingEngine
void WellDrawingEngine::outer_rotation()
{
  current_id+=f_step;
  if(current_id>=num_fields)
  {
    f_step*=-1;
    current_id+=(f_step+f_step);
  }
  if(current_id<0)
  {
    f_step*=-1;
    current_id+=(f_step+f_step);
  }
}


//===========================================================================
/// global set_field_sizes(int dx, int dy, int wall_delta, int base_delta)
///     set and calculate base vars
/// tags WellDrawingEngine
void WellDrawingEngine::set_field_sizes(int idx, int idy,
					int ipix_wall_delta, 
					int ipix_base_delta)
{
  pix_dx=idx;
  pix_dy=idy;
  pix_wall_delta=ipix_wall_delta;
  pix_base_delta=ipix_base_delta;
  pix_base_width=pix_base_delta*MAX_WIDTH;
  pix_max_delta=pix_wall_delta*MAX_DEPTH;
  pix_max_width=pix_base_width + 2*pix_max_delta;
}

//===========================================================================
/// global calc_grid_points()
///     calculate all points for each field, rotate it with inner and 
///     outer alpha angle
/// tags WellDrawingEngine
void WellDrawingEngine::calc_grid_points()
{
  int gi,i,j,w,x,y,n;
  float alpha;
  center_x=pix_dx+pix_max_width/2;
  center_y=pix_dy+pix_max_width/2;
  for(n=0;n<num_fields;n++) //For each field
  {
    //Calculate points for the walls
    for(gi=0;gi<MAX_PERIMETER;gi++)
      for(j=0;j<MAX_DEPTH+1;j++)
      {
	w=gi/MAX_WIDTH;   //calc wall number
	i=gi-w*MAX_WIDTH;  //and delta inside it
	switch(w)
	{
	case 0:
	  x=j*pix_wall_delta + (i*(pix_max_width - 2*pix_wall_delta*j))/MAX_WIDTH;
	  y=j*pix_wall_delta;
	  break;
	case 1:
	  x=pix_max_width - j*pix_wall_delta;
	  y=j*pix_wall_delta + (i*(pix_max_width - 2*pix_wall_delta*j))/MAX_WIDTH;
	  break;
	case 2:
	  x=pix_max_width - (j*pix_wall_delta +
			     (i*(pix_max_width - 2*pix_wall_delta*j))/MAX_WIDTH);
	  y=pix_max_width - j*pix_wall_delta;
	  break;
	case 3:
	  x=j*pix_wall_delta;
	  y=pix_max_width - (j*pix_wall_delta + 
			     (i*(pix_max_width - 2*pix_wall_delta*j))/MAX_WIDTH);
	  break;
	}
	//	dbgprintf(("i=%2d, j=%2d -> x=%4d, y=%4d\n",i,j,x,y));
	x+=pix_dx;
	y+=pix_dy;
	alpha=
	  inner_max_alpha*float(j)/float(MAX_DEPTH)*float(n)/float(num_fields-1)+
	  float(n)*outer_max_alpha/float(num_fields-1);

	if(alpha!=0.0) //lets make rotation
	{
	  points[n].wall_points[gi][j].x=int(
	    float((x-center_x))*cos(alpha) - 
	    float((y-center_y))*sin(alpha) +
	    center_x);

	  points[n].wall_points[gi][j].y=int(
	    float((x-center_x))*sin(alpha) + 
	    float((y-center_y))*cos(alpha) +
	    center_y);

	} else
	{
	  points[n].wall_points[gi][j].x=x;
	  points[n].wall_points[gi][j].y=y;
	}
      }
    for(j=0;j<MAX_DEPTH+1;j++)
      points[n].wall_points[MAX_PERIMETER][j]=points[n].wall_points[0][j];

    //Calculate points for the base (floor) of the glass
    for(i=0;i<MAX_WIDTH+1;i++)
      for(j=0;j<MAX_WIDTH+1;j++)
      {
	x=pix_dx + pix_max_delta + i*pix_base_delta;
	y=pix_dy + pix_max_delta + j*pix_base_delta;
	alpha=
	  float(n)*inner_max_alpha/float(num_fields-1) + 
	  float(n)*outer_max_alpha/float(num_fields-1);
	if(alpha!=0.0) //lets make rotation
	{
	  points[n].base_points[i][j].x=int(
	    float((x-center_x))*cos(alpha) - 
	    float((y-center_y))*sin(alpha) +
	    center_x);

	  points[n].base_points[i][j].y=int(
	    float((x-center_x))*sin(alpha) + 
	    float((y-center_y))*cos(alpha) +
	    center_y);

	} else
	{
	  points[n].base_points[i][j].x=x;
	  points[n].base_points[i][j].y=y;
	}
      }
  }
}
