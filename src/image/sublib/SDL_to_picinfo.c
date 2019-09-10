#define  JPEG_IMAGES
#include "SDL.h"
#include "picinfo.h"
#include "SDL_image.h"

void im_sdl_surface_to_picinfo(SDL_Surface* surface, PICINFO* pic)
{
  int i;

  pic->w=surface->w;
  pic->h=surface->h;

  if(surface->format->BytesPerPixel==1)
  {
    pic->type=PIC8;
    pic->pic=(byte*)surface->pixels;
    surface->pixels=0;
    for(i=0;i<256;i++)
    {
      pic->pal[i*3]  =surface->format->palette->colors[i].r;
      pic->pal[i*3+1]=surface->format->palette->colors[i].g;
      pic->pal[i*3+2]=surface->format->palette->colors[i].b;
    }
    SDL_FreeSurface(surface);
  }
  else
  {
    pic->type=PIC24;
    pic->pic=(byte*)surface->pixels;
    surface->pixels=0;
    SDL_FreeSurface(surface);
  }  
}

int  im_load_image_through_loader(char* fname, PICINFO* to_pic, 
		   int from_mem_size, 
		   SDL_Surface* (*img_loader)(SDL_RWops*))
{
  SDL_RWops    *rw;
  SDL_Surface  *surface;

  if(!from_mem_size)
    rw=SDL_RWFromFile(fname,"rb");
  else
    rw=SDL_RWFromMem((void*)fname, from_mem_size);

  surface=img_loader(rw);

  SDL_FreeRW(rw);

  im_sdl_surface_to_picinfo(surface,to_pic);

  return 1;
}

/******************************************************************/

int  im_load_gif(char* fname, PICINFO* to_pic, int from_mem_size)
{

  return im_load_image_through_loader(fname,to_pic,
				      from_mem_size,IMG_LoadGIF_RW);
}

