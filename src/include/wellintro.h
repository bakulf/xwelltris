#ifndef WELLINTRO_H
#define WELLINTRO_H

#include "welltris.h"
#include "wellobject.h"
#include "wellkey.h"
#include "wellimagefont.h"
#include "wellswitch.h"
#include "wellinput.h"

class WellBase;

class WellIntro: public WellObject
{
 protected:
  ObjectCaller      object_on_exit,
                    object_on_new_game,
                    object_on_top_nine;

  WellKey           *key_exit, *key_new_game, *key_top_nine, 
                    *key_plus, *key_minus;

  WellImageFont     *txt_level;
  WellBase          *well;

  WellSwitch        *sw_rotation, *sw_mixed, *sw_next_piece,
                    *sw_squares[TOTAL_SQUARES];

  WellInput         *inp_player;

  int               start_level;
  
  void              draw_start_level();
  char*             get_next_token(char* from, char* to);

 public:
  WellIntro();
  virtual bool      process_event(wEvent);
  virtual void      show();
  virtual void      redraw();
  virtual void      hide();
  virtual bool      hide_by_call(wEvent);
  
  void              set_object_on_exit(ObjectCaller obj)

    {
      object_on_exit=obj;
    };
  void              set_object_on_new_game(ObjectCaller obj)

    {
      object_on_new_game=obj;
    };
  void              set_object_on_top_nine(ObjectCaller obj)

    {
      object_on_top_nine=obj;
    };
  void              set_well_base(WellBase* o);
  void              put_all_to_game();
  void              load_defaults();
  void              load_options();
  void              save_options();
};

#endif
