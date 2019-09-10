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
// $Id: xwellengine.cxx,v 1.3 2003/02/20 15:09:41 leo Exp $

/// module description
/// This module contains methods of class XWellEngine which implements X11 calls
/// for the game. So this is X11 dependent part. It receives XEvents, 
/// make convertions, many initions and calls.


#include "version.h"
#include "globals.h"
#include "xwellengine.h"
#include "xwellinput.h"
#include "image/image.h"
#include "bilist.h"
#include "commonfuncs.h"
#include "wellclass.h"

Display *disp;
Colormap defcmp;

const unsigned long TIMESTEP=10000l;

//Colors for welltris pieces and grid (x color names)
static char    *game_color_names[MAX_GAME_COLORS] = 
{
  /* 0*/  "Red", 
  /* 1*/  "Green", 
  /* 2*/  "Blue", 
  /* 3*/  "Yellow", 
  /* 4*/  "Magenta", 
  /* 5*/  "Cyan", 
  /* 6*/  "FireBrick", 
  /* 7*/  "SpringGreen",
  /* 8*/  "CornflowerBlue", 
  /* 9*/  "Khaki", 
  /*10*/  "Plum", 
  /*11*/  "Violet",
  /*12*/  "DarkTurquoise", 
  /*13*/  "Gold", 
  /*14*/  "Orchid", 
  /*15*/  "Turquoise",
  /*16*/  "Orange", 
  /*17*/  "OrangeRed", 
  /*18*/  "VioletRed", 
  /*19*/  "BlueViolet",
  /*20*/  "SeaGreen", 
  /*21*/  "Pink", 
  /*22*/  "ForestGreen", 
  /*23*/  "SkyBlue",
  /*24*/  "Coral", 
  /*25*/  "Wheat", 
  /*26*/  "GoldenRod", 
  /*27*/  "IndianRed",
  /*28*/  "SpringGreen", 
  /*29*/  "CornflowerBlue", 
  /*30*/  "Thistle", 
  /*31*/  "Aquamarine",
  /*32*/  "CadetBlue", 
  /*33*/  "LightSteelBlue", 
  /*34*/  "NavyBlue", 
  /*35*/  "SteelBlue",
  /*36*/  "YellowGreen", 
  /*37*/  "DarkViolet", 
  /*38*/  "MediumSeaGreen", 
  /*39*/  "DarkSlateGray",
  /*40*/  "LightGray", 
  /*41*/  "MediumVioletRed", 
  /*42*/  "Sienna", 
  /*43*/  "MediumAquamarine",
  /*44*/  "MediumBlue", 
  /*45*/  "Navy", 
  /*46*/  "DarkOliveGreen", 
  /*47*/  "DarkGreen",
  /*48*/  "DimGray", 
  /*49*/  "Tan", 
  /*50*/  "MediumTurquoise", 
  /*51*/  "DarkSlateBlue",
  /*52*/  "Maroon", 
  /*53*/  "Gray", 
  /*54*/  "#303030",

  /*55*/  "Black",
  /*56*/  "#00aa00",
  /*56*/  "#00ee00",
  /*57*/  "#aa0000",
  /*58*/  "#aaaaaa",
  /*59*/  "#cccc00",
  /*60*/  "#eeee00"
};


//===========================================================================
/// global new_well_engine(argc,argv)
///     Creates new WellEngine object, X11 realization (create XWellEngine)
/// tags new_well_engine
WellEngine* new_well_engine(int argc, char** argv)
{
  return new XWellEngine(argc,argv);
}

//===========================================================================
/// global XWellEngine(argc,argv)
///     Constructor of class that incupsulates work with X window
///     This class must always be allocated through 'new'
/// args
///     +argc: number of commanline args
///     +argv: command line args
/// tags XWellEngine
XWellEngine::XWellEngine(int argc, char** argv):WellEngine(argc,argv)
{
  int i;
  char *dispchr=0;
  for(i=1;i<argc;i++)
    if((strcmp("-display",argv[i])==0 ||
	strcmp("-d",      argv[i])==0) && i+1<argc)
      dispchr=argv[i+1];
  if((disp=XOpenDisplay(dispchr))==NULL)
  {
    fprintf(stderr,
	    "%s: connection to X server failed on display %s.....\n",
	    argv[0],dispchr);
    exit(1);
  }
  dbgprintf(("Opened X display at [%s]\n",dispchr));

  scr=DefaultScreen(disp);
  colormap=defcmp=DefaultColormap(disp,scr);
  whitepixel=WhitePixel(disp,scr);
  blackpixel=BlackPixel(disp,scr);

  xcon=ConnectionNumber(disp);

  init_imagengine();
  init_mainwindow(argc,argv);
  load_images();
  init_colors();
}

