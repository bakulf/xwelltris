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
// $Id: sdlwelldrawing.cxx,v 1.4 2003/02/21 11:40:33 leo Exp $

/// module description
/// SDL implementation of WellDrawing class - drawing routines for the game.
/// Draw field on pixmap, then maps it to the screen. Uses dirty rectangles 
/// algorithm for mapping areas to screen from pixmap

#include "sdlwelldrawing.h"
#include "sdlwellengine.h"
#include "sdl_gfxprimitives.h"

//===========================================================================
/// global SDLWellDrawingEngine()
///     constructor of the class - init's variables.
/// tags SDLWellDrawingEngine
SDLWellDrawingEngine::SDLWellDrawingEngine(SDLWellEngine* myengine)
{
    engine=myengine;
    if(engine==0)
	engine=static_cast<SDLWellEngine*>(default_well_engine);
    mainw=engine->get_main_window();
    colors=engine->get_colors();
    RGBA=engine->get_RGBA();
}

//===========================================================================
/// global init(int n_fields, int idx, int idy, unsigned il, unsigned ih)
///     init fields - creates x pixmaps and gcs
/// tags SDLWellDrawingEngine
void SDLWellDrawingEngine::init(int inum_fields, int idx, int idy,
				unsigned int il, unsigned int ih)
{

    int i;
    SDL_Surface *surface;

    WellDrawingEngine::init(inum_fields,idx,idy,il,ih);
    fields=new SDL_Surface*;

    fg_color=colors[GridColor];
  
    for(i=0;i<1;i++)
    {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	surface = SDL_CreateRGBSurface(SDL_SWSURFACE, l, h, 32,
				       0xff000000, 
				       0x00ff0000,
				       0x0000ff00,
				       0x000000ff);
#else
	surface = SDL_CreateRGBSurface(SDL_SWSURFACE, l, h, 32,
				       0x000000ff,
				       0x0000ff00,
				       0x00ff0000,
				       0xff000000);
#endif

	fields[i]=SDL_DisplayFormat(surface);
	SDL_FreeSurface(surface);
	if(fields[i]>0)
	    clear_field();
    }
    bg_color=colors[BackColor];
    fg_color=colors[GridColor];
}

//===========================================================================
/// global draw_square(int color_idx, int i, int j)
///     Draw square (floor polygon) on all fields with given color
/// tags SDLWellDrawingEngine
void SDLWellDrawingEngine::draw_square(int color, int i, int j)
{
    int idx=current_id,sign_x13,sign_y13,sign_x24,sign_y24;

    Uint8 *rcolor=&RGBA[color*4];

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

    trap_x[0] = points[idx].base_points[i][j].x+sign_x13;
    trap_y[0] = points[idx].base_points[i][j].y+sign_y13;
    trap_x[1] = points[idx].base_points[i+1][j].x+sign_x24;
    trap_y[1] = points[idx].base_points[i+1][j].y+sign_y24;
    trap_x[2] = points[idx].base_points[i+1][j+1].x-sign_x13;
    trap_y[2] = points[idx].base_points[i+1][j+1].y-sign_y13;
    trap_x[3] = points[idx].base_points[i][j+1].x-sign_x24;
    trap_y[3] = points[idx].base_points[i][j+1].y-sign_y24;

    filledPolygonRGBA(*fields, trap_x, trap_y, MAX_SIDES, 
		      rcolor[0],
		      rcolor[1],
		      rcolor[2],
		      rcolor[3]
		      );

    //Now draw the grid
    trap_x[0] = points[idx].base_points[i][j].x;
    trap_y[0] = points[idx].base_points[i][j].y;
    trap_x[1] = points[idx].base_points[i+1][j].x;
    trap_y[1] = points[idx].base_points[i+1][j].y;
    trap_x[2] = points[idx].base_points[i+1][j+1].x;
    trap_y[2] = points[idx].base_points[i+1][j+1].y;
    trap_x[3] = points[idx].base_points[i][j+1].x;
    trap_y[3] = points[idx].base_points[i][j+1].y;

    polygonRGBA(*fields, trap_x, trap_y, MAX_SIDES, 
		RGBA[GridColor2*4],
		RGBA[GridColor2*4+1],
		RGBA[GridColor2*4+2],
		RGBA[GridColor2*4+3]
		);

    new_dirty_rec(delta_x,delta_y);
    dirty_add_xy(trap_x[0],trap_y[0]);
    dirty_add_xy(trap_x[1],trap_y[1]);
    dirty_add_xy(trap_x[2],trap_y[2]);
    dirty_add_xy(trap_x[3],trap_y[3]);
    finish_dirty_rec();
}

