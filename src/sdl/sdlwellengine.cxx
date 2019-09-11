// docm_prefix(///)
/****************************************************************************
*  Copyright (C) 2002-2003 by Leo Khramov
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
// $Id: sdlwellengine.cxx,v 1.5 2003/02/21 11:40:33 leo Exp $

/// module description
/// This module contains methods of class SDLWellEngine which implements SDL calls
/// for the game. So this is SDL dependent part. It receives Events, 
/// make convertions, many initions and calls.

#include "version.h"
#include "globals.h"
#include "sdlwellengine.h"
#include "sdlwellimagefont.h"
#include "sdlwellinput.h"
#include "sdlwelldrawing.h"
#include "SDL_image.h"
#include "sdl_gfxprimitives.h"
#include "commonfuncs.h"

//Colors for welltris pieces and grid (x color names)
static Uint8    game_color_names[MAX_GAME_COLORS][3] = 
{
    /* 0  "Red"             */ { 255,  0,  0},  
    /* 1  "Green"           */ {   0,255,  0}, 
    /* 2  "Blue"            */ {   0,  0,255}, 
    /* 3  "Yellow"          */ { 255,255,  0},
    /* 4  "Magenta"         */ { 255,  0,255}, 
    /* 5  "Cyan"            */ {   0,255,255}, 
    /* 6  "FireBrick"       */ { 178, 34, 34}, 
    /* 7  "SpringGreen",    */ {   0,255,127}, 
    /* 8  "CornflowerBlue"  */ { 100,149,237},  
    /* 9  "Khaki"           */ { 240,230,140},
    /*10  "Plum"            */ { 221,160,221}, 
    /*11  "Violet"          */ { 238,130,238},
    /*12  "DarkTurquoise"   */ {   0,206,209}, 
    /*13  "Gold"            */ { 255,215,  0},
    /*14  "Orchid"          */ { 218,112,214}, 
    /*15  "Turquoise"       */ {  64,224,208},
    /*16  "Orange"          */ { 255,165,  0},
    /*17  "OrangeRed"       */ { 255, 69,  0}, 
    /*18  "VioletRed"       */ { 208, 32,144}, 
    /*19  "BlueViolet"      */ { 138, 43,226},
    /*20  "SeaGreen"        */ {  46,139, 87}, 
    /*21  "Pink"            */ { 255,192,203}, 
    /*22  "ForestGreen"     */ {  34,139, 34},  
    /*23  "SkyBlue"         */ { 135,206,235},
    /*24  "Coral"           */ { 255,127, 80}, 
    /*25  "Wheat"           */ { 245,222,179},
    /*26  "GoldenRod"       */ { 218,165, 32},
    /*27  "IndianRed"       */ { 205, 92, 92},
    /*28  "SpringGreen"     */ {   0,255,127}, 
    /*29  "CornflowerBlue"  */ { 100,149,237}, 
    /*30  "Thistle"         */ { 216,191,216}, 
    /*31  "Aquamarine"      */ { 127,255,212},
    /*32  "CadetBlue"       */ {  95,158,160},
    /*33  "LightSteelBlue"  */ { 176,196,222}, 
    /*34  "NavyBlue"        */ {   0,  0,128},
    /*35  "SteelBlue"       */ {  70,130,180}, 
    /*36  "YellowGreen"     */ { 154,205, 50}, 
    /*37  "DarkViolet"      */ { 148,  0,211}, 
    /*38  "MediumSeaGreen"  */ {  60,179,113}, 
    /*39  "DarkSlateGray"   */ {  47, 79, 79},
    /*40  "LightGray"       */ { 211,211,211}, 
    /*41  "MediumVioletRed" */ { 199, 21,133},
    /*42  "Sienna"          */ { 160, 82, 45}, 
    /*43  "MediumAquamarine"*/ { 102,205,170},
    /*44  "MediumBlue"      */ {   0,  0,205},
    /*45  "Navy"            */ {   0,  0,128},
    /*46  "DarkOliveGreen"  */ {  85,107, 47}, 
    /*47  "DarkGreen"       */ {   0,100,  0},
    /*48  "DimGray"         */ { 105,105,105}, 
    /*49  "Tan"             */ { 210,180,140},
    /*50  "MediumTurquoise" */ {  72,209,204},
    /*51  "DarkSlateBlue"   */ {  72, 61,139}, 
    /*52  "Maroon"          */ { 176, 48, 96}, 
    /*53  "Gray"            */ { 190,190,190},
    /*54  "#303030"         */ {0x30,0x30,0x30},
    
    /*55  "Black"           */ {   0,  0,  0},
    /*56  "#00aa00"         */ {   0,0xaa, 0},
    /*56  "#00ee00"         */ {   0,0xee, 0},
    /*57  "#aa0000"         */ {0xaa,  0,  0},
    /*58  "#aaaaaa"         */ {0xaa,0xaa,0xaa},
    /*59  "#cccc00"         */ {0xcc,0xcc, 0},
    /*60  "#eeee00"         */ {0xee,0xee, 0}
};

