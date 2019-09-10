// docm_prefix(///)
/****************************************************************************
*  Copyright (C) 2002 by Leo Khramov
*  email:   leo@xnc.dubna.su
* 
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
 ****************************************************************************/
/*
#  Original algorithm with polyominoes by
#
#  David A. Bagley, bagleyd@bigfoot.com
#
#  Q. Alex Zhao, azhao@cc.gatech.edu
#
$Id: wellclass.cxx,v 1.3 2003/02/21 11:40:33 leo Exp $

*/

/// module description
/// Main class that is the game itself - contains game board, generate figures
/// drop it to the field, and interacts with the user.

#include "wellclass.h"
#include "wellengine.h"
#include "welldrawing.h"
#include "commonfuncs.h"

static int      speeds[NUM_LEVELS] = {
  100, 92, 84, 76, 68, 60, 53, 46, 39, 32, 26, 20, 15, 11, 8, 5, 3, 1, 0};

//===========================================================================
/// global WellBase()
///   Constructor of base class - loads polyominoes and init structs
/// tags WellBase
WellBase::WellBase() : WellObject()
{
  int             turn_style, number_polyominoes; 

  dre=0;

  level=0;
  rows=score=0;
  pause=steps_to_rotate=0;
  player_name[0]=0;

  set_default_options();

  SRAND(time(NULL));
  read_polyominoes();

  turn_style = tris[squares - MIN_SQUARES].mode[diagonal].turn_style;
  if (turn_style < 0 || turn_style >= MAX_TYPES)
  {
    fprintf(stderr, "Welltris: corrupted input file %s\n",
		   POLYOMINOFILE);
    fprintf(stderr, "\tturn_style = %d.\n", turn_style);
    exit(1);
  }
  number_polyominoes = 
    tris[squares - MIN_SQUARES].mode[diagonal].number[mixed][turn_style];
  if (number_polyominoes <= 0 || number_polyominoes > MAX_START_POLYOMINOES)
  {
    fprintf(stderr, "Welltris: corrupted input file %s\n",
		   POLYOMINOFILE);
    fprintf(stderr, "\tnumber_polyominoes = %d.\n",
		   number_polyominoes);
    exit(1);
  }
  geo=get_geo_by_name("the_game");
}

//===========================================================================
/// global new_game()
///     set params for new game
/// tags WellBase
void WellBase::new_game()
{
  int i,j;
  for (j = 0; j < MAX_DEPTH; j++)
    for (i = 0; i < MAX_PERIMETER; i++)
    {
      wall[j][i].pmid = EMPTY;
      wall[j][i].cid = 0;
    }
  for (j = 0; j < MAX_WIDTH; j++)
    for (i = 0; i < MAX_WIDTH; i++)
    {
      base[j][i].pmid = EMPTY;
      base[j][i].cid = 0;
    }
  for (i = 0; i < MAX_SIDES; i++)
    frozen_wall[i] = 0;

  dre->set_inner_alpha(30);
  dre->set_outer_alpha(0);
  dre->set_field_sizes(20,20,14,18);
  dre->calc_grid_points();
  dre->set_field_id(0);

  rows=score=0;

  if(rotation)
    steps_to_rotate=level;
  else
    steps_to_rotate=0;

  new_level_calc();

  new_thing();
  check_freeze();
  new_thing();
  
  player_score=new Score;
  player_score->set_player(player_name);
  player_score->squares=squares;
  player_score->set_params(score,rows,level);
  player_score->next_piece=showNext;
  player_score->rotation=rotation;
  player_score->mixed=mixed;
}

//===========================================================================
/// global init(WellEngine*)
///     init class and other related stuff
/// tags WellBase
void WellBase::init(WellEngine* we)
{
  //Additional init's  such as read defaults....
  dbgprintf(("WellBase: game board size: %dx%d\n",VIEW_L,VIEW_H));
  //And Draw Engine inits
  if(dre==0)
    dre=we->new_well_drawing_engine();
  dre->init(NUM_ROT_STEPS,VIEW_X,VIEW_Y,VIEW_L,VIEW_H);

  txt_score=we->new_well_image_font(imFont1,FONT2_L,FONT2_H,FONT2_DX,FONT2_DY);
  txt_score->set_screen_region(SCORE_REG_X,SCORE_REG_Y,
			       SCORE_REG_L,SCORE_REG_H);

  txt_lines=we->new_well_image_font(imFont1,FONT1_L,FONT1_H,FONT1_DX,FONT1_DY);
  txt_lines->set_screen_region(LINES_REG_X,LINES_REG_Y,
			       LINES_REG_L,LINES_REG_H);

  txt_levelup=we->new_well_image_font(imFont1,FONT1_L,FONT1_H,FONT1_DX,FONT1_DY);
  txt_levelup->set_screen_region(LEVUP_REG_X,LEVUP_REG_Y,
				 LEVUP_REG_L,LEVUP_REG_H);

  txt_level=we->new_well_image_font(imFont1,FONT1_L,FONT1_H,FONT1_DX,FONT1_DY);
  txt_level->set_screen_region(LEVEL_REG_X,LEVEL_REG_Y,
			       LEVEL_REG_L,LEVEL_REG_H);

  txt_first=we->new_well_image_font(imFont1,FONT1_L,FONT1_H,FONT1_DX,FONT1_DY);
  txt_first->set_screen_region(CHAL1_REG_X,CHAL1_REG_Y,
			       CHAL_REG_L,CHAL_REG_H);

  txt_second=we->new_well_image_font(imFont1,FONT1_L,FONT1_H,FONT1_DX,FONT1_DY);
  txt_second->set_screen_region(CHAL2_REG_X,CHAL2_REG_Y,
				CHAL_REG_L,CHAL_REG_H);

  txt_third=we->new_well_image_font(imFont1,FONT1_L,FONT1_H,FONT1_DX,FONT1_DY);
  txt_third->set_screen_region(CHAL3_REG_X,CHAL3_REG_Y,
			       CHAL_REG_L,CHAL_REG_H);

}


//===========================================================================
/// global new_level_calc()
///     calculate params for new level
/// tags WellBase
void WellBase::new_level_calc()
{
  rows_levelup=THRESHOLD(level);
  if(shown)
  {
    if(rotation)
      make_rotation();
    default_well_engine->del_timer(this);
    default_well_engine->add_timer(this,speeds[level]);
  }
}

