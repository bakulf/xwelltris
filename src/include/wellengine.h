#ifndef WELLENGINE_H
#define WELLENGINE_H

#include "wellobject.h"
#include "bilist.h"
#include "welldrawing.h"
#include "wellimagefont.h"
#include "welltopnine.h"
#include "wellintro.h"
#include "wellkey.h"
#include "wellswitch.h"
#include "wellinput.h"

class WellBase;

//Class used in Time queue for delivering time events to objects
struct TimeObject
{
  WellObject* obj;
  int         ticks_left;
  int         ticks;
  TimeObject()  {ticks=ticks_left=0; obj=0;};
  TimeObject(WellObject* wo, int t=1) { obj=wo;ticks_left=ticks=t;};
  void restart_ticks() { ticks_left=ticks; };
  bool operator == (TimeObject& o2) {return o2.obj==obj;};
};

typedef bilist<TimeObject>  TimeList;   //List of objects that want timers 
typedef bilist<WellObject*> ObjectList; //List of all objects that waht events

struct  WellImage
{
  Images   id;   //Image id
  int      l,h;  //Size of image
  WellImage() { id=imNone; l=h=0;};
};

class WellEngine
{
 protected:

  TimeList        timelist;
  ObjectList      objectlist;

  unsigned int    mainl,mainh; // size of main window
  bool            done_loop_var;

  WellImage       images[MAX_IMAGES];

  virtual void    init_mainwindow(int,char**)=0;
  virtual int     wait_for_timers()=0;
  virtual bool    process_time_event();
  virtual bool    process_event_for_all(wEvent);

 public:
  WellEngine(int,char**);
  ~WellEngine();

  virtual void    show_main()=0;
  virtual void    event_loop()=0;

  virtual bool    add_timer(WellObject* wo, int ticks);
  virtual bool    del_timer(WellObject* wo);
  virtual bool    add_object(WellObject* wo);
  virtual bool    del_object(WellObject* wo);
  virtual void    clear_objectlist();
  virtual void    done_loop() { done_loop_var=true;};

  virtual WellDrawingEngine* new_well_drawing_engine();
  virtual WellTopNine*       new_well_top_nine();
  virtual WellIntro*         new_well_intro();
  virtual WellKey*           new_well_key(const char*);
  virtual WellInput*         new_well_input(const char*) { return 0;};
  virtual WellSwitch*        new_well_switch(const char*);
  virtual WellImageFont*     new_well_image_font(Images id, 
						 unsigned int ifl, 
						 unsigned int ifh, 
						 int idx, int idy) { return 0;};

  virtual WellBase*          new_well_base();

  virtual bool    load_image(Images id, const char* name) { return false;};
  virtual void    set_main_background_image(Images id) {};
  virtual void    screen_copy(Geo*) {};
  virtual void    screen_clear(Geo*) {};
  virtual void    load_images();
  virtual void    udelay(int msec) {};
};

extern WellEngine* default_well_engine;
extern WellEngine* new_well_engine(int argc, char** argv);

#endif
