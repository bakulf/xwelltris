#ifndef WELLTOPNINE_H
#define WELLTOPNINE_H

#include "globals.h"
#include "welltris.h"
#include "wellobject.h"
#include "wellimagefont.h"
#include "wellkey.h"

struct Score
{
  int  place;
  char player[PLAYER_NAME_LEN+1];
  char date_time[L_DATE];
  int  score;
  int  lines;
  int  level;
  int  squares;
  bool mixed;
  bool rotation;
  bool next_piece;
  time_t start_t;
  int  play_time;

  Score();
  void              save(FILE* fp);
  bool              load(FILE* fp);
  void              set_current_time();
  char*             get_next_token(char* from, char* to);
  void              calc_play_time();
  void              set_player(char* pl);
  void              set_params(int iscore, int ilines, int ilevel) 
                     { 
		       score=iscore; lines=ilines; level=ilevel;
		     };
};

struct ScoreTable
{
  Score *table[TOP_NINE];

  ScoreTable();
  ~ScoreTable();

  void              sort_by_score();
  void              save(FILE* fp);
  void              set_squares(int isquares);
  Score*            set_and_get(int idx, Score *toset);
};

class WellTopNine: public WellObject
{
 protected:
  ScoreTable        tables[MAX_SQUARES];
  WellImageFont*    text_lines[TOP_NINE];
  ObjectCaller      object_on_exit;

  Score*            current_score;
  int               page;

  WellKey           *key_exit;

 public:
  WellTopNine();

  virtual bool      process_event(wEvent);
  virtual void      show();
  virtual void      hide();
  virtual void      redraw_table();
  bool              save_scores();
  bool              load_scores();
  Score*            try_add_to_table(Score* new_one);
  Score*            find_challenger(Score* me);
  void              set_object_on_exit(ObjectCaller o) 
    { 
      object_on_exit=o;
    };
  void              set_page(int pg) { page=pg;};
};

extern WellTopNine *default_top_nine;


#endif