//==========================================================================
/// global set_default_option()
///   set options needed by the game to its default values
/// tags WellBase
void WellBase::set_default_options()
{
  bonusNow=false;
  showNext=true;
  beep=false;
  diagonal=false;
  mixed=true;
  bonus=true;
  grid=true;
  cw=true;
  gradualAppear=false;
  squares=4;
  rotation=true;
}

//===========================================================================
/// global read_polyominoes()
///   reading polyominoes (our game figures) info from file
/// tags WellBase
void WellBase::read_polyominoes()
{
  char fname[L_MAXPATH];
  int c, i, j, k, sq, polyomino, sum, start, n, size, height, diag,
    game, toss = 0;
  int counter[MAX_SQUARES - MIN_SQUARES + 1];
  int start_counter[MAX_SQUARES - MIN_SQUARES + 1][MAX_MODES][MAX_TYPES];
  FILE *fp;

  find_full_path_for_file(POLYOMINOFILE,fname,ReadOnly);

  for (sq = 0; sq <= MAX_SQUARES - MIN_SQUARES; sq++)
  {
    counter[sq] = 0;
    for (polyomino = 0; polyomino < MAX_POLYOMINOES; polyomino++)
      for (j = 0; j < MAX_SQUARES; j++)
        for (i = 0; i < MAX_SQUARES; i++)
          tris[sq].polyomino[polyomino].shape[j][i] = 0;
    for (j = 0; j < MAX_TYPES; j++)
    {
      for (polyomino = 0; polyomino < MAX_START_POLYOMINOES; polyomino++)
      {
        tris[sq].mode[NODIAG].start[polyomino][j] = 0;
        tris[sq].mode[DIAGONAL].start[polyomino][j] = 0;
      }
      start_counter[sq][NODIAG][j] = 0;
      start_counter[sq][DIAGONAL][j] = 0;
      tris[sq].mode[NODIAG].number[NOMIX][j] = 0;
      tris[sq].mode[DIAGONAL].number[NOMIX][j] = 0;
      tris[sq].mode[NODIAG].number[MIXED][j] = 0;
      tris[sq].mode[DIAGONAL].number[MIXED][j] = 0;
    }
    tris[sq].mode[NODIAG].turn_style = NONE;
    tris[sq].mode[DIAGONAL].turn_style = NONE;
  }
  if ((fp = fopen(fname, "r")) == (FILE *)NULL)
  {
    (void) fprintf(stderr, "Can not open input file, %s\n.", POLYOMINOFILE);
    exit(1);
  }
  else
  {
    while ((c = getc(fp)) != EOF)
    {
      if (c == '/')
      {
        while ((c = getc(fp)) != '/') {}
        c = getc(fp);
      }
      if (c == '+')
      {
        (void) fscanf(fp, "%d", &k);
        sq = k - MIN_SQUARES;
        for (i = 0; i < MAX_TYPES; i++)
        {
          (void) fscanf(fp, "%d", &k);
          tris[sq].mode[NODIAG].number[NOMIX][i] = k;
        }
        (void) fscanf(fp, "%d", &k);
        tris[sq].mode[NODIAG].turn_style = k;
        (void) fscanf(fp, "%d", &k);
        tris[sq].diagonal_switch = k;
        if (tris[sq].diagonal_switch == true)
        {
          for (i = 0; i < MAX_TYPES; i++)
          {
            (void) fscanf(fp, "%d", &k);
            tris[sq].mode[DIAGONAL].number[NOMIX][i] = k;
          }
          (void) fscanf(fp, "%d", &k);
          tris[sq].mode[DIAGONAL].turn_style = k;
        }
        c = getc(fp);
      }
      if (c == '*')
      {
        (void) fscanf (fp, "%d", &size);
        c = getc(fp);
      }
      if (c == '~') /* Useful for debugging Things */
      {
        (void) fscanf (fp, "%d", &toss);
        c = getc(fp);
      }
      if (c == '#')
      {
        (void) fscanf(fp, "%d", &k);
        sq = k - MIN_SQUARES;
        (void) fscanf(fp, "%d", &n);
        if (tris[sq].diagonal_switch == true)
          (void) fscanf(fp, "%d", &n);
        for (polyomino = 0; polyomino <= toss; polyomino++)
          while ((c = getc(fp)) != EOF && c != '\n');
        for (polyomino = 0; polyomino < n - toss; polyomino++)
        {
          sum = polyomino + counter[sq];
            /* This is only there to "read" input file */
          (void) fscanf(fp, "%d", &k);
          (void) fscanf(fp, "%d", &k);
          tris[sq].polyomino[sum].rotation =
            k + counter[sq] - toss;
          (void) fscanf(fp, "%d", &k);
          tris[sq].polyomino[sum].reflection =
            k + counter[sq] - toss;
          for (game = JUMPIN; game <= GRADUAL; game++)
          {
            (void) fscanf(fp, "%d", &height);
            if (!gradualAppear && game == JUMPIN)
              tris[sq].polyomino[sum].start_height = height;
            else if (gradualAppear && game == GRADUAL)
              tris[sq].polyomino[sum].start_height = height;
            for (diag = NODIAG; diag <= tris[sq].diagonal_switch; diag++)
            {
              (void) fscanf(fp, "%d", &start);
              if (game == JUMPIN)
              {
                if (sq == 0 ||
                    tris[sq - 1].mode[diag].turn_style == NONE)
                {
                  i = start_counter[sq][diag][NONE];
                  tris[sq].mode[diag].start[i][NONE] = sum;
                  start_counter[sq][diag][NONE]++;
                }
                if ((sq == 0 ||
                     tris[sq - 1].mode[diag].turn_style < ALL) &&
                    start != NONE)
                {
                  i = start_counter[sq][diag][NOREFL];
                  tris[sq].mode[diag].start[i][NOREFL] = sum;
                  start_counter[sq][diag][NOREFL]++;
                }
                if (start == ALL)
                {
                  i= start_counter[sq][diag][ALL];
                  tris[sq].mode[diag].start[i][ALL] = sum;
                  start_counter[sq][diag][ALL]++;
                }
              }
            }
          }
	  tris[sq].polyomino[sum].size = size;
          for (j = 0; j < size; j++)
            for (i = 0; i < size; i++)
            {
              (void) fscanf(fp, "%d", &k);
              tris[sq].polyomino[sum].shape[j][i] = k;
            }
        }
        counter[sq] += n - toss;
        toss = 0;
      }
    }
    (void) fclose(fp);
    for (i = 0; i <= MAX_SQUARES - MIN_SQUARES; i++)
      for (j = 0; j < MAX_TYPES; j++)
        for (k = 0; k <= i; k++)
        {
          tris[i].mode[NODIAG].number[MIXED][j] +=
            tris[k].mode[NODIAG].number[NOMIX][j];
          if (tris[i].diagonal_switch == true)
             /*since k & i are not independent*/
            tris[i].mode[DIAGONAL].number[MIXED][j] +=
              tris[k].mode[DIAGONAL].number[NOMIX][j];
        }
  }
}


