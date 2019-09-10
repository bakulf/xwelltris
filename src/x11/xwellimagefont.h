#ifndef XWELLIMAGEFONT_H
#define XWELLIMAGEFONT_H

#include "wellimagefont.h"
#include "xwellengine.h"

class XWellImageFont: public WellImageFont
{
 protected:
  Pixmap       font_pixmap;
  XWellEngine  *xengine;
  Window       mainw;
  GC           maingc;
  virtual      void draw_symbol(int ix, int iy, 
				unsigned int il,
				unsigned int ih,
				char symb);
 public:
  XWellImageFont(Images id, unsigned int ifl, unsigned int ifh, int idx, int idy);
  virtual      void clear_region();
  virtual      void clear_region(Colors);
};

#endif