//===========================================================================
/// global draw_trapazoid(int color_idx, int i,int j)
///     draw trapazoid - wall polygon with given color
/// tags SDLWellDrawingEngine
void SDLWellDrawingEngine::draw_trapazoid(int color, int i, int j)
{
    SDL_Surface *win=*fields;  
    int    idx=current_id,sign_x13,sign_y13,sign_x24,sign_y24;
    
    Uint8 *rcolor=&RGBA[color*4];
  
    
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
      w=i/MASDL_WIDTH;
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

    
    trap_x[0] = points[idx].wall_points[i][j].x+sign_x13;
    trap_y[0] = points[idx].wall_points[i][j].y+sign_y13;
    trap_x[1] = points[idx].wall_points[i+1][j].x+sign_x24;
    trap_y[1] = points[idx].wall_points[i+1][j].y+sign_y24;
    trap_x[2] = points[idx].wall_points[i+1][j+1].x-sign_x13;
    trap_y[2] = points[idx].wall_points[i+1][j+1].y-sign_y13;
    trap_x[3] = points[idx].wall_points[i][j+1].x-sign_x24;
    trap_y[3] = points[idx].wall_points[i][j+1].y-sign_y24;
    filledPolygonRGBA(win, trap_x, trap_y, MAX_SIDES, 
		      rcolor[0],
		      rcolor[1],
		      rcolor[2],
		      rcolor[3]
		      );
    //Now draw the grid
    trap_x[0] = points[idx].wall_points[i][j].x;
    trap_y[0] = points[idx].wall_points[i][j].y;
    trap_x[1] = points[idx].wall_points[i+1][j].x;
    trap_y[1] = points[idx].wall_points[i+1][j].y;
    trap_x[2] = points[idx].wall_points[i+1][j+1].x;
    trap_y[2] = points[idx].wall_points[i+1][j+1].y;
    trap_x[3] = points[idx].wall_points[i][j+1].x;
    trap_y[3] = points[idx].wall_points[i][j+1].y;
    polygonRGBA(win, trap_x, trap_y, MAX_SIDES, 
		RGBA[GridColor*4],
		RGBA[GridColor*4+1],
		RGBA[GridColor*4+2],
		RGBA[GridColor*4+3]
		);

    if(idx==current_id)
    {
	new_dirty_rec(delta_x,delta_y);
	dirty_add_xy(trap_x[0],trap_y[0]);
	dirty_add_xy(trap_x[1],trap_y[1]);
	dirty_add_xy(trap_x[2],trap_y[2]);
	dirty_add_xy(trap_x[3],trap_y[3]);
	finish_dirty_rec();
    }
}

//===========================================================================
/// global draw_grid()
///     draw grid - all wall and floor polygons 
/// tags SDLWellDrawingEngine
void SDLWellDrawingEngine::draw_grid()
{
    int idx,i,j;
    SDL_Surface* win;
    idx=current_id;

    win=*fields;

    for(i=0;i<MAX_PERIMETER;i++)
	for(j=0;j<MAX_DEPTH;j++)
	{
	    lineRGBA(win,points[idx].wall_points[i][j].x,
		     points[idx].wall_points[i][j].y,
		     points[idx].wall_points[i][j+1].x,
		     points[idx].wall_points[i][j+1].y,
		     RGBA[GridColor*4],
		     RGBA[GridColor*4+1],
		     RGBA[GridColor*4+2],
		     RGBA[GridColor*4+3]
		     );
	    lineRGBA(win,points[idx].wall_points[i][j].x,
		     points[idx].wall_points[i][j].y,
		     points[idx].wall_points[i+1][j].x,
		     points[idx].wall_points[i+1][j].y,
		     RGBA[GridColor*4],
		     RGBA[GridColor*4+1],
		     RGBA[GridColor*4+2],
		     RGBA[GridColor*4+3]
		     );
	}
    for(i=0;i<MAX_WIDTH+1;i++)
	for(j=0;j<MAX_WIDTH+1;j++)
	{
	    if(j==0 || j==MAX_WIDTH)
		fg_color=GridColor2*4;
	    else
		fg_color=GridColor*4;
      
	    if(i<MAX_WIDTH)
		lineRGBA(win,points[idx].base_points[i][j].x,
			 points[idx].base_points[i][j].y,
			 points[idx].base_points[i+1][j].x,
			 points[idx].base_points[i+1][j].y,
			 RGBA[fg_color],
			 RGBA[fg_color+1],
			 RGBA[fg_color+2],
			 RGBA[fg_color+3]
			 );
	    
	    if(i==0 || i==MAX_WIDTH)
		fg_color=GridColor2*4;
	    else
		fg_color=GridColor*4;
      
	    if(j<MAX_WIDTH)
		lineRGBA(win,points[idx].base_points[i][j].x,
			 points[idx].base_points[i][j].y,
			 points[idx].base_points[i][j+1].x,
			 points[idx].base_points[i][j+1].y, 
			 RGBA[fg_color],
			 RGBA[fg_color+1],
			 RGBA[fg_color+2],
			 RGBA[fg_color+3]
			 );
	}
    flush_field(*fields, 0, 0, l, h);
}