//===========================================================================
/// global show()
///     install itself in lists for proper processing events, draw itself.
/// tags WellBase
void WellBase::show()
{
  default_well_engine->set_main_background_image(imBoardBG);
  dbgprintf(("WellBase registering event handlers\n"));
  shown=true;
  default_well_engine->add_timer(this,speeds[level]);
  default_well_engine->add_object(this);
  dre->clear_field(0);
  draw_field();
  draw_thing();

  redraw_all();
}

//===========================================================================
/// global hide()
///     unregister itself from any events
/// tags WellBase
void WellBase::hide()
{
  shown=false;
  dbgprintf(("WellBase unregistering from engine\n"));
  default_well_engine->del_timer(this);
  default_well_engine->del_object(this);
}

//===========================================================================
/// global new_thing()
///   Generate new figure random gen for field
/// tags WellBase
void WellBase::new_thing()
{
    curThing = nextThing;
    nextThing.random_number = LRAND();
    nextThing.random_rotation = NRAND(MAX_SIDES);
    nextThing.random_reflection = NRAND(2);
    nextThing.bonus = bonusNow;
    bonusNow = false;
    redo_next();
}

//===========================================================================
/// global redo_next()
///   fill nextThing vars with data from polyomino
/// tags WellBase
void WellBase::redo_next()
{
    int turn_style = tris[squares - MIN_SQUARES].mode[diagonal].turn_style;
    int next_start, i;

    nextThing.squares = squares - MIN_SQUARES + nextThing.bonus;
    nextThing.squares = nextThing.squares > MAX_SQUARES - MIN_SQUARES ? 
      MAX_SQUARES - MIN_SQUARES: nextThing.squares;
    next_start = nextThing.random_number % tris[nextThing.squares].
      mode[diagonal].number[(nextThing.bonus) ? NOMIX : mixed][turn_style];
    nextThing.color_number = next_start;
    dbgprintf(("NEXT COLOR number is %d\n",next_start));
    if (mixed && !nextThing.bonus)
    {
      nextThing.squares = 0;
      while (next_start >=
             tris[nextThing.squares].mode[diagonal].number[NOMIX][turn_style])
      {
        next_start -=
          tris[nextThing.squares].mode[diagonal].number[NOMIX][turn_style];
        nextThing.squares++;
      }
    }
    nextThing.polyomino_number =
      tris[nextThing.squares].mode[diagonal].start[next_start][turn_style];
    if (tris[nextThing.squares].mode[diagonal].turn_style > NONE)
      for (i = 0; i < nextThing.random_rotation; i++)
        nextThing.polyomino_number = tris[nextThing.squares].polyomino
          [nextThing.polyomino_number].rotation;
    if (tris[nextThing.squares].mode[diagonal].turn_style == ALL)
      for (i = 0; i < nextThing.random_reflection; i++)
        nextThing.polyomino_number = tris[nextThing.squares].polyomino
          [nextThing.polyomino_number].reflection;
    nextThing.size =
      tris[nextThing.squares].polyomino[nextThing.polyomino_number].size;
    nextThing.ypos = -tris[nextThing.squares].polyomino
                          [nextThing.polyomino_number].start_height;
}


//===========================================================================
/// global put_box()
///   Putting figure into game field
/// tags WellBase
void WellBase::put_box()
{
    int             i, j, xi, yj;
    int             x = curThing.xpos, y = curThing.ypos;

    for (i = 0; i < curThing.size; i++)
      for (j = 0; j < curThing.size; j++)
        if ((y + j >= 0) && tris[curThing.squares].polyomino
              [curThing.polyomino_number].shape[j][i]) {
          xi = (x + i) % MAX_PERIMETER;
          yj = y + j;
          if (yj < MAX_DEPTH) { // Draw on the wall of glass
            wall[yj][xi].pmid = tris[curThing.squares].polyomino
              [curThing.polyomino_number].shape[j][i];
	    wall[yj][xi].cid = curThing.color_number;
            frozen_wall[xi / MAX_WIDTH] = MAX_SIDES;
          } else { // Draw on the floor (base)
            int base_i, base_j;
            wall_to_base(&base_i, &base_j, xi, yj);
            base[base_j][base_i].pmid = tris[curThing.squares].polyomino
              [curThing.polyomino_number].shape[j][i];
	    base[base_j][base_i].cid = curThing.color_number;
          }
	}
}


//===========================================================================
/// global overlapping()
///   check overlapping our figure in current position with another ones
///   on game field
/// tags WellBase
bool WellBase::overlapping()
{
    int             i, j, xi, yj;
    int             x = curThing.xpos, y = curThing.ypos;

    for (i = 0; i < curThing.size; i++)
      for (j = 0; j < curThing.size; j++)
	if (tris[curThing.squares].polyomino
                [curThing.polyomino_number].shape[j][i]) {
          xi = (x + i) % MAX_PERIMETER;
          yj = y + j;
          if (yj < MAX_DEPTH) {
            if (frozen_wall[xi / MAX_WIDTH])
	      return true; 
	    if (gradualAppear) {
/* This method one can turn polyomino to an area above of screen, also
   part of the polyomino may not be visible initially */          
              if ((yj >= 0) && (wall[yj][xi].pmid >= 0))
                return true;
            } else {
/* This method does not allow turning polyomino to an area above screen */
              if ((yj < 0) || (wall[yj][xi].pmid >= 0))
	        return true;
            }
          }
          else if (yj < MAX_DEPTH + MAX_WIDTH) {
            int base_i, base_j;
            wall_to_base(&base_i, &base_j, xi, yj);
            if (base[base_j][base_i].pmid >= 0)
	      return true;
          }
          else
            return true;
        }

    return false;
}

