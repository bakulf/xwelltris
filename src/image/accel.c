/****************************************************************************
*  Copyright (C) 1996 by Leo Khramov
*  email:     old@sunct2.jinr.dubna.su
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
#include "accel.h"
#include <X11/Xlib.h>

char *imd;
void  (*im_coding)(unsigned int,unsigned int,unsigned int,char**);
void im_truetruecoding(unsigned int ir,unsigned int ig,unsigned int ib,char **imdata)
{
  int j;
  imd=*imdata;
  ir>>=red_rsh;
  ir<<=red_lsh;
  ig>>=green_rsh;
  ig<<=green_lsh;
  ib>>=blue_rsh;
  ib<<=blue_lsh;
  ir=ir | ig | ib;
  imd+=bdep;
  for(j=0;j<bdep;j++)
  {
     *(--imd)=(char)(ir & 0xff);
     ir>>=8;
  }
  
  (*imdata)+=bdep;
}

void im_truenocoding(unsigned int ir,unsigned int ig,unsigned int ib,char **imdata)
{
 *((*imdata)++)=ir&0xff;
 *((*imdata)++)=ig&0xff;
 *((*imdata)++)=ib&0xff;
}

char *im_alloc_true(int l,int h)
{
   if (bdep == 1) {
      l *= 3;
   } else {
      l *= bdep;
      if (l % b_pad)
         l += b_pad - l % b_pad;
   }
   return (char*) malloc(l*h);
}

void im_setzero(char* s,int len)
{
  for ( ; len>0; len--) 
    *s++ = 0;
}

void im_memcopy(char *src, char* dest, int len)
{

  if (src==dest || len<=0) 
    return;    /* nothin' to do */
  
  if (src<dest && src+len>dest) 
  {  /* do backward copy */
    src = src + len - 1;
    dest = dest + len - 1;
    for ( ; len>0; len--, src--, dest--) 
      *dest = *src;
  }

  else 
  {
    for ( ; len>0; len--, src++, dest++)
      *dest = *src;
  }
}
    

extern int vclass;

unsigned int im_get_linew_true(unsigned int w)
{
  unsigned line_w;
  if(vclass==TrueColor)
    {
      line_w = w * bdep;
      if (line_w % b_pad)
        line_w += b_pad - line_w % b_pad;
    } else
      line_w=w*3;  //24 bit non-converted images in PseudoColor before convertion.
  return line_w;
}
