#ifndef IMAGE_H
#define IMAGE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "globals.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "picinfo.h"
#include <math.h>

extern Display* disp;
extern Colormap         defcmp;
extern int conv24;
extern int vclass;
void init_imagengine();
int im_convert_true_to_pseudo(PICINFO& pic,int max_colors);
int im_getpixels(unsigned long* pt);
Colormap im_get_colormap();
void im_freecmp(Colormap XColorMap,unsigned long *pt,int iptnum);
void im_populate_pallete(unsigned long* pal);
void im_clear_global_pal();
char *getpicinfo();
Pixmap LoadPixmap(char *fname,int type,int& l,int& h,int cmptype=CMP_AUTO);
Pixmap LoadPixmapFromMem(char *fname,int type,int& l,int& h,int cmptype=CMP_AUTO,int siz=0);
XImage *LoadXImage(char *file,int cmptype,int type);
XImage* encoding256(PICINFO& im,int ncol);
int LoadGIF(char* fname,PICINFO* pinfo);
int deGIF(char* fbu,PICINFO* pinfo,int fsiz);
extern "C" int LoadPCD(char* fname,PICINFO* pinfo);
extern "C" int LoadPNG(char *fname,PICINFO* pinfo);
int LoadXBM(char* fname,PICINFO* pinfo);
int LoadXPM(char* fname,PICINFO* pinfo);
int LoadTarga(char* fname,PICINFO* pinfo);
extern "C" int LoadTIFF(char* ,PICINFO*);
int autodetect(char*);
void im_reset_coding();
void im_set_coding();
extern int im_find_closest_color(XColor* orig, XColor* tocol);
extern Status im_AllocColor(Display* d, Colormap cmap, XColor *cl);
extern void (*im_coding256)(char **,int);
extern void im_256truecoding(char**,int);
extern void im_init_translation();
extern "C" int LoadJFIF(char *fname, PICINFO *pinfo);
extern "C" int LoadPCX(char *fname, PICINFO *pinfo);
extern int LoadBMP(char *fname, PICINFO *pinfo);
extern "C" void x11_create_lut(unsigned long, unsigned long, unsigned long);
extern "C" void set_method(int method);


#endif
//////////////////////////////////////////End of file///////////////////////////////////////////

/* ------------ End of file -------------- */