//===========================================================================
/// global wall_to_base(int* base_x,int* base_y,int wall_x,int wall_y)
///    Translate wall coordinates to floor (base) one.
/// tags WellBase
void WellBase::wall_to_base(int* base_x,int* base_y,int wall_x,int wall_y)
{
  switch (wall_x / MAX_WIDTH)
  {
    case 0:
      *base_x = wall_x;
      *base_y = wall_y - MAX_DEPTH;
      break;
    case 1:
      *base_x = MAX_WIDTH - 1 + MAX_DEPTH - wall_y;
      *base_y = wall_x - MAX_WIDTH;
      break;
    case 2:
      *base_x = MAX_PERIMETER - 1 - MAX_WIDTH - wall_x;
      *base_y = MAX_WIDTH - 1 + MAX_DEPTH - wall_y;
      break;
    case 3:
      *base_x = wall_y - MAX_DEPTH;
      *base_y = MAX_PERIMETER - 1 - wall_x;
      break;
    default:
     (void) fprintf (stderr, "wall_to_base kinds range 0-3, got %d.\n",
               wall_x / MAX_WIDTH);
      exit(-1);
  }
}

//===========================================================================
/// global at_bottom()
///    Do we at bottom of the glass? (any piece of our figure)
/// tags WellBase
bool WellBase::at_bottom()
{
    int             i, j, xi, yj;
    int             x = curThing.xpos, y = curThing.ypos;

    for (i = 0; i < curThing.size; i++)
      for (j = 0; j < curThing.size; j++)
	if (tris[curThing.squares].polyomino
                [curThing.polyomino_number].shape[j][i]) {
          xi = (x + i) % MAX_PERIMETER;
          yj = y + j;
    	  if (yj < -1)
            return false;
          if (yj < MAX_DEPTH - 1) {
            if (frozen_wall[xi / MAX_WIDTH])
	      return true; 
            if (wall[yj + 1][xi].pmid >= 0)
	      return true;
          }
          else if (yj < MAX_DEPTH + MAX_WIDTH - 1) {
            int base_i, base_j;
            wall_to_base(&base_i, &base_j, xi, yj + 1);
            if (base[base_j][base_i].pmid >= 0)
	      return true;
          }
          else
            return true;
        }

    return false;
}

//===========================================================================
/// global at_base_fully()
///    Do we fully on the floor? (all pieces of our figure)
/// tags WellBase
bool WellBase::at_base_fully()
{
    int             i, j;

    for (i = 0; i < curThing.size; i++)
      for (j = 0; j < curThing.size; j++)
	if (curThing.ypos + j < MAX_DEPTH && tris[curThing.squares].polyomino
              [curThing.polyomino_number].shape[j][i])
          return false;
     return true;
}            

//===========================================================================
/// global at_base_partially()
///    Do we on the floor? (at least one of our piece)
/// tags WellBase
bool WellBase::at_base_partially()
{
    int             i, j;

    for (i = 0; i < curThing.size; i++)
      for (j = 0; j < curThing.size; j++)
        if (curThing.ypos + j >= MAX_DEPTH && tris[curThing.squares].polyomino
              [curThing.polyomino_number].shape[j][i])
          return true;
     return false;
}
         
//===========================================================================
/// global wall_change(thing_t old,thing_t new_t)
///    Do we change the wall from last position?
/// tags WellBase
bool WellBase::wall_change(thing_t old,thing_t new_t)
{
    int             w = -1, i, j;
    int             x = curThing.xpos;

    for (i = 0; i < old.size; i++)
      for (j = 0; j < old.size; j++)
	if (tris[old.squares].polyomino[old.polyomino_number].shape[j][i]) {
          if (w == -1)
            w = ((x + i) % MAX_PERIMETER) / MAX_WIDTH;
          else if (w != ((x + i) % MAX_PERIMETER) / MAX_WIDTH)
            return true;
        }
    for (i = 0; i < new_t.size; i++)
      for (j = 0; j < new_t.size; j++)
	if (tris[new_t.squares].polyomino[new_t.polyomino_number].shape[j][i])
          if (w != ((x + i) % MAX_PERIMETER) / MAX_WIDTH)
            return true;
    return false;
}


//===========================================================================
/// global try_move(Actions move)
///    Trying to move/rotate/fall our figure according to the given move action
/// tags WellBase
void WellBase::try_move(Actions move)
{
    thing_t         old;
    int             i;
    bool            skip = false;

    old = curThing;

    switch (move) {
    case FALL:
	curThing.ypos ++;
	break;

    case DROP:
	do {
	    curThing.ypos ++;
	    score += level;
	} while (!overlapping());
	curThing.ypos --;
	break;

    case LEFT:
	curThing.xpos = (curThing.xpos + MAX_PERIMETER - 1) % MAX_PERIMETER;
        if (at_base_fully() || (at_base_partially() && wall_change(old, curThing)))
          skip = true;
	break;

    case RIGHT:
	curThing.xpos = (curThing.xpos + 1) % MAX_PERIMETER;
        if (at_base_fully() || (at_base_partially() && wall_change(old, curThing)))
          skip = true;
	break;

    case ROTATE:
        if (tris[squares - MIN_SQUARES].mode[diagonal].turn_style > NONE)
        {
          if (cw)
            for (i = 0; i < MAX_SIDES - 1; i++)
              curThing.polyomino_number = tris[curThing.squares].polyomino
                [curThing.polyomino_number].rotation;
          else /* ccw */
            curThing.polyomino_number = tris[curThing.squares].polyomino
              [curThing.polyomino_number].rotation;
	  curThing.xpos = old.xpos;
	  curThing.ypos = old.ypos;
          if (at_base_partially() && wall_change(old, curThing))
            skip = true;
	}
	break;

    case REFLECT: /* reflect on y axis */
        if (tris[squares - MIN_SQUARES].mode[diagonal].turn_style == ALL)
        {
          curThing.polyomino_number = tris[curThing.squares].polyomino
            [curThing.polyomino_number].reflection;
	  curThing.xpos = old.xpos;
	  curThing.ypos = old.ypos;
          if (at_base_partially() && wall_change(old, curThing))
            skip = true;
	}
	break;
    default:
      skip=true;
      break;
    }

    if (!skip && !overlapping())
	draw_thing_diff(&old);
    else
	curThing = old;
}