//===========================================================================
/// global XWellEngine(argc,argv)
///     Destructor of class that incupsulates work with X window
/// tags XWellEngine
XWellEngine::~XWellEngine()
{
  XCloseDisplay(disp);
  disp=0;
}

//===========================================================================
/// global init_mainwindow(argc,argv)
///     create main game window and all other stuff around it
/// tags XWellEngine
void XWellEngine::init_mainwindow(int argc,char **argv)
{
  char *winname="X Welltris "VERSION;
  XSetWindowAttributes xswa;
  XWindowAttributes xwinattr;
  XGCValues         gcv;
  
  if((pxsh=XAllocSizeHints())==NULL)
  {
    fprintf(stderr,"xwelltris: Error allocating size hints\n");
    exit(1);
  }
 
  pxsh->flags=(USSize | PSize | PMinSize | PMaxSize);
  pxsh->height=mainh;
  pxsh->width=mainl;
  pxsh->min_width=pxsh->width;
  pxsh->min_height=pxsh->height;
  pxsh->max_width=pxsh->width;
  pxsh->max_height=pxsh->height;

  mainw=XCreateSimpleWindow(disp,DefaultRootWindow(disp),
			   0,0,pxsh->width,pxsh->height,
			   0,whitepixel,blackpixel);
  if((pch=XAllocClassHint())==NULL)
  {
    fprintf(stderr,"xwelltris: Error allocating ClassHint\n");
    exit(1);
  }
  pch->res_name="xwelltris";
  pch->res_class="XWELLTRIS";
  if(XStringListToTextProperty(&winname,1,&wname)==0)
  {
    fprintf(stderr,"xwelltris: Error creating TextProperty\n");
    exit(1);
  }       
  if(XStringListToTextProperty(&pch->res_name,1,&iname)==0)
  {
    fprintf(stderr,"xwelltris: Error creating TextProperty\n");
    exit(1);
  }       
  if((pxwmh=XAllocWMHints())==NULL)
  {
    fprintf(stderr,"welltris: Error allocating WMHints\n");
    exit(0);
  }
  pxwmh->flags=(InputHint|StateHint);
  pxwmh->input=True;
  pxwmh->initial_state=NormalState;
  XSetWMProperties(disp,mainw,&wname,&iname,argv,argc,pxsh,pxwmh,pch);
 
  gcv.foreground=whitepixel;
  gcv.background=blackpixel;
  maingc=XCreateGC(disp,mainw,(GCForeground | GCBackground),&gcv);
  xswa.colormap=defcmp;
  xswa.bit_gravity=CenterGravity;
  XChangeWindowAttributes(disp,mainw,(CWColormap | CWBitGravity),&xswa);
  XSelectInput(disp,mainw,
	       ExposureMask | 
	       FocusChangeMask | 
	       StructureNotifyMask |
	       KeyPressMask |
	       ButtonPressMask |
	       PointerMotionMask |
	       ButtonReleaseMask |
	       FocusChangeMask);
  wmDeleteWindow = XInternAtom(disp, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(disp, mainw, &wmDeleteWindow, 1);
  XSetGraphicsExposures(disp,maingc,False);
}

//===========================================================================
/// global show_main()
///     show main window
/// tags XWellEngine
void XWellEngine::show_main()
{
  XMapRaised(disp,mainw);
  XFlush(disp);
}

//===========================================================================
/// global load_image(Images id, char* name)
///     Load image into memory and register it with given id for use
/// tags XWellEngine
bool XWellEngine::load_image(Images id, char* name)
{
  char buf[L_MAXPATH];

  if(id>=MAX_IMAGES)
    return false;

  find_full_path_for_file(name,buf);
  image_pixmaps[id]=LoadPixmap(buf,F_AUTO,images[id].l,images[id].h,CMP_OWN);
  images[id].id=id;
  return true;
}


//===========================================================================
/// global screen_copy(Geo*)
///     copy part of image to the screen
/// tags XWellEngine
void XWellEngine::screen_copy(Geo *pgeo)
{
  if(pgeo->im==imNone)
  {
    screen_clear(pgeo);
    return;
  }
  XCopyArea(disp,image_pixmaps[pgeo->im],mainw,maingc,
	    pgeo->fromx,pgeo->fromy,pgeo->l,pgeo->h,
	    pgeo->tox,pgeo->toy);
}

//===========================================================================
/// global screen_clear(Geo*)
///     clear part of screen
/// tags XWellEngine
void XWellEngine::screen_clear(Geo *pgeo)
{

  XClearArea(disp,mainw,pgeo->tox,pgeo->toy,pgeo->l,pgeo->h,False);
}

//===========================================================================
/// global set_main_background_image(Images id)
///     set image with given id to background of main window
/// tags XWellEngine
void XWellEngine::set_main_background_image(Images id) 
{
  if(images[id].id!=imNone)
  {
    XSetWindowBackgroundPixmap(disp,mainw, image_pixmaps[id]);
    XClearWindow(disp,mainw);
  }
}


//===========================================================================
/// global event_loop()
///     main event loop - process all events and make game moves
/// tags XWellEngine
void XWellEngine::event_loop()
{
  MouseEvent mev;
  struct timezone tzone;
  int count=0;
  done_loop_var=false;
  dbgprintf(("Entered in event_loop\n"));

  gettimeofday(&now, &tzone);
  gettimeofday(&nexttime, &tzone);

  while(!done_loop_var)
  {
    if(wait_for_timers()==0) //Time out
    {
      process_time_event();
    }

    if(XPending(disp))
    {
      dbgprintf(("Got xevent: %d for window 0x%x\n",xev.type,xev.xany.window));
      XNextEvent(disp,&xev);
      switch(xev.type)
      {
      case Expose:
	dbgprintf(("Expose event\n"));
	process_event_for_all(wEvent(eExpose,&xev));
	break;
	
      case KeyPress:
	dbgprintf(("KeyPressed event\n"));
	process_event_for_all(wEvent(eKeyPress,&xev));
	break;

      case ButtonPress:
	dbgprintf(("MouseButtonPress event\n"));
	mev.mx=xev.xbutton.x;
	mev.my=xev.xbutton.y;
	mev.bstate=xev.xbutton.button==Button1 ? But1Press : But2Press;
	process_event_for_all(wEvent(eMousePress,&mev));
	break;

      case MotionNotify:
	dbgprintf(("MouseMotion event\n"));
	mev.mx=xev.xmotion.x;
	mev.my=xev.xmotion.y;
	mev.bstate=xev.xbutton.button==Button1 ? But1Press : But2Press;
	process_event_for_all(wEvent(eMouseMove,&mev));
	break;

      case ClientMessage:
	dbgprintf(("ClientMessage event\n"));
	if (xev.xclient.format == 32 && 
	    xev.xclient.data.l[0] == wmDeleteWindow)
	{
	  dbgprintf(("Got WM_DELETE_WINDOW\n"));
	  process_event_for_all(wEvent(eDelete));
	  done_loop_var=true;
	}
	break;

      case FocusOut:
	dbgprintf(("FocusOut event\n"));
	process_event_for_all(wEvent(eFocusOut));
	break;

      }  
    }
  }
  dbgprintf(("Exiting from event_loop\n"));
}

//===========================================================================
/// global wait_for_timers()
///     calculate delay and wait for events and timers
/// tags XWellEngine
int XWellEngine::wait_for_timers()
{
  struct timeval  delayt;
  struct timezone tzone;
  int             ret=0;
  fd_set          writefd, exceptfd, readfd;  

  FD_ZERO(&writefd);
  FD_ZERO(&exceptfd);
  FD_ZERO(&readfd);
  FD_SET(xcon, &readfd);

  gettimeofday(&now, &tzone);
  delayt.tv_sec = nexttime.tv_sec - now.tv_sec;
  delayt.tv_usec = nexttime.tv_usec - now.tv_usec;
  correct_to_real_time(delayt);
  if (((long) delayt.tv_sec > 0) ||
      (((long) delayt.tv_sec == 0) &&
       ((long) delayt.tv_usec > 0))) 
  {
    ret=select(xcon + 1, &readfd, &writefd, &exceptfd, &delayt);
  }
  if(ret==0) //Timed out
    gettimeofday(&now, &tzone);
  nexttime.tv_sec=now.tv_sec;
  nexttime.tv_usec=now.tv_usec + TIMESTEP;
  correct_to_real_time(nexttime);

  return ret;
}

//===========================================================================
/// global correct_to_real_time(struct timeval&)
///     corrects timval to real values
/// tags XWellEngine
void XWellEngine::correct_to_real_time(struct timeval& tv)
{
  while (tv.tv_usec < 0) 
  {
    tv.tv_sec --;
    tv.tv_usec += MILLION;
  }
  while (tv.tv_usec >= MILLION) 
  {
    tv.tv_sec ++;
    tv.tv_usec -= MILLION;
  }
}


//===========================================================================
/// global udelay(int msec)
///   Unix portable delay :)
/// tags XWellEngine
void XWellEngine::udelay(int msec)
{ 
  struct timeval tout;
  tout.tv_sec=0;
  tout.tv_usec=msec;
  select(0,0,0,0,&tout);
}



//===========================================================================
/// local init_colors()
///     allocate all nessesary colors
/// tags XWellEngine
void XWellEngine::init_colors()
{
  XColor          tmp;
  int             i;

  colormap=im_get_colormap();

  if(!colormap)
    colormap=defcmp;
  else
    XSetWindowColormap(disp,mainw,colormap);

  for(i=COLOR_DELTA;i<MAX_GAME_COLORS;i++)
  {
    if (XParseColor(disp, colormap, game_color_names[i], &tmp) == 0) 
    {
      fprintf(stderr, "Welltris: invalid color '%s'.\n",
	      game_color_names[i]);
      game_colors[i]=blackpixel;
    }
    if (XAllocColor(disp, colormap, &tmp) == 0) {
        (void) fprintf(stderr, "Welltris: can't allocate color '%s'.\n",
		       game_color_names[i]);
        game_colors[i]=blackpixel;
    }
    game_colors[i]=tmp.pixel;
  }

  if(colormap!=defcmp)
  {
    dbgprintf(("Allocating colors for Pseudo Colormap\n"));
    for(i=0;i<FREE_COLORS;i++)
    {
      if (XParseColor(disp, colormap, game_color_names[i], &tmp) == 0) 
      {
	fprintf(stderr, "Welltris: invalid color '%s'.\n",
		game_color_names[i]);
	game_colors[i]=blackpixel;
      }
      if (XAllocColor(disp, colormap, &tmp) == 0) {
        (void) fprintf(stderr, "Welltris: can't allocate color '%s'."
		       "Using black instead\n",
		       game_color_names[i]);
        game_colors[i]=blackpixel;
      }
      game_colors[i]=tmp.pixel;
    }
    for(i=FREE_COLORS;i<MAX_GAME_COLORS - SYSCOLORS;i++)
      game_colors[i]=game_colors[i-FREE_COLORS];

  } else
  {
    for(i=0;i<MAX_GAME_COLORS - SYSCOLORS;i++)
    {
      if (XParseColor(disp, colormap, game_color_names[i], &tmp) == 0) 
      {
	fprintf(stderr, "Welltris: invalid color '%s'.\n",
		game_color_names[i]);
	game_colors[i]=blackpixel;
      }
      if (XAllocColor(disp, colormap, &tmp) == 0) {
        (void) fprintf(stderr, "Welltris: can't allocate color '%s'.\n",
		       game_color_names[i]);
        game_colors[i]=blackpixel;
      }
      game_colors[i]=tmp.pixel;
    }
  }
}

//===========================================================================
/// global new_well_drawing_engine()
///     creates drawing engine for well board - version for X Window
/// tags XWellEngine
WellDrawingEngine* XWellEngine::new_well_drawing_engine()
{
  return new XWellDrawingEngine(this);
}

//===========================================================================
/// global new_well_top_nine()
///     creates new top nine object - version for X Window
/// tags XWellEngine
WellTopNine* XWellEngine::new_well_top_nine()
{
  return new WellTopNine;
}

//===========================================================================
/// global new_well_intro()
///     creates new introduction object - version for X Window
/// tags XWellEngine
WellIntro* XWellEngine::new_well_intro()
{
  return new WellIntro;
}

//===========================================================================
/// global new_well_key(char*)
///     creates new key object - version for X Window
/// tags XWellEngine
WellKey* XWellEngine::new_well_key(char* name)
{
  return new WellKey(name);
}

//===========================================================================
/// global new_well_switch(char*)
///     creates new switch object - version for X Window
/// tags XWellEngine
WellSwitch* XWellEngine::new_well_switch(char* name)
{
  return new WellSwitch(name);
}

//===========================================================================
/// global new_well_input(char*)
///     creates new input object - version for X Window
/// tags XWellEngine
WellInput* XWellEngine::new_well_input(char* name)
{
  return new XWellInput(name);
}

//===========================================================================
/// global new_well_base()
///     creates new game object - version for X Window
/// tags XWellEngine
WellBase* XWellEngine::new_well_base()
{
  return new WellBase;
}

//===========================================================================
/// global new_well_image_font()
///     creates image font object - version for X Window
/// tags XWellEngine
WellImageFont*     XWellEngine::new_well_image_font(Images id, 
						    unsigned int ifl, 
						    unsigned int ifh, 
						    int idx, int idy)
{
  return new XWellImageFont(id,ifl,ifh,idx,idy);
}
