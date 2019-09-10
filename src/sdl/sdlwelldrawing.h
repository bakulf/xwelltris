// docm_prefix(///)
#ifndef SDLWELLDRAWING_H
#define SDLWELLDRAWING_H

#include "globals.h"
#include "SDL.h"
#include "welldrawing.h"

class SDLWellEngine;

struct SDLPoint
{
    int x,y;
};

/// module description
/// Definition of the SDLWellDrawingEngine class, 
/// public child from WellDrawingEngine -> engine for drawing game field
class SDLWellDrawingEngine:public WellDrawingEngine
{
 protected:
  SDLWellEngine  *engine;
  SDL_Surface    **fields;
  SDL_Surface    *mainw;
  Uint32         *colors;
  Uint8          *RGBA;
  Uint32         bg_color, fg_color;

  Sint16         trap_x[MAX_SIDES];
  Sint16         trap_y[MAX_SIDES];

  void           flush_field(SDL_Surface* win, int ix, int iy, int il, int ih);


 public:
  SDLWellDrawingEngine(SDLWellEngine* myengine);
  virtual void init(int,int,int,unsigned int,unsigned int);
  virtual void flush_all();
  virtual void flush_dirty();
  virtual void field_sync();
  virtual void draw_grid();
  virtual void draw_square(int color, int i, int j);
  virtual void draw_trapazoid(int color, int i, int j);
  virtual void sync();
  virtual void clear_field(int i);
  virtual Actions key_to_action(void* event);
  virtual void  draw_line(int x1, int y1, int x2, int y2, int color_idx,
			  Canvas where=screen);
  virtual void  draw_rect(int x1, int y1, unsigned int il, 
				 unsigned int ih, int color_idx,
			  Canvas where=screen);
  virtual void  fill_rect(int x1, int y1, unsigned int il, 
				 unsigned int ih, int color_idx,
			  Canvas where=screen);
  virtual void  pixmap_copy(Geo *pgeo);
};

#endif
