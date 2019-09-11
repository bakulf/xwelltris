#ifndef SDLWELLENGINE_H
#define SDLWELLENGINE_H

#include "globals.h"
#include "welltris.h"

#include "SDL.h"

#include "wellengine.h"

class SDLWellEngine:public WellEngine
{
 protected:
    SDL_Surface     *mainw;
    SDL_Surface     *pics[MAX_IMAGES];
    Uint32           game_colors[MAX_GAME_COLORS];
    Uint8            game_RGBA[MAX_GAME_COLORS*4];
    SDL_Surface     *bg_image;

    SDL_Event        key_event;
    bool             key_event_got;
    int              key_ticks;

    virtual void     init_mainwindow(int,char**);
    virtual int      wait_for_timers();
    
    virtual void     init_colors();
    void             set_key_event(SDL_Event&);
    void             reset_key_event();
    void             process_key_event();
    
 public:
    SDLWellEngine(int argc,char** argv);
    ~SDLWellEngine();
    virtual void     show_main();
    virtual void     event_loop();
    virtual bool     load_image(Images id, const char* name);
    virtual void     set_main_background_image(Images id);
    virtual WellImageFont*     new_well_image_font(Images id, 
						   unsigned int ifl, 
						   unsigned int ifh, 
						   int idx, int idy);
    virtual WellInput*         new_well_input(const char*);
    virtual WellDrawingEngine* new_well_drawing_engine();

    SDL_Surface*     get_pixmap_of_image(Images id) { return pics[id];};
    SDL_Surface*     get_main_window() { return mainw;};
    Uint32*          get_colors() { return game_colors;};
    Uint8*           get_RGBA() { return game_RGBA;};

    virtual void     udelay(int msec);
    virtual void     screen_copy(Geo*);
    virtual void     screen_clear(Geo*);
};

#endif
