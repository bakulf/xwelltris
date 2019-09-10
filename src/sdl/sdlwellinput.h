#ifndef SDLWELLINPUT_H
#define SDLWELLINPUT_H

#include "wellinput.h"
#include "sdlwellengine.h"

class SDLWellInput:public WellInput
{
  SDLWellEngine      *engine;
  SDL_Surface        *mainw;

  virtual void       draw_text();
  void               process_key(SDL_Event *xev);
  void               show_cursor();
  void               hide_cursor();

 public:
  SDLWellInput(char *iname);

  virtual bool       process_event(wEvent);

};


#endif
