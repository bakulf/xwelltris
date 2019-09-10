
#include "SDL.h"


/* 
 * Calculate the pad-aligned scanline width of a surface
 */
Uint16 SDL_CalculatePitch(SDL_Surface *surface)
{
  Uint16 pitch;
  
  /* Surface should be 4-byte aligned for speed */
  pitch = surface->w*surface->format->BytesPerPixel;
  switch (surface->format->BitsPerPixel) 
  {
  case 1:
    pitch = (pitch+7)/8;
    break;
  case 4:
    pitch = (pitch+1)/2;
    break;
  default:
    break;
  }
  pitch = (pitch + 3) & ~3;       /* 4-byte aligning */
  return(pitch);
}

/*
 * Free a previously allocated format structure
 */
void SDL_FreeFormat(SDL_PixelFormat *format)
{
  if ( format ) 
  {
    if ( format->palette ) 
    {
      if ( format->palette->colors ) 
      {
	free(format->palette->colors);
      }
      free(format->palette);
    }
    free(format);
  }
}

/* Helper functions */
/*
 * Allocate a pixel format structure and fill it according to the given info.
 */
SDL_PixelFormat *SDL_AllocFormat(int bpp,
			Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
{
	SDL_PixelFormat *format;
	Uint32 mask;

	/* Allocate an empty pixel format structure */
	format = (SDL_PixelFormat*)malloc(sizeof(*format));
	if ( format == NULL ) {
		SDL_OutOfMemory();
		return(NULL);
	}
	memset(format, 0, sizeof(*format));
	format->alpha = SDL_ALPHA_OPAQUE;

	/* Set up the format */
	format->BitsPerPixel = bpp;
	format->BytesPerPixel = (bpp+7)/8;
	switch (bpp) {
		case 1:
			/* Create the 2 color black-white palette */
			format->palette = (SDL_Palette *)malloc(
							sizeof(SDL_Palette));
			if ( format->palette == NULL ) {
				SDL_FreeFormat(format);
				SDL_OutOfMemory();
				return(NULL);
			}
			(format->palette)->ncolors = 2;
			(format->palette)->colors = (SDL_Color *)malloc(
				(format->palette)->ncolors*sizeof(SDL_Color));
			if ( (format->palette)->colors == NULL ) {
				SDL_FreeFormat(format);
				SDL_OutOfMemory();
				return(NULL);
			}
			format->palette->colors[0].r = 0xFF;
			format->palette->colors[0].g = 0xFF;
			format->palette->colors[0].b = 0xFF;
			format->palette->colors[1].r = 0x00;
			format->palette->colors[1].g = 0x00;
			format->palette->colors[1].b = 0x00;
			format->Rloss = 8;
			format->Gloss = 8;
			format->Bloss = 8;
			format->Aloss = 8;
			format->Rshift = 0;
			format->Gshift = 0;
			format->Bshift = 0;
			format->Ashift = 0;
			format->Rmask = 0;
			format->Gmask = 0;
			format->Bmask = 0;
			format->Amask = 0;
			break;

		case 4:
			/* Create the 16 color VGA palette */
			format->palette = (SDL_Palette *)malloc(
							sizeof(SDL_Palette));
			if ( format->palette == NULL ) {
				SDL_FreeFormat(format);
				SDL_OutOfMemory();
				return(NULL);
			}
			(format->palette)->ncolors = 16;
			(format->palette)->colors = (SDL_Color *)malloc(
				(format->palette)->ncolors*sizeof(SDL_Color));
			if ( (format->palette)->colors == NULL ) {
				SDL_FreeFormat(format);
				SDL_OutOfMemory();
				return(NULL);
			}
			/* Punt for now, will this ever be used? */
			memset((format->palette)->colors, 0,
				(format->palette)->ncolors*sizeof(SDL_Color));

			/* Palettized formats have no mask info */
			format->Rloss = 8;
			format->Gloss = 8;
			format->Bloss = 8;
			format->Aloss = 8;
			format->Rshift = 0;
			format->Gshift = 0;
			format->Bshift = 0;
			format->Ashift = 0;
			format->Rmask = 0;
			format->Gmask = 0;
			format->Bmask = 0;
			format->Amask = 0;
			break;

		case 8:
			/* Create an empty 256 color palette */
			format->palette = (SDL_Palette *)malloc(
							sizeof(SDL_Palette));
			if ( format->palette == NULL ) {
				SDL_FreeFormat(format);
				SDL_OutOfMemory();
				return(NULL);
			}
			(format->palette)->ncolors = 256;
			(format->palette)->colors = (SDL_Color *)malloc(
				(format->palette)->ncolors*sizeof(SDL_Color));
			if ( (format->palette)->colors == NULL ) {
				SDL_FreeFormat(format);
				SDL_OutOfMemory();
				return(NULL);
			}
			memset((format->palette)->colors, 0,
				(format->palette)->ncolors*sizeof(SDL_Color));

			/* Palettized formats have no mask info */
			format->Rloss = 8;
			format->Gloss = 8;
			format->Bloss = 8;
			format->Aloss = 8;
			format->Rshift = 0;
			format->Gshift = 0;
			format->Bshift = 0;
			format->Ashift = 0;
			format->Rmask = 0;
			format->Gmask = 0;
			format->Bmask = 0;
			format->Amask = 0;
			break;

		default:
			/* No palette, just packed pixel info */
			format->palette = NULL;
			format->Rshift = 0;
			format->Rloss = 8;
			if ( Rmask ) {
				for ( mask = Rmask; !(mask&0x01); mask >>= 1 )
					++format->Rshift;
				for ( ; (mask&0x01); mask >>= 1 )
					--format->Rloss;
			}
			format->Gshift = 0;
			format->Gloss = 8;
			if ( Gmask ) {
				for ( mask = Gmask; !(mask&0x01); mask >>= 1 )
					++format->Gshift;
				for ( ; (mask&0x01); mask >>= 1 )
					--format->Gloss;
			}
			format->Bshift = 0;
			format->Bloss = 8;
			if ( Bmask ) {
				for ( mask = Bmask; !(mask&0x01); mask >>= 1 )
					++format->Bshift;
				for ( ; (mask&0x01); mask >>= 1 )
					--format->Bloss;
			}
			format->Ashift = 0;
			format->Aloss = 8;
			if ( Amask ) {
				for ( mask = Amask; !(mask&0x01); mask >>= 1 )
					++format->Ashift;
				for ( ; (mask&0x01); mask >>= 1 )
					--format->Aloss;
			}
			format->Rmask = Rmask;
			format->Gmask = Gmask;
			format->Bmask = Bmask;
			format->Amask = Amask;
			break;
	}
	/* Calculate some standard bitmasks, if necessary 
	 * Note:  This could conflict with an alpha mask, if given.
	 */
	if ( (bpp > 8) && !format->Rmask && !format->Gmask && !format->Bmask ) {
		/* R-G-B */
		if ( bpp > 24 )
			bpp = 24;
		format->Rloss = 8-(bpp/3);
		format->Gloss = 8-(bpp/3)-(bpp%3);
		format->Bloss = 8-(bpp/3);
		format->Rshift = ((bpp/3)+(bpp%3))+(bpp/3);
		format->Gshift = (bpp/3);
		format->Bshift = 0;
		format->Rmask = ((0xFF>>format->Rloss)<<format->Rshift);
		format->Gmask = ((0xFF>>format->Gloss)<<format->Gshift);
		format->Bmask = ((0xFF>>format->Bloss)<<format->Bshift);
	}
	return(format);
}

/*
 * Free a surface created by the above function.
 */
void SDL_FreeSurface (SDL_Surface *surface)
{
	/* Free anything that's not NULL, and not the screen surface */
  if (surface == NULL)
  {
    return;
  }
  if ( --surface->refcount > 0 ) {
    return;
  }
  if ( surface->format ) 
  {
    SDL_FreeFormat(surface->format);
    surface->format = NULL;
  }
  if ( surface->pixels &&
       ((surface->flags & SDL_PREALLOC) != SDL_PREALLOC) ) 
  {
    free(surface->pixels);
  }
  free(surface);
}



/* Public routines */
/*
 * Create an empty RGB surface of the appropriate depth
 */
SDL_Surface * SDL_CreateRGBSurface (Uint32 flags,
			int width, int height, int depth,
			Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
{
	SDL_Surface *surface;

	flags &= ~SDL_HWSURFACE;

	/* Allocate the surface */
	surface = (SDL_Surface *)malloc(sizeof(*surface));
	if ( surface == NULL ) 
	{
		return(NULL);
	}
	surface->flags = SDL_SWSURFACE;

	surface->format = SDL_AllocFormat(depth, Rmask, Gmask, Bmask, Amask);

	if ( surface->format == NULL ) 
	{
		free(surface);
		return(NULL);
	}
	if ( Amask ) 
	{
	  surface->flags |= SDL_SRCALPHA;
	}
	surface->w = width;
	surface->h = height;
	surface->pitch = SDL_CalculatePitch(surface);
	surface->pixels = NULL;
	surface->offset = 0;
	//	surface->hwdata = NULL;
	//	surface->locked = 0;
	//	surface->map = NULL;
	surface->format_version = 0;

	if ( surface->w && surface->h ) 
	{
	  surface->pixels = (void*)malloc(surface->h*surface->pitch);
	  if ( surface->pixels == NULL ) 
	  {
	    SDL_FreeSurface(surface);
	    return(NULL);
	  }
	  /* This is important for bitmaps */
	  memset(surface->pixels, 0, surface->h*surface->pitch);
	}

	/* The surface is ready to go */
	surface->refcount = 1;
	return(surface);
}

int SDL_SetColorKey(SDL_Surface *surface, Uint32 flag, Uint32 key)
{
  return 0;
}


