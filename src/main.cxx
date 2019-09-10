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
// $Id: main.cxx,v 1.3 2003/02/19 09:51:39 leo Exp $

/// module description
/// Main module - init the game, create main objects and entering in event loop

#include "wellengine.h"
#include "wellclass.h"
#include "wellintro.h"

WellBase    *game_object;
WellIntro   *game_intro;

//===========================================================================
/// global main(int, char**)
///     main function - start of the game
/// tags main
int main(int argc, char** argv)
{
  default_well_engine=new_well_engine(argc,argv);

  game_intro=      default_well_engine->new_well_intro();
  default_top_nine=default_well_engine->new_well_top_nine();
  game_object=     default_well_engine->new_well_base();

  default_top_nine->load_scores();
  game_intro->set_well_base(game_object);
  game_object->init(default_well_engine);

  game_object->set_object_on_game_over(ObjectCaller(default_top_nine,
						    &WellObject::show_by_call));

  game_intro->set_object_on_new_game(ObjectCaller(game_object, 
						  &WellObject::process_event));

  game_intro->set_object_on_top_nine(ObjectCaller(default_top_nine, 
						  &WellObject::show_by_call));

  default_top_nine->set_object_on_exit(ObjectCaller(game_intro,
						    &WellObject::show_by_call));
  default_well_engine->show_main();
  game_intro->show();

  default_well_engine->event_loop();

  default_top_nine->save_scores();
  game_intro->save_options();

  delete game_object;
  delete default_top_nine;
  delete default_well_engine;
  return 0;
}
