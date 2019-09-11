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
// $Id: welltopnine.cxx,v 1.3 2003/02/23 06:33:40 leo Exp $
#include "welltopnine.h"
#include "wellengine.h"
#include "commonfuncs.h"

WellTopNine *default_top_nine;

//===========================================================================
/// global Score()
///     constructor, set all to zeros and current date
/// tags Score
Score::Score() 
{ 
  player[0]=date_time[0]=0; 
  level=score=lines=squares=play_time=0;
  mixed=rotation=next_piece=false;
  set_current_time();
}

//===========================================================================
/// global get_next_token(char* from, char* to)
///     cut buffer by tokens
/// tags Score
char* Score::get_next_token(char* from, char* to)
{
  int  delta=0;
  char *orig_to=to;
  *to=0;
  if(*from==';')
    from++;
  while(*from==' ')
    from++;
  while(*from!=0 && *from!=';' && *from!='\n' && *from!='\r' && *from!=0)
    *to++=*from++;
  while(to!=orig_to && *to==' ')
  {
    to--;
    delta=1;
  }
  to+=delta;
  *to=0;
  return from;
}

//===========================================================================
/// global save(FILE*)
///     save score_line to file
/// tags Score
void Score::save(FILE* fp)
{
  fprintf(fp,"%d;%-12s;%s;%8d;%5d;%2d;%d;%d;%d;%d\n",
	  squares,player,date_time,score,lines,level,
	  mixed ? 1 : 0, rotation ? 1 : 0,play_time,
	  next_piece ? 1 : 0); 
}

//===========================================================================
/// global load(FILE*)
///     load score_line from file
/// tags Score
bool Score::load(FILE* fp)
{
  char buf[L_MAXPATH];
  char *ptr=buf;
  char token[L_MAXPATH];
  while(1)
  {
    if(fgets(buf,L_MAXPATH-1,fp)==0)
      return false;
    if(buf[0]=='#') //Skipping comments
      continue;

    //squares
    ptr=get_next_token(ptr,token);
    squares=atoi(token);
    //player
    ptr=get_next_token(ptr,token);
    strncpy(player,token,PLAYER_NAME_LEN-1);
    player[PLAYER_NAME_LEN-1]=0;
    //date_time
    ptr=get_next_token(ptr,token);
    strncpy(date_time,token,L_DATE-1);
    date_time[L_DATE-1]=0;
    //score
    ptr=get_next_token(ptr,token);
    score=atoi(token);
    //lines
    ptr=get_next_token(ptr,token);
    lines=atoi(token);
    //level
    ptr=get_next_token(ptr,token);
    level=atoi(token);
    //mixed
    ptr=get_next_token(ptr,token);
    mixed= atoi(token) ? true : false;
    //rotation
    ptr=get_next_token(ptr,token);
    rotation= atoi(token) ? true : false;
    //play_time
    ptr=get_next_token(ptr,token);
    play_time=atoi(token);
    //rotation
    ptr=get_next_token(ptr,token);
    next_piece= atoi(token) ? true : false;
    break;
  }
  dbgprintf(("scores: %d;%s;%s;%8d;%5d;%2d;%d;%d\n",
	  squares,player,date_time,score,lines,level,
	  mixed ? 1 : 0, rotation ? 1 : 0)); 
  return true;
}

//===========================================================================
/// global set_current_time()
///     set current date and time to date (format YYYY-MM-DD HH:MI)
/// tags Score
void Score::set_current_time()
{
  struct tm *time_str;
  time(&start_t);
  time_str=localtime(&start_t);
  strftime(date_time,L_DATE,"%Y-%m-%d",time_str);
}

//===========================================================================
/// global calc_play_time()
///     calculate playing time from start_t till now
/// tags Score
void Score::calc_play_time()
{
  time_t now_t;
  time(&now_t);
  play_time=now_t - start_t;
}

//===========================================================================
/// global set_player(const char* pl)
///     set player name
/// tags Score
void Score::set_player(const char* name)
{
  if(name)
    strncpy(player,name,PLAYER_NAME_LEN);
  else
    strcpy(player,"unknown");
  player[PLAYER_NAME_LEN-1]=0;
}

//////////////////////////ScoreTable struct methods/////////////////////////



