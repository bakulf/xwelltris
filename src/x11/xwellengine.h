#ifndef XWELLENGINE_H
#define XWELLENGINE_H

#include "globals.h"
#include "welltris.h"

#include "xheaders.h"

#include "wellengine.h"
#include "xwelldrawing.h"
#include "xwellimagefont.h"

extern Display *disp;
extern Colormap defcmp;

class XWellEngine:public WellEngine
{
 protected:
  int              scr;    //DefaultScreen of X display
  int              xcon;   //X Connection file descriptor
  Window           mainw;
  GC               maingc;
  XWMHints         *pxwmh;
  XSizeHints       *pxsh;
  XClassHint       *pch;
  XTextProperty    wname;
  XTextProperty    iname;
  Colormap         colormap;
  Atom             wmDeleteWindow;
  XEvent           xev;    // X Event we got from x-server
  Pixmap           image_pixmaps[MAX_IMAGES]; //X pixmaps of images

  unsigned long    whitepixel,blackpixel;

  unsigned long    game_colors[MAX_GAME_COLORS];

  struct timeval   nexttime,now;


  virtual void     init_mainwindow(int,char**);
  virtual int      wait_for_timers();
  virtual void     init_colors();
  void             correct_to_real_time(struct timeval& tv);

 public:
  XWellEngine(int argc,char** argv);
  ~XWellEngine();
  virtual void     show_main();
  virtual void     event_loop();

  virtual WellDrawingEngine* new_well_drawing_engine();
  virtual WellTopNine*       new_well_top_nine();
  virtual WellIntro*         new_well_intro();
  virtual WellKey*           new_well_key(const char*);
  virtual WellSwitch*        new_well_switch(const char*);
  virtual WellInput*         new_well_input(const char*);
  virtual WellImageFont*     new_well_image_font(Images id, 
						 unsigned int ifl, 
						 unsigned int ifh, 
						 int idx, int idy);
  virtual WellBase*          new_well_base();

  Window           get_main_window() {return mainw;};
  GC               get_main_gc() {return maingc;};
  Pixmap           get_pixmap_of_image(Images id) { return image_pixmaps[id];};
  unsigned long*   get_colors() { return game_colors; };
  virtual bool     load_image(Images id, const char* name);
  virtual void     set_main_background_image(Images id);
  virtual void     screen_copy(Geo*);
  virtual void     screen_clear(Geo*);
  virtual void     udelay(int msec);
};

#endif






