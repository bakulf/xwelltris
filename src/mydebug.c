/****************************************************************************
*  Copyright (C) 2001 by Leo Khramov
*  email:     leo@xnc.dubna.su
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

//This file contains C functions useful for debugging xnc -> printing.
#include "mydebug.h"

int debprintf(const char *format,...)
{

  va_list args;
  int n;

  va_start(args, format);
  n = fprintf(stderr, "[dbg] ");
  n += vfprintf(stderr, format, args);
  va_end(args);
  fflush(stderr);
  return (n);
}

