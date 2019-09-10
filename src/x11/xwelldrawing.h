// docm_prefix(///)
#ifndef XWELLDRAWING_H
#define XWELLDRAWING_H

#include "globals.h"

#include "xheaders.h"

#include "welldrawing.h"

class XWellEngine;

/// module description
/// Definition of the XWellDrawingEngine class, 
/// public child from WellDrawingEngine -> engine for drawing game field
class XWellDrawingEngine:public WellDrawingEngine
{
 protected:
  XWellEngine  *xengine;
  Pixmap       *fields;
  GC           *gcs,maingc;
  Window       mainw;
  unsigned long *colors;
  int          scr;
  XPoint       trapazoid_list[MAX_SIDES];


 public:
  XWellDrawingEngine(XWellEngine* myengine);
  virtual void init(int,int,int,unsigned int,unsigned int);
  virtual void flush_all();
  virtual void flush_dirty();
  virtual void draw_grid();
  virtual void draw_square(int color, int i, int j);
  virtual void draw_trapazoid(int color, int i, int j);
  virtual void sync();
  virtual void clear_field();
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
