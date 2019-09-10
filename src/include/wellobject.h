#ifndef WELLOBJECT_H
#define WELLOBJECT_H

#include "geometry.h"

enum EvType { 
  eTimeOut, 
  eKeyPress, 
  eMouseMove, 
  eMousePress,
  eExpose,
  eDelete,
  eFocusOut,
  
  aEmpty,
  aGameOver,
  aIntroExit,
  aKeyPressed,
  aSwitchChanged,
  aInputDone
};


enum ButtonState { But1Press, But2Press, But3Press }; 

struct wEvent
{
  EvType type;
  void   *data;
  wEvent(EvType t,void *idata=0) { type=t;data=idata;};
};

struct MouseEvent
{
  int         mx,my;
  ButtonState bstate;
};

//Basic class of all game objects
class WellObject
{
 protected:
  bool    shown;
  Geo     *geo;
  char    object_name[GEO_NAME+1];

 public:
  WellObject() { shown=false; geo=0; object_name[0]=0;};
  virtual bool process_event(wEvent)=0;
  virtual void show() { shown=true;};
  virtual bool show_by_call(wEvent) { show(); return true;};
  virtual void hide() { shown=false;};
  virtual bool hide_by_call(wEvent) { hide(); return true;};
  virtual void redraw() {};
  virtual bool redraw_by_call(wEvent) { redraw(); return true;};  
};

class ObjectCaller
{
  WellObject   *object;
  bool         (WellObject::*method)(wEvent);

 public:
  ObjectCaller() { object=0; method=0;};
  ObjectCaller(WellObject* o,bool (WellObject::*m)(wEvent)) { object=o; method=m;};
  bool call(wEvent ev) 
  {
    if(!object)
      return false;
    return (object->*method)(ev);
  };
};

#endif