//===========================================================================
/// global draw_box(int color,int i,int j)
///    Draw our box (piece of figure) on our virtual square (walls + floor)
/// tags WellBase
void WellBase::draw_box(int color,int i,int j)
{
  if (j < MAX_DEPTH) { //Draw trapazoid on the wall
        dre->draw_trapazoid(color, i, j);
    }
  else { // Draw square on the floor
        int base_i, base_j;
        wall_to_base(&base_i, &base_j, i, j);
        dre->draw_square(color, base_i, base_j);
    }
}

//===========================================================================
/// global draw_thing()
///    for all squares in figure do draw_box -> draw whole figure on the field
/// tags WellBase
void WellBase::draw_thing()
{
    int             i, j, xi;
    
    for (i = 0; i < curThing.size; i++)
      for (j = 0; j < curThing.size; j++)
        if (tris[curThing.squares].polyomino
                [curThing.polyomino_number].shape[j][i]) {
          xi = (curThing.xpos + i) % MAX_PERIMETER;
	  draw_box(curThing.color_number,xi, curThing.ypos + j);
        }
}

//===========================================================================
/// global draw_thing_diff(thing_t* old)
///   Make diff between old and new position on figure and Draw its
///   actually Draw new one and clear empty old pieces :)
/// tags WellBase
void WellBase::draw_thing_diff(thing_t* old)
{
    int             i, j, ox, oy;

    for (i = 0; i < curThing.size; i++)
      for (j = 0; j < curThing.size; j++)
        if ((curThing.ypos + j >= 0) && tris[curThing.squares].polyomino
              [curThing.polyomino_number].shape[j][i]) 
	{
	  draw_box(curThing.color_number,
            (curThing.xpos + i) % MAX_PERIMETER, curThing.ypos + j);
        }

    for (i = 0; i < curThing.size; i++)
      for (j = 0; j < curThing.size; j++) 
      {
        ox = (old->xpos + i - curThing.xpos + MAX_PERIMETER) % MAX_PERIMETER;
        oy = old->ypos + j - curThing.ypos;
	if (tris[old->squares].polyomino[old->polyomino_number].shape[j][i] &&
	    ((ox >= curThing.size) || (oy < 0) || (oy >= curThing.size) ||
	     !tris[curThing.squares].polyomino
                  [curThing.polyomino_number].shape[oy][ox])) 
	{
              draw_box(BackColor,(old->xpos + i) % MAX_PERIMETER, old->ypos + j);
	}
      }
}

//===========================================================================
/// global fill_lines()
///   Fill all glass with random squares. Just for test only
/// tags WellBase
void WellBase::fill_lines()
{
    int             i, j;
    int turn_style = tris[squares - MIN_SQUARES].mode[diagonal].turn_style;
    
    for (j = 0; j < MAX_DEPTH + MAX_WIDTH; j++)
      for (i = 0; i < MAX_PERIMETER; i++) 
	{
                draw_box(NRAND(tris[squares - MIN_SQUARES].mode
                          [diagonal].number[mixed][turn_style]),
			   i, j);
	}
}
							
//===========================================================================
/// global freeze_wall(int w)
///   Freeze wall - make some blinks while freezing :)
/// tags WellBase
void WellBase::freeze_wall(int w)
{
  int i, j;

  for (j = 0; j < MAX_DEPTH; j++)
    for (i = 0; i < MAX_WIDTH; i++)
     {
      if (wall[j][i+w*MAX_WIDTH].pmid < 0)
	  dre->draw_trapazoid(FreezeColor, w * MAX_WIDTH + i, j);
     }
  dre->sync();
  udelay(FLUSH_DELAY);
  for (j = 0; j < MAX_DEPTH; j++)
    for (i = 0; i < MAX_WIDTH; i++)
     {
      if (wall[j][i+w*MAX_WIDTH].pmid < 0)
	  dre->draw_trapazoid (BackColor, w * MAX_WIDTH + i, j);
     }
  dre->sync();
  udelay(FLUSH_DELAY);
  for (j = 0; j < MAX_DEPTH; j++)
    for (i = 0; i < MAX_WIDTH; i++)
     {
      if (wall[j][i+w*MAX_WIDTH].pmid < 0)
	  dre->draw_trapazoid (FreezeColor, w * MAX_WIDTH + i, j);
     }
  if (beep)
    dre->bell(BVOLUME);
  dre->sync();
}

//===========================================================================
/// global defreeze_wall(int w)
///   Return wall to normal state
/// tags WellBase
void WellBase::defreeze_wall(int w)
{
  int i, j;

  for (j = 0; j < MAX_DEPTH; j++)
    for (i = 0; i < MAX_WIDTH; i++)
     {
      if (wall[j][i+w*MAX_WIDTH].pmid < 0)
	  dre->draw_trapazoid (BackColor, w * MAX_WIDTH + i, j);
     }
  if (beep)
    dre->bell(BVOLUME);
  dre->sync();
}

void WellBase::udelay(int msec)
{
  default_well_engine->udelay(msec);
}

//===========================================================================
/// global draw_field()
///   Draw field with squares and frozen walls
/// tags WellBase
void WellBase::draw_field()
{
    int             i, j;

    for (i = 0; i < MAX_PERIMETER; i++)
        for (j = 0; j < MAX_DEPTH; j++) 
	{
	    if (wall[j][i].pmid >= 0)
		dre->draw_trapazoid(wall[j][i].cid, i, j);
	    else
	    {
	      if (frozen_wall[i / MAX_WIDTH])
	        dre->draw_trapazoid(FreezeColor, i, j);
	      else
	        dre->draw_trapazoid(BackColor, i, j);
	    }
        }
    for (i = 0; i < MAX_WIDTH; i++)
      for (j = 0; j < MAX_WIDTH; j++)
	if (base[j][i].pmid >= 0)
	  dre->draw_square(base[j][i].cid, i, j);
	else
	  dre->draw_square(BackColor, i, j);	  
    dre->clear_dirty_list();
    dre->field_sync();
}

