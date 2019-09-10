#ifndef WELLKEY_H
#define WELLKEY_H

#include "wellobject.h"

class WellKey: public WellObject
{
 protected:
  ObjectCaller      object_on_press;
  bool              highlighted;

  bool              press(int mx, int my);
  bool              highlight(int mx, int my);

 public:
  WellKey(char* keyname);
  void              set_object_on_press(ObjectCaller o) { object_on_press=o;};
  virtual bool      process_event(wEvent);
  virtual void      show();
  virtual void      hide();
  virtual void      redraw();
};

#endif