const unsigned long TIMESTEP=5000l;
const int KEY_TICKS=5;
const int FIRST_KEY_TICKS=25;
//===========================================================================
/// global new_well_engine(argc,argv)
///     Creates new WellEngine object, SDL realization (create SDLWellEngine)
/// tags new_well_engine
WellEngine* new_well_engine(int argc, char** argv)
{
    return new SDLWellEngine(argc,argv);
}

//===========================================================================
/// global SDLWellEngine(argc,argv)
///     Constructor of class that incupsulates work with SDL window
///     This class must always be allocated through 'new'
/// args
///     +argc: number of commanline args
///     +argv: command line args
/// tags SDLWellEngine
SDLWellEngine::SDLWellEngine(int argc, char** argv):WellEngine(argc,argv)
{
    if( SDL_Init(SDL_INIT_VIDEO) < 0 ) 
    {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        exit(1);
    }
    init_mainwindow(argc,argv);
    init_colors();
    load_images();
    SDL_EnableUNICODE(1);
    bg_image=0;
}

//===========================================================================
/// global SDLWellEngine(argc,argv)
///     Destructor of class that incupsulates work with SDL window
/// tags SDLWellEngine
SDLWellEngine::~SDLWellEngine()
{
    SDL_Quit();
}

//===========================================================================
/// global init_mainwindow(argc,argv)
///     create main game window and all other stuff around it
/// tags SDLWellEngine
void SDLWellEngine::init_mainwindow(int argc,char **argv)
{
    mainw = SDL_SetVideoMode(mainl, mainh, 16, 
			     SDL_ANYFORMAT);
    if( mainw == 0 ) 
    {
        fprintf(stderr, "Unable to set %dx%dx16 video: %s\n",mainl, mainh, 
		SDL_GetError());
        SDL_Quit();
	exit(1);
    }
    dbgprintf(("VIDEO_INIT: Set 800x600x%d video mode\n", mainw->format->BitsPerPixel));
}

//===========================================================================
/// global show_main()
///     show main window
/// tags SDLWellEngine
void SDLWellEngine::show_main()
{
    SDL_UpdateRect(mainw, 0,0, mainl, mainh);
}


void SDLWellEngine::set_key_event(SDL_Event& event)
{
    key_event=event;
    key_event_got=true;
    process_event_for_all(wEvent(eKeyPress,&key_event));
    key_ticks=FIRST_KEY_TICKS;
}

void SDLWellEngine::reset_key_event()
{
    key_event_got=false;
}

void SDLWellEngine::process_key_event()
{
    if(key_ticks)
    {
	key_ticks--;
	return;
    }
    process_event_for_all(wEvent(eKeyPress,&key_event));
    key_ticks=KEY_TICKS;
}


//===========================================================================
/// global event_loop()
///     main event loop - process all events and make game moves
/// tags SDLWellEngine
void SDLWellEngine::event_loop()
{
    SDL_Event  event;
    MouseEvent mev;
    done_loop_var=false;
    key_event_got=false;
    dbgprintf(("Entered in event_loop\n"));
    
    while(!done_loop_var)
    {
	while(SDL_PollEvent(&event))
	{
	    switch (event.type) 
	    {
	    case SDL_VIDEOEXPOSE:
		dbgprintf(("Got expose event - processing\n"));
		process_event_for_all(wEvent(eExpose,&event));
		break;
	    case SDL_KEYDOWN:
		dbgprintf(("The '%s' key was pressed!\n",
			   SDL_GetKeyName(event.key.keysym.sym)));
		set_key_event(event);
		key_event=event;
		key_event_got=true;
		break;

	    case SDL_KEYUP:
		reset_key_event();
		break;

	    case SDL_MOUSEMOTION:
		mev.mx=event.motion.x;
		mev.my=event.motion.y;
		mev.bstate=event.motion.state==SDL_BUTTON_LEFT ? But1Press : But2Press;
		process_event_for_all(wEvent(eMouseMove,&mev));
		dbgprintf(("Mouse moved by to (%d,%d) with state: %d\n", 
			   event.motion.x, event.motion.y, event.motion.state));
		break;
	    case SDL_MOUSEBUTTONDOWN:
		dbgprintf(("MouseButtonPress event\n"));
		mev.mx=event.button.x;
		mev.my=event.button.y;
		mev.bstate=event.button.button==SDL_BUTTON_LEFT ? But1Press : But2Press;
		process_event_for_all(wEvent(eMousePress,&mev));
		break;
	    case SDL_QUIT:
		done_loop_var=true;
	    }
	}
	if(key_event_got)
	    process_key_event();
	udelay(TIMESTEP);
	process_time_event();
    }
}


