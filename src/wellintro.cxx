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
// $Id: wellintro.cxx,v 1.3 2003/02/27 08:27:51 leo Exp $

/// module description
/// WellIntro class - implements Introduction screen with options switches
/// new game, top nine and exit keys 

#include "wellclass.h"
#include "wellintro.h"
#include "wellengine.h"
#include "welltopnine.h"
#include "version.h"

//===========================================================================
/// global WellIntro()
///     constructor - fill table
/// tags WellIntro
WellIntro::WellIntro() : WellObject()
{
  int i;
  char buf[L_MAXPATH];
  start_level=0;
  txt_level=default_well_engine->new_well_image_font(imFont1,FONT3_L,FONT3_H,
						     FONT3_DX,FONT3_DY);
  txt_level->set_screen_region(674,403,34,20);

  key_exit=default_well_engine->new_well_key("intro_key_exit");
  key_new_game=default_well_engine->new_well_key("intro_key_new_game");
  key_top_nine=default_well_engine->new_well_key("intro_key_top_nine");
  key_plus=default_well_engine->new_well_key("intro_key_plus");
  key_minus=default_well_engine->new_well_key("intro_key_minus");

  key_exit->set_object_on_press(ObjectCaller(this,&WellObject::hide_by_call));
  key_new_game->set_object_on_press(ObjectCaller(this,&WellObject::hide_by_call));
  key_top_nine->set_object_on_press(ObjectCaller(this,&WellObject::hide_by_call));
  key_plus->set_object_on_press(ObjectCaller(this,&WellObject::process_event));
  key_minus->set_object_on_press(ObjectCaller(this,&WellObject::process_event));

  sw_rotation=default_well_engine->new_well_switch("intro_sw_rotation");
  sw_rotation->set_object_on_switch(ObjectCaller(this,
						 &WellObject::process_event));
  sw_next_piece=default_well_engine->new_well_switch("intro_sw_next");
  sw_next_piece->set_object_on_switch(ObjectCaller(this,
						 &WellObject::process_event));
  sw_mixed=default_well_engine->new_well_switch("intro_sw_mixed");
  sw_mixed->set_object_on_switch(ObjectCaller(this,
						 &WellObject::process_event));
  for(i=0;i<TOTAL_SQUARES;i++)
  {
    sprintf(buf,"intro_sw_square%d",i);
    sw_squares[i]=default_well_engine->new_well_switch(buf);
    sw_squares[i]->set_object_on_switch(ObjectCaller(this,
						     &WellObject::process_event));
    sw_squares[i]->set_mode(OnlySet);
  }
  
  inp_player=default_well_engine->new_well_input("intro_inp_player");
  inp_player->set_object_on_enter(ObjectCaller(this,
					       &WellObject::hide_by_call));
  inp_player->set_max_len(PLAYER_NAME_LEN-1);
}


//===========================================================================
/// global process_event(wEvent)
///     stub that process events
/// tags WellIntro
bool WellIntro::process_event(wEvent ev)
{
  int i;
  WellObject* wo=(WellObject*)ev.data;

  switch(ev.type)
  {
  case eExpose:
    redraw();
    break;
  case eKeyPress:
    hide_by_call(wEvent(aKeyPressed,key_new_game));
    return false;

  case aKeyPressed:
    if(wo==key_plus && start_level+1<NUM_LEVELS)
    {
      start_level++;
      draw_start_level();
      return true;
    }
    if(wo==key_minus && start_level>0)
    {
      start_level--;
      draw_start_level();
      return true;
    }
    break;

  case aSwitchChanged:
    if(wo==sw_rotation)
    {
      well->set_rotation(sw_rotation->get_value());
      break;
    }
    if(wo==sw_next_piece)
    {
      well->set_next_piece(sw_next_piece->get_value());
      break;
    }
    if(wo==sw_mixed)
    {
      well->set_mixed(sw_mixed->get_value());
      break;
    }
    for(i=0;i<TOTAL_SQUARES;i++)
      if(wo==sw_squares[i])
      {
	well->set_squares(MIN_SQUARES+i);
      } else
	sw_squares[i]->set_value(false);
    break;
  }
  return true;
}

