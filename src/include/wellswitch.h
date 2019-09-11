#ifndef WELLSWITCH_H
#define WELLSWITCH_H

#include "wellobject.h"

enum SwitchMode { OnlySet, OnlyReset, SetReset};

class WellSwitch: public WellObject
{
 protected:
  ObjectCaller      object_on_switch;
  bool              highlighted;
  bool              value;
  SwitchMode        mode;

  bool              press(int mx, int my);
  bool              highlight(int mx, int my);

 public:
  WellSwitch(const char* keyname);
  void              set_object_on_switch(ObjectCaller o) { object_on_switch=o;};
  virtual bool      process_event(wEvent);
  virtual void      show();
  virtual void      hide();
  virtual void      redraw();
  void              set_value(bool v) { value=v; if(shown) redraw();};
  bool              get_value() { return value;};
  void              set_mode(SwitchMode m) { mode=m;};
};

#endif