//===========================================================================
/// global draw_next()
///   Draw next figure (cheating?)
///   Currently not implemented
/// tags WellBase
void WellBase::draw_next()
{
    int             x, y;
    int             i, j;

    Colors  color= nextThing.bonus ? BonusColor : GridColor;
    Colors  color2= nextThing.bonus ? BonusColor2 : GridColor2;

    dbgprintf(("Next color: %d, size: %d\n",nextThing.color_number,
	       nextThing.size));
    x=NEXT_CENTER_X - nextThing.size*(NEXT_BOX_SIZE+2)/2;
    y=NEXT_CENTER_Y - nextThing.size*(NEXT_BOX_SIZE+2)/2;
    
    for (i = 0; i < nextThing.size; i++)
      for (j = 0; j < nextThing.size; j++)
	if (tris[nextThing.squares].polyomino
                [nextThing.polyomino_number].shape[j][i])
	{
	  dre->draw_rect(x+ (NEXT_BOX_SIZE+2)*i, y+ (NEXT_BOX_SIZE+2)*j,
			 NEXT_BOX_SIZE, NEXT_BOX_SIZE, color);
	  dre->draw_rect(x+ (NEXT_BOX_SIZE+2)*i +1, y+ (NEXT_BOX_SIZE+2)*j +1,
			 NEXT_BOX_SIZE-2, NEXT_BOX_SIZE-2, color2);
	}
    dre->sync();
}

//===========================================================================
/// global clear_next()
///   Clear next figure
///   Currently not implemented
/// tags WellBase
void WellBase::clear_next()
{
  dre->fill_rect(NEXT_CENTER_X-NEXT_X_SIZE/2+1,NEXT_CENTER_Y-NEXT_Y_SIZE/2+1,
		 NEXT_X_SIZE-1,NEXT_Y_SIZE-1,BackColor);
}

//===========================================================================
/// global draw_status()
///   draw status of the game, i.e. scores, lines...
///   Currently not implemented
/// tags WellBase
void WellBase::draw_status() 
{
  int   delta_score;
  Score *pscore2=0,*pscore1=0;
  char buf[L_MAXPATH];
  char buf2[PLAYER_NAME_LEN];

  if(!shown)
    return;

  dbgprintf(("score: %d, lines: %d, level %d, levelup at: +%d\n",
	     score, rows, level, rows_levelup - rows));
  sprintf(buf,"%d",score);
  txt_score->set_text(buf);
  txt_score->draw_text(RightJust);
  sprintf(buf,"%d",rows);
  txt_lines->set_text(buf);
  txt_lines->draw_text(RightJust);
  sprintf(buf,"+%d",rows_levelup - rows + 1);
  txt_levelup->set_text(buf);
  txt_levelup->draw_text(RightJust);
  sprintf(buf,"%d",level);
  txt_level->set_text(buf);
  txt_level->draw_text(RightJust);
  dbgprintf(("score done\n"));
  pscore2=default_top_nine->find_challenger(player_score);
  if(pscore2)
    pscore1=default_top_nine->find_challenger(pscore2);
  
  do {
    if(pscore1)
    {
      delta_score=pscore1->score - player_score->score;
      delta_score= delta_score>=100000 ? 99999 : delta_score;
      strncpy(buf2,pscore1->player,CHAL_NAME_LEN);
      buf2[CHAL_NAME_LEN]=0;
      sprintf(buf,"%d.%-9s+%05d",pscore1->place,buf2,
	      delta_score);
      dbgprintf(("draw_status: %s\n",buf));
      txt_first->set_text(buf);
      txt_first->draw_text();

      strncpy(buf2,pscore2->player,CHAL_NAME_LEN);
      buf2[CHAL_NAME_LEN]=0;
      delta_score=pscore2->score - player_score->score;
      delta_score= delta_score>=100000 ? 99999 : delta_score;
      sprintf(buf,"%d.%-9s+%05d",pscore2->place,buf2,
	      delta_score);
      txt_second->set_text(buf);
      txt_second->draw_text();

      sprintf(buf,"%d.You are here",pscore2->place+1);
      txt_third->set_text(buf);
      txt_third->draw_text();
      break;
    }
    if(pscore2)
    {
      strncpy(buf2,pscore2->player,CHAL_NAME_LEN);
      buf2[CHAL_NAME_LEN]=0;
      delta_score=pscore2->score - player_score->score;
      delta_score= delta_score>=100000 ? 99999 : delta_score;
      sprintf(buf,"%d.%-9s+%05d",pscore2->place,buf2,
	      delta_score);
      txt_first->set_text(buf);
      txt_first->draw_text();

      sprintf(buf,"%d.You are here",pscore2->place+1);
      txt_second->set_text(buf);
      txt_second->draw_text();

      txt_third->clear_region();
      break;
    }
    sprintf(buf,"%d.You are here",1);
    txt_first->set_text(buf);
    txt_first->draw_text();
    
    txt_second->clear_region();

    txt_third->clear_region();
  } while(0);

  if(rotation)
    default_well_engine->screen_copy(&geo[GEO_ROT_IDX]);
  if(showNext)
    default_well_engine->screen_copy(&geo[GEO_NEXT_IDX]);
  if(mixed)
    default_well_engine->screen_copy(&geo[GEO_MIXED_IDX]);
  dre->sync();
}

//===========================================================================
/// global draw_grid()
///   draw grid of the field
/// tags WellBase
void WellBase::draw_grid()
{
  dre->draw_grid();
}

