#ifndef WELLCLASS_H
#define WELLCLASS_H

#include "globals.h"      
#include "welltris.h"
#include "wellobject.h"
#include "welldrawing.h"
#include "wellengine.h"
#include "wellimagefont.h"
#include "welltopnine.h"

//define structures for work with figures
struct thing_t
{
  int             squares, polyomino_number;
  int             xpos, ypos;
  int             size, color_number;
  int             bonus, random_rotation, random_reflection;
  long            random_number;
  bool            merged;
  thing_t() 
  { 
    squares=polyomino_number=xpos=ypos=size=0;
    color_number=bonus=random_rotation=random_reflection=0; 
    random_number=0;
    merged=false;
  };
};

struct Polyominoes 
{
  int             rotation;
  int             reflection;
  int             start_height;
  int             shape[MAX_SQUARES][MAX_SQUARES];
  int             size;
};

struct Mode
{
  int             number[MAX_MODES][MAX_TYPES];
  int             start[MAX_START_POLYOMINOES][MAX_TYPES];
  int             turn_style;
};

struct Polytris 
{
    Polyominoes     polyomino[MAX_POLYOMINOES];
    Mode            mode[MAX_MODES];
    int             diagonal_switch;
};

struct Field
{
  int             pmid, cid;
};


class WellBase: public WellObject
{
 protected:
  Polytris tris[MAX_SQUARES - MIN_SQUARES + 1];
  int      frozen_wall[MAX_SIDES];
  thing_t  curThing, nextThing;
  Field    wall[MAX_DEPTH+MAX_WIDTH][MAX_PERIMETER];
  Field    base[MAX_WIDTH][MAX_WIDTH];

  int      level, score, rows, rows_levelup;

  bool     bonusNow;

  //Options
  bool     showNext;
  bool     beep;
  bool     diagonal;
  bool     mixed;
  bool     bonus;
  bool     grid;
  bool     cw;
  bool     gradualAppear;
  bool     rotation;
  int      squares, pause, steps_to_rotate;

  //Drawing Engine

  WellDrawingEngine *dre;
  WellImageFont     *txt_score;
  WellImageFont     *txt_lines;
  WellImageFont     *txt_levelup;
  WellImageFont     *txt_level;

  WellImageFont     *txt_first;
  WellImageFont     *txt_second;
  WellImageFont     *txt_third;

  Score             *player_score;

  ObjectCaller      game_over_object;

  char              player_name[PLAYER_NAME_LEN];

  void              set_default_options();

  void              read_polyominoes();
  void              new_thing();
  void              redo_next();
  void              put_box();
  bool              overlapping();
  bool              at_bottom();
  bool              at_base_fully();
  bool              at_base_partially();
  bool              wall_change(thing_t old,thing_t new_t);
  bool              all_frozen();
  void              wall_to_base(int* base_x,int* base_y,int wall_x,int wall_y);
  void              try_move(Actions move);
  void              draw_thing_diff(thing_t* old);
  void              draw_thing();
  void              draw_field();
  void              fill_lines();
  void              freeze_wall(int w);
  void              defreeze_wall(int w);
  void              drop_wall(int);
  void              check_freeze();
  int               check_lines();
  void              check_update_lines();
  bool              process_action(Actions action);
  void              make_pause();
  void              make_rotation();

  virtual  void     draw_status();
  virtual  void     draw_next();
  virtual  void     clear_next();
  virtual  void     draw_grid();
  virtual  void     draw_box(int color, int x, int y);

 public:
  WellBase();
  void              init(WellEngine*);
  void              udelay(int msec);
  bool              move_one_step(Actions);
  void              game_over(); 
  void              new_game();
  void              new_level_calc();
  void              redraw_all();
  void              set_level(int il) { level=il; dbgprintf(("Level now is %d\n",level));};
  void              set_rotation(bool r) { rotation=r;};
  void              set_mixed(bool r) { mixed=r;};
  void              set_next_piece(bool r) { showNext=r;};
  void              set_squares(int i) { squares=i;};
  void              set_object_on_game_over(ObjectCaller obj)
    { 
      game_over_object=obj;
    };
  void              set_player_name(char *pn)
    {
      strcpy(player_name,pn);
    };
  virtual  bool     process_event(wEvent);
  virtual  void     show();
  virtual  void     hide();
};

#endif