//===========================================================================
/// global show()
///     show object
/// tags WellIntro
void WellIntro::show()
{

  dbgprintf(("WellIntro::show() called\n"));
  default_well_engine->set_main_background_image(imIntroBG);
  default_well_engine->add_object(this);
  key_exit->show();
  key_new_game->show();
  key_top_nine->show();
  key_plus->show();
  key_minus->show();
  sw_rotation->show();
  sw_next_piece->show();
  sw_mixed->show();
  for(int i=0;i<TOTAL_SQUARES;i++)
    sw_squares[i]->show();
  draw_start_level();
  shown=true;
}

//===========================================================================
/// global hide()
///     hide object (empty)
/// tags WellIntro
void WellIntro::hide()
{
  if(shown)
  {
    shown=false;
    default_well_engine->del_object(this);
    key_exit->hide();
    key_new_game->hide();
    key_top_nine->hide();
    key_plus->hide();
    key_minus->hide();
    sw_rotation->hide();
    sw_next_piece->hide();
    sw_mixed->hide();
    for(int i=0;i<TOTAL_SQUARES;i++)
      sw_squares[i]->hide();
  }
}


//===========================================================================
/// global hide_by_call()
///     hide object
/// tags WellIntro
bool WellIntro::hide_by_call(wEvent ev)
{
  WellObject* wo=(WellObject*)ev.data;

  hide();

  put_all_to_game();
  save_options();

  if(wo==inp_player)
  {
    inp_player->hide();
    return object_on_new_game.call(wEvent(aIntroExit,this));
  }

  if(wo==key_exit)
  {
    default_well_engine->done_loop();
    return true;
  }

  if(wo==key_new_game)
  {
    inp_player->show();
  }

  if(wo==key_top_nine)
  {
    return object_on_top_nine.call(wEvent(aIntroExit,this));
  }

  return true;
}


//===========================================================================
/// global draw_start_level()
///     draw start level on the screen
/// tags WellIntro
void WellIntro::draw_start_level()
{
  char buf[L_MAXPATH];
  sprintf(buf,"%02d",start_level);
  txt_level->set_text(buf);
  txt_level->draw_text();
  well->set_level(start_level);
}


//===========================================================================
/// global redraw()
///     redraw all scene object
/// tags WellIntro
void WellIntro::redraw()
{
  draw_start_level();
  key_exit->redraw();
  key_new_game->redraw();
  key_top_nine->redraw();
  key_plus->redraw();
  key_minus->redraw();
  sw_rotation->redraw();
  sw_next_piece->redraw();
  sw_mixed->redraw();
  for(int i=0;i<TOTAL_SQUARES;i++)
    sw_squares[i]->redraw();
}

//===========================================================================
/// global put_all_to_game()
///     put all values to the game
/// tags WellIntro
void WellIntro::put_all_to_game()
{
  int i;
  well->set_rotation(sw_rotation->get_value());
  well->set_mixed(sw_mixed->get_value());
  well->set_next_piece(sw_next_piece->get_value());
  well->set_level(start_level);
  for(i=0;i<TOTAL_SQUARES;i++)
    if(sw_squares[i]->get_value())
    {
      well->set_squares(i+MIN_SQUARES);
      default_top_nine->set_page(i+1);
      dbgprintf(("Set top_nine page to %d\n",i+1));
    }
  well->set_player_name(inp_player->get_text());
}


//===========================================================================
/// global load_defaults()
///     set default values and try to load it from file
/// tags WellIntro
void WellIntro::load_defaults()
{
  sw_rotation->set_value(true);
  sw_mixed->set_value(true);
  sw_next_piece->set_value(true);
  start_level=0;
  sw_squares[4-MIN_SQUARES]->set_value(true);
  inp_player->set_text(getenv("USER"));

  load_options();

  put_all_to_game();
}

//===========================================================================
/// global set_well_base(WellBase*)
///     set pointer to the game object, loads and set default values
/// tags WellIntro
void WellIntro::set_well_base(WellBase* o) 
{ 
  well=o;
  load_defaults();
}