//===========================================================================
/// global drop_wall(int wall_num)
///   Drop all squres on the wall to the floor after row is cleared
/// tags WellBase
void WellBase::drop_wall(int w)
{
  int i, j, k, l, fits, lines, base_x, base_y;
  lines = 0;
  for (j = MAX_DEPTH - 1; j >= 0 && lines == 0; j--)
    for (i = 0; i < MAX_WIDTH; i++)
      if (wall[j][w * MAX_WIDTH + i].pmid != EMPTY)
        lines = MAX_DEPTH - j;
  if (lines > 0) {
    fits = true;
    j = 0;
    while (j < (MAX_WIDTH / 2 + lines - 1) && fits) {
      j++;
      for (l = MAX_DEPTH - j; l < MAX_DEPTH; l++)
        for (i = 0; i < MAX_WIDTH; i++)
          if (wall[l][w * MAX_WIDTH + i].pmid != EMPTY) {
            wall_to_base(&base_x, &base_y,
              w * MAX_WIDTH + i, l + j);
            if (base[base_y][base_x].pmid != EMPTY)
              fits = false;
          }
    }
    if (!fits)
      j--;
    if (j > 0)
    {
      for (l = MAX_DEPTH - 1; l >= 0; l--)
        for (i = 0; i < MAX_WIDTH; i++)
          if (wall[l][w * MAX_WIDTH + i].pmid != EMPTY) {
            k = w * MAX_WIDTH + i;
            if (l + j >= MAX_DEPTH) {
              wall_to_base(&base_x, &base_y, k, l + j);
              base[base_y][base_x] = wall[l][k];
              wall[l][k].pmid = EMPTY;
              dre->draw_trapazoid(BackColor, k, l);
              dre->draw_square(wall[l][k].cid, base_x, base_y);
            } else {
              wall[l + j][k] = wall[l][k];
              wall[l][k].pmid = EMPTY;
              dre->draw_trapazoid(BackColor, k, l);
              dre->draw_trapazoid(wall[l][k].cid, k, l + j);
            }
            dre->sync();
          }
      if (beep)
        dre->bell(BVOLUME);
      dre->sync();
    }
  }
}

//===========================================================================
/// global all_frozen()
///   True if all walls are frozen -> end of the game
/// tags WellBase
bool WellBase::all_frozen()
{
  int w;

  for (w = 0; w < MAX_SIDES; w++)
    if (frozen_wall[w] == MAX_SIDES) {
      freeze_wall(w);
    }
  for (w = 0; w < MAX_SIDES; w++)
    if (frozen_wall[w] == 0)
      return false;
  return true;
}

//===========================================================================
/// global check_freeze()
///    Check frozen walls - decrease frozen counter and do defreeze 
//     and dropwall
/// tags WellBase
void WellBase::check_freeze()
{
  int w;
  bool all_frozen=true;

  for (w = 0; w < MAX_SIDES; w++) {
    if (frozen_wall[w] == 0)
    {
      all_frozen=false;
      drop_wall(w);
    }
    else 
    {
      frozen_wall[w]--;
      if (frozen_wall[w] == 0) 
      {
        defreeze_wall(w);
        drop_wall(w);
	all_frozen=false;
      }
    }
  }

  if(all_frozen)
  {
    dbgprintf(("check_freeze - all frozen - doing game_over\n"));
    throw aGameOver;
  }

  /* not all frozen else this is an infinite loop */
  /* curThing.xpos = (MAX_WIDTH - curThing.size) / 2; */
  curThing.xpos = NRAND(MAX_WIDTH - curThing.size + 1);
  do {
    w = NRAND(MAX_SIDES);
  } while (frozen_wall[w]); //Hmm, very strange method of choosing the wall???
  curThing.xpos += w * MAX_WIDTH;
}

//===========================================================================
/// global check_lines()
///    Checking for full lines on the floor and deleting it - make free space
/// tags WellBase
int WellBase::check_lines()
{
    int             lSet[2][MAX_WIDTH], nset[2];
    int             i, j, k;

    nset[0] = nset[1] = 0;

    //First check horizontal lines, increase nset[0] and lSet[0][line]
    for (j = 0; j < MAX_WIDTH; j++) 
      {
        lSet[0][j] = 0;
        for (i = 0; i < MAX_WIDTH; i++)
	  if (base[j][i].pmid >= 0)
	    lSet[0][j] ++;
        if (lSet[0][j] == MAX_WIDTH)
	  nset[0] ++;
      }
    //Then the same for vertical ones
    for (i = 0; i < MAX_WIDTH; i++) 
      {
        lSet[1][i] = 0;
        for (j = 0; j < MAX_WIDTH; j++)
	  if (base[j][i].pmid >= 0)
	    lSet[1][i] ++;
        if (lSet[1][i] == MAX_WIDTH)
	  nset[1] ++;
      }
    if (nset[0] || nset[1]) //If we have full lines
      {
	// Now flashing full lines awhile
	for (k = 0; k < NUM_FLASHES; k++) 
	  {
	    int color;
	    color=(k%2) ?  BackColor : FlashColor;
            for (j = 0; j < MAX_WIDTH; j++) 
	      {
                if (lSet[0][j] == MAX_WIDTH)
		  {
		    for (i = 0; i < MAX_WIDTH; i++)
		      dre->draw_square(color, i, j);
		  }
                if (lSet[1][j] == MAX_WIDTH)
		  {
		    for (i = 0; i < MAX_WIDTH; i++)
		      dre->draw_square(color, j, i);
		  }
	      }
	    dre->flush_dirty();
	    dre->sync();
	    udelay(FLUSH_DELAY/2);
	}

        if (nset[0] > 0)  //Now delete full horizontal lines
          for (j = 0; j < MAX_WIDTH; j++)
            if (lSet[0][j] == MAX_WIDTH) 
	      {
		if (j < MAX_WIDTH / 2) 
                //If upper part of field then move other lines down to center
		  for (i = 0; i < MAX_WIDTH; i++) 
		    {
		      for (k = j; k > 0; k--)
			base[k][i] = base[k - 1][i];
		      base[0][i].pmid = EMPTY;
		    }
		else //else move them up
		  {
		    for (i = 0; i < MAX_WIDTH; i++) 
		      {
			for (k = j; k < MAX_WIDTH - 1; k++)
			  base[k][i] = base[k + 1][i];
			base[MAX_WIDTH - 1][i].pmid = EMPTY;
		      }
		    for (k = j; k < MAX_WIDTH - 1; k++) 
                    //Fixing lSet position of moved lines
		      lSet[0][k]=lSet[0][k+1];
		    lSet[0][MAX_WIDTH-1]=0;
		    j--; //Fixing index
		  }
	      }
        if (nset[1] > 0)  //Do the same for vertical lines
          for (i = 0; i < MAX_WIDTH; i++)
            if (lSet[1][i] == MAX_WIDTH) 
	      {
		if (i < MAX_WIDTH / 2)
		  for (j = 0; j < MAX_WIDTH; j++) 
		    {
		      for (k = i; k > 0; k--)
			base[j][k] = base[j][k - 1];
		      base[j][0].pmid = EMPTY;
		    }
		else
		  {
		    for (j = 0; j < MAX_WIDTH; j++) 
		      {
			for (k = i; k < MAX_WIDTH - 1; k++)
			  base[j][k] = base[j][k + 1];
			base[j][MAX_WIDTH - 1].pmid = EMPTY;
		      }
		    for (k = i; k < MAX_WIDTH - 1; k++)
		      lSet[1][k]=lSet[1][k+1];
		    lSet[1][MAX_WIDTH-1]=0;
		    i--;
		  }
	      }

        //Now reDraw new field
	for (j = 0; j < MAX_WIDTH; j++)
	  for (i = 0; i < MAX_WIDTH; i++)
	    {
	      if (base[j][i].pmid >= 0)
		dre->draw_square(base[j][i].cid, i, j);
	      else
		dre->draw_square(BackColor, i, j);
	    }
	//All done, beeping...
	if (beep)
	  dre->bell(BVOLUME);
	dre->flush_dirty();
	dre->sync();
    }

    return (nset[0] + nset[1]);
}