//===========================================================================
/// global ScoreTable()
///     constructor of scoretable
/// tags ScoreTable
ScoreTable::ScoreTable()
{
  int i;
  for(i=0;i<TOP_NINE;i++)
  {
    table[i]=new Score;
    table[i]->place=i+1;
  }
}

//===========================================================================
/// global ~ScoreTable()
///     destructor of scoretable
/// tags ScoreTable
ScoreTable::~ScoreTable()
{
  int i;
  for(i=0;i<TOP_NINE;i++)
    delete table[i];
}

//===========================================================================
/// global sort_by_score()
///     sort table by score
/// tags ScoreTable
void ScoreTable::sort_by_score()
{
  int i,j;
  Score *ptr;

  for(i=0;i<TOP_NINE;i++)
    table[i]->place=i+1;

  for(j=0;j<TOP_NINE-1;j++)
    for(i=j+1;i<TOP_NINE;i++)
      if(table[j]->score<table[i]->score)
      {
	ptr=table[i];
	table[i]=table[j];
	table[j]=ptr;

	table[j]->place=j+1;
	table[i]->place=i+1;
      }
}

//===========================================================================
/// global save(FILE* fp)
///     save table to give file descr
/// tags ScoreTable
void ScoreTable::save(FILE* fp)
{
  int i;
  for(i=0;i<TOP_NINE;i++)
    table[i]->save(fp);
}

//===========================================================================
/// global set_squares(int isquare)
///     set squares to given number
/// tags ScoreTable
void ScoreTable::set_squares(int isquares)
{
  int i;
  for(i=0;i<TOP_NINE;i++)
    table[i]->squares=isquares;
}

//===========================================================================
/// global set_and_get(int idx,Score*)
///     set new Score and get old one. If new == 0 just return old
/// tags ScoreTable
Score* ScoreTable::set_and_get(int idx, Score* new_one)
{
  Score  *old_one;
  if(idx<0 || idx>=TOP_NINE)
    return new_one;

  old_one=table[idx];
  if(new_one)
  {
    table[idx]=new_one;
    table[idx]->place=idx+1;
  }
  return old_one;
}




//////////////////////////////WellTopNine class///////////////////////////////



//===========================================================================
/// global WellTopNine()
///     constructor - fill table
/// tags WellTopNine
WellTopNine::WellTopNine() : WellObject()
{
  int i;
  current_score=0;
  page=3;

  geo=get_geo_by_name("top_nine_elements");

  for(i=0;i<MAX_SQUARES;i++)
    tables[i].set_squares(i+1);
  for(i=0;i<TOP_NINE;i++)
  {
    text_lines[i]=default_well_engine->new_well_image_font(imFont1,
							   FONT3_L,FONT3_H,
							   FONT3_DX,FONT3_DY);
    text_lines[i]->set_screen_region(TOPNINE_TXT_X,
				     TOPNINE_TXT_Y + i*TOPNINE_TXT_STEPY,
				     TOPNINE_TXT_L,TOPNINE_TXT_H);
  }
  key_exit=default_well_engine->new_well_key("top_key_exit");
  key_exit->set_object_on_press(ObjectCaller(this,&WellObject::hide_by_call));
}

//===========================================================================
/// global save_scores()
///     save score table to file 
/// tags WellTopNine
bool WellTopNine::save_scores()
{
  int   i;
  char  fname[L_MAXPATH];
  
  if(!find_full_path_for_file(SCOREFILE,fname,ReadWrite))
  {
#ifndef WIN32
    sprintf(fname, "%s/.xwelltris",getenv("HOME"));
    mkdir(fname,0755);
    strcat(fname,"/");
    strcat(fname,SCOREFILE);
#endif
  }

  FILE* fp=fopen(fname,"w");
  if(!fp)
  {
    fprintf(stderr,"PANIC: can't save scores file: %s - %s\n",
	    fname,strerror(errno));
    return false;
  }
  for(i=0;i<MAX_SQUARES;i++)
    tables[i].save(fp);
  fclose(fp);
  return true;
}

