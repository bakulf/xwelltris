#ifndef PICINFO_H
#define PICINFO_H
#include "config.h"
#include "globdef.h"
#ifdef HAVE_SYS_SIGEVENT_H
#include <sys/sigevent.h>
#endif
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#ifndef __FreeBSD__                                                            
#elif ! defined __OpenBSD__
#include <malloc.h>
#endif
#include <fcntl.h>
#include <unistd.h>

/*
#undef PARM
#ifdef __STDC__
#  define PARM(a) a
#else
#  define PARM(a) ()
#  define const
#endif
 */
#define MONO(rr,gg,bb) (((rr)*11+(gg)*16+(bb)*5)>>5) /*0.33r+0.5g+0.17b*/
#ifndef PCD_DEFS
#define RANGE(a,b,c) {if(a<b) a=b;if(a>c) a=c;}
#endif

#define GETWD(x) getcwd(x,sizeof(x))

#define True 1
#define False 0
#define TRUE 1
#define FALSE 0
#define F_COLORS    0
#define F_FORMAT    1
#define PIC8        0
#define PIC24       1
#define F_FULLCOLOR 0
#define F_GREYSCALE 1
#define F_BWDITHER  2
#define F_REDUCED   3

#define CMP_AUTO 0
#define CMP_OWN 1
#define CMP_COM 2
#define F_GIF       0
#define F_PM        1
#define F_PBMRAW    2
#define F_PBMASCII  3
#define F_XBM       4
#define F_SUNRAS    5
#define F_BMP       6
#define F_PS        7
#define F_IRIS      8
#define F_JPEG      9
#define F_AUTO    11
#define F_TIFF      10
#define F_PCX      12
#define F_TARGA 13
#define F_XPM 14
#define F_PCD 15
#define F_PNG 16

#define CONV24_FAST 5
#define CONV24_SLOW 6
#define CONV24_BEST 7

#define ALG_NONE 0
#define ALG_BLUR3 2
#define ALG_BLUR5 3
#define ALG_BLUR7 4
#define ALG_BLUR9 1
#define ALG_EDGE  5
#define ALG_TINF 6
#define ALG_OIL 7
#define ALG_GRAY 8

typedef unsigned char byte;
#ifndef JPEG_IMAGES
#ifndef PCD_DEFS
#ifndef u_short
typedef unsigned short u_short;
#endif
#ifndef u_char
typedef  unsigned char u_char;
#endif
#ifndef u_int
typedef unsigned int u_int;
#endif
#ifndef u_long
typedef unsigned long u_long;
#endif
#endif
#endif

extern int numcols;
extern float normaspect;
extern int conv24;
extern int noqcheck;
extern int picType,pWIDE,pHIGH,ncols;
extern byte bMap[256],gMap[256],rMap[256];
extern byte* pic;
extern byte colAllocOrder[256];

typedef struct { byte *pic;                  /* image data */
                 int   w, h;                 /* size */
                 int   type;                 /* PIC8 or PIC24 */

                 byte  pal[768];
                                             /* colormap, if PIC8 */

                 int   frmType;              /* def. Format type to save in */
                 int   colType;              /* def. Color type to save in */
                 char  fullInfo[128];        /* Format: field in info box */
                 char  shrtInfo[128];        /* short format info */
                 char *comment;              /* comment text */

                 int   numpages;             /* # of page files, if >1 */
                 char  pagebname[64];        /* basename of page files */
               } PICINFO;
#endif
/* ------------ End of file -------------- */