//===========================================================================
/// global clear_field()
///     clear field with given index with BackColor (black) 
/// tags SDLWellDrawingEngine
void SDLWellDrawingEngine::clear_field()
{
    boxRGBA(*fields, 0, 0, l, h, 
	    RGBA[BackColor*4],
	    RGBA[BackColor*4+1],
	    RGBA[BackColor*4+2],
	    RGBA[BackColor*4+3]
	    );
    flush_field(*fields, 0, 0, l, h);
}

//===========================================================================
/// global flush_all()
///     copy all the field with current_id to the screen
/// tags SDLWellDrawingEngine
void SDLWellDrawingEngine::flush_all()
{
    SDL_Rect src,dest;
    src.x = 0;
    src.y = 0;
    src.w = l;
    src.h = h;
    dest.x = delta_x;
    dest.y = delta_y;
    dest.w = l;
    dest.h = h;
    SDL_BlitSurface(*fields, &src, mainw, &dest);
    /* Update the changed portion of the screen */
    SDL_UpdateRects(mainw, 1, &dest);
}

//===========================================================================
/// global flush_field()
///     copy all the field with current_id to the screen
/// tags SDLWellDrawingEngine
void SDLWellDrawingEngine::flush_field(SDL_Surface* win, 
				       int ix, int iy,
				       int il, int ih)
{
    SDL_Rect dest;
    dest.x = ix;
    dest.y = iy;
    dest.w = il;
    dest.h = ih;
    /* Update the changed portion of the screen */
    SDL_UpdateRects(win, 1, &dest);
}

//===========================================================================
/// global flush_field()
///     copy all the field with current_id to the screen
/// tags SDLWellDrawingEngine
void SDLWellDrawingEngine::field_sync()
{
    flush_field(*fields, 0, 0, l, h);
}
//===========================================================================
/// global sync()
///     flush event in SDL queue
/// tags SDLWellDrawingEngine
void SDLWellDrawingEngine::sync()
{
    //Nothing to do
    //    flush_field(*fields, 0, 0, l, h);
}

//===========================================================================
/// global flush_dirty()
///     copy list of dirty rectangle areas to the screen
/// tags SDLWellDrawingEngine
void SDLWellDrawingEngine::flush_dirty()
{
    SDL_Rect *from, *to;
    int      idx=0;

    if(!get_max_dirties())
	return;

    from=new SDL_Rect[get_max_dirties()];
    to  =new SDL_Rect[get_max_dirties()];

    DirtyList *plist;

    do 
    {
	plist=dirty_list.get_next();
	if(plist)
	{
	    DirtyRect& drec=plist->get_object();
	    //Here we need to draw dirty rec on the screen -> will be in overloaded childs
	    from[idx].x=drec.get_src_x();
	    from[idx].y=drec.get_src_y();
	    from[idx].w=to[idx].w=drec.l;
	    from[idx].h=to[idx].h=drec.h;
	    to[idx].x=drec.get_dest_x();
	    to[idx].y=drec.get_dest_y();

	    SDL_BlitSurface(*fields, &from[idx], mainw, &to[idx]);
	    plist->del_self();
	    delete plist;
	    idx++;
	}
    } while(plist);  

    SDL_UpdateRects(mainw, idx, to);

    delete from;
    delete to;
}

