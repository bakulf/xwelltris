#ifndef WELLIMAGEFONT_H
#define WELLIMAGEFONT_H

#include "globals.h"
#include "welltris.h"
#include "wellsimpledraw.h"

enum Justify { RightJust, CenterJust, LeftJust };

class WellImageFont: public WellSimpleDraw
{
 protected:
  Images       font_id;
  unsigned int font_l, font_h;
  int          dx,dy;
  char         text[L_MAXPATH];
  int          text_l;
  int          screen_x,screen_y,screen_l,screen_h;
  
  virtual      void draw_symbol(int ix, int iy, 
				unsigned int il,
				unsigned int ih,
				char symb)=0;
 public:
  WellImageFont(Images id, unsigned int ifl, unsigned int ifh, int idx, int idy);
  virtual      void set_text(char* itext);
  virtual      void set_font(unsigned int ifl, unsigned int ifh, 
			     int idx, int idy);
  virtual      void set_screen_region(int ix, int iy, int il, int ih);
  virtual      void draw_text(Justify just=LeftJust);
  virtual      void draw_text(char* text, int text_l, Justify just=LeftJust);
  virtual      void draw_text(char* text, int text_l, Colors idx, Justify just=LeftJust);
  virtual      void clear_region()=0;
  virtual      void clear_region(Colors idx)=0;
};

#endif
