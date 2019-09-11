#ifndef WELLINPUT_H
#define WELLINPUT_H

#include "globals.h"
#include "wellobject.h"
#include "wellimagefont.h"

class WellInput: public WellObject
{
 protected:
  ObjectCaller      object_on_enter;
  char              buf[L_MAXPATH];
  int               buflen,pos,maxlen;
  WellImageFont     *fnt;

  virtual void      draw_text();

 public:
  WellInput(const char* keyname);
  void              set_object_on_enter(ObjectCaller o) { object_on_enter=o;};
  virtual bool      process_event(wEvent);
  virtual void      show();
  virtual void      hide();
  virtual void      redraw();
  void              set_text(const char* txt);
  void              set_max_len(int ml) { maxlen=ml;};
  virtual const char* get_text() { return buf;};
};

#endif
