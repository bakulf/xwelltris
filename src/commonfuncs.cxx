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
// $Id: commonfuncs.cxx,v 1.3 2003/02/27 08:27:51 leo Exp $

#include "commonfuncs.h"


//===========================================================================
/// global find_full_path_for_file(const char* fname,
///                                char* ret_full_pathname, FMode mode)
///     find file in global and local dirs and return full pathname
/// tags common_funcs
bool find_full_path_for_file(const char* name, char* ret_full_pathname,
                             FMode mode)
{
#ifndef WIN32
  int    fd;
  char   *home=getenv("HOME");
  mode_t fmode=O_RDONLY;

  switch(mode)
  {
  case ReadOnly:
    fmode=O_RDONLY;
    break;
  case ReadWrite:
    fmode=O_RDWR;
    break;
  }

  do 
  {    
    sprintf(ret_full_pathname,"%s/%s",GLOBAL_SEARCH,name); //First check in GLOBAL_SEARCH dir
    fd=open(ret_full_pathname,fmode);
    if(fd>0)
      break;

    sprintf(ret_full_pathname,"%s/%s/%s",home,LOCAL_SEARCH,name); //Then in LOCAL one
    fd=open(ret_full_pathname,fmode);
    if(fd>0)
      break;

    sprintf(ret_full_pathname,"./%s",name); //Then in current dir
    fd=open(ret_full_pathname,fmode);
    if(fd>0)
      break;

    sprintf(ret_full_pathname,"./data/%s",name); //And finally in ./data
    fd=open(ret_full_pathname,fmode);
    if(fd>0)
      break;

    fprintf(stderr,"ERROR: Can't find file %s in:\n\t./\n\t./data\n\t%s/%s\n\t%s\n",
	    name,home,LOCAL_SEARCH,GLOBAL_SEARCH);
    return false;
  } while(0);

  close(fd);
  return true;
#else
  FILE *fp;
  sprintf(ret_full_pathname,"data\\%s",name); //And finally in ./data
  fp=fopen(ret_full_pathname, "r");
  if(fp)
  {
      fclose(fp);
      return true;
  }
  return false;
#endif
}