//===========================================================================
/// global save_options()
///     save options to file
/// tags WellIntro
void WellIntro::save_options()
{
    int i;
    char fname[L_MAXPATH];
    FILE *fp;
#ifndef WIN32    
    sprintf(fname,"%s/%s",getenv("HOME"),XWELL_DIR);
    mkdir(fname,0755);
    sprintf(fname,"%s/%s/%s",getenv("HOME"),XWELL_DIR,RC_FILE);
#else
	sprintf(fname,"data\\%s", RC_FILE);
#endif
    fp=fopen(fname, "w");
    if(fp==0)
    {
	fprintf(stderr,"Error in opening resource file for writing:\n\t%s\n",
		strerror(errno));
	return;
    }
    
    fprintf(fp,"#--------------- Options file for xwelltris %s ----------------#\n",VERSION);
    fprintf(fp,"#This file generated automatically, so don't edit it by hands\n\n");
    fprintf(fp,  "last_player:      %s\n",inp_player->get_text());
    if(sw_rotation->get_value())
	fprintf(fp,"use_rotation:     yes\n");
    else
	fprintf(fp,"use_rotation:     no\n");
    
    if(sw_mixed->get_value())
	fprintf(fp,"use_mixed:        yes\n");
    else
	fprintf(fp,"use_mixed:        no\n");
    
    if(sw_next_piece->get_value())
	fprintf(fp,"use_next_piece:   yes\n");
    else
	fprintf(fp,"use_next_piece:   no\n");
    
    fprintf(fp,  "start_level:      %d\n",start_level);
    
    for(i=0;i<TOTAL_SQUARES;i++)
	if(sw_squares[i]->get_value())
	    break;
    fprintf(fp,  "tris_squares:     %d\n", i+MIN_SQUARES);
    fprintf(fp,"#------------------ End of file ---------------------#\n");
    
    fclose(fp);
}

//===========================================================================
/// global load_options()
///     load options from file
/// tags WellIntro
void WellIntro::load_options()
{
  int  i,n;
  char fname[L_MAXPATH];
  char buf[L_MAXPATH];
  char tok[L_MAXPATH], *pbuf;
  FILE* fp;
#ifndef WIN32
  sprintf(fname,"%s/%s/%s",getenv("HOME"),XWELL_DIR,RC_FILE);
#else
  sprintf(fname,"data\\%s",RC_FILE);
#endif
  fp=fopen(fname, "r");
  if(fp==0)
    return;
  
  while(fgets(buf,L_MAXPATH-1,fp))
  {
    if(buf[0]=='#')
      continue;

    pbuf=get_next_token(buf,tok);
    if(!strcmp(tok,"use_rotation"))
    {
      get_next_token(pbuf,tok);
      if(!strcmp(tok,"yes"))
	sw_rotation->set_value(true);
      else
	sw_rotation->set_value(false);
      continue;
    }

    if(!strcmp(tok,"use_mixed"))
    {
      get_next_token(pbuf,tok);
      if(!strcmp(tok,"yes"))
	sw_mixed->set_value(true);
      else
	sw_mixed->set_value(false);
      continue;
    }

    if(!strcmp(tok,"use_next_piece"))
    {
      get_next_token(pbuf,tok);
      if(!strcmp(tok,"yes"))
	sw_next_piece->set_value(true);
      else
	sw_next_piece->set_value(false);
      continue;
    }

    if(!strcmp(tok,"last_player"))
    {
      get_next_token(pbuf,tok);
      inp_player->set_text(tok);
      continue;
    }
      
    if(!strcmp(tok,"start_level"))
    {
      get_next_token(pbuf,tok);
      start_level=atoi(tok);
      if(start_level<0 || start_level>=NUM_LEVELS)
	start_level=0;
      continue;
    }

    if(!strcmp(tok,"tris_squares"))
    {
      get_next_token(pbuf,tok);
      n=atoi(tok);
      n-=MIN_SQUARES;
      if(n<0 || n>=TOTAL_SQUARES)
	n=0;
      for(i=0;i<TOTAL_SQUARES;i++)
	sw_squares[i]->set_value(n==i ? true : false);
      continue;
    }
  }
  fclose(fp);
}

//===========================================================================
/// global get_next_token(char* from, char* to)
///     cut buffer by tokens
/// tags WellIntro
char* WellIntro::get_next_token(char* from, char* to)
{
  int  delta=0;
  char *orig_to=to;
  *to=0;
  if(*from==':')
    from++;
  while(*from==' ')
    from++;
  while(*from!=0 && *from!=':' && *from!='\n' && *from!='\r' && *from!=0)
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

