#ifndef XWELLINPUT_H
#define XWELLINPUT_H

#include "wellinput.h"
#include "xheaders.h"
#include "xwellengine.h"

class XWellInput:public WellInput
{
  XWellEngine        *xengine;
  Window             mainw;
  GC                 maingc;

  virtual void       draw_text();
  void               process_key(XEvent *xev);
 public:
  XWellInput(char *iname);

  virtual bool       process_event(wEvent);

};


#endif
