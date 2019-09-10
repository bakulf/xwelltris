#ifndef SDLWELLIMAGEFONT_H
#define SDLWELLIMAGEFONT_H

#include "wellimagefont.h"
#include "sdlwellengine.h"

class SDLWellImageFont: public WellImageFont
{
 protected:
    SDLWellEngine *engine;
    SDL_Surface  *font_surf;
    SDL_Surface  *mainw;
    virtual      void draw_symbol(int ix, int iy, 
				  unsigned int il,
				  unsigned int ih,
				  char symb);
 public:
    SDLWellImageFont(Images id, unsigned int ifl, unsigned int ifh, int idx, int idy);
    virtual      void clear_region();
    virtual      void clear_region(Colors);
};

#endif


