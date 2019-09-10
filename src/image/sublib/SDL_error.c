/*
    Copyright (C) 1999, 2000, 2001  Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org

    Modified for using with XNC image engine by Leo <leo@xnc.dubna.su>
    26-03-2002.
*/

#include "SDL.h"

/* Very common errors go here */
void SDL_Error(SDL_errorcode code)
{
        switch (code) {
                case SDL_ENOMEM:
                        SDL_SetError("Out of memory");
                        break;
                case SDL_EFREAD:
                        SDL_SetError("Error reading from datastream");
                        break;
                case SDL_EFWRITE:
                        SDL_SetError("Error writing to datastream");
                        break;
                case SDL_EFSEEK:
                        SDL_SetError("Error seeking in datastream");
                        break;
                default:
                        SDL_SetError("Unknown SDL error");
                        break;
        }
}

void SDL_SetError (const char *fmt, ...)
{
  fprintf(stderr, "SDL_error: %s\n",fmt);
}

void SDL_ClearError(void) 
{
}
