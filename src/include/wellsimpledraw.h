#ifndef WELLSIMPLEDRAW_H
#define WELLSIMPLEDRAW_H

#include "bilist.h"

enum Canvas { screen=0, pixmap=1 };

struct WPoint
{
  int x,y;
  WPoint() { x=y=0;};
};

struct DirtyRect
{
  int delta_x,delta_y; //here is the delta between dest screen and our dirty screen
  int x,y;
  unsigned int l,h;
  DirtyRect() { x=y=delta_x=delta_y=0;l=h=0;};
  DirtyRect(int dx, int dy,
	    int ix=0, int iy=0, 
	    unsigned int il=0, unsigned int ih=0)
  {
    delta_x=dx; delta_y=dy;
    x=ix; y=iy;
    l=il; h=ih;
  };
  int get_src_x() { return x;};
  int get_src_y() { return y;};
  int get_dest_x() { return delta_x+x;};  
  int get_dest_y() { return delta_y+y;};  
};

typedef bilist<DirtyRect> DirtyList;

class WellSimpleDraw
{
 protected:
  DirtyList     dirty_list;
  int           max_x,max_y,min_x,min_y;
  DirtyRect     current_dirty;
  int           max_dirties;
 
 public:
  WellSimpleDraw();
  ~WellSimpleDraw();
  virtual void  new_dirty_rec(int dx=0, int dy=0);
  virtual void  dirty_add_xy(int,int);
  virtual void  dirty_add_rec(DirtyRect);
  virtual void  finish_dirty_rec();
  virtual void  clear_dirty_list();
  virtual void  draw_line(int x1, int y1, int x2, int y2, int color_idx,
			  Canvas where=screen) {};
  virtual void  draw_rect(int x1, int y1, unsigned int il, 
				 unsigned int ih, int color_idx,
			  Canvas where=screen) {};
  virtual void  fill_rect(int x1, int y1, unsigned int il, 
				 unsigned int ih, int color_idx,
			  Canvas where=screen) {};
  int           get_max_dirties() { return max_dirties;};
};

#endif
