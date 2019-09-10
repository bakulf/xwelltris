#ifndef DRAWING_H
#define DRAWING_H

#include "globals.h"
#include "welltris.h"
#include "wellsimpledraw.h"
#include "geometry.h"

struct FieldPoints
{
  WPoint       wall_points[MAX_PERIMETER+1][MAX_DEPTH+1];
  WPoint       base_points[MAX_WIDTH+1][MAX_WIDTH+1];
};

class WellDrawingEngine: public WellSimpleDraw
{
 protected:
  int          num_fields;
  unsigned int l,h;
  int          delta_x,delta_y;
  int          current_id;

  int          pix_max_width;
  int          pix_base_width;
  int          pix_base_delta;
  int          pix_wall_delta;
  int          pix_max_delta;
  int          pix_dx,pix_dy;
  int          center_x,center_y;
  int          f_step;

  float        inner_max_alpha,outer_max_alpha;

  FieldPoints  *points;

 public:
  virtual  void     init(int,int,int,unsigned int,unsigned int);
  virtual  void     draw_square(int color, int x, int y);
  virtual  void     draw_trapazoid(int color, int x, int y);
  virtual  void     sync();
  virtual  void     field_sync() {};
  virtual  void     bell(int);
  virtual  void     flush_all();
  virtual  void     flush_dirty();
  virtual  void     clear_field(int i) {};
  virtual  void     set_field_id(int id) { current_id=id;};
  virtual  void     draw_grid() {};
  virtual  Actions  key_to_action(void* event) { return NOTHING; };
  virtual  void     calc_grid_points();
  virtual  void     set_field_sizes(int idx, int idy,
				    int ipix_wall_delta, 
				    int ipix_base_delta);
  virtual  void     set_inner_alpha(float ialpha) { inner_max_alpha=
						      ialpha*float(W_PI/180.0);}; 
  virtual  void     set_outer_alpha(float ialpha) { outer_max_alpha=
						      ialpha*float(W_PI/180.0);}; 
  virtual  void     outer_rotation();
  virtual  void     pixmap_copy(Geo *pgeo) {};
};


#endif