//===========================================================================
/// global check_update_lines()
///    Calculating statistics ans SCORES! :) And redraw status
/// tags WellBase
void WellBase::check_update_lines()
{
  int lines,mult;
  int oldrows=rows;
  lines = check_lines();
  score += lines + !gradualAppear + !grid;
  mult= showNext ? 5 : 7;
  mult += (rotation ? 1 : 0);
  score += mult * level * level * lines * lines * lines *
    squares * squares * (bonus + diagonal + 1) / (16 * (1 + mixed));
  rows += lines;
  player_score->set_params(score,rows,level);
  if (rows > rows_levelup)
    {
      level++;
      new_level_calc();
      if (bonus)
	bonusNow = true;
      player_score->set_params(score,rows,level);
      draw_status();
    } else
      if(oldrows!=rows)
	draw_status();
}

//===========================================================================
/// global move_one_step(Actions move)
///    Move figure one step down/left/right
/// tags WellBase
bool WellBase::move_one_step(Actions move)
{
    if ((move == DROP) || ((move == FALL) && at_bottom())) {
	try_move(move);
	put_box();
	check_update_lines();
	new_thing();
	if (showNext) {
	    clear_next();
	    draw_next();
	}
	dre->sync();	/* discard all events */
	if (all_frozen())
	    throw aGameOver;
        check_freeze();
	if (overlapping())
	    throw aGameOver;
	check_update_lines();
	draw_thing();
	return true;
    } else {
	try_move(move);
	if (rows > THRESHOLD(level)) {
	    level++;
	    draw_status();
            if (bonus)
              bonusNow = true;
	}
    }
    return false;
}

//===========================================================================
/// global game_over()
///     game is over :( you are looser
/// tags WellBase
void WellBase::game_over()
{
  dbgprintf(("WellBase::game_over() called\n"));
  hide();
  player_score->calc_play_time();
  player_score=default_top_nine->try_add_to_table(player_score);
  delete player_score;
  game_over_object.call(wEvent(aGameOver,this));
}

//===========================================================================
/// global redraw_all()
///     redraw all game
/// tags WellBase
void WellBase::redraw_all()
{
  if(!shown)
    return;

  draw_status();
  dre->flush_all();
  clear_next();
  if(showNext)
    draw_next();
  dre->flush_all();

  while(steps_to_rotate>0)
  {
    make_rotation();
    steps_to_rotate--;
    udelay(FLUSH_DELAY/10);
  }

  dre->sync();
}

//===========================================================================
/// global make_pause()
///     set/uset game pause
/// tags WellBase
void WellBase::make_pause()
{
  pause^=1;
  if(pause)
  {
    default_well_engine->del_timer(this);
    dre->clear_field(0);
    dre->pixmap_copy(&geo[GEO_PAUSE_IDX]);
    dre->flush_all();
    dre->sync();
  } else
  {
    default_well_engine->add_timer(this,speeds[level]);
    dre->clear_field(0);
    draw_field();
    draw_thing();
    redraw_all();
  }
}

//===========================================================================
/// global make_rotation()
///     rotate field step by step
/// tags WellBase
void WellBase::make_rotation()
{
    dre->outer_rotation();
    dre->clear_field(0);
    draw_field();
    draw_thing();
    dre->flush_all();
    dre->sync();
}

//===========================================================================
/// global process_event()
///     process all events for this game object
///     return false if we don't want to process this event for other objects
/// tags WellBase
bool WellBase::process_event(wEvent ev)
{
  Actions action;

  switch(ev.type)
  {
  case eTimeOut:
    dbgprintf(("WellBase got time out event\n"));
    try
    {
      move_one_step(FALL);
      dre->flush_dirty();
    }
    catch(EvType)
    {
      game_over();
      return false;
    }
    break;

  case eExpose:
    dbgprintf(("WellBase got expose event\n"));
    redraw_all();
    break;

  case eFocusOut:
    if(!pause)
      make_pause();
    return false;

  case eKeyPress:
    action=dre->key_to_action(ev.data);
    try 
    {
      return process_action(action);
    }
    catch(EvType)
    {
      game_over();
      return false;
    }

  case eDelete:
    hide();
    break;

  case aIntroExit:
    new_game();
    show();
    break;
  }
  return true;
}

//===========================================================================
/// global process_action(Actions)
///     process actions - moves/drops/other
/// tags WellBase
bool WellBase::process_action(Actions action)
{
  switch(action)
  {
  case LEFT:
  case RIGHT:
  case ROTATE:
  case FALL:
  case DROP:
    if(pause)
      return false;
    move_one_step(action);
    dre->flush_dirty();
    return false;

  case PAUSE_GAME:
    make_pause();
    return false;
    
  case OUTER_ROTATION:
    if(pause)
      return false;
    make_rotation();
    return false;
    
  case END_GAME:
    game_over();
    return false;

  case LEVEL_UP:
    if(pause)
      return false;
    if(level+1<NUM_LEVELS)
    {
      level++;
      new_level_calc();
      draw_status();
    }
    return false;
  case LEVEL_DOWN:
    if(pause)
      return false;
    if(level-1>0)
    {
      level--;
      new_level_calc();
      draw_status();
    }
    return false;
  }
  return true;
}