//===========================================================================
/// global key_to_action(void* event)
///     convert keys to actions in the game
/// tags SDLWellDrawingEngine
Actions SDLWellDrawingEngine::key_to_action(void* event)
{
    SDL_Event *ev=(SDL_Event*)event;
    switch(ev->key.keysym.sym)
    {
    case SDLK_KP4:
    case SDLK_LEFT:
    case SDLK_j:
	return RIGHT;
	
    case SDLK_KP6:
    case SDLK_RIGHT:
    case SDLK_l:
	return LEFT;
	
    case SDLK_UP:
    case SDLK_DOWN:
    case SDLK_KP5:
    case SDLK_k:
	return ROTATE;
    
    case SDLK_SPACE:
    case SDLK_KP0:
	return DROP;

    case SDLK_BREAK:
    case SDLK_PAUSE:
    case SDLK_p:
	return PAUSE_GAME;

    case SDLK_F10:
    case SDLK_q:
    case SDLK_ESCAPE:
	return END_GAME;

    case SDLK_KP_MULTIPLY:
	return OUTER_ROTATION;

    case SDLK_KP_PLUS:
	return LEVEL_UP;

    case SDLK_KP_MINUS:
	return LEVEL_DOWN;

    }
    return NOTHING;
}


//===========================================================================
/// global draw_line(...)
///     draw line with given coords and color
/// tags SDLWellDrawingEngine
void SDLWellDrawingEngine::draw_line(int x1, int y1, int x2, int y2, int color_idx,
				     Canvas where)
{
    Uint8 *color=&RGBA[color_idx*4];
    switch(where)
    {
    case screen:
	lineRGBA(mainw,x1,y1,x2,y2,
		      color[0],
		      color[1],
		      color[2],
		      color[3]
		 );
	break;
    case pixmap:
	lineRGBA(*fields,x1,y1,x2,y2,
		      color[0],
		      color[1],
		      color[2],
		      color[3]
		 );
	break;
    }
}

//===========================================================================
/// global draw_rect(...)
///     draw rectangle with given coords and color
/// tags SDLWellDrawingEngine
void SDLWellDrawingEngine::draw_rect(int x1, int y1, 
				     unsigned int il, 
				     unsigned int ih, int color_idx,
				     Canvas where)
{
    Uint8 *color=&RGBA[color_idx*4];
    switch(where)
    {
    case screen:
	rectangleRGBA(mainw,x1,y1,x1+il-1,y1+ih-1,
		      color[0],
		      color[1],
		      color[2],
		      color[3]
		      );
	flush_field(mainw, x1,y1, il, ih);
	break;
    case pixmap:
	rectangleRGBA(*fields,x1,y1,x1+il-1,y1+ih-1,
		      color[0],
		      color[1],
		      color[2],
		      color[3]
		      );
	flush_field(*fields, x1,y1, il, ih);
	break;
    }
}

//===========================================================================
/// global fill_rect(...)
///     fill rectangle with given coords and color
/// tags SDLWellDrawingEngine
void SDLWellDrawingEngine::fill_rect(int x1, int y1, 
				     unsigned int il, 
				     unsigned int ih, int color_idx,
				     Canvas where)
{
    Uint8 *color=&RGBA[color_idx*4];
    switch(where)
    {
    case screen:
	boxRGBA(mainw,x1,y1,x1+il-1,y1+ih-1,
		 color[0],
		 color[1],
		 color[2],
		 color[3]
		 );
	flush_field(mainw, x1,y1, il, ih);
	break;
    case pixmap:
	boxRGBA(*fields,x1,y1,x1+il-1,y1+ih-1,
		color[0],
		color[1],
		color[2],
		color[3]
		);
	flush_field(*fields, x1,y1, il, ih);
	break;
    }
}

//===========================================================================
/// global pixmap_copy(Geo*)
///     copy part of image to the screen
/// tags SDLWellDrawingEngine
void SDLWellDrawingEngine::pixmap_copy(Geo *pgeo)
{
    SDL_Rect src,dest;
    if(pgeo->im==imNone)
	return;
    
    src.x=pgeo->fromx;
    src.y=pgeo->fromy;
    src.w=dest.w=pgeo->l;
    src.h=dest.h=pgeo->h;
    dest.x=pgeo->tox;
    dest.y=pgeo->toy;

    SDL_BlitSurface(engine->get_pixmap_of_image(pgeo->im), 
		    &src, *fields, &dest);
    SDL_UpdateRects(*fields, 1, &dest);
}