//===========================================================================
/// global load_scores()
///     load score table from file
/// tags WellTopNine
bool WellTopNine::load_scores()
{
  int   i;
  int   idx[MAX_SQUARES];
  char  fname[L_MAXPATH];
  Score *pscore=new Score;

  find_full_path_for_file(SCOREFILE,fname,ReadWrite);

  FILE* fp=fopen(fname,"r");

  if(!fp)
  {
    fprintf(stderr,"ERROR: can't load scores file: %s - %s\n",
	    fname,strerror(errno));
    return false;
  }

  for(i=0;i<MAX_SQUARES;i++)
    idx[i]=0;

  while(pscore->load(fp))
  {
    i=pscore->squares-1;
    pscore=tables[i].set_and_get(idx[i],pscore);
    idx[i]++;
  }
  fclose(fp);

  for(i=0;i<MAX_SQUARES;i++)
    tables[i].sort_by_score();

  delete pscore;
  return true;
}

//===========================================================================
/// global try_add_to_table()
///   trying to add given player score to table of winners
///   return looser pointer (the same one if you are a looser :)  
/// tags WellTopNine
Score* WellTopNine::try_add_to_table(Score* new_one)
{
  Score  *pscore;
  if(new_one==0)
    return 0;
  page=new_one->squares-1;
  current_score=new_one;
  if(tables[page].table[TOP_NINE-1]->score < new_one->score)
  {
    pscore=tables[page].table[TOP_NINE-1];
    tables[page].table[TOP_NINE-1]=new_one;
    tables[page].sort_by_score();
    return pscore;
  }
  return new_one;
}

//===========================================================================
/// global find_challenger(Score* me)
///     Find challenger, whose score > than mine
//      return 0 if you are the first
/// tags WellTopNine
Score* WellTopNine::find_challenger(Score* me)
{
  int i;
  ScoreTable *pcur;

  if(me==0)
    return 0;
  pcur= &tables[me->squares-1];
  for(i=TOP_NINE-1;i>=0;i--)
    if(pcur->table[i]->score > me->score)
      return pcur->table[i];
  return 0;
}


//===========================================================================
/// global process_event(wEvent)
///     stub that process events (empty here)
/// tags WellTopNine
bool WellTopNine::process_event(wEvent)
{
  return true;
}

//===========================================================================
/// global show()
///     stub that show object (empty)
/// tags WellTopNine
void WellTopNine::show()
{
  key_exit->show();
  default_well_engine->set_main_background_image(imScoreBG);
  default_well_engine->add_object(this);
  redraw_table();
  shown=true;  
  save_scores();
}

//===========================================================================
/// global hide()
///     stub that hide object (empty)
/// tags WellTopNine
void WellTopNine::hide()
{
  shown=false;
  default_well_engine->del_object(this);
  key_exit->hide();
  object_on_exit.call(wEvent(aEmpty,this));
}

//===========================================================================
/// global redraw_table()
///     redraw all data for score table according to current_score and page
/// tags WellTopNine
void WellTopNine::redraw_table()
{
  int i,j;
  char buf[L_MAXPATH];
  Score  *pscore;

  for(i=0;i<TOP_NINE;i++)
  {
    pscore=tables[page].table[i];
    sprintf(buf,"%d.%-15s %s %7d %4d %2d",
	    pscore->place,pscore->player,pscore->date_time,
	    pscore->score,pscore->lines,pscore->level);
    if(pscore!=current_score)
    {
      text_lines[i]->set_font(FONT3_L,FONT3_H,
			      FONT3_DX,FONT3_DY);
      text_lines[i]->set_screen_region(TOPNINE_TXT_X,
				       TOPNINE_TXT_Y + i*TOPNINE_TXT_STEPY,
				       TOPNINE_TXT_L,TOPNINE_TXT_H);
    } else
    {
      dbgprintf(("Highlighting item %d\n",i));
      text_lines[i]->set_font(FONT5_L,FONT5_H,
			      FONT5_DX,FONT5_DY);
      text_lines[i]->set_screen_region(TOPNINE_TXT_X,
				       TOPNINE_TXT_Y + i*TOPNINE_TXT_STEPY,
				       TOPNINE_TXT_L,TOPNINE_TXT_H);
    }
    text_lines[i]->set_text(buf);
    text_lines[i]->draw_text();
    for(j=0;j<3;j++)
    {
      geo[j].toy=TOPNINE_TXT_Y + i*TOPNINE_TXT_STEPY - 2;
    }
    if(pscore->rotation)
      default_well_engine->screen_copy(&geo[0]);
    if(pscore->next_piece)
      default_well_engine->screen_copy(&geo[1]);
    if(pscore->mixed)
      default_well_engine->screen_copy(&geo[2]);
  }
}