//===========================================================================
/// global wait_for_timers()
///     calculate delay and wait for events and timers
/// tags SDLWellEngine
int SDLWellEngine::wait_for_timers()
{
    return 0;
}

//===========================================================================
/// global load_image(Images id, const char* name)
///     Load image into memory and register it with given id for use
/// tags SDLWellEngine
bool SDLWellEngine::load_image(Images id, const char* name)
{
  char buf[L_MAXPATH];

  if(id>=MAX_IMAGES)
    return false;

  find_full_path_for_file(name,buf);
  pics[id]=IMG_Load(buf);
  images[id].id=id;
  dbgprintf(("Loaded image: %s\n",buf));
  return true;
}


//===========================================================================
/// global set_main_background_image(Images id)
///     set image with given id to background of main window
/// tags SDLWellEngine
void SDLWellEngine::set_main_background_image(Images id) 
{
    SDL_Rect dest;
    if(images[id].id!=imNone)
    {
	dest.x = 0;
	dest.y = 0;
	dest.w = pics[id]->w;
	dest.h = pics[id]->h;
	SDL_BlitSurface(pics[id], 0, mainw, &dest);
	/* Update the changed portion of the screen */
	SDL_UpdateRects(mainw, 1, &dest);
	bg_image=pics[id];
    }
}

//===========================================================================
/// local init_colors()
///     allocate all nessesary colors
/// tags SDLWellEngine
void SDLWellEngine::init_colors()
{
    int i;
    for(i=0;i<MAX_GAME_COLORS;i++)
    {
	game_colors[i]=SDL_MapRGBA(mainw->format, 
				   game_color_names[i][0],
				   game_color_names[i][1],
				   game_color_names[i][2], 255);
	game_RGBA[i*4+0]=game_color_names[i][0];
	game_RGBA[i*4+1]=game_color_names[i][1];
	game_RGBA[i*4+2]=game_color_names[i][2];
	game_RGBA[i*4+3]=255;
    }
}


//===========================================================================
/// global new_well_image_font()
///     creates image font object - version for SDL Window
/// tags SDLWellEngine
WellImageFont*     SDLWellEngine::new_well_image_font(Images id, 
						    unsigned int ifl, 
						    unsigned int ifh, 
						    int idx, int idy)
{
  return new SDLWellImageFont(id,ifl,ifh,idx,idy);
}

//===========================================================================
/// global new_well_input(char*)
///     creates new input object - version for SDL Window
/// tags SDLWellEngine
WellInput* SDLWellEngine::new_well_input(const char* name)
{
  return new SDLWellInput(name);
}

//===========================================================================
/// global new_well_drawing_engine()
///     creates drawing engine for well board - version for SDL Window
/// tags SDLWellEngine
WellDrawingEngine* SDLWellEngine::new_well_drawing_engine()
{
  return new SDLWellDrawingEngine(this);
}

//===========================================================================
/// global udelay(int msec)
///   Portable delay :)
/// tags XWellEngine
void SDLWellEngine::udelay(int msec)
{ 
    SDL_Delay(msec/1000);
}

//===========================================================================
/// global screen_copy(Geo*)
///     copy part of image to the screen
/// tags SDLWellEngine
void SDLWellEngine::screen_copy(Geo *pgeo)
{
    SDL_Rect src,dest;
    if(pgeo->im==imNone)
    {
	screen_clear(pgeo);
	return;
    }
    src.x=pgeo->fromx;
    src.y=pgeo->fromy;
    src.w=dest.w=pgeo->l;
    src.h=dest.h=pgeo->h;
    dest.x=pgeo->tox;
    dest.y=pgeo->toy;

    SDL_BlitSurface(pics[pgeo->im],
		    &src, mainw, &dest);
    SDL_UpdateRects(mainw, 1, &dest);
}

//===========================================================================
/// global screen_clear(Geo*)
///     clear part of screen
/// tags SDLWellEngine
void SDLWellEngine::screen_clear(Geo *pgeo)
{
    SDL_Rect dest;
    dest.x=pgeo->tox;
    dest.y=pgeo->toy;
    dest.w=pgeo->l;
    dest.h=pgeo->h;

    if(!bg_image)
	SDL_FillRect(mainw, &dest, game_colors[BackColor]);
    else
	SDL_BlitSurface(bg_image, &dest, mainw, &dest);

    SDL_UpdateRects(mainw, 1, &dest);
}

